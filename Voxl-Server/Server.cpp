#include "Server.h"

#include <ctime>

#include <file/FileUtilities.h>
#include <IWorld.h>
#include <IClientConnection.h>
#include <IWorldGenerator.h>
#include <time/Timer.h>
#include <VoxelRegistry.h>
#include <IGameMode.h>
#include <VoxelInfo.h>
#include <other/ServiceLocator.h>
#include <IConnectionManager.h>
#include <IPacketManager.h>


#include "ConnectionManager.h"
#include "DefaultGameMode.h"
#include "DefaultWorldGenerator.h"
#include "JsonUtilities.h"
#include "PacketHandler_Authenticate.h"
#include "PacketHandler_ChatMessage.h"
#include "time/GameLoop.h"
#include "World.h"
#include "PacketHandler_ChunkSubscribe.h"
#include "PacketHandler_ChunkUnsubscribe.h"
#include "PacketHandler_Request.h"
#include "PacketHandler_VoxelUpdate.h"


#define VOXEL_TYPES_FILE_NAME "voxeltypes.json"
#define SERVER_SETTINGS_FILE_NAME "serverdata.json"

#undef DeleteFile

namespace voxl
{
    Server::Server() : m_State(ServerState::SHUT_DOWN), m_RequestedState(ServerState::SHUT_DOWN), m_RequestedStateSaveState(true)
    {
        
    }

    Server::~Server()
    {
        //Required for unique ptr forward declare.
    }

    void Server::Tick(double a_DeltaTime)
    {
        /*
         * Update each of the clients.
         */
        m_ConnectionManager->ProcessClientConnections();

        /*
         * Update each of the gamemodes
         */


        /*
         * Update each of the worlds.
         */
        for(auto& world : m_Worlds)
        {
            world.second->Tick(a_DeltaTime);
        }
    }

    void Server::RegisterWorldGenerator(const std::string& a_Name, std::shared_ptr<IWorldGenerator>& a_Generator)
    {
        m_Generators.insert(std::make_pair(a_Name, a_Generator));
    }

    VoxelRegistry& Server::GetVoxelRegistry()
    {
        return *m_VoxelRegistry;
    }

