#pragma once
#include <IWorldGenerator.h>

namespace voxl
{
    class DefaultWorldGenerator : public IWorldGenerator
    {
    public:
        void Generate(const std::uint64_t a_Seed, IChunk& a_Chunk) override;
        void Populate(const std::uint64_t a_Seed, IChunk& a_Chunk) override;
        std::string GetName() const override;
    };
}