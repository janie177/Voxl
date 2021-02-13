#pragma once
#include <cinttypes>

namespace voxl
{
    /*
     * Information about a voxel type. This data is static and defined at compile time.
     */
    struct VoxelInfo
    {
        std::uint16_t id;               //The ID of the block.
        bool transparent;               //True if this block has partially translucent textures.
        bool collision;                 //True if collision is enabled and blocks objects from entering.
        std::uint8_t passThroughSpeed;  //The speed at which one can pass through this block's substance if collision is disabled. 0 is the slowest and 255 is the fastest.
        std::uint8_t strength;          //The strength of this block which determines how fast it breaks when under stress.
        std::uint8_t emissiveLight;     //The amount of light emitted by this block. When 0, no light is emitted and when 255 the most light is emitted.
    };
}