    void Server::Start()
    {
        if(m_State == ServerState::SHUT_DOWN)
        {
            //So that it keeps running.
            m_State = ServerState::STARTING;
            m_RequestedState = ServerState::RUNNING;

            //Create the logger.
            CreateLogger();

            utilities::Timer timer;
            m_Logger->log(utilities::Severity::Info, "Server starting up.");
            //Store settings for restarts or other queries.

            //Load the settings file.
            if(LoadSettingsFile())
            {
                m_Logger->log(utilities::Severity::Info, "Server settings loaded successfully.");
            }
            else
            {
                //Create from defaults.
                CreateSettingsFile(ServerSettings());
                if (!LoadSettingsFile())
                {
                    m_Logger->log(utilities::Severity::Fatal, "Could not load or create server settings file!");
                    return;
                }
                else
                {
                    m_Logger->log(utilities::Severity::Info, "Default server settings file created and loaded successfully.");
                }
            }

            //Load the voxel data file.
            if(LoadVoxelTypesFile())
            {
                m_Logger->log(utilities::Severity::Info, "Voxel types file loaded successfully.");
            }
            else
            {
                if (!utilities::FileUtilities::FileExists(VOXEL_TYPES_FILE_NAME))
                {
                    CreateVoxelTypesFile();
                    if (!LoadVoxelTypesFile())
                    {
                        m_Logger->log(utilities::Severity::Fatal, "Could not load or create voxel types file!");
                        return;
                    }
                    else
                    {
                        m_Logger->log(utilities::Severity::Info, "Default voxel types file created and loaded successfully.");
                    }
                }
                else
                {
                    std::cout << "Voxel type file corrupted. Delete or repair it before starting the server." << std::endl;
                    return;
                }

            }

            //Register the default world generator and default gamemode.
            std::shared_ptr<IWorldGenerator> generator = std::make_shared<DefaultWorldGenerator>();
            RegisterWorldGenerator("default", generator);
            std::shared_ptr<IGameMode> defGm = std::make_shared<DefaultGameMode>();
            RegisterGameMode("default", defGm);


            /*
             * Initialize the gamemodes and worlds configured.
             */
            for(auto & gameMode : m_Settings.gameModes)
            {
                //Create the gamemode instance.
                auto ptr = CreateGamemode(gameMode.gameMode);
                if(ptr == nullptr)
                {
                    m_Logger->log(utilities::Severity::Fatal, "Gamemode defined in server settings '" + gameMode.gameMode + "' does not exist.");
                    return;
                }

                //Create the worlds configured.
                for(auto& world : gameMode.worlds)
                {
                    IWorld* worldPtr;
                    if (WorldExists(world))
                    {
                        //Exists on disk but not in memory.
                        if ((worldPtr = GetWorld(world)) == nullptr)
                        {
                            worldPtr = LoadWorld(world);

                        }
                        else
                        {
                            m_Logger->log(utilities::Severity::Fatal, "World defined for gamemode '" + gameMode.gameMode + "' was already loaded! Defined for multiple gamemodes?");
                            return;
                        }
                    }
                    else
                    {
                        //Create a world with default settings.
                        worldPtr = CreateWorld(WorldSettings());
                    }

                    if(worldPtr == nullptr)
                    {
                        m_Logger->log(utilities::Severity::Fatal, "World defined for gamemode '" + gameMode.gameMode + "' could not be loaded.");
                        return;
                    }

                    //Set the gamemode for the world.
                    worldPtr->SetGameMode(ptr);
                }


            }

            //Setup the connection server
            m_ConnectionManager = std::make_unique<ConnectionManager>();
            if (!m_ConnectionManager->Start(m_Settings))
            {
                m_Logger->log(utilities::Severity::Fatal, "Could not set up connection server!");
                return;
            }
            m_Logger->log(utilities::Severity::Info, "Connection server has been successfully set up!");
            m_Logger->log(utilities::Severity::Info, "Listening for connections at ip '" + m_Settings.ip + ":" + std::to_string(m_Settings.port) + "'.");

            //Mark server as running.
            m_State = ServerState::RUNNING;
            m_Logger->log(utilities::Severity::Info, "Server finished starting up in " + std::to_string(timer.measure(utilities::TimeUnit::MILLIS)) + " milliseconds.");
        }
        else
        {
            m_Logger->log(utilities::Severity::Warning, "Server start signal received, but server is already running.");
            return;
        }

        /*
         * Register packet handlers
         */
        //Player connection and chat
        auto& packetManager = m_ConnectionManager->GetPacketManager();
        packetManager.Register(PacketType::AUTHENTICATE, std::make_unique<PacketHandler_Authenticate>(*m_ConnectionManager));   //Authenticates a user.
        packetManager.Register(PacketType::CHAT_MESSAGE, std::make_unique<PacketHandler_ChatMessage>(*m_ConnectionManager));    //Distributes chat messages    
        packetManager.Register(PacketType::REQUEST, std::make_unique<PacketHandler_Request>(m_VoxelInfoFile));                  //Handles any sort of request.

        //Chunk data related.
        packetManager.Register(PacketType::CHUNK_SUBSCRIBE, std::make_unique<PacketHandler_ChunkSubscribe>());
        packetManager.Register(PacketType::CHUNK_UNSUBSCRIBE, std::make_unique<PacketHandler_ChunkUnsubscribe>());
        packetManager.Register(PacketType::VOXEL_UPDATE, std::make_unique<PacketHandler_VoxelUpdate>());

        /*
         * Main game loop.
         */
        utilities::GameLoop gameLoop(1, m_Settings.tps, false);
        long long tick = 0;

        while (true)
        {
            const auto data = gameLoop.update();

            //Tick
            if (data.tick)
            {
                Tick(data.deltaTick);
                ++tick;
            }

            //Remove this but for now it's nice to see TPS.
            if (tick == 300)
            {
                m_Logger->log(utilities::Severity::Info, "Tps: " + std::to_string(data.tps) + ".");
                tick = 0;
            }

            if (m_RequestedState != ServerState::RUNNING)
            {
                break;
            }
        }

        //Shutdown requested.
        if(m_RequestedState == ServerState::SHUT_DOWN)
        {
            DoShutDown(m_RequestedStateSaveState);
            m_State = ServerState::SHUT_DOWN;
        }
    }

    IWorld* Server::GetWorld(const std::string& a_Name)
    {
        auto itr = m_Worlds.find(a_Name);
        if(itr != m_Worlds.end())
        {
            return itr->second.get();
        }
        return nullptr;
    }

