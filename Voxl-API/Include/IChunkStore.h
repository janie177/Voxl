#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>

#include "IChunk.h"

namespace voxl
{
    /*
     * ChunkStore stores chunks of the given type.
     * Functionality to iterate over the chunks is provided in the form of iterators.
     */
    class IChunkStore
    {
    public:
        //used to iterate over chunks.
        typedef IChunk* iterator;
        typedef const IChunk* const_iterator;

    public:
        virtual ~IChunkStore() = default;

        /*
         * Get the chunk at the given chunk coordinates.
         * If the chunk is not loaded, returns nullptr.
         */
        virtual IChunk* GetChunk(const glm::ivec3& a_Coordinates) = 0;

        /*
         * Store the given chunk in memory.
         */
        virtual IChunk* LoadChunk(std::unique_ptr<IChunk>&& a_Chunk) = 0;

        /*
         * Unload the chunk at the given coordinates from memory.
         * Returns true if the chunk was unloaded. False if not loaded in the first place.
         */
        virtual bool UnloadChunk(const glm::ivec3& a_Coordinates) = 0;

        /*
         * Unload all chunks in this chunk store.
         */
        virtual void UnloadAll() = 0;

        /*
         * Get the amount of chunks loaded in memory.
         */
        virtual size_t GetNumLoadedChunks() = 0;

    public:
        //Iterators to allow range based for loops to iterater over the internal data.
        virtual iterator begin() = 0;
        virtual iterator end() = 0;
        virtual const_iterator begin() const = 0;
        virtual const_iterator end() const = 0;
    };
}
