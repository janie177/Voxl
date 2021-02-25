#include "World.h"

#include <IGameMode.h>
#include <IChunkStore.h>
#include <IWorldGenerator.h>
#include <IVoxelEditor.h>
#include <IServer.h>

#include <cassert>

#include <logging/Logger.h>
#include <other/ServiceLocator.h>

#include "ClientConnection.h"

#include "ChunkStore.h"
#include "file/FileUtilities.h"
#include "JsonUtilities.h"
#include "VoxelEditor.h"

#undef CreateFile

namespace voxl
{

    World::World(const std::string& a_Name) : m_State(WorldState::UNLOADED)
    {
        m_Settings.name = a_Name;
    }

    const std::string& World::GetName()
    {
        if (m_State != WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't retrieve name of world that was not yet loaded.");
        }
        return m_Settings.name;
    }

    void World::Save()
    {
        if(m_State != WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't save world that was not yet loaded.");
            return;
        }

        //Mark as saving so that no tick will happen if checked.
        m_State = WorldState::SAVING;

        //TODO add a_Async bool to function and then run in other thread after changing state.

        SaveWorldSettings(m_Settings);

        //Save each chunk as well.
        for (auto& chunk : *m_ChunkStore)
        {
            chunk.Save(*this);
        }

        m_State = WorldState::RUNNING;
    }

    void World::Unload()
    {
        if (m_State != WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't unload a world that was not yet loaded.");
            return;
        }

        //TODO ensure players are added to another world.

        //Unload all the chunks.
        m_ChunkStore->UnloadAll();

        m_State = WorldState::UNLOADED;
    }

    bool World::Load(IServer& a_Server)
    {
        if(m_State != WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Warning, "Tried to load world '" + m_Settings.name + "' that was not unloaded.");
            return false;
        }

        //Mark as being loaded.
        m_State = WorldState::LOADING;

        //Load the level data json file.
        const auto fileName = "worlds/" + m_Settings.name + "/" + LEVEL_DATA_FILE_NAME;
        if (utilities::FileUtilities::FileExists(fileName))
        {
            std::ifstream ifs(fileName);
            nlohmann::json json = nlohmann::json::parse(ifs);

            if(!JsonUtilities::VerifyValue("name", json, m_Settings.name) || m_Settings.name.empty())
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid name defined.");
                m_State = WorldState::UNLOADED;
                return false;
            }

