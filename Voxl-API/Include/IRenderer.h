#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "IWindow.h"

namespace voxl
{
    class IParticleEffect;
    class IChunkMesh;
    class IStaticMesh;
    class ISkeletalMesh;
    class IWindow;
    class Chunk;

    /*
     * Struct containing all configurable the settings used for rendering.
     */
    struct RenderSettings
    {
        WindowSettings windowSettings;
        bool vSync = false;
    };


    /*
     * Data required to construct a chunk mesh.
     */
    struct ChunkMeshSettings
    {
        Chunk* chunk = nullptr;
    };

    struct StaticMeshSettings
    {
        //TODO
    };

    struct SkeletalMeshSettings
    {
        //TODO
    };

    struct SkeletalMeshDrawData
    {
        //TODO
    };

    struct StaticMeshInstance
    {
        IStaticMesh* mesh = nullptr;
        glm::mat4* transforms = nullptr;
        size_t numInstances = 0;
    };

    struct StaticMeshDrawData
    {
        StaticMeshInstance* instances = nullptr;
        size_t numInstances = 0;
    };

    struct ChunkMeshDrawData
    {
        IChunkMesh* chunkMeshes = nullptr;
        size_t numChunks = 0;
    };

    class IRenderer
    {
    public:

        virtual ~IRenderer() = default;

        /*
         * Initialize the rendering system.
         */
        virtual void Init(const RenderSettings& a_Settings) = 0;

        /*
         * Resize the rendering resolution to the given window.
         */
        virtual void Resize(IWindow* a_Window) = 0;

        /*
         * Get the window used by the renderer.
         */
        virtual std::shared_ptr<IWindow> GetWindow() = 0;

        /*
         * Create a chunk mesh from the given data.
         */
        virtual std::shared_ptr<IChunkMesh> CreateChunkMesh(const ChunkMeshSettings& a_Data) = 0;

        /*
         * Create a static mesh from the given data.
         */
        virtual std::shared_ptr<IStaticMesh> CreateStaticMesh(const StaticMeshDrawData& a_Data) = 0;

        /*
         * Create a skeletal mesh from the given data.
         */
        virtual std::shared_ptr<ISkeletalMesh> CreateSkeletalMesh(const SkeletalMeshDrawData& a_Data) = 0;

        /*
         * Called before the next frame is rendered.
         * Called first in the chain.
         */
        virtual void PrepareForFrame() = 0;

        /*
         * Pass the chunks to be rendered to the renderer.
         */
        virtual void FrameData(const ChunkMeshDrawData& a_Data) = 0;

        /*
         * Pass the static meshes to be rendered for the next frame.
         */
        virtual void FrameData(const StaticMeshDrawData& a_Data) = 0;

        /*
         * Pass the skeletal meshes to be rendered for the next frame.
         */
        virtual void FrameData(const SkeletalMeshDrawData& a_Data) = 0;

        /*
         * Finalize the drawing of the frame.
         * Called last in the chain.
         */
        virtual void DrawFrame() = 0;
    };
}
