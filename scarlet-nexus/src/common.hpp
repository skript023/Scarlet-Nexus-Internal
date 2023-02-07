#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>
#include <D3D11.h>
#include <wrl/client.h>

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <chrono>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>
#include <new>

#include <sstream>
#include <string>
#include <string_view>

#include <algorithm>
#include <functional>
#include <utility>

#include <stack>
#include <vector>

#include <typeinfo>
#include <type_traits>

#include <exception>
#include <stdexcept>

#include <any>
#include <optional>
#include <variant>

#include <regex>
#include <tlhelp32.h>
#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

#include "settings.hpp"
#include "logger.hpp"
#include "subprocess.hpp"

#define MOD_NAME "Ellohim Private Menu"

#define PRODUCT_NAME "Ellohim"

#define DISTANCE_SPAWN 10

#define CUSTOM_HUD "Madang Hese Udud Hese Teu Boga Duit Wuhan Kontol"
#define NODISCARD [[nodiscard]]

namespace big
{
	using namespace std::chrono_literals;
	
	template <typename T>
	using comptr = Microsoft::WRL::ComPtr<T>;

#ifdef _DEBUG
	inline const std::string g_base_url = "http://localhost:8000.com/api/v1";
#else
	inline const std::string g_base_url = "https://big-base.000webhostapp.com/api/v1";
#endif
	inline uint32_t g_injection_code{};
	inline HMODULE g_hmodule{};
	inline HANDLE g_main_thread{};
	inline DWORD g_main_thread_id{};
	inline std::atomic_bool g_running{ true };
}
