#pragma once
#include <IGame.h>
#include <unordered_map>

namespace voxl
{
    class Game : public IGame
    {
    public:
        explicit Game(const std::string& a_Name);

        std::shared_ptr<IGameMode> GetGameMode() override;
        void Tick(float a_DeltaTime) override;
        void Save() override;
        void Unload() override;
        std::string GetName() override;
        bool LoadSettingsFile(GameSettings& a_Settings) override;
        bool CreateSettingsFile(const GameSettings& a_Settings) override;
        IWorld* GetWorld(const std::string& a_Name) override;
        bool UnloadWorld(const std::string& a_Name, bool a_Save) override;
        IWorld* LoadWorld(const std::string& a_Name) override;
        bool WorldExists(const std::string& a_Name) override;
        IWorld* CreateWorld(const WorldSettings& a_Settings) override;
        bool DeleteWorld(const std::string& a_Name) override;
        void AddPlayer(IClientConnection& a_Connection) override;
        void RemovePlayer(IClientConnection& a_Connection) override;
        std::vector<IWorld*> GetWorlds() override;
        bool Load() override;
    private:
        GameSettings m_Settings;
        std::shared_ptr<IGameMode> m_GameMode;
        std::string m_Name;
        std::unordered_map<std::string, std::unique_ptr<IWorld>> m_Worlds;
    };
}
