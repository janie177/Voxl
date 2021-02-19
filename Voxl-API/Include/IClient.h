#pragma once
#include "IRenderer.h"

namespace voxl
{
    class IServerConnection;
    class IChunkStore;
    class VoxelRegistry;

    struct ClientSettings
    {
        //Rendering related settings.
        RenderSettings renderSettings;

        std::uint32_t tps = 30;
        std::uint32_t fps = 120;

        //Server to connect to.
        std::string ip = "127.0.0.1";
        std::uint32_t port = 28280;

        //User settings
        std::string userName = "Kevin";
    };

    class IClient
    {
    public:
        virtual ~IClient() = default;

        /*
         * Currently loaded chunks are stored here.
         */
        virtual IChunkStore& GetChunkStore() = 0;

        /*
         * The renderer.
         */
        virtual IRenderer& GetRenderer() = 0;

        /*
         * Get the connection handler.
         */
        virtual IServerConnection& GetConnection() = 0;

        /*
         * Get the voxel registry.
         */
        virtual VoxelRegistry& GetVoxelRegistry() = 0;

        /*
         * The clients entry point.
         */
        virtual void Start() = 0;

        /*
         * Load client settings into the given object.
         */
        virtual bool LoadSettings(ClientSettings& a_Settings) = 0;

        /*
         * Save the given client settings to file.
         */
        virtual bool SaveSettings(const ClientSettings& a_Settings) = 0;
    };
}
