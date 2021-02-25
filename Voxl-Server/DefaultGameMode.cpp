#include "DefaultGameMode.h"
#include "Game.h"
#include "World.h"

namespace voxl
{
    void DefaultGameMode::Save()
    {
        //This gamemode saves all worlds.
        for (auto& world : m_Game->GetWorlds())
        {
            if (world->GetWorldState() == WorldState::RUNNING)
            {
                world->Save();
            }
        }
    }

    std::string DefaultGameMode::GetName() const
    {
        //TODO
        return "";
    }

    void DefaultGameMode::Unload()
    {

    }
}
