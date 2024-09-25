#pragma once
#include "unreal_engine/SDK/CoreUObject_classes.hpp"

namespace big
{
    class ufunction
    {
        inline static SDK::UFunction* m_function;
        inline static SDK::UClass* m_class;
        std::map<std::string, SDK::UFunction*> m_functions_cache;
        std::map<std::string, SDK::UClass*> m_classes_cache;
    public:
        explicit ufunction();
        ~ufunction();

        bool push_cache(std::string name, SDK::UFunction* function);
        template <typename T = SDK::UClass*>
        T get_class(std::string name)
        {
            if (auto classes = m_classes_cache.find(name); classes != m_classes_cache.end())
            {
                return classes->second;
            }
            if (auto uclass = SDK::UClass::FindClassFast(name))
            {
                return static_cast<T>(uclass);
            }

            return nullptr;
        }
        SDK::UFunction* get_native(std::string const& name);
        static void execute_native_function(std::string className, std::string functionName, void* parameters);
        static void execute_native_function(SDK::UClass* classInstance, std::string functionName, void* parameters);
    };

    inline ufunction* g_ufunction;
}