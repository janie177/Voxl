#pragma once

namespace voxl
{
    class IServerConnection;
    class IChunkStore;
    class IRenderer;

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
         * The clients entry point.
         */
        virtual void Run() = 0;
    };
}