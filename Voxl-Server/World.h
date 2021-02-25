#pragma once
#include <IWorld.h>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "Player.h"

#define LEVEL_DATA_FILE_NAME "leveldata.json"

namespace voxl
{
    class IClientConnection;

    class World : public IWorld
	{
    public:
        World(const std::string& a_Name);

        const std::string& GetName() override;
        void Save() override;
        void Unload() override;
        bool Load(IServer& a_Server) override;
        void Tick(double a_DeltaTime) override;
        void SetWorldGenerator(std::shared_ptr<IWorldGenerator>& a_Generator) override;
        IWorldGenerator& GetWorldGenerator() override;
        IGameMode& GetGameMode() override;
        IChunkStore& GetChunkStore() override;
        IVoxelEditor& GetVoxelEditor() override;
        const WorldSettings& GetSettings() const override;
        void SaveWorldSettings(const WorldSettings& a_Settings) override;
        IEntity& AddEntity(std::unique_ptr<IEntity>&& a_Entity) override;
        IPlayer& AddPlayer(std::unique_ptr<IPlayer>&& a_Player) override;
        void QueueEntityAdd(std::unique_ptr<IEntity>&& a_Entity) override;
        IEntity* GetEntity(std::uint64_t a_Id) override;
        IPlayer* GetPlayer(std::uint64_t a_Id) override;
        WorldState GetWorldState() const override;
	private:
        WorldSettings m_Settings;
        WorldState m_State;

        std::unique_ptr<IVoxelEditor> m_VoxelEditor;
        std::unique_ptr<IChunkStore> m_ChunkStore;
        std::shared_ptr<IWorldGenerator> m_Generator;
        std::shared_ptr<IGameMode> m_GameMode;

        //Entities in the world.
        std::unordered_map<std::uint64_t, std::unique_ptr<IEntity>> m_Entities;
        std::unordered_map<std::uint64_t, std::unique_ptr<IPlayer>> m_Players;

        //Entities waiting to be added when the next game cycle happens.
        std::vector<std::unique_ptr<IEntity>> m_EntityQueue;
	};

}

