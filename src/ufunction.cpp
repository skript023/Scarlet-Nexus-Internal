#include "ufunction.hpp"
#include "unreal_engine/SDK/Engine_classes.hpp"
#include "unreal_engine/SDK/CoreUObject_classes.hpp"

namespace big
{
    ufunction::ufunction()
    {
        g_ufunction = this;
    }

    ufunction::~ufunction()
    {
        g_ufunction = nullptr;
    }

    bool ufunction::push_cache(std::string name, SDK::UFunction *function)
    {
        if (!m_functions_cache.insert({name, function}).second)
        {
            LOG(WARNING) << "Duplicate function pointer in ufunction";

            return false;
        }

        return true;
    }

    SDK::UFunction *ufunction::get_native(std::string const& name)
    {
        if (auto function = m_functions_cache.find(name); function != m_functions_cache.end())
        {
            return function->second;
        }
        if (auto function = SDK::UObject::FindObjectFast<SDK::UFunction>(name); function)
        {
            push_cache(name, function);

            return function;
        }
        
        return nullptr;
    }
    void ufunction::execute_native_function(std::string className, std::string functionName, void *parameters)
    {
        auto classes = SDK::UObject::FindClassFast(className);
        auto function = SDK::UObject::FindObjectFast<SDK::UFunction>(functionName);

        if (classes && function)
        {
            classes->ProcessEvent(function, parameters);
        }
    }
    void ufunction::execute_native_function(SDK::UClass *classInstance, std::string functionName, void *parameters)
    {
        auto function = g_ufunction->get_native(functionName);

        if (function && classInstance)
        {
            classInstance->ProcessEvent(m_function, parameters);
        }
    }
}