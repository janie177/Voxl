#include "Client.h"

#include <iostream>
#include <time/GameLoop.h>
#include <VoxelRegistry.h>

#include "ClientChunkStore.h"
#include "input/InputQueue.h"
#include "Renderer.h"
#include "ServerConnection.h"
#include "ClientPacketManager.h"
#include "PacketHandler_IncomingMessage.h"

namespace voxl
{
    IChunkStore& Client::GetChunkStore()
    {
        assert(m_Running);

        return *m_ChunkStore;
    }

    IRenderer& Client::GetRenderer()
    {
        assert(m_Running);
        return *m_Renderer;
    }

    IServerConnection& Client::GetConnection()
    {
        assert(m_Running);
        return *m_ServerConnection;
    }

    void Client::Start()
    {
        //Load the settings.
        LoadSettings(m_Settings);

        //Initialize systems.
        m_ChunkStore = std::make_unique<ClientChunkStore>();
        m_Renderer = std::make_unique<Renderer>();

        //Initialize the connection and connect to the server.
        Packet_Authenticate authentication;
        strcpy_s(authentication.name, m_Settings.userName.length() + 1, m_Settings.userName.c_str());
        m_ServerConnection = std::make_unique<ServerConnection>(5000);

        if(!m_ServerConnection->Setup())
        {
            std::cout << "Could not set up client connection." << std::endl;
            return;
        }

        if(!m_ServerConnection->Connect(m_Settings.ip, m_Settings.port, authentication))
        {
            std::cout << "Could not connect to remote server at " << m_Settings.ip << ":" << std::to_string(m_Settings.port) << std::endl;
            return;
        }

        //Wait for connection to either succeed or fail.
        while(true)
        {
            auto state = m_ServerConnection->GetConnectionState();
            if(state != ConnectionState::CONNECTING)
            {
                if(state == ConnectionState::DISCONNECTED)
                {
                    std::cout << "Could not connect to server." << std::endl;
                    return;
                }
                else
                {
                    std::cout << "Connected to server." << std::endl;
                }
                
                break;
            }
        }

        //Register the classes to handle certain packets.
        m_ServerConnection->GetPacketManager().Register(PacketType::CHAT_MESSAGE, std::make_unique<PacketHandler_IncomingMessage>());

        //TODO
        //Retrieve the voxel registry from the server and load it client-sided.
        //m_VoxelRegistry = std::make_unique<VoxelRegistry>();



        //Start a game loop with the settings FPS and TPS.
        utilities::GameLoop loop{ 144, 30 };
        utilities::InputQueue input;

        long counter = 0;

        m_Running = true;
        while(m_Running)
        {
            auto data = loop.update();

            if(data.tick)
            {
                ////Retrieve input data.
                ////TODO move to window class to listen for actual input.
                //auto inputData = input.getQueuedEvents();
                //utilities::MouseEvent mEvent;
                //utilities::KeyboardEvent kEvent;

                ////Process mouse input.
                //while(inputData.getNextEvent(mEvent))
                //{
                //    
                //}

                ////Process keyboard input.
                //while(inputData.getNextEvent(kEvent))
                //{
                //    
                //}

                //Process incoming packets.
                m_ServerConnection->ProcessPackets();

                ++counter;

                //Send a test packet.
                if(counter % 100 == 0)
                {
                    std::cout << "Sending chat package." << std::endl;
                    Packet_ChatMessage message;
                    message.timeStamp = 0;
                    std::string msg = "Hello! This is a test message broadcast every few seconds.";
                    strcpy_s(message.message, msg.length() + 1, msg.c_str());
                    m_ServerConnection->SendTypedPacket(message);
                    counter = 0;
                }

                //TODO
                //Tick the rest of the game.
            }

            if(data.frame)
            {
                //TODO render
            }
        }

        //TODO deinitialize systems.
        m_ServerConnection->Shutdown();
    }

    VoxelRegistry& Client::GetVoxelRegistry()
    {
        assert(m_Running);
        return *m_VoxelRegistry;
    }

    void Client::LoadSettings(ClientSettings& a_Settings)
    {

    }

    void Client::SaveSettings(const ClientSettings& a_Settings)
    {

    }
}
