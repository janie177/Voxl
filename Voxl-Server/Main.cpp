#include "time/GameLoop.h"
#include "Server.h"
#include <chrono>
#include <IClientConnection.h>;

int main()
{
    std::unique_ptr<voxl::Server> server = std::make_unique<voxl::Server>();

    //Start the server on another thread.
    std::thread thread(&voxl::Server::Start, server.get());

    //Wait till the server goes into start mode.
    while (true)
    {
        if(server->GetState() != voxl::ServerState::SHUT_DOWN)
        {
            thread.detach();
            break;
        }
    }

    //As long as the server is running, listen for input.
    std::string input;
    while(server->GetState() != voxl::ServerState::SHUT_DOWN)
    {
        std::cin >> input;

        if(!input.empty())
        {
            std::cout << "Command received: " << input << std::endl;
        }

        if(input == "stop")
        {
            server->ShutDown(true);

            //Wait for actual shutdown.
            while(true)
            {
                if(server->GetState() == voxl::ServerState::SHUT_DOWN)
                {
                    std::cin.ignore();
                    break;
                }
            }
        }

        else if(input == "players")
        {
            std::cout << "Users: " << std::endl;
            auto& manager = server->GetConnectionManager();
            for(auto& user : manager.GetConnectedClients())
            {
                std::cout << " - " << user->GetUsername() << std::endl;
            }

        }

        input.clear();
    }

    getchar();

    return 0;
}
