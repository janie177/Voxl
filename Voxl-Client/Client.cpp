#include "Client.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <time/GameLoop.h>
#include <VoxelRegistry.h>

#include <fstream>

#include "ClientChunkStore.h"
#include "input/InputQueue.h"
#include "Renderer.h"
#include "ServerConnection.h"
#include "ClientPacketManager.h"
#include "file/FileUtilities.h"
#include "PacketHandler_IncomingMessage.h"
#include "JsonUtilities.h"

#define CLIENT_SETTINGS_FILE "client.json"

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
        if(!LoadSettings(m_Settings))
        {
            std::cout << "Could not load client settings. Creating new defaults and saving to file." << std::endl;
            SaveSettings(m_Settings);
        }

        //Initialize systems.
        m_ChunkStore = std::make_unique<ClientChunkStore>();
        m_Renderer = std::make_unique<Renderer>();

        if(!m_Renderer->Init(m_Settings.renderSettings))
        {
            std::cout << "Could not initialize rendering system." << std::endl;
            return;
        }

        //Initialize the connection and connect to the server.
        Packet_Authenticate authentication;
        strcpy_s(authentication.name, m_Settings.userName.length() + 1, m_Settings.userName.c_str());
        m_ServerConnection = std::make_unique<ServerConnection>(5000);

        if(!m_ServerConnection->Setup())
        {
            std::cout << "Could not set up client connection." << std::endl;
            ShutDown();
            return;
        }

        if(!m_ServerConnection->Connect(m_Settings.ip, m_Settings.port, authentication))
        {
            std::cout << "Could not connect to remote server at " << m_Settings.ip << ":" << std::to_string(m_Settings.port) << std::endl;
            ShutDown();
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

        //Get the window from the renderer.
        auto window = m_Renderer->GetWindow();

        //Count the ticks.
        long counter = 0;

        /*
         * Keep the game loop going for as long as the server is online and the window is open.
         */
        while(m_ServerConnection->GetConnectionState() == ConnectionState::CONNECTED && !window->IsClosed())
        {
            const auto data = loop.update();

            if(data.tick)
            {
                //Retrieve input data.
                auto inputData = window->PollInput();

                utilities::MouseEvent mEvent;
                utilities::KeyboardEvent kEvent;

                //Process mouse input.
                while(inputData.getNextEvent(mEvent))
                {
                    if(mEvent.action == utilities::MouseAction::CLICK)
                    {
                        std::cout << "CLICKED!" << std::endl;
                    }
                }

                //Process keyboard input.
                while(inputData.getNextEvent(kEvent))
                {
                    if(kEvent.action == utilities::KeyboardAction::KEY_PRESSED)
                    {
                        std::cout << char(kEvent.keyCode) << " PRESSED!" << std::endl;
                    }
                }

                //Process incoming packets.
                m_ServerConnection->ProcessPackets();

                ++counter;

                //Send a test packet.
                if(counter % 200 == 0)
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

        ShutDown();
    }

    void Client::ShutDown()
    {
        //Save settings if they changed.
        if (!SaveSettings(m_Settings))
        {
            std::cout << "Could not save client settings." << std::endl;
        }

        //Shutdown systems.
        if(m_ServerConnection != nullptr) m_ServerConnection->Shutdown();
        if(m_Renderer != nullptr) m_Renderer->ShutDown();

        std::cout << "Disconnected and client shut down." << std::endl;
    }

    VoxelRegistry& Client::GetVoxelRegistry()
    {
        assert(m_Running);
        return *m_VoxelRegistry;
    }

    bool Client::LoadSettings(ClientSettings& a_Settings)
    {
        if (!utilities::FileUtilities::FileExists(CLIENT_SETTINGS_FILE))
        {
            return false;
        }

        std::ifstream inStream(CLIENT_SETTINGS_FILE);
        nlohmann::json file = nlohmann::json::parse(inStream);

        //Default settings object.
        ClientSettings def;

        //Basic client settings.
        JsonUtilities::VerifyValue("name", file, def.userName);
        JsonUtilities::VerifyValue("fps", file, def.fps);
        JsonUtilities::VerifyValue("serverIP", file, def.ip);
        JsonUtilities::VerifyValue("serverPort", file, def.port);

        //Graphics related.
        JsonUtilities::VerifyValue("verticalSync", file, def.renderSettings.vSync);
        JsonUtilities::VerifyValue("resolutionX", file, def.renderSettings.windowSettings.dimensions.x);
        JsonUtilities::VerifyValue("resolutionY", file, def.renderSettings.windowSettings.dimensions.y);
        JsonUtilities::VerifyValue("fullScreen", file, def.renderSettings.windowSettings.fullScreen);

        //Overwrite with the loaded and default settings.
        a_Settings = def;
        return true;
    }

    bool Client::SaveSettings(const ClientSettings& a_Settings)
    {
        nlohmann::json file;
        auto voxelTypes = nlohmann::json::array();

        //General info and connection.
        file["name"] = a_Settings.userName;
        file["fps"] = a_Settings.fps;
        file["serverIP"] = a_Settings.ip;
        file["serverPort"] = a_Settings.port;

        //Graphics and window.
        file["verticalSync"] = a_Settings.renderSettings.vSync;
        file["resolutionX"] = a_Settings.renderSettings.windowSettings.dimensions.x;
        file["resolutionY"] = a_Settings.renderSettings.windowSettings.dimensions.y;
        file["fullScreen"] = a_Settings.renderSettings.windowSettings.fullScreen;

        //Write to file.
        std::ofstream stream(CLIENT_SETTINGS_FILE);
        stream << file;
        return true;
    }
}
