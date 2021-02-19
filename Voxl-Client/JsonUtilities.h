#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace voxl
{
    class JsonUtilities
    {
    public:
        template<typename T>
        static bool VerifyValue(const std::string& a_Key, const nlohmann::json& a_Json, T& a_Result)
        {
            const auto itr = a_Json.find(a_Key);
            if(itr == a_Json.end())
            {
                return false;
            }

            a_Result = itr.value().get<T>();

            return true;
        }
    };
}
