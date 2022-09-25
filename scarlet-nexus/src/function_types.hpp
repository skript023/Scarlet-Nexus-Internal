#pragma once
#include "common.hpp"
#include "class/unreal_engine_class.hpp"

namespace big::functions
{
	using run_script_threads_t = bool(*)(std::uint32_t ops_to_execute);
	using process_event_t = void(*)(UObject* _this, UFunction* func, void* params);
}
