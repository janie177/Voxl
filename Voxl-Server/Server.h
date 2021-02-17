#pragma once
#include <map>
#include <IServer.h>

#include "VoxelRegistry.h"
#include "IWorld.h"
#include "IGameMode.h"
#include "IWorldGenerator.h"
#include "IConnectionManager.h"

namespace voxl
{
    class Server final : public IServer
    {
    public:
        Server();

        void Tick(double a_DeltaTime) override;
        void RegisterWorldGenerator(const std::string& a_Name, std::shared_ptr<IWorldGenerator>& a_Generator) override;
        VoxelRegistry& GetVoxelRegistry() override;
        void Start() override;
        IWorld* GetWorld(const std::string& a_Name) override;
        IWorld* LoadWorld(const std::string& a_Name) override;
        std::vector<IWorld*> GetWorlds() override;
        void ShutDown(bool a_SaveAll) override;
        bool UnloadWorld(const std::string& a_Name, bool a_Save) override;
        void RegisterGameMode(const std::string& a_Name, std::unique_ptr<IGameMode>&& a_GameMode) override;
        IWorld* CreateWorld(const WorldSettings& a_Settings) override;
        bool DeleteWorld(const std::string& a_Name) override;
        utilities::Logger& GetLogger() override;
        std::shared_ptr<IWorldGenerator> GetWorldGenerator(const std::string& a_Name) override;
        std::unique_ptr<IGameMode> CreateGameMode(const std::string& a_Name) override;
        const ServerSettings& GetServerSettings() const override;
        IConnectionManager& GetConnectionManager() override;
        bool WorldExists(const std::string& a_Name) override;
        ServerState GetState() override;

    private:
        void CreateLogger();
        void DoShutDown(bool a_SaveAll);

    public:
        bool LoadSettingsFile() override;
        void CreateSettingsFile(const ServerSettings& a_Settings) override;
        bool LoadVoxelTypesFile() override;
        void CreateVoxelTypesFile() override;
    private:
        ServerSettings m_Settings;
        ServerState m_State;
        ServerState m_RequestedState;
        bool m_RequestedStateSaveState;

        std::unique_ptr<utilities::Logger> m_Logger;

        //Registries containing stuff.
        std::map<std::string, std::unique_ptr<IWorld>> m_Worlds;
        std::map<std::string, std::shared_ptr<IWorldGenerator>> m_Generators;
        std::map<std::string, std::unique_ptr<IGameMode>> m_GameModes;
        std::unique_ptr<VoxelRegistry> m_VoxelRegistry;
        std::unique_ptr<IConnectionManager> m_ConnectionManager;
    };
}
