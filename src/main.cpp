#include "common.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "benchmark.hpp"
#include "ufunction.hpp"
#include "script_mgr.hpp"
#include "fiber_pool.hpp"
#include "file_manager.hpp"

#include "server/server_module.hpp"
#include "event_loop/backend_events.hpp"

#include "services/notification/notification_service.hpp"

DWORD APIENTRY main_thread(LPVOID)
{
	using namespace big;

	while (!FindWindow("UnrealWindow", "ScarletNexus  "))
		std::this_thread::sleep_for(1s);

	benchmark initialization_benchmark("Initialization");

	std::filesystem::path base_dir = std::getenv("appdata");
	base_dir /= FOLDER_NAME;

	g_file_manager.init(base_dir);

	auto logger_instance = std::make_unique<logger>("Scarlet Nexus", g_file_manager.get_project_file(std::format("./{}.log", LOG_NAME)));

	try
	{
		LOG(RAW_GREEN_TO_CONSOLE) << R"kek(
  _____                _      _   _   _                  _______        _                 
 / ____|              | |    | | | \ | |                |__   __|      (_)                
| (___   ___ __ _ _ __| | ___| |_|  \| | _____  ___   _ ___| |_ __ __ _ _ _ __   ___ _ __ 
 \___ \ / __/ _` | '__| |/ _ \ __| . ` |/ _ \ \/ / | | / __| | '__/ _` | | '_ \ / _ \ '__|
 ____) | (_| (_| | |  | |  __/ |_| |\  |  __/>  <| |_| \__ \ | | | (_| | | | | |  __/ |   
|_____/ \___\__,_|_|  |_|\___|\__|_| \_|\___/_/\_,\____|___/_|_|  \__,_|_|_| |_|\___|_| 
)kek";
		g_settings.load();
		LOG(HACKER) << "Settings initialized.";
		
		auto pointers_instance = std::make_unique<pointers>();
		LOG(HACKER) << "Pointers initialized.";

		auto renderer_instance = std::make_unique<renderer>();
		LOG(HACKER) << "Renderer initialized.";

		auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
		LOG(HACKER) << "Fiber pool initialized.";

		auto thread_pool_instance = std::make_unique<thread_pool>();
		LOG(HACKER) << "Thread Pool initialized.";

		auto hooking_instance = std::make_unique<hooking>();
		LOG(HACKER) << "Hooking initialized.";

		g_pointers->update();

		auto ufunctions_instance = std::make_unique<ufunction>();
		LOG(HACKER) << "Ufunctions initialized.";

		LOG(HACKER) << "Service registered.";

		auto server_instance = std::make_unique<server_module>();
		LOG(HACKER) << "Server initialized.";

		g_script_mgr.add_script(std::make_unique<script>(&backend_events::player_skill_event));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::script_func));
		LOG(HACKER) << "Scripts registered.";

		g_hooking->enable();
		LOG(HACKER) << "Hooking enabled.";

		initialization_benchmark.get_runtime();
		initialization_benchmark.reset();

		while (g_running)
		{
			g_settings.attempt_save();
			std::this_thread::sleep_for(2s);
		}

		g_hooking->disable();
		LOG(HACKER) << "Hooking disabled.";

		std::this_thread::sleep_for(1000ms);

		g_script_mgr.remove_all_scripts();
		LOG(HACKER) << "Scripts unregistered.";

		server_instance.reset();
		LOG(HACKER) << "Server unregistered.";
		
		LOG(HACKER) << "Service unregistered.";

		hooking_instance.reset();
		LOG(HACKER) << "Hooking uninitialized.";

		fiber_pool_instance.reset();
		LOG(HACKER) << "Fiber pool uninitialized.";

		g_thread_pool->destroy();
		LOG(HACKER) << "Destroyed thread pool.";

		thread_pool_instance.reset();
		LOG(HACKER) << "Thread Pool uninitialized.";

		renderer_instance.reset();
		LOG(HACKER) << "Renderer uninitialized.";

		pointers_instance.reset();
		LOG(HACKER) << "Pointers uninitialized.";

		g_settings.attempt_save();
		LOG(HACKER) << "Settings saved and uninitialized.";
	}
	catch (std::exception const& ex)
	{
		LOG(WARNING) << ex.what();
		MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
	}

	LOG(HACKER) << "Farewell!";
	logger_instance.reset();

	CloseHandle(g_main_thread);
	FreeLibraryAndExitThread(g_hmodule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, &main_thread, nullptr, 0, &g_main_thread_id);
		break;
	case DLL_PROCESS_DETACH:
		g_running = false;
		break;
	}

	return true;
}
