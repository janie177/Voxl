#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "IPacketHandler.h"
#include "PacketType.h"
#include "VoxelRegistry.h"
#include "JsonUtilities.h"

namespace voxl
{
    class ConnectionManager;

    enum class VoxelInfoReceiveState
    {
        WAITING,    //Waiting for a response.
        PROCESSING, //Responce received, processing now.
        SUCCESS,    //Successfully loaded.
        FAILED      //Failed.
    };

    class PacketHandler_VoxelInfo : public PacketHandler<Packet_VoxelInfo>
    {
    public:
        PacketHandler_VoxelInfo(std::unique_ptr<VoxelRegistry>& a_Registry) : m_Registry(a_Registry)
        {
            m_State = VoxelInfoReceiveState::WAITING;
        }

        bool OnResolve(Packet_VoxelInfo& a_Data, IConnection* a_Sender) override
        {
            m_State = VoxelInfoReceiveState::PROCESSING;

            char* start = reinterpret_cast<char*>(&a_Data) + sizeof(Packet_VoxelInfo);
            char* end = start + a_Data.size;

            //Iterate over the array.
            nlohmann::json file = nlohmann::json::parse(start, end, nullptr, false, false);

            if(m_Registry != nullptr)
            {
                std::cout << "Error: Voxel registry already initialized while handling VoxelInfo packet." << std::endl;
                m_State = VoxelInfoReceiveState::FAILED;
                return false;
            }

            nlohmann::basic_json<> voxelTypes;
            if (file.is_discarded() || !JsonUtilities::VerifyValue("voxelTypes", file, voxelTypes))
            {
                std::cout << "Cannot verify VoxelInfo data packet. Warped Json?" << std::endl;
                m_State = VoxelInfoReceiveState::FAILED;
                return false;
            }

            //Create the registry with enough space to fit every voxel type.
            m_Registry = std::make_unique<VoxelRegistry>(voxelTypes.size());

            //Loop over every item registered.
            for (auto const& value : voxelTypes)
            {
                //Start with the default settings.
                VoxelInfo info;

                //Load every element. When an element is not found it simply remains the default.
                JsonUtilities::VerifyValue("name", value, info.name);
                JsonUtilities::VerifyValue("description", value, info.description);
                JsonUtilities::VerifyValue("id", value, info.id);
                JsonUtilities::VerifyValue("collision", value, info.collision);
                JsonUtilities::VerifyValue("emissiveLight", value, info.emissiveLight);
                JsonUtilities::VerifyValue("passThroughSpeed", value, info.passThroughSpeed);
                JsonUtilities::VerifyValue("strength", value, info.strength);

                JsonUtilities::VerifyValue("animationFrames", value, info.graphics.animationFrames);
                JsonUtilities::VerifyValue("mesh", value, info.graphics.mesh);
                JsonUtilities::VerifyValue("textureIndex", value, info.graphics.textureIndex);
                JsonUtilities::VerifyValue("transparent", value, info.graphics.transparent);

                //Register with the registry.
                m_Registry->Register(info);
            }

            
            m_State = VoxelInfoReceiveState::SUCCESS;
            return true;
        }

        VoxelInfoReceiveState GetState()
        {
            return m_State;
        }

    private:
        //Reference to the voxel registry.
        std::unique_ptr<VoxelRegistry>& m_Registry;
        VoxelInfoReceiveState m_State;
        
    };
}