    IWorld* Server::LoadWorld(const std::string& a_Name)
    {
        //Create world, load, then return the pointer.
        auto world = std::make_unique<World>(a_Name, m_Settings.worldsDirectory);
        if (world->Load(*this))
        {
            const auto inserted = m_Worlds.insert(std::make_pair(a_Name, std::move(world)));
            return inserted.first->second.get();
        }
        return nullptr;
    }

    std::vector<IWorld*> Server::GetWorlds()
    {
        std::vector<IWorld*> worlds;
        worlds.reserve(m_Worlds.size());

        for(auto& pair : m_Worlds)
        {
            worlds.push_back(pair.second.get());
        }

        return worlds;
    }

    void Server::ShutDown(bool a_SaveAll)
    {
        if(m_State == ServerState::RUNNING)
        {
            //Set state to shutdown.
            m_RequestedState = ServerState::SHUT_DOWN;
            m_RequestedStateSaveState = a_SaveAll;
        }
        else
        {
            assert(0 && "Can't stop a server that isn't running.");
        }
    }

    bool Server::UnloadWorld(const std::string& a_Name, bool a_Save)
    {
        auto found = m_Worlds.find(a_Name);

        if (found != m_Worlds.end())
        {
            if(a_Save)
            {
                found->second->Save();
            }

            found->second->Unload();
            m_Worlds.erase(found);
            m_Logger->log(utilities::Severity::Info, "World '" + a_Name +  "' has been unloaded.");
            return true;
        }
        m_Logger->log(utilities::Severity::Warning, "Tried to unload world that was not loaded or did not exist.");
        return false;
    }

    void Server::RegisterGameMode(const std::string& a_Name, std::shared_ptr<IGameMode>& a_GameMode)
    {
        a_GameMode->Load();
        m_GameModes.insert(std::make_pair(a_Name, a_GameMode));
    }

    IWorld* Server::CreateWorld(const WorldSettings& a_Settings)
    {
        const std::filesystem::path worldPath = m_Settings.worldsDirectory + "/" + a_Settings.name;

        //World already exists on disk.
        if(utilities::FileUtilities::FileExists(worldPath))
        {
            m_Logger->log(utilities::Severity::Warning, "Could not create world '" + a_Settings.name + "' because it already exists on file.");
            return nullptr;
        }

        //World already exists in memory.
        const auto found = m_Worlds.find(a_Settings.name);
        if (found != m_Worlds.end())
        {
            m_Logger->log(utilities::Severity::Warning, "Could not create world '" + a_Settings.name + "' because it already exists in memory.");
            return nullptr;
        }

        //Create the world and place it in the registry.
        auto world = std::make_unique<World>(a_Settings.name, m_Settings.worldsDirectory);

        //World doesn't exist yet so create the default settings file.
        world->SaveWorldSettings(a_Settings);

        if(!world->Load(*this))
        {
            m_Logger->log(utilities::Severity::Error, "Newly created world '" + a_Settings.name + "' could not be loaded.");
            return nullptr;
        }

        //Insert into the registry.
        const auto inserted = m_Worlds.emplace(a_Settings.name, std::move(world));

        m_Logger->log(utilities::Severity::Info, "World '" + a_Settings.name + "' has been created.");

        return inserted.first->second.get();
    }

    bool Server::DeleteWorld(const std::string& a_Name)
    {
        bool deleted = false;

        //Delete the world from memory.
        auto found = m_Worlds.find(a_Name);

        if(found != m_Worlds.end())
        {
            found->second->Unload();
            m_Worlds.erase(found);
            deleted = true;
        }
        
        //Delete the world from the disk.
        const std::filesystem::path worldPath = m_Settings.worldsDirectory + "/" + a_Name;

        if(utilities::FileUtilities::FileExists(worldPath))
        {
           utilities::FileUtilities::DeleteFile(worldPath);
           deleted = true;
        }

        if(deleted)
        {
            m_Logger->log(utilities::Severity::Info, "World '" + a_Name + "' has been deleted.");
        }
        else
        {
            m_Logger->log(utilities::Severity::Warning, "World '" + a_Name + "' could not be deleted because it doesn't exist.");
        }

        return deleted;
    }

