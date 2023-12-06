#include "common.hpp"
#include "process/process.hpp"

int main()
{
	std::cout << ("Scarlet Nexus Trainer") << std::endl;

	std::cout << ("press ENTER to continue\n") << std::endl;

	std::cout << "Waiting for " << process::name << std::endl;
	while (!process::is_process_running())
		std::this_thread::sleep_for(100ms);

	process::pid = process::get_process_id_by_name();
	std::cout << process::name << " found... | " << process::pid;

	if (process::create_remote_thread("ScarletNexus.dll"))
		std::cout << ("DLL Injection done.\n") << std::endl;
	else
		std::cout << ("Could not inject the DLL.") << std::endl;

	std::this_thread::sleep_for(100ms);

	std::cout << "Closing in 10 seconds." << std::endl;

	std::this_thread::sleep_for(10000ms);

	return 0;
}