#pragma once
#include "common.hpp"

namespace big
{
    template<std::size_t... args>
    __forceinline constexpr auto sum() -> std::size_t
    {
        return (args + ... + 0);
    }
    
    template <typename T, std::size_t... sz>
    __forceinline constexpr auto make_flattened_array(std::array<T, sz>... ar)
    {
        constexpr std::size_t NB_ARRAY = sizeof...(ar);
        
        T* datas[NB_ARRAY] = {&ar[0]...};
        constexpr std::size_t lengths[NB_ARRAY] = {ar.size()...};
        
        constexpr std::size_t FLATLENGTH = sum<ar.size()...>();
        
        std::array<T, FLATLENGTH> flat_a = {0};
        
        auto index = 0;
        for(auto i = 0; i < NB_ARRAY; i++)
        {
            for(auto j = 0; j < lengths[i]; j++)
            {
                flat_a[index] = datas[i][j];
                index++;
            }
        }
        
        return flat_a;
    }
    
    template <std::size_t N>
    __forceinline auto to_shellcode_array(const char(&first)[N]) -> std::array<std::uint8_t, N - 1>
    {
        return *(std::array<uint8_t, N-1>*)(static_cast<const char*>(first));
    }
    
    template <typename T>
    __forceinline auto to_shellcode_array(T first) -> std::array<std::uint8_t, sizeof(T)>
    {
        return *(std::array<uint8_t, sizeof(first)>*)(&first);
    }
    
    template <typename... Pack>
    __forceinline auto make_shellcode(Pack&&... args)
    {
        return make_flattened_array(
            to_shellcode_array(std::forward<Pack>(args))...
            );
    }
}