    utilities::Logger& Server::GetLogger()
    {
        return *m_Logger;
    }

    std::shared_ptr<IWorldGenerator> Server::GetWorldGenerator(const std::string& a_Name)
    {
        auto found = m_Generators.find(a_Name);

        if(found != m_Generators.end())
        {
            return found->second;
        }
        return nullptr;
    }

    std::shared_ptr<IGameMode> Server::CreateGamemode(const std::string& a_Name)
    {
        const auto found = m_GameModes.find(a_Name);
        if(found != m_GameModes.end())
        {
            return found->second;
        }
        return nullptr;
    }

    const ServerSettings& Server::GetServerSettings() const
    {
        return m_Settings;
    }

    void Server::CreateLogger()
    {
        //All this to get a timestamp for the log name.
        std::string time;
        struct std::tm newtime;
        auto t = std::time(nullptr);
        localtime_s(&newtime, &t);
        std::ostringstream oss;
        oss << std::put_time(&newtime, "%d-%m-%Y-%H-%M-%S");
        time = oss.str();
        std::string name = "logs/Log-" + time + ".txt";

        m_Logger = std::make_unique<utilities::Logger>(name);
        m_Logger->setConsoleLogging(true);
        m_Logger->setFileLogging(true);

        //Set up the service locator for the logger so it can be statically retrieved anywhere.
        utilities::ServiceLocator<utilities::Logger>::setService(m_Logger.get());
    }

    void Server::DoShutDown(bool a_SaveAll)
    {
        utilities::Timer timer;
        m_Logger->log(utilities::Severity::Info, "Server stop signal received.");

        //Disconnect all clients.
        m_ConnectionManager->DisconnectClients();

        //Stop the connections.
        m_ConnectionManager->Stop();

        //Shutdown all worlds.
        for (auto& pair : m_Worlds)
        {
            if (a_SaveAll)
            {
                pair.second->Save();
            }
            pair.second->Unload();
        }

        //Clear all things in memory
        m_Worlds.clear();
        m_GameModes.clear();
        m_Generators.clear();
        m_VoxelRegistry.reset();

        //Mark server as shut down.
        m_Logger->log(utilities::Severity::Info, "Server shut-down finished in " + std::to_string(timer.measure(utilities::TimeUnit::MILLIS)) + " milliseconds.");
        m_Logger->saveToFile();
        m_Logger.reset();
    }

    bool Server::LoadSettingsFile()
    {
        if(!utilities::FileUtilities::FileExists(SERVER_SETTINGS_FILE_NAME))
        {
            return false;
        }

        //Load from file.
        {
            std::ifstream inStream(SERVER_SETTINGS_FILE_NAME);
            nlohmann::json file = nlohmann::json::parse(inStream);
            const ServerSettings defaultSettings;

            nlohmann::basic_json<> gameModes;
            if (file.is_discarded() || !JsonUtilities::VerifyValue("gameModes", file, gameModes) || gameModes.empty())
            {
                std::cout << "Could not load server settings file. Invalid Json format." << std::endl;
                return false;
            }

            if (!JsonUtilities::VerifyValue("tps", file, m_Settings.tps) || m_Settings.tps <= 0)
            {
                m_Logger->log(utilities::Severity::Warning, "Tps not defined in server settings. Default value restored.");
                m_Settings.tps = defaultSettings.tps;
            }

            if (!JsonUtilities::VerifyValue("worldsDirectory", file, m_Settings.worldsDirectory))
            {
                m_Logger->log(utilities::Severity::Warning, "World directory not defined in server settings. Default value restored.");
                m_Settings.worldsDirectory = defaultSettings.worldsDirectory;
            }

            if (!JsonUtilities::VerifyValue("maximumConnections", file, m_Settings.maximumConnections) || m_Settings.maximumConnections <= 0)
            {
                m_Logger->log(utilities::Severity::Warning, "Maximum connections not configured in settings. Default value restored.");
                m_Settings.maximumConnections = defaultSettings.maximumConnections;
            }
            if (!JsonUtilities::VerifyValue("ip", file, m_Settings.ip) || m_Settings.ip.empty())
            {
                m_Logger->log(utilities::Severity::Warning, "IP not configured in settings. Default value restored.");
                m_Settings.ip = defaultSettings.ip;
            }

            if (!JsonUtilities::VerifyValue("port", file, m_Settings.port) || m_Settings.port <= 0)
            {
                m_Logger->log(utilities::Severity::Warning, "Port not configured in settings. Default value restored.");
                m_Settings.port = defaultSettings.port;
            }

            /*
             * Load gamemodes.
             */
            m_Settings.gameModes.resize(gameModes.size());

            //Extract the gamemodes from the file.
            int i = 0;
            for(auto const& gameMode : gameModes)
            {
                GameModeSettings gameModeSettings;
                JsonUtilities::VerifyValue("gameMode", gameMode, gameModeSettings.gameMode);
                JsonUtilities::VerifyValue("worlds", gameMode, gameModeSettings.worlds);
                m_Settings.gameModes[i] = gameModeSettings;
                ++i;
            }
        }

        //Store in case defaults were reset.
        CreateSettingsFile(m_Settings);

        return true;
    }

