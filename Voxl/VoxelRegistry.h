#pragma once
#include <cinttypes>
#include <vector>
#include <cassert>

#include "VoxelInfo.h"

namespace voxl
{
    /*
     * VoxelRegistry keeps track of all the types of voxels.
     */
    class VoxelRegistry
    {
    public:
        /*
         * Construct a voxel registry with enough space to store a_MaximumEntries voxel types.
         */
        VoxelRegistry(const std::uint32_t a_MaximumEntries) : m_MaximumEntries(a_MaximumEntries)
        {
            m_VoxelTypes.resize(a_MaximumEntries);
        }

        template<typename T>
        inline const VoxelInfo& GetVoxelInfo(T a_Id)
        {
            static_assert(std::is_integral_v<T>, "Identifier used to look up a voxeltype has to be integer type.");
            assert(a_Id < m_MaximumEntries && "ID out of bounds!");

            const auto& info = m_VoxelTypes[static_cast<size_t>(a_Id)];
            assert(info.id == a_Id && "Unregistered voxel type!");

            return info;
        }

        inline bool Register(const VoxelInfo& a_Information)
        {
            assert(a_Information.id < m_MaximumEntries && "Voxel registry isn't large enough to store an ID that high.");

            if (a_Information.id >= m_MaximumEntries)
            {
                throw std::exception("Not enough space in voxel registry to register an ID that high.");
                return false;
            }

            m_VoxelTypes.emplace_back(a_Information);
            return true;
        }

    private:
        //Registry containing the voxels.
        std::vector<VoxelInfo> m_VoxelTypes;
        std::uint32_t m_MaximumEntries;
    };
}