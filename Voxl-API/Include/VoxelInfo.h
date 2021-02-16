#pragma once
#include <cinttypes>
#include <string>

namespace voxl
{
    /*
     * Information about a voxel type. This data is static and defined at compile time.
     */
    struct VoxelInfo
    {
        std::string name = "Kevin";             //The name of the voxel.
        std::uint16_t id = 0;                   //The ID of the block.
        bool collision = true;                  //True if collision is enabled and blocks objects from entering.
        std::uint8_t passThroughSpeed = 255;    //The speed at which one can pass through this block's substance if collision is disabled. 0 is the slowest and 255 is the fastest.
        std::uint8_t strength = 1;              //The strength of this block which determines how fast it breaks when under stress.
        std::uint8_t emissiveLight = 0;         //The amount of light emitted by this block. When 0, no light is emitted and when 255 the most light is emitted.

        //Graphics related settings used to display this voxel client-sided.
        struct
        {
            bool mesh = false;                  //True if this voxel is a 3D mesh and not a cubic block.
            int textureIndex = 0;               //The texture index for this voxel in the texture atlas.
            std::uint32_t animationFrames = 0;  //When not 0, defined the amount of frames of animation in the texture atlas. When 0 no animation happens.
            bool transparent = false;           //True if this block has partially translucent textures.
        } graphics;
    };
}