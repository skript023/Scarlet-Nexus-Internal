#pragma once
#include "common.hpp"
#include "class/vector.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include "abstract_submenu.hpp"
#include "tabs_menu.hpp"

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

	class ui_manager
	{
		std::vector<std::unique_ptr<abstract_submenu>> m_all_tabs;
		std::stack<abstract_submenu*, std::vector<abstract_submenu*>> m_tabs_stack;
	public:
		explicit ui_manager() = default;
		~ui_manager() noexcept = default;

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

		template <typename SubmenuType, typename... Args>
		void add_tab(Args&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<Args>(args)...);
			if (m_submenu_stack.empty())
			{
				m_submenu_stack.push(sub.get());
			}

			m_all_tabs.push_back(std::move(sub));
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

		void switch_to_tabmenu(std::uint32_t id)
		{
			for (auto&& sub : m_all_tabs)
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
		void draw_stroke_text(float x, float y, Color color, std::string_view str);
		void draw_filled_rect(float x, float y, float w, float h, Color color);
		void draw_circle_filled(float x, float y, float radius, Color color);
		void draw_circle(float x, float y, float radius, Color color, int segments);
		void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color color, float thickne);
		void draw_triangle_filled(float x1, float y1, float x2, float y2, float x3, float y3, Color color);
		void draw_line(float x1, float y1, float x2, float y2, Color color, float thickness);
		void draw_corner_box(float x, float y, float w, float h, float borderPx, Color color);
		void draw_cube(ImVec2 const& screen_location, float yaw, ImVec2 const& size, Color colour);
		ImVec2 rotate_point_2d(const ImVec2& point, const ImVec2& center, float yaw);
	public:
		std::mutex m_Mutex;

		bool m_opened = true;
		bool m_bool_option = false;
		bool m_bool_slider_int_option = false;
		bool m_bool_slider_float_option = false;

		// Offsets
		Vector2 m_padding = { 7.f, 11.f };

		// Header
		float m_header_height = 100.f;
		Color m_header_background_color{ 255, 255, 255, 255 };

		// Submenu bar
		float m_submenu_bar_height = 45.f;

		// Tabbar
		size_t m_selected_tab{ 0 };
		

		// Options
		float m_option_height = 45.f;
		float m_submenu_rect_width = 10.f;

		// Smooth Scrolling
		float lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}

		//Scrollbar
		float m_scrollbar_height = 100.f;

		float m_current_coord = g_settings.window.m_pos.y;
		float m_current_tab_coord = g_settings.window.m_pos.y;

		// Footer
		float m_footer_height = 45.f;

		// Description
		float description_height_padding = 10.f;
		float description_height = 40.f;
		
		float description_padding = 21.f;
		Color description_sprite_color{ 255, 255, 255, 255 };

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
		bool m_left_tab_pressed = false;
		bool m_right_tab_pressed = false;
		void reset_input();

		float m_draw_base_y{};
		void draw_header();
		void draw_tabs();
		void draw_submenu_bar(abstract_submenu* sub);
		void draw_option(abstract_option* opt, bool selected);
		void draw_slider(float x, float y, float current_value, float min_value, float max_value);
		void draw_scrollbar(int selected_option, int total_options, int options_per_page);
		void draw_checkbox(float x, float y, float size, bool is_checked);
		void draw_footer();
		void draw_description();

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

	inline ui_manager g_ui_manager{};
}