#include "Client.h"
#include <time/GameLoop.h>

#include "input/InputQueue.h"

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

        //TODO initialize chunk store.

        //TODO initialize renderer.

        //TODO initialize connection and connect to server.

        //TODO retrieve voxel data from server and feed to voxel registry.



        //Start a game loop with the settings FPS and TPS.
        utilities::GameLoop loop{ 144, 30 };
        utilities::InputQueue input;

        //Loop
        m_Running = true;
        while(m_Running)
        {
            auto data = loop.update();

            if(data.tick)
            {
                //Retrieve input data.
                //TODO move to window class to listen for actual input.
                auto inputData = input.getQueuedEvents();
                utilities::MouseEvent mEvent;
                utilities::KeyboardEvent kEvent;

                //Process mouse input.
                while(inputData.getNextEvent(mEvent))
                {
                    
                }

                //Process keyboard input.
                while(inputData.getNextEvent(kEvent))
                {
                    
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
