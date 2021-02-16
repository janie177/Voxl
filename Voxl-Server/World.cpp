#include "World.h"

#include <IGameMode.h>
#include <IChunkStore.h>
#include <IWorldGenerator.h>
#include <IVoxelEditor.h>
#include <IServer.h>

#include <cassert>

#include <logging/Logger.h>
#include <other/ServiceLocator.h>


#include "ChunkStore.h"
#include "file/FileUtilities.h"
#include "JsonUtilities.h"
#include "VoxelEditor.h"

namespace voxl
{

    World::World(const std::string& a_Name, const std::string& a_WorldDirectory) : m_Loaded(false), m_WorldDirectory(a_WorldDirectory)
    {
        m_Settings.name = a_Name;
    }

    const std::string& World::GetName()
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't retrieve name of world that was not yet loaded.");
        }
        return m_Settings.name;
    }

    void World::Save()
    {
        if(!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't save world that was not yet loaded.");
            return;
        }

        //Save the gamemode data.
        m_GameMode->Save(*this);

        //Only save the world if enabled.
        if(!m_Settings.enableSaving)
        {
            return;
        }

        //TODO save all players to their player directory m_Settings.playerDirectory.

        SaveWorldSettings(m_Settings);

        //Save each chunk as well.
        for (auto& chunk : *m_ChunkStore)
        {
            chunk.Save(*this);
        }
    }

    void World::Unload()
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't unload a world that was not yet loaded.");
            return;
        }

        //TODO move all players away to another world.

        //Unload all the chunks.
        m_ChunkStore->UnloadAll();
    }

    bool World::Load(IServer& a_Server)
    {
        //Load the level data json file.
        const auto fileName = m_WorldDirectory + "/" + m_Settings.name + "/" + LEVEL_DATA_FILE_NAME;
        if (utilities::FileUtilities::FileExists(fileName))
        {
            std::ifstream ifs(fileName);
            nlohmann::json json = nlohmann::json::parse(ifs);

            if(!JsonUtilities::VerifyValue("name", json, m_Settings.name) || m_Settings.name.empty())
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid name defined.");
                return false;
            }

            if (!JsonUtilities::VerifyValue("generator", json, m_Settings.generator) || m_Settings.generator.empty())
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid generator defined.");
                return false;
            }

            if (!JsonUtilities::VerifyValue("playersDirectory", json, m_Settings.playersDirectory) || m_Settings.playersDirectory.empty())
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid players directory defined.");
                return false;
            }

            if (!JsonUtilities::VerifyValue("seed", json, m_Settings.seed))
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid seed defined.");
                return false;
            }

            if (!JsonUtilities::VerifyValue("gamemode", json, m_Settings.gameMode) || m_Settings.gameMode.empty())
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid game-mode defined.");
                return false;
            }

            if (!JsonUtilities::VerifyValue("enableSaving", json, m_Settings.enableSaving))
            {
                utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid saving boolean defined.");
                return false;
            }
        }
        else
        {
            //No LevelData file was found. To solve this create a new one manually.
            //This is not automatically resolved because generator and game-mode missing may do damage to the world.
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Tried to load world '" + m_Settings.name + "' that does not have a valid levelData file.");
            return false;
        }

        //Load data the game-mode may require.
        m_GameMode = a_Server.CreateGameMode(m_Settings.gameMode);
        if (m_GameMode == nullptr)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "World '" + m_Settings.name + "' does not have a valid game-mode '" + m_Settings.gameMode + "'.");
            return false;
        }
        m_GameMode->Load(*this);

        //Get the generator.
        m_Generator = a_Server.GetWorldGenerator(m_Settings.generator);
        if(m_Generator == nullptr)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "World '" + m_Settings.name + "' does not have a valid generator '" + m_Settings.generator + "'.");
            return false;
        }

        //Create the right types of voxel editor and chunk store.
        m_VoxelEditor = std::make_unique<VoxelEditor>();
        m_ChunkStore = std::make_unique<ChunkStore>();

        //Done!
        m_Loaded = true;
        utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Info, "Successfully loaded world '" + m_Settings.name + "'.");

        return true;
    }

    void World::Tick(double a_DeltaTime)
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Error, "Can't tick world '" + m_Settings.name + "' that was not yet loaded.");
            return;
        }

        //Apply pending updates to the world.
        m_VoxelEditor->ApplyPendingChanges(*m_ChunkStore);

        //TODO update players in the world.

        for(auto& chunk : *m_ChunkStore)
        {
            //TODO ensure that chunks are ready and not generating or anything. Chunk loading is already handled by the incoming packets from players.
            //TODO check connected clients per chunk, and update them based on distance.
            //TODO then save and unload chunks that are no longer active.

            //TODO update entities and other logic if chunk is still loaded.
            
        }

        m_GameMode->Tick(a_DeltaTime, *this);
    }

    void World::SetGameMode(std::unique_ptr<IGameMode>&& a_GameMode)
    {
        m_GameMode->Save(*this);
        m_GameMode = std::move(a_GameMode);
        m_GameMode->Load(*this);
        m_Settings.gameMode = m_GameMode->GetName();
    }

    void World::SetWorldGenerator(std::shared_ptr<IWorldGenerator>& a_Generator)
    {
        m_Generator = a_Generator;
        m_Settings.generator = m_Generator->GetName();
    }

    IWorldGenerator& World::GetWorldGenerator()
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve generator for world that was not yet loaded.");
        }
        assert(m_Loaded);
        return *m_Generator;
    }

    IGameMode& World::GetGameMode()
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve game-mode for world that was not yet loaded.");
        }
        assert(m_Loaded);
        return *m_GameMode;
    }

    IChunkStore& World::GetChunkStore()
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve chunk-store for world that was not yet loaded.");
        }
        assert(m_Loaded);
        return *m_ChunkStore;
    }

    IVoxelEditor& World::GetVoxelEditor()
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve voxel-editor for world that was not yet loaded.");
        }
        assert(m_Loaded);
        return *m_VoxelEditor;
    }

    const WorldSettings& World::GetSettings() const
    {
        if (!m_Loaded)
        {
            utilities::ServiceLocator<utilities::Logger>::getService().log(utilities::Severity::Fatal, "Can't retrieve settings for world that was not yet loaded.");
        }
        assert(m_Loaded);
        return m_Settings;
    }

    void World::SaveWorldSettings(const WorldSettings& a_Settings)
    {
        //Save the world settings as a Json file.
        nlohmann::json json;
        json["name"] = m_Settings.name;
        json["generator"] = m_Settings.generator;
        json["playersDirectory"] = m_Settings.playersDirectory;
        json["seed"] = m_Settings.seed;
        json["gamemode"] = m_Settings.gameMode;
        json["enableSaving"] = m_Settings.enableSaving;

        //Write to disk.
        const std::string path = m_WorldDirectory + "/" + m_Settings.name + "/" + LEVEL_DATA_FILE_NAME;
        utilities::FileUtilities::CreateFile(path);
        std::ofstream dataFile(path);
        dataFile << json;
    }
}
