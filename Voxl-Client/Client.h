#pragma once
#include <IClient.h>

#include <IChunkStore.h>
#include <IRenderer.h>
#include <VoxelRegistry.h>
#include "ServerConnection.h"

namespace voxl
{
    class Client : public IClient
    {
    public:
        IChunkStore& GetChunkStore() override;
        IRenderer& GetRenderer() override;
        IServerConnection& GetConnection() override;
        VoxelRegistry& GetVoxelRegistry() override;

        void Start() override;
        void LoadSettings(ClientSettings& a_Settings) override;
        void SaveSettings(const ClientSettings& a_Settings) override;
    private:
        bool m_Running;
        std::unique_ptr<IChunkStore> m_ChunkStore;
        std::unique_ptr<IRenderer> m_Renderer;
        std::unique_ptr<ServerConnection> m_ServerConnection;
        std::unique_ptr<VoxelRegistry> m_VoxelRegistry;

        ClientSettings m_Settings;
    };
}