    void Server::CreateSettingsFile(const ServerSettings& a_Settings)
    {
        nlohmann::json file;
        file["tps"] = a_Settings.tps;
        file["worldsDirectory"] = a_Settings.worldsDirectory;
        file["maximumConnections"] = a_Settings.maximumConnections;
        file["ip"] = a_Settings.ip;
        file["port"] = a_Settings.port;

        //Create an array of gamemodes, containing gamemode objects which each contain a name and a list of worlds.
        auto gamemodes = nlohmann::json::array();
        auto gamemode = nlohmann::json::object();
        auto worlds = nlohmann::json::array();
        worlds.push_back("world");
        gamemode["gameMode"] = "default";
        gamemode["worlds"] = worlds;
        gamemodes.push_back(gamemode);
        file["gameModes"] = gamemodes;

        //Write to file.
        std::ofstream stream(SERVER_SETTINGS_FILE_NAME);
        stream << file;
    }

    bool Server::LoadVoxelTypesFile()
    {
        if (!utilities::FileUtilities::FileExists(VOXEL_TYPES_FILE_NAME))
        {
            return false;
        }

        std::ifstream inStream(VOXEL_TYPES_FILE_NAME);
        nlohmann::json file = nlohmann::json::parse(inStream, nullptr, false, false);

        nlohmann::basic_json<> voxelTypes;
        if(file.is_discarded() || !JsonUtilities::VerifyValue("voxelTypes", file, voxelTypes))
        {
            std::cout << "Could not load existing voxel types file. Invalid Json format." << std::endl;
            return false;
        }

        //Create a voxel registry. Allocate exactly the amount of space needed to store all the data.
        m_VoxelRegistry = std::make_unique<VoxelRegistry>(voxelTypes.size());

        //Loop over every item registered.
        for(auto const& value : voxelTypes)
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
            m_VoxelRegistry->Register(info);
        }

        /*
         * Store the voxel data file for distribution to clients.
         */
        std::string str = file.dump();
        m_VoxelInfoFile.insert(m_VoxelInfoFile.end(), str.begin(), str.end());

        return true;
    }

    void Server::CreateVoxelTypesFile()
    {
        nlohmann::json file;
        VoxelInfo info;
        auto element = nlohmann::json::object();
        auto voxelTypes = nlohmann::json::array();

        element["name"] = info.name;
        element["description"] = info.description;
        element["id"] = info.id;
        element["collision"] = info.collision;
        element["emissiveLight"] = info.emissiveLight;
        element["passThroughSpeed"] = info.passThroughSpeed;
        element["strength"] = info.strength;
        element["animationFrames"] = info.graphics.animationFrames;
        element["mesh"] = info.graphics.mesh;
        element["textureIndex"] = info.graphics.textureIndex;
        element["transparent"] = info.graphics.transparent;

        //Store the data as part of an element.
        voxelTypes.push_back(element);
        file["voxelTypes"] = voxelTypes;

        //Write to file.
        std::ofstream stream(VOXEL_TYPES_FILE_NAME);
        stream << file;
    }

    bool Server::WorldExists(const std::string& a_Name)
    {
        return utilities::FileUtilities::FileExists(m_Settings.worldsDirectory + "/" + a_Name) || GetWorld(a_Name) != nullptr;
    }

    ServerState Server::GetState()
    {
        return m_State;
    }

    IConnectionManager& Server::GetConnectionManager()
    {
        return *m_ConnectionManager;
    }
}
