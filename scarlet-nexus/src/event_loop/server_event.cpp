#include "script.hpp"
#include "backend_events.hpp"
#include "api/integration.hpp"
#include "api/schedule.hpp"

namespace big
{
	void backend_events::server_task()
	{
		g_schedule->task([] { g_web_server->heartbeat(); })->duration(1min);
	}

	void backend_events::server_event()
	{
		TRY_CLAUSE
		{
			while (true)
			{
				backend_events::server_task();

				script::get_current()->yield(1s);
			}
		}
		EXCEPT_CLAUSE
	}
}