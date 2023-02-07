#include "schedule.hpp"
#include "script.hpp"
#include "thread_pool.hpp"

namespace big
{
	schedule::schedule()
	{
		g_schedule = this;
	}

	schedule::~schedule()
	{
		g_schedule = nullptr;
	}

	void schedule::tick()
	{
		if (!m_wake_time.has_value() || m_wake_time.value() <= std::chrono::high_resolution_clock::now())
		{
			g_thread_pool->push(m_jobs.top());
		}
	}

	schedule* schedule::task(std::function<void()> job)
	{
		m_jobs.push(std::move(job));

		return this;
	}

	void schedule::duration(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		if (time.has_value())
		{
			m_wake_time = std::chrono::high_resolution_clock::now() + time.value();
		}
		else
		{
			m_wake_time = std::nullopt;
		}
	}

	void schedule::schedule_event()
	{
		TRY_CLAUSE
		{
			while (true)
			{
				g_schedule->tick();

				script::get_current()->yield();
			}
		}
		EXCEPT_CLAUSE
	}
}