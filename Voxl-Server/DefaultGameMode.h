#pragma once
#include <IGameMode.h>

namespace voxl
{
    class Game;

    class DefaultGameMode : public IGameMode
    {
    public:
        void Tick(double a_DeltaTime) override;
        void Save() override;
        void Load() override;
        std::string GetName() const override;
        std::shared_ptr<IGameMode> Clone(IGame& a_Game) override;
        ClientRules GetRules(IClientConnection& a_Client) override;
        VoxelRegistry& GetVoxelRegistry() override;
        EntityRegistry& GetEntityRegistry() override;
        void CreatePlayer(IClientConnection& a_Client) override;
        void RemovePlayer(IClientConnection& a_Client) override;
        IGame& GetGame() override;
        bool LoadVoxelTypesFile() override;
        void CreateVoxelTypesFile() override;
        void Unload() override;
    private:
        std::shared_ptr<Game> m_Game;
    };
}