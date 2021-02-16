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


#include "ConnectionManager.h"
#include "DefaultGameMode.h"
#include "DefaultWorldGenerator.h"
#include "JsonUtilities.h"
#include "World.h"


#define VOXEL_TYPES_FILE_NAME "voxeltypes.json"
#define SERVER_SETTINGS_FILE_NAME "serverdata.json"

namespace voxl
{
    Server::Server() : m_Running(false)
    {
        
    }

    void Server::Tick(double a_DeltaTime)
    {
        /*
         * Update each of the clients.
         */
        m_ConnectionManager->ProcessClientConnections();

        /*
         * Update each of the worlds.
         */
        for(auto& world : GetWorlds())
        {
            world->Tick(a_DeltaTime);
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

    bool Server::Start()
    {
        if(!m_Running)
        {
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
                    return false;
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
                CreateVoxelTypesFile();
                if(!LoadVoxelTypesFile())
                {
                    m_Logger->log(utilities::Severity::Fatal, "Could not load or create voxel types file!");
                    return false;
                }
                else
                {
                    m_Logger->log(utilities::Severity::Info, "Default voxel types file created and loaded successfully.");
                }
            }

            //Register the default world generator and default gamemode.
            RegisterGameMode("default", std::make_unique<DefaultGameMode>());
            std::shared_ptr<IWorldGenerator> generator = std::make_shared<DefaultWorldGenerator>();
            RegisterWorldGenerator("default", generator);

            //Load the default worlds.
            for(auto & world : m_Settings.defaultWorlds)
            {
                if(WorldExists(world))
                {
                    //Exists on disk but not in memory.
                    if(GetWorld(world) == nullptr)
                    {
                        LoadWorld(world);
                    }
                }
                else
                {
                    //Create a world with default settings.
                    CreateWorld(WorldSettings());
                }
            }

            //Setup the connection server
            m_ConnectionManager = std::make_unique<ConnectionManager>();
            if (!m_ConnectionManager->Start(m_Settings))
            {
                m_Logger->log(utilities::Severity::Fatal, "Could not set up connection server!");
                return false;
            }
            m_Logger->log(utilities::Severity::Info, "Connection server has been successfully set up!");
            m_Logger->log(utilities::Severity::Info, "Listening for connections at ip '" + m_Settings.ip + ":" + std::to_string(m_Settings.port) + "'.");

            //Mark server as running.
            m_Running = true;
            m_Logger->log(utilities::Severity::Info, "Server finished starting up in " + std::to_string(timer.measure(utilities::TimeUnit::MILLIS)) + " milliseconds.");
            return true;
        }
        else
        {
            m_Logger->log(utilities::Severity::Warning, "Server start signal received, but server is already running.");
            return false;
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

    bool Server::HasShutDown()
    {
        return !m_Running;
    }

    void Server::ShutDown(bool a_SaveAll)
    {
        if(m_Running)
        {
            utilities::Timer timer;
            m_Logger->log(utilities::Severity::Info, "Server stop signal received.");

            //Disconnect all clients.
            m_ConnectionManager->DisconnectClients();

            //Stop the connections.
            m_ConnectionManager->Stop();

            //Shutdown all worlds.
            for(auto& pair : m_Worlds)
            {
                if(a_SaveAll)
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
            m_Logger.reset();

            //Mark server as shut down.
            m_Running = false;
            m_Logger->log(utilities::Severity::Info, "Server shut-down finished in " + std::to_string(timer.measure(utilities::TimeUnit::MILLIS)) + " milliseconds.");
            m_Logger->saveToFile();
        }
        else
        {
            assert(0 && "Can't stop a server that isn't running.");
        }
    }

    void Server::Restart(bool a_SaveAll)
    {
        if(m_Running)
        {
            m_Logger->log(utilities::Severity::Info, "Server restarting.");
            ShutDown(a_SaveAll);
            Start();
        }
        else
        {
            assert(0 && "Can't restart a server that isn't running.");
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

    void Server::RegisterGameMode(const std::string& a_Name, std::unique_ptr<IGameMode>&& a_GameMode)
    {
        m_GameModes.insert(std::make_pair(a_Name, std::move(a_GameMode)));
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

    std::unique_ptr<IGameMode> Server::CreateGameMode(const std::string& a_Name)
    {
        auto found = m_GameModes.find(a_Name);
        if(found != m_GameModes.end())
        {
            auto clone = found->second->Clone();
            return clone;
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

            if (!JsonUtilities::VerifyValue("defaultWorlds", file, m_Settings.defaultWorlds))
            {
                m_Logger->log(utilities::Severity::Warning, "No default worlds configured defined in server settings.");
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
        file["defaultWorlds"] = a_Settings.defaultWorlds;
        file["maximumConnections"] = a_Settings.maximumConnections;
        file["ip"] = a_Settings.ip;
        file["port"] = a_Settings.port;

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
        nlohmann::json file = nlohmann::json::parse(inStream);
        nlohmann::basic_json<> voxelTypes;
        if(!JsonUtilities::VerifyValue("voxelTypes", file, voxelTypes))
        {
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

        return true;
    }

    void Server::CreateVoxelTypesFile()
    {
        nlohmann::json file;
        VoxelInfo info;
        auto element = nlohmann::json::object();
        auto voxelTypes = nlohmann::json::array();

        element["name"] = info.name;
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

    IConnectionManager& Server::GetConnectionManager()
    {
        return *m_ConnectionManager;
    }
}
