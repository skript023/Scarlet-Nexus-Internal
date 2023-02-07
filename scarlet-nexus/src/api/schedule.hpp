#pragma once

namespace big
{
	class schedule
	{
	public:
		explicit schedule();
		virtual ~schedule() noexcept;

		schedule(schedule const& that) = delete;
		schedule& operator=(schedule const& that) = delete;
		schedule(schedule&& that) = delete;
		schedule& operator=(schedule&& that) = delete;

		void tick();
		schedule* task(std::function<void()> job);
		void duration(std::optional<std::chrono::high_resolution_clock::duration> time);
		static void schedule_event();
	private:
		std::stack<std::function<void()>> m_jobs;
		std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;
	};

	inline schedule* g_schedule;
}