            if (!JsonUtilities::VerifyValue("generator", json, m_Settings.generator) || m_Settings.generator.empty())
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid generator defined.");
                m_State = WorldState::UNLOADED;
                return false;
            }

            if (!JsonUtilities::VerifyValue("seed", json, m_Settings.seed))
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid seed defined.");
                m_State = WorldState::UNLOADED;
                return false;
            }

            if (!JsonUtilities::VerifyValue("renderDistance", json, m_Settings.renderDistance))
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid render distance defined.");
                m_State = WorldState::UNLOADED;
                return false;
            }
        }
        else
        {
            //No LevelData file was found. To solve this create a new one manually.
            //This is not automatically resolved because generator and game-mode missing may do damage to the world.
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid levelData file.");
            m_State = WorldState::UNLOADED;
            return false;
        }

        //Get the generator.
        m_Generator = a_Server.GetWorldGenerator(m_Settings.generator);
        if(m_Generator == nullptr)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "World '" + m_Settings.name + "' does not have a valid generator '" + m_Settings.generator + "'.");
            m_State = WorldState::UNLOADED;
            return false;
        }

        //Create the right types of voxel editor and chunk store.
        m_VoxelEditor = std::make_unique<VoxelEditor>();
        m_ChunkStore = std::make_unique<ChunkStore>();

        //Done!
        m_State = WorldState::RUNNING;
        utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Info, "Successfully loaded world '" + m_Settings.name + "'.");

        return true;
    }

    void World::Tick(double a_DeltaTime)
    {
        if (m_State != WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't tick world '" + m_Settings.name + "' that was not yet loaded.");
            return;
        }

        //Apply pending updates to the world.
        m_VoxelEditor->ApplyPendingChanges(*m_ChunkStore);

        //Spawn entities queued for spawning.
        for(auto& entity : m_EntityQueue)
        {
            AddEntity(std::move(entity));
        }
        m_EntityQueue.clear();

        //Update players in the world.
        auto player = m_Players.begin();
        while(player != m_Players.end())
        {
            if(player->second->IsMarkedForDestroy())
            {
                //TODO remove from chunk.
                player = m_Players.erase(player);
            }
            else
            {
                player->second->Tick(a_DeltaTime);
                ++player;
            }
        }

        //Update all entities.
        auto entity = m_Entities.begin();
        while (entity != m_Entities.end())
        {
            if (entity->second->IsMarkedForDestroy())
            {
                //TODO remove from chunks.
                entity = m_Entities.erase(entity);
            }
            else
            {
                entity->second->Tick(a_DeltaTime);
                ++entity;
            }
        }

        for(auto& chunk : *m_ChunkStore)
        {
            //TODO Check connected clients per chunk.
            //TODO Save chunks with no active connections.
            //TODO check every connection for every chunk to see if it's still valid.
            //TODO Check if still controlling an entity that is near the chunk (world render distance).
            //TODO if not remove the player.

            //Tick all chunks that are ready to be ticked.
            if(chunk.GetState() == ChunkState::READY)
            {
                //Tick the chunk.
                chunk.Tick(a_DeltaTime);
            }
        }
    }

    void World::SetWorldGenerator(std::shared_ptr<IWorldGenerator>& a_Generator)
    {
        m_Generator = a_Generator;
        m_Settings.generator = m_Generator->GetName();
    }

    IWorldGenerator& World::GetWorldGenerator()
    {
        if (m_State == WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve generator for world that was not yet loaded.");
            assert(0);
        }
        return *m_Generator;
    }

    IGameMode& World::GetGameMode()
    {
        if (m_State == WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve game-mode for world that was not yet loaded.");
            assert(0);
        }
        return *m_GameMode;
    }

    IChunkStore& World::GetChunkStore()
    {
        if (m_State == WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve chunk-store for world that was not yet loaded.");
            assert(0);
        }
        return *m_ChunkStore;
    }

    IVoxelEditor& World::GetVoxelEditor()
    {
        if (m_State == WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve voxel-editor for world that was not yet loaded.");
            assert(0);
        }
        return *m_VoxelEditor;
    }

    const WorldSettings& World::GetSettings() const
    {
        if (m_State == WorldState::UNLOADED)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve settings for world that was not yet loaded.");
            assert(0);
        }
        return m_Settings;
    }

    void World::SaveWorldSettings(const WorldSettings& a_Settings)
    {
        //Save the world settings as a Json file.
        nlohmann::json json;
        json["name"] = m_Settings.name;
        json["generator"] = m_Settings.generator;
        json["seed"] = m_Settings.seed;
        json["renderDistance"] = m_Settings.renderDistance;

        //Write to disk.
        const std::string path = "worlds/" + m_Settings.name + "/" + LEVEL_DATA_FILE_NAME;
        utilities::FileUtilities::CreateFile(path);
        std::ofstream dataFile(path);
        dataFile << json;
    }

    IEntity& World::AddEntity(std::unique_ptr<IEntity>&& a_Entity)
    {
        //TODO add to chunk.

        const auto id = a_Entity->GetUniqueId();
        const auto result = m_Entities.insert(std::make_pair(id, std::move(a_Entity)));
        return *(result.first->second);
    }

    IPlayer& World::AddPlayer(std::unique_ptr<IPlayer>&& a_Player)
    {
        const auto id = a_Player->GetUniqueId();
        const auto result = m_Players.insert(std::make_pair(id, std::move(a_Player)));
        return *(result.first->second);
    }

    void World::QueueEntityAdd(std::unique_ptr<IEntity>&& a_Entity)
    {
        m_EntityQueue.emplace_back(std::move(a_Entity));
    }

    IEntity* World::GetEntity(std::uint64_t a_Id)
    {
        const auto found = m_Entities.find(a_Id);
        if (found != m_Entities.end())
        {
            return found->second.get();
        }
        return nullptr;
    }

    IPlayer* World::GetPlayer(std::uint64_t a_Id)
    {
        const auto found = m_Players.find(a_Id);
        if(found != m_Players.end())
        {
            return found->second.get();
        }
        return nullptr;
    }

    WorldState World::GetWorldState() const
    {
        return m_State;
    }
}
