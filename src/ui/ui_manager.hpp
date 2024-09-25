#pragma once
#include "common.hpp"
#include "class/vector.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include "abstract_submenu.hpp"

namespace big
{
	class Timer
	{
	public:
		explicit Timer(std::chrono::milliseconds delay) :
			m_Timer(std::chrono::high_resolution_clock::now()),
			m_Delay(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(delay))
		{
		}

		bool Update()
		{
			auto now = std::chrono::high_resolution_clock::now();
			if ((now.time_since_epoch() - m_Timer.time_since_epoch()).count() >= m_Delay.count())
			{
				m_Timer = now;
				return true;
			}

			return false;
		}

		void SetDelay(std::chrono::milliseconds delay)
		{
			m_Delay = delay;
		}
	private:
		std::chrono::high_resolution_clock::time_point m_Timer;
		std::chrono::high_resolution_clock::duration m_Delay;
	};

	struct Color
	{
		Color(std::uint8_t r = 0, std::uint8_t g = 0, std::uint8_t b = 0, std::uint8_t a = 255) :
			r(r), g(g), b(b), a(a)
		{}

		std::uint8_t r;
		std::uint8_t g;
		std::uint8_t b;
		std::uint8_t a;
	};
	
	static_assert(sizeof(Color) == sizeof(std::uint8_t) * 4);

	class ui_manager
	{
	public:
		explicit ui_manager();
		~ui_manager() noexcept;

		ui_manager(ui_manager const&) = delete;
		ui_manager& operator=(ui_manager const&) = delete;

		ui_manager(ui_manager&&) = delete;
		ui_manager& operator=(ui_manager&&) = delete;

		template <typename SubmenuType, typename ...TArgs>
		void add_submenu(TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_submenu_stack.empty())
			{
				m_submenu_stack.push(sub.get());
			}

			m_all_submenu.push_back(std::move(sub));
		}

		void switch_to_submenu(std::uint32_t id)
		{
			for (auto&& sub : m_all_submenu)
			{
				if (sub->get_id() == id)
				{
					m_submenu_stack.push(sub.get());

					return;
				}
			}
		}

		void tick();
		void game_tick();
	public:
		std::mutex m_Mutex;

		bool m_opened = true;
		bool m_mouse_locked = false;
		Vector2 m_pos = { 25.f, 25.f };
		float m_width = 480.f;
		std::size_t m_option_per_page = 11;
		bool m_sounds = true;
		bool m_bool_option = false;

		// Offsets
		Vector2 m_padding = { 7.f, 11.f };

		// Input
		std::int32_t m_open_delay = 200;
		std::int32_t m_back_delay = 300;
		std::int32_t m_enter_delay = 300;
		std::int32_t m_vectical_delay = 120;
		std::int32_t m_horizontal_delay = 120;

		// Header
		float m_header_height = 100.f;
		Color m_header_background_color{ 255, 255, 255, 255 };

		// Submenu bar
		float m_submenu_bar_height = 45.f;
		Color m_submenu_bar_background_color{ 24, 24, 24, 255 };
		Color m_submenu_bar_text_color{ 153, 153, 155, 255 };

		// Options
		float m_option_height = 45.f;
		float m_submenu_rect_width = 10.f;
		Color m_toggle_on_color{ 124, 201, 252, 255 };
		Color m_toggle_off_color{ 46, 46, 46, 255 };
		Color m_submenu_rect_color{ 94, 94, 94, 180 };
		Color m_option_selected_text_color{ 10, 10, 10, 255 };
		Color m_option_unselected_text_color{ 153, 153, 153, 255 };
		Color m_option_selected_background_color{ 178, 178, 178, 255 };
		Color m_option_unselected_background_color{ 0, 0, 0, 160 };

		// Smooth Scrolling
		float lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}

		float m_current_coord = m_pos.y;

		// Footer
		float m_FooterHeight = 45.f;
		Color m_FooterBackgroundColor{ 24, 24, 24, 255 };

		void check_for_input();
		void handle_input();
	private:
		bool m_open_key_pressed = false;
		bool m_back_key_pressed = false;
		bool m_enter_key_pressed = false;
		bool m_up_key_pressed = false;
		bool m_down_key_pressed = false;
		bool m_left_key_pressed = false;
		bool m_right_key_pressed = false;
		void reset_input();

		float m_draw_base_y{};
		void draw_header();
		void draw_submenu_bar(abstract_submenu* sub);
		void draw_option(abstract_option* opt, bool selected);
		void draw_footer();

		void draw_rect(float x, float y, float width, float height, Color color, ImDrawList* draw_list = ImGui::GetBackgroundDrawList());
		void draw_sprite(ID3D11ShaderResourceView* image, float x, float y, float width, float height, Color color, ImDrawList* drawlist = ImGui::GetBackgroundDrawList());
		void draw_left_text(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list = ImGui::GetForegroundDrawList());
		void draw_centered_text(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list = ImGui::GetForegroundDrawList());
		void draw_right_text(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list = ImGui::GetForegroundDrawList());
		Vector2 get_sprite_scale(float size);

		// Helpers
		ImRect get_rect(ImVec2 pos, ImVec2 size);
		void play_sound(const char* name);

		std::vector<std::unique_ptr<abstract_submenu>> m_all_submenu;
		std::stack<abstract_submenu*, std::vector<abstract_submenu*>> m_submenu_stack;
	};

	inline ui_manager* g_ui_manager{};
}