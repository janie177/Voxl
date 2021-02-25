#include "Game.h"
#include <IGameMode.h>
#include <IWorld.h>

namespace voxl
{
    Game::Game(const std::string& a_Name)
    {
        m_Settings.gameName = a_Name;
    }

    std::shared_ptr<IGameMode> Game::GetGameMode()
    {
        return m_GameMode;
    }

    void Game::Tick(float a_DeltaTime)
    {
        //Tick all worlds that are currently not saving or loading.
        for(auto& world : m_Worlds)
        {
            if(world.second->GetWorldState() == WorldState::RUNNING)
            {
                world.second->Tick(a_DeltaTime);
            }
        }
    }

    void Game::Save()
    {
        m_GameMode->Save();
    }

    void Game::Unload()
    {
        m_GameMode->Unload();
    }

    std::string Game::GetName()
    {
        return m_Name;
    }

    bool Game::LoadSettingsFile(GameSettings& a_Settings)
    {
        //TODO load the settings file from the gamemodes/m_Name/ folder.

        return true;
    }

    bool Game::CreateSettingsFile(const GameSettings& a_Settings)
    {
        //TODO create the settings in gamemodes/m_Name/ folder.
        return true;
    }

    IWorld* Game::GetWorld(const std::string& a_Name)
    {
        auto found = m_Worlds.find(a_Name);
        if(found != m_Worlds.end())
        {
            return found->second.get();
        }
        return nullptr;
    }

    bool Game::UnloadWorld(const std::string& a_Name, bool a_Save)
    {
        //TODO return true if unloaded.
        return true;
    }

    IWorld* Game::LoadWorld(const std::string& a_Name)
    {
        //TODO see if world isn't already loaded and load if exists.
        return nullptr;
    }

    bool Game::WorldExists(const std::string& a_Name)
    {
        //TODO check if world exists.
        return true;
    }

    IWorld* Game::CreateWorld(const WorldSettings& a_Settings)
    {
        //TODO create world with name if doesn't exist.
        return nullptr;
    }

    bool Game::DeleteWorld(const std::string& a_Name)
    {
        //TODO delete world if it exists.
        //TODO unload if it is loaded.
        return true;
    }

    void Game::AddPlayer(IClientConnection& a_Connection)
    {
        m_GameMode->CreatePlayer(a_Connection);
    }

    void Game::RemovePlayer(IClientConnection& a_Connection)
    {
        m_GameMode->RemovePlayer(a_Connection);
    }

    std::vector<IWorld*> Game::GetWorlds()
    {
        std::vector<IWorld*> worlds;
        worlds.reserve(m_Worlds.size());
        for(auto& world : m_Worlds)
        {
            worlds.push_back(world.second.get());
        }
        return worlds;
    }

    bool Game::Load()
    {
        //TODO initialize this game. Load settings and worlds from file.

        m_GameMode->Load();
    }
}
