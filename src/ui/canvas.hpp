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

	class canvas
	{
		std::vector<std::unique_ptr<abstract_submenu>> m_all_tabs;
		std::stack<abstract_submenu*, std::vector<abstract_submenu*>> m_tabs_stack;
		static canvas& instance()
		{
			static canvas i{};

			return i;
		}
	public:
		static void tick() { instance().tick_impl(); }
		static void check_for_input() { instance().check_for_input_impl(); }
		static void handle_input() { instance().handle_input_impl(); }
		template <typename SubmenuType, typename ...TArgs>
		static void add_submenu(TArgs&&... args) { instance().add_submenu_impl<SubmenuType>(std::forward<TArgs>(args)...); }
		template <typename TabmenuType, typename... Args>
		static void add_tab(Args&&... args) { instance().add_tab_impl<TabmenuType>(std::forward<Args>(args)...); }
		static void set_bool_option(bool* v) { instance().m_bool_option = *v; }
		static void set_bool_slider_float(bool* v) { instance().m_bool_slider_float_option = *v; }
		static void set_bool_slider_int(bool* v) { instance().m_bool_slider_int_option = *v; }
		static void switch_to_submenu(std::uint32_t id) { instance().switch_to_submenu_impl(id); }
		static void switch_to_tabmenu(std::uint32_t id) { instance().switch_to_tabmenu_impl(id); }
		static bool is_opened() { return instance().m_opened; }
	private:
		explicit canvas() = default;
		~canvas() noexcept = default;

		canvas(canvas const&) = delete;
		canvas& operator=(canvas const&) = delete;

		canvas(canvas&&) = delete;
		canvas& operator=(canvas&&) = delete;

		template <typename SubmenuType, typename ...TArgs>
		void add_submenu_impl(TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_submenu_stack.empty())
			{
				m_submenu_stack.push(sub.get());
			}

			m_all_submenu.push_back(std::move(sub));
		}

		template <typename TabmenuType, typename... Args>
		void add_tab_impl(Args&&... args)
		{
			auto sub = std::make_unique<TabmenuType>(std::forward<Args>(args)...);
			if (m_submenu_stack.empty())
			{
				m_submenu_stack.push(sub.get());
			}

			m_all_tabs.push_back(std::move(sub));
		}

		void switch_to_submenu_impl(std::uint32_t id)
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

		void switch_to_tabmenu_impl(std::uint32_t id)
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

		void tick_impl();
		void game_tick();
	public:
		void draw_stroke_text_impl(float x, float y, Color color, std::string_view str);
		void draw_filled_rect_impl(float x, float y, float w, float h, Color color);
		void draw_circle_filled_impl(float x, float y, float radius, Color color);
		void draw_circle_impl(float x, float y, float radius, Color color, int segments);
		void draw_triangle_impl(float x1, float y1, float x2, float y2, float x3, float y3, Color color, float thickne);
		void draw_triangle_filled_impl(float x1, float y1, float x2, float y2, float x3, float y3, Color color);
		void draw_line_impl(float x1, float y1, float x2, float y2, Color color, float thickness);
		void draw_corner_box_impl(float x, float y, float w, float h, float borderPx, Color color);
		void draw_cube_impl(ImVec2 const& screen_location, float yaw, ImVec2 const& size, Color colour);
		ImVec2 rotate_point_2d_impl(const ImVec2& point, const ImVec2& center, float yaw);
	public:
		std::mutex m_mutex;

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

		void check_for_input_impl();
		void handle_input_impl();
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
		void draw_sprite(D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle, float x, float y, float width, float height, Color color, ImDrawList* drawlist = ImGui::GetBackgroundDrawList());
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
}