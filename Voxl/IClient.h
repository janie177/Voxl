#pragma once

namespace voxl
{
    class IChunkStore;
    class IRenderer;

    class IClient
    {
    public:
        virtual ~IClient() = 0;

        /*
         * Currently loaded chunks are stored here.
         */
        virtual IChunkStore& GetChunkStore() = 0;

        /*
         * The renderer.
         */
        virtual IRenderer& GetRenderer() = 0;

        /*
         * The game's entry point.
         */
        virtual void Run() = 0;
    };
}