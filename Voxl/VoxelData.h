#pragma once
#include <cinttypes>

namespace voxl
{
   /*
    * Voxel data as stored in the world.
    */
    struct VoxelData
    {
        std::uint16_t id = 0;           //The ID of the block type at this voxel.
        std::uint8_t metaData = 0;      //The metadata of the voxel.
        std::uint8_t lightLevel = 0;    //The light level at this voxel.
    };
}