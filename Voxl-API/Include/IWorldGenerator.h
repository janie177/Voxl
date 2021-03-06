#pragma once
#include <glm/vec3.hpp>
#include <string>

namespace voxl
{
    class IChunk;

    /*
     * Interface used for world generation.
     * The world generators should be stateless as they can be shared by multiple worlds.
     */
    class IWorldGenerator
    {
    public:

        virtual ~IWorldGenerator() = default;

        /*
         * Generate a chunk using the seed and its coordinates.
         */
        virtual void Generate(const std::uint64_t a_Seed, IChunk& a_Chunk) = 0;

        /*
         * Populate an already generated chunk using a seed.
         */
        virtual void Populate(const std::uint64_t a_Seed, IChunk& a_Chunk) = 0;

        /*
         * Get the name of this world generator.
         */
        virtual std::string GetName() const = 0;
    };
}
