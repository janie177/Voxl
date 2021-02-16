#pragma once
#include "IWorld.h"
#include <nlohmann/json.hpp>

#define LEVEL_DATA_FILE_NAME "leveldata.json"

namespace voxl
{
	class World : public IWorld
	{
    public:
        World(const std::string& a_Name, const std::string& a_WorldDirectory);

        const std::string& GetName() override;
        void Save() override;
        void Unload() override;
        bool Load(IServer& a_Server) override;
        void Tick(double a_DeltaTime) override;
        void SetGameMode(std::unique_ptr<IGameMode>&& a_GameMode) override;
        void SetWorldGenerator(std::shared_ptr<IWorldGenerator>& a_Generator) override;
        IWorldGenerator& GetWorldGenerator() override;
        IGameMode& GetGameMode() override;
        IChunkStore& GetChunkStore() override;
        IVoxelEditor& GetVoxelEditor() override;
        const WorldSettings& GetSettings() const override;
        void SaveWorldSettings(const WorldSettings& a_Settings) override;
	private:
        bool m_Loaded;
        WorldSettings m_Settings;

        std::unique_ptr<IVoxelEditor> m_VoxelEditor;
        std::unique_ptr<IChunkStore> m_ChunkStore;
        std::unique_ptr<IGameMode> m_GameMode;
        std::shared_ptr<IWorldGenerator> m_Generator;
        std::string m_WorldDirectory;
	};

}

