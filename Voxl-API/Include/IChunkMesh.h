#pragma once

namespace voxl
{
    class IChunk;

    enum class ChunkMeshState
    {
        GENERATING,
        READY
    };

    /*
     * Interface for chunk meshes.
     * These are used client-sided to generate all meshes required to display a chunk.
     * Solid and transparent meshes have to be separately generated.
     */
    class IChunkMesh
    {
    public:

        virtual ~IChunkMesh() = default;

        /*
         * Returns true when this chunk mesh has finished loading.
         */
        virtual bool IsLoaded() = 0;

        /*
         * Generate a chunk mesh for the given chunk.
         */
        virtual void Generate(IChunk* a_Chunk) = 0;

        /*
         * Get the state of the chunk mesh.
         */
        virtual ChunkMeshState GetState() = 0;

        /*
         * Delete the chunk mesh from memory.
         */
        virtual void Destroy() = 0;
    };
}