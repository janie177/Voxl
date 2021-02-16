#pragma once
#include <string>
#include <filesystem>

namespace utilities
{
    class FileUtilities
    {
    public:
        static bool FileExists(const std::filesystem::path& a_Path);
        static bool DeleteFile(const std::filesystem::path& a_Path);
        static bool CreateFile(const std::filesystem::path& a_Path);
    };

    inline bool FileUtilities::FileExists(const std::filesystem::path& a_Path)
    {
        return std::filesystem::exists(a_Path);
    }

    inline bool FileUtilities::DeleteFile(const std::filesystem::path& a_Path)
    {
        return std::filesystem::remove_all(a_Path);
    }

    inline bool FileUtilities::CreateFile(const std::filesystem::path& a_Path)
    {
        if(!FileExists(a_Path))
        {
            std::filesystem::create_directories(a_Path.parent_path());
            return true;
        }
        return false;
    }
}
