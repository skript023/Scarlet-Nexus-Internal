#pragma once
#include "common.hpp"

namespace big
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();
		void dx_on_opened();

		void script_init();
		void script_on_tick();
		static void script_func();
	public:
		bool m_opened{};
		bool m_open_key_pressed{};
		bool m_back_key_pressed{};
		bool m_enter_key_pressed{};
		bool m_up_key_pressed{};
		bool m_down_key_pressed{};
		bool m_left_key_pressed{};
		bool m_right_key_pressed{};
	};

	inline gui g_gui;
}
