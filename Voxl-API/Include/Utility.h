#pragma once
#include <cinttypes>
#include <cassert>
#include <glm/glm.hpp>

/*
 * Defines used in the API.
 */
constexpr int CHUNK_SIZE = 16;       //The size of a chunk in voxels in all dimensions.
constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;

namespace voxl
{
    /*
     * Convert X, Y and Z coordinates to an array index for dynamic chunk indexing.
     * The coordinates are local to the block within the chunk.
     */
    constexpr inline std::uint32_t GetVoxelIndex(const glm::uvec3& a_Coordinates)
    {
        assert(a_Coordinates.z < CHUNK_SIZE && a_Coordinates.y < CHUNK_SIZE && a_Coordinates.x < CHUNK_SIZE);
        return (a_Coordinates.z * CHUNK_SIZE) + (a_Coordinates.y * CHUNK_SIZE_SQUARED) + a_Coordinates.x;
    }

    /*
     * Convert a 1D array index into 3D coordinates.
     * This returns the coordinates local to the chunk.
     */
    constexpr inline glm::uvec3 GetVoxelCoordinates(std::uint32_t a_VoxelIndex)
    {
        glm::uvec3 coords{};
        coords.x = a_VoxelIndex % CHUNK_SIZE;
        coords.y = a_VoxelIndex / (CHUNK_SIZE_SQUARED);
        coords.z = (a_VoxelIndex / CHUNK_SIZE) % CHUNK_SIZE;
        return coords;
    }

    /*
     * Convert a chunk position to world coordinates.
     */
    constexpr inline glm::vec3 ChunkToWorld(const glm::ivec3& a_Coordinates)
    {
        return glm::vec3(a_Coordinates.x * CHUNK_SIZE, a_Coordinates.y * CHUNK_SIZE, a_Coordinates.z * CHUNK_SIZE);
    }

    /*
     * Convert world coordinates to chunk coordinates. Block specific information is lost.
     */
    constexpr inline glm::uvec3 WorldToChunk(const glm::vec3& a_Coordinates)
    {
        glm::ivec3 chunk{};
        chunk.x = static_cast<std::int32_t>(a_Coordinates.x < 0 ? ((a_Coordinates.x - (CHUNK_SIZE - 1)) / static_cast<std::int32_t>(CHUNK_SIZE)) : (a_Coordinates.x / static_cast<std::int32_t>(CHUNK_SIZE)));
        chunk.y = static_cast<std::int32_t>(a_Coordinates.y < 0 ? ((a_Coordinates.y - (CHUNK_SIZE - 1)) / static_cast<std::int32_t>(CHUNK_SIZE)) : (a_Coordinates.y / static_cast<std::int32_t>(CHUNK_SIZE)));
        chunk.z = static_cast<std::int32_t>(a_Coordinates.z < 0 ? ((a_Coordinates.z - (CHUNK_SIZE - 1)) / static_cast<std::int32_t>(CHUNK_SIZE)) : (a_Coordinates.z / static_cast<std::int32_t>(CHUNK_SIZE)));
        return chunk;
    }

    /*
     * Retrieve the chunk and block coordinates from a world position.
     * The chunk coordinates are stored in a_Chunk.
     * The block coordinates local to the chunk are stored in a_Block.
     */
    constexpr inline void WorldToVoxel(const glm::vec3& a_Coordinates, glm::ivec3& a_Chunk, glm::uvec3& a_Block)
    {
        //Calculate the coordinates of the chunk the block resides is.
        a_Chunk = WorldToChunk(a_Coordinates);

        //Calculate the coordinates of the block locally in the chunk.
        a_Block.x = static_cast<std::int32_t>(a_Coordinates.x) - (a_Chunk.x * static_cast<std::int32_t>(CHUNK_SIZE));
        a_Block.y = static_cast<std::int32_t>(a_Coordinates.y) - (a_Chunk.y * static_cast<std::int32_t>(CHUNK_SIZE));
        a_Block.z = static_cast<std::int32_t>(a_Coordinates.z) - (a_Chunk.z * static_cast<std::int32_t>(CHUNK_SIZE));
    }
}