#include "ui_manager.hpp"
#include "renderer.hpp"
#include "pointers.hpp"
#include "fonts/icon_list.hpp"
#include "utility/unreal_engine_utility.hpp"

namespace big
{
	void ui_manager::tick()
	{
		std::lock_guard lock(m_Mutex);

		if (m_opened)
		{
			m_draw_base_y = g_settings.window.m_pos.y;
			draw_header();
			if (!m_submenu_stack.empty())
			{
				auto sub = m_submenu_stack.top();
				sub->reset();
				sub->execute();

				if (!m_all_tabs.empty()) draw_tabs();

				if (sub->get_num_option() != 0)
				{
					draw_rect(
						g_settings.window.m_pos.x,
						g_settings.window.m_pos.y + m_header_height + (m_all_tabs.empty() ? (m_header_height * 1.5f) : (m_submenu_bar_height * 1.5f)),
						g_settings.window.m_width,
						m_option_height * (sub->get_num_option() > g_settings.window.m_option_per_page ? g_settings.window.m_option_per_page : sub->get_num_option()),
						g_settings.window.m_option_unselected_background_color);

					std::size_t startPoint = 0;
					std::size_t endPoint = sub->get_num_option() > g_settings.window.m_option_per_page ? g_settings.window.m_option_per_page : sub->get_num_option();
					if (sub->get_num_option() > g_settings.window.m_option_per_page && sub->get_selected_option() >= g_settings.window.m_option_per_page)
					{
						startPoint = sub->get_selected_option() - g_settings.window.m_option_per_page + 1;
						endPoint = sub->get_selected_option() + 1;
					}

					for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
					{
						if (i == sub->get_selected_option())
						{
							m_current_coord = lerp(m_current_coord, m_draw_base_y + (m_option_height / 2.f), 0.2f);
							draw_rect(
								g_settings.window.m_pos.x,
								m_current_coord,
								g_settings.window.m_width,
								m_option_height,
								g_settings.window.m_option_selected_background_color);
						}

						draw_option(sub->get_option(i), i == sub->get_selected_option());
					}
					draw_scrollbar(sub->get_selected_option(), sub->get_num_option(), g_settings.window.m_option_per_page);
				}
				draw_footer();
				draw_description();
			}
		}
	}

	void ui_manager::game_tick()
	{
		// if (m_opened)
		// 	PAD::DISABLE_CONTROL_ACTION(0, 27, true); // Disable phone

		// if (m_opened/* && g_Settings.m_LockMouse*/)
		// 	PAD::DISABLE_ALL_CONTROL_ACTIONS(0); // Disable Everything
	}

	void ui_manager::check_for_input()
	{
		reset_input();

		m_open_key_pressed = unreal_engine::is_key_pressed(g_settings.window.open_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_BACK);
		m_back_key_pressed = unreal_engine::is_key_pressed(g_settings.window.back_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_B);
		m_enter_key_pressed = unreal_engine::is_key_pressed(g_settings.window.enter_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_A);
		m_up_key_pressed = unreal_engine::is_key_pressed(g_settings.window.up_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_UP);
		m_down_key_pressed = unreal_engine::is_key_pressed(g_settings.window.down_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_DOWN);
		m_left_key_pressed = unreal_engine::is_key_pressed(g_settings.window.left_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_LEFT);
		m_right_key_pressed = unreal_engine::is_key_pressed(g_settings.window.right_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_RIGHT);
		m_left_tab_pressed = unreal_engine::is_key_pressed(g_settings.window.left_tab_key) || (unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_LEFT_SHOULDER));
		m_right_tab_pressed = unreal_engine::is_key_pressed(g_settings.window.right_tab_key) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	}

	void ui_manager::handle_input()
	{
		static Timer openTimer(0ms);
		openTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_open_delay));
		if (m_open_key_pressed && openTimer.Update())
		{
			m_opened ^= true;

			if (g_settings.window.m_sounds)
				play_sound(m_opened ? "SELECT" : "BACK");
		}

		static Timer backTimer(0ms);
		backTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_back_delay));
		if (m_opened && m_back_key_pressed && backTimer.Update())
		{
			if (g_settings.window.m_sounds)
				play_sound("BACK");

			if (m_submenu_stack.size() <= 1)
			{
				return;
			}
			else
			{
				m_submenu_stack.pop();
			}
		}

		if (m_opened && !m_submenu_stack.empty())
		{
			auto sub = m_submenu_stack.top();

			static Timer enterTimer(0ms);
			enterTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_enter_delay));
			if (m_enter_key_pressed && sub->get_num_option() != 0 && enterTimer.Update())
			{
				if (g_settings.window.m_sounds)
					play_sound("SELECT");

				if (const auto opt = sub->get_option(sub->get_selected_option()))
					opt->handle_action(OptionAction::EnterPress);
			}

			static Timer upTimer(0ms);
			upTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_vectical_delay));
			if (m_up_key_pressed && sub->get_num_option() != 0 && upTimer.Update())
			{
				if (g_settings.window.m_sounds)
					play_sound("NAV_UP_DOWN");

				sub->ScrollBackward();
			}

			static Timer downTimer(0ms);
			downTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_vectical_delay));
			if (m_down_key_pressed && sub->get_num_option() != 0 && downTimer.Update())
			{
				if (g_settings.window.m_sounds)
					play_sound("NAV_UP_DOWN");

				sub->ScrollForward();
			}

			static Timer leftTimer(0ms);
			leftTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_horizontal_delay));
			if (m_left_key_pressed && sub->get_num_option() != 0 && leftTimer.Update())
			{
				if (g_settings.window.m_sounds)
					play_sound("NAV_LEFT_RIGHT");

				if (const auto opt = sub->get_option(sub->get_selected_option()))
					opt->handle_action(OptionAction::LeftPress);
			}

			static Timer rightTimer(0ms);
			rightTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_horizontal_delay));
			if (m_right_key_pressed && sub->get_num_option() != 0 && rightTimer.Update())
			{
				if (g_settings.window.m_sounds)
					play_sound("NAV_LEFT_RIGHT");

				if (const auto opt = sub->get_option(sub->get_selected_option()))
					opt->handle_action(OptionAction::RightPress);
			}
		}

		if (m_opened && !m_all_tabs.empty())
		{
			static Timer tabSwitchTimer(0ms);
			tabSwitchTimer.SetDelay(std::chrono::milliseconds(g_settings.window.m_tabbar_switch)); // Delay between switches

			if (m_right_tab_pressed && tabSwitchTimer.Update())
			{
				if (m_selected_tab < m_all_tabs.size() - 1)
				{
					m_selected_tab += 1;

					switch_to_tabmenu(m_all_tabs[m_selected_tab]->get_id());
				}
			}

			if (m_left_tab_pressed && tabSwitchTimer.Update())
			{
				if (m_selected_tab > 0)
				{
					m_selected_tab -= 1;

					switch_to_tabmenu(m_all_tabs[m_selected_tab]->get_id());
				}
			}
		}
	}

	void ui_manager::reset_input()
	{
		m_open_key_pressed = false;
		m_back_key_pressed = false;
		m_enter_key_pressed = false;
		m_up_key_pressed = false;
		m_down_key_pressed = false;
		m_left_key_pressed = false;
		m_right_key_pressed = false;
	}

	void ui_manager::draw_header()
	{
		draw_sprite(
			g_renderer->m_header,
			g_settings.window.m_pos.x,
			m_draw_base_y + (m_submenu_bar_height / 2.f),
			g_settings.window.m_width,
			m_header_height,
			m_header_background_color);

		m_draw_base_y += m_header_height;
	}

	void ui_manager::draw_tabs()
	{
		int max_visible_tabs = 3;
		size_t total_tabs = m_all_tabs.size();

		float tab_width = g_settings.window.m_width / max_visible_tabs;

		size_t start_index = (m_selected_tab > 0) ? m_selected_tab - 1 : 0;
		size_t end_index = std::min(start_index + max_visible_tabs, total_tabs);

		if (m_selected_tab == total_tabs - 1 && total_tabs > max_visible_tabs)
		{
			start_index = total_tabs - max_visible_tabs;
		}

		static float current_tab_x = 0.0f;
		float target_tab_x = g_settings.window.m_pos.x + ((m_selected_tab - start_index) * tab_width);
		current_tab_x = lerp(current_tab_x, target_tab_x, 0.1f);

		for (size_t i = start_index; i < end_index; ++i)
		{
			bool is_selected = (i == m_selected_tab);

			float tab_x = (is_selected) ? current_tab_x : g_settings.window.m_pos.x + ((i - start_index) * tab_width);

			draw_rect(tab_x, m_draw_base_y + (m_submenu_bar_height / 2), tab_width, m_submenu_bar_height,
				is_selected ? g_settings.window.m_tab_selected_color : g_settings.window.m_tab_unselected_color);

			draw_centered_text(
				m_all_tabs[i]->get_name(),
				tab_x + (tab_width / 2),
				m_draw_base_y + (m_submenu_bar_height / 2) + 11.f,
				is_selected ? g_settings.window.m_tab_selected_text_color : g_settings.window.m_tab_unselected_text_color,
				g_renderer->m_font);
		}

		float line_y = m_draw_base_y + m_submenu_bar_height;
		draw_rect(g_settings.window.m_pos.x, line_y + 19.f, g_settings.window.m_width, 2.0f, Color(255, 255, 255, 255));

		m_draw_base_y += m_submenu_bar_height;
	}

	void ui_manager::draw_submenu_bar(abstract_submenu* sub)
	{
		char leftText[64] = {};
		std::strncpy(&leftText[0], sub->get_name(), sizeof(leftText) - 1);
		std::transform(std::begin(leftText), std::end(leftText), std::begin(leftText), [](char c) { return static_cast<char>(toupper(c)); });

		char centerText[64] = {};
		std::strncpy(&centerText[0], "Ellohim", sizeof(centerText) - 1);
		std::transform(std::begin(centerText), std::end(centerText), std::begin(centerText), [](char c) { return static_cast<char>(toupper(c)); });

		char rightText[32] = {};
		std::snprintf(rightText, sizeof(rightText) - 1, "%zu / %zu", sub->get_selected_option() + 1, sub->get_num_option());

		draw_rect(
			g_settings.window.m_pos.x,
			m_draw_base_y + (m_submenu_bar_height / 2.f),
			g_settings.window.m_width, m_submenu_bar_height,
			g_settings.window.m_submenu_bar_background_color);

		float line_y = m_draw_base_y + m_submenu_bar_height;
		draw_rect(g_settings.window.m_pos.x, line_y + 19.f, g_settings.window.m_width, 2.0f, Color(255, 255, 255, 255));

		draw_left_text(
			&leftText[0],
			g_settings.window.m_pos.x + m_padding.x,
			m_draw_base_y + (m_submenu_bar_height / 2.f) + m_padding.y,
			g_settings.window.m_submenu_bar_text_color,
			g_renderer->m_font);
		draw_centered_text(
			&centerText[0],
			g_settings.window.m_pos.x + (g_settings.window.m_width / 2.f),
			m_draw_base_y + (m_submenu_bar_height / 2.f) + m_padding.y,
			g_settings.window.m_submenu_bar_text_color,
			g_renderer->m_font);
		draw_right_text(
			&rightText[0],
			g_settings.window.m_pos.x + (g_settings.window.m_width - m_padding.x),
			m_draw_base_y + (m_submenu_bar_height / 2.f) + m_padding.y,
			g_settings.window.m_submenu_bar_text_color,
			g_renderer->m_font);

		m_draw_base_y += m_submenu_bar_height;
	}

	void ui_manager::draw_option(abstract_option* opt, bool selected)
	{
		if (opt->get_flag(OptionFlag::BoolSliderInt))
		{
			draw_checkbox(
				g_settings.window.m_pos.x + m_padding.x,
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y / 2.f),
				30.f,
				m_bool_slider_int_option
			);
		}
		
		if (opt->get_flag(OptionFlag::BoolSliderFloat))
		{
			draw_checkbox(
				g_settings.window.m_pos.x + m_padding.x,
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y / 2.f),
				30.f,
				m_bool_slider_float_option
			);
		}

		if (opt->get_flag(OptionFlag::Toggle))
		{
			draw_checkbox(
				g_settings.window.m_pos.x + m_padding.x,
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y / 2.f),
				30.f,
				m_bool_option
			);
		}

		auto is_toggle = opt->get_flag(OptionFlag::Toggle) || opt->get_flag(OptionFlag::BoolSliderFloat) || opt->get_flag(OptionFlag::BoolSliderInt);

		draw_left_text(
			opt->get_left_text(),
			g_settings.window.m_pos.x + m_padding.x + (is_toggle ? 45.f : 0),
			m_draw_base_y + (m_option_height / 2.f) + m_padding.y,
			selected ? g_settings.window.m_option_selected_text_color : g_settings.window.m_option_unselected_text_color,
			g_renderer->m_font);
		draw_right_text(
			opt->get_right_text(),
			g_settings.window.m_pos.x + (g_settings.window.m_width - m_padding.x),
			m_draw_base_y + (m_option_height / 2.f) + m_padding.y,
			selected ? g_settings.window.m_option_selected_text_color : g_settings.window.m_option_unselected_text_color,
			g_renderer->m_font);

		if (opt->get_flag(OptionFlag::Enterable))
		{
			draw_right_text(
				">>",
				g_settings.window.m_pos.x + (g_settings.window.m_width - m_padding.x),
				m_draw_base_y + (m_option_height / 2.f) + m_padding.y,
				selected ? g_settings.window.m_option_selected_text_color : g_settings.window.m_option_unselected_text_color,
				g_renderer->m_font);
		}

		if (opt->get_flag(OptionFlag::BoolSliderInt))
		{
			draw_slider(
				g_settings.window.m_pos.x + (g_settings.window.m_width * 0.45f) - m_padding.x,
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y + 4.f),
				opt->get_integer(), 
				opt->get_min_integer(), 
				opt->get_max_integer());
		}
		
		if (opt->get_flag(OptionFlag::BoolSliderFloat))
		{
			draw_slider(
				g_settings.window.m_pos.x + (g_settings.window.m_width * 0.45f) - m_padding.x,
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y + 4.f),
				opt->get_float(), 
				opt->get_min_float(), 
				opt->get_max_float());
		}

		/*if (opt->get_flag(OptionFlag::Toggle))
		{
			draw_sprite(
				g_renderer->m_toggle_gpu_handle,
				g_settings.window.m_pos.x + (g_settings.window.m_width - (m_padding.x * 5.f)),
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y / 2.f),
				32.f,
				32.f,
				m_bool_option ? g_settings.window.m_toggle_on_color : g_settings.window.m_toggle_off_color);
		}*/

		m_draw_base_y += m_option_height;
	}

	void ui_manager::draw_slider(float x, float y, float current_value, float min_value, float max_value)
	{
		const float slider_width = g_settings.window.m_width * 0.55f; // Width of the slider
		const float slider_height = 10.f; // Height of the slider line
		const float knob_size = 10.0f; // Size of the knob

		// Draw the slider track
		draw_rect(
			x,
			y,
			slider_width,
			slider_height,
			g_settings.window.m_slider_track_color
		);

		// Calculate the knob position based on the current value
		float normalized_value = (current_value - min_value) / (max_value - min_value);
		float knob_x = (normalized_value * slider_width);

		draw_rect(
			x,
			y,
			knob_x,
			slider_height,
			g_settings.window.m_slider_knob_color
		);
	}

	void ui_manager::draw_scrollbar(int selected_option, int total_options, int options_per_page)
	{
		if (total_options > options_per_page)
		{
			const float content_height = m_option_height * options_per_page;
			const float scrollbar_height = (float)options_per_page / total_options * m_scrollbar_height;
			const float scrollbar_position = (float)selected_option / total_options * content_height;
			float default_position = m_tabs_stack.empty() ? (m_option_height * 0.5f) : (m_submenu_bar_height * 1.5f);

			draw_rect(
				g_settings.window.m_pos.x + (g_settings.window.m_width - m_submenu_rect_width) + 20.f, // X position of scrollbar
				g_settings.window.m_pos.y + m_header_height + default_position + scrollbar_position,                // Y position of scrollbar
				m_submenu_rect_width,                              // Width of scrollbar
				scrollbar_height,                                  // Height of scrollbar
				g_settings.window.m_submenu_rect_color,                              // Color of scrollbar
				ImGui::GetForegroundDrawList()
			);
		}
	}

	void ui_manager::draw_checkbox(float x, float y, float size, bool is_checked)
	{
		const float border_thickness = 1.0f; // Thickness of the white border

		if (!is_checked)
		{
			// Draw the border using lines
			draw_line(x, y, x + size, y, g_settings.window.m_toggle_on_color, border_thickness);                 // Top border
			draw_line(x, y, x, y + size, g_settings.window.m_toggle_on_color, border_thickness);                 // Left border
			draw_line(x + size, y, x + size, y + size, g_settings.window.m_toggle_on_color, border_thickness);   // Right border
			draw_line(x, y + size, x + size, y + size, g_settings.window.m_toggle_on_color, border_thickness);   // Bottom border
		}

		if (is_checked)
		{
			// Draw the inner checkbox background with alpha blending
			draw_rect(
				x + border_thickness,              // X position (inside the border)
				y + border_thickness,              // Y position (inside the border)
				size - (2 * border_thickness),     // Width (inside the border)
				size - (2 * border_thickness),     // Height (inside the border)
				g_settings.window.m_toggle_on_color         // Color of the checkbox background (with alpha)
			);
		}
	}

	void ui_manager::draw_footer()
	{
		// Calculate the position for the line (just above the footer)
		float line_y = m_draw_base_y + (m_footer_height / 2.f);  // Adjust the offset (2.0f) for the thickness of the line

		// Draw the line on top of the footer
		draw_rect(
			g_settings.window.m_pos.x,       // x position (same as the footer)
			line_y,        // y position (right above the footer)
			g_settings.window.m_width,       // line width (same as the footer width)
			2.0f,          // line height (adjust thickness if needed)
			Color(255, 255, 255, 255)); // You can define a separate color for the line

		draw_rect(
			g_settings.window.m_pos.x,
			m_draw_base_y + (m_footer_height / 2.f) + 2.f,
			g_settings.window.m_width,
			m_footer_height,
			g_settings.window.m_footer_background_color);

		draw_left_text(GAME_NAME, g_settings.window.m_pos.x + m_padding.x, m_draw_base_y + (m_footer_height / 2.f) + m_padding.y, Color(255, 255, 255, 255), g_renderer->m_font);
		draw_right_text("1.0", g_settings.window.m_pos.x + (g_settings.window.m_width - m_padding.x), m_draw_base_y + (m_footer_height / 2.f) + m_padding.y, Color(255, 255, 255, 255), g_renderer->m_font);

		m_draw_base_y += m_footer_height;
	}

	void ui_manager::draw_description()
	{
		const char* description{};

		if (!m_submenu_stack.empty())
		{
			auto sub = m_submenu_stack.top();
			if (sub->get_num_option())
			{
				if (auto opt = sub->get_option(sub->get_selected_option()))
				{
					description = opt->get_description();
				}
			}
		}

		if (!description || !*description)
			return;

		m_draw_base_y += description_height_padding;

		draw_rect(
			g_settings.window.m_pos.x,
			m_draw_base_y + (description_height / 2.f),
			g_settings.window.m_width,
			description_height,
			g_settings.window.description_background_color);

		draw_left_text(
			std::format("{}   {}", ICON_FA_INFO_CIRCLE, description).c_str(),
			g_settings.window.m_pos.x + m_padding.x,
			m_draw_base_y + (description_height / 2.f) + description_height_padding,
			g_settings.window.description_text_color, g_renderer->m_font);

		m_draw_base_y += description_height;
	}

	void ui_manager::draw_rect(float x, float y, float width, float height, Color color, ImDrawList* draw_list)
	{
		const auto Position = ImVec2(x, y);
		const auto Size = ImVec2(width, height);
		const auto Rect = get_rect(Position, Size);

		draw_list->AddRectFilled(Rect.Max, Rect.Min, IM_COL32(color.r, color.g, color.b, color.a));
	}

	void ui_manager::draw_sprite(ID3D11ShaderResourceView* image, float x, float y, float width, float height, Color color, ImDrawList* drawlist)
	{
		const auto Position = ImVec2(x, y);
		const auto Size = ImVec2(width, height);
		const auto Rect = get_rect(Position, Size);

		drawlist->AddImage((void*)image,
			Rect.Min,
			Rect.Max,
			ImVec2(0, 0), ImVec2(1, 1),
			IM_COL32(color.r, color.g, color.b, color.a)
		);
	}

	void ui_manager::draw_left_text(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list)
	{
		const auto Position = ImVec2(x, y);
		ImGui::PushFont(font);
		draw_list->AddText(Position, IM_COL32(color.r, color.g, color.b, color.a), text);
		ImGui::PopFont();
	}

	void ui_manager::draw_centered_text(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list)
	{
		const auto Position = ImVec2((x - (ImGui::CalcTextSize(text).x / 2.f)), y);
		ImGui::PushFont(font);
		draw_list->AddText(Position, IM_COL32(color.r, color.g, color.b, color.a), text);
		ImGui::PopFont();
	}

	void ui_manager::draw_right_text(const char* text, float x, float y, Color color, ImFont* font, ImDrawList* draw_list)
	{
		const auto Position = ImVec2((x - ImGui::CalcTextSize(text).x), y);
		ImGui::PushFont(font);
		draw_list->AddText(Position, IM_COL32(color.r, color.g, color.b, color.a), text);
		ImGui::PopFont();
	}

	Vector2 ui_manager::get_sprite_scale(float size)
	{
		int x = g_pointers->m_resolution->x;
		int y = g_pointers->m_resolution->y;

		Vector2 sz = { (static_cast<float>(y) / static_cast<float>(x)) * size, size };

		return sz; 
	}

	ImRect ui_manager::get_rect(ImVec2 pos, ImVec2 size)
	{
		const auto ItemSize = ImGui::CalcItemSize(ImVec2(size.x, size.y), 0.0f, 0.0f);
		return ImRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + ItemSize.x, pos.y + ItemSize.y));
	}

	void ui_manager::play_sound(const char* name)
	{
		
	}

	void ui_manager::draw_stroke_text(float x, float y, Color color, std::string_view str)
	{
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1.f), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f / 255.0f, 1.f / 255.0f, 1.f / 255.0f, 255 / 255.0f)), str.data());
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y + 1.f), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f / 255.0f, 1.f / 255.0f, 1.f / 255.0f, 255 / 255.0f)), str.data());
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x - 1.f, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f / 255.0f, 1.f / 255.0f, 1.f / 255.0f, 255 / 255.0f)), str.data());
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x + 1.f, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f / 255.0f, 1.f / 255.0f, 1.f / 255.0f, 255 / 255.0f)), str.data());
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)), str.data());
	}

	void ui_manager::draw_filled_rect(float x, float y, float w, float h, Color color)
	{
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)), 0, 0);
	}

	void ui_manager::draw_circle_filled(float x, float y, float radius, Color color)
	{
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)));
	}

	void ui_manager::draw_circle(float x, float y, float radius, Color color, int segments)
	{
		ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)), segments);
	}

	void ui_manager::draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color color, float thickne)
	{
		ImGui::GetForegroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)), thickne);
	}

	void ui_manager::draw_triangle_filled(float x1, float y1, float x2, float y2, float x3, float y3, Color color)
	{
		ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)));
	}

	void ui_manager::draw_line(float x1, float y1, float x2, float y2, Color color, float thickness)
	{
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)), thickness);
	}

	void ui_manager::draw_corner_box(float x, float y, float w, float h, float borderPx, Color color)
	{
		draw_filled_rect(x + borderPx, y, w / 3, borderPx, color);
		draw_filled_rect(x + w - w / 3 + borderPx, y, w / 3, borderPx, color);
		draw_filled_rect(x, y, borderPx, h / 3, color);
		draw_filled_rect(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);
		draw_filled_rect(x + borderPx, y + h + borderPx, w / 3, borderPx, color);
		draw_filled_rect(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color);
		draw_filled_rect(x + w + borderPx, y, borderPx, h / 3, color);
		draw_filled_rect(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);
	}

	void ui_manager::draw_cube(ImVec2 const& screen_location, float yaw, ImVec2 const& size, Color colour)
	{
		auto draw_list = ImGui::GetForegroundDrawList();
		auto color = ImGui::ColorConvertFloat4ToU32(ImVec4(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f));

		ImVec2 vertices[8] = {
			ImVec2(screen_location.x, screen_location.y),
			ImVec2(screen_location.x + size.x, screen_location.y),
			ImVec2(screen_location.x, screen_location.y + size.y),
			ImVec2(screen_location.x + size.x, screen_location.y + size.y),
			ImVec2(screen_location.x + size.x * 0.5f, screen_location.y + size.y * 0.5f),
			ImVec2(screen_location.x + size.x * 1.5f, screen_location.y + size.y * 0.5f),
			ImVec2(screen_location.x + size.x * 0.5f, screen_location.y + size.y * 1.5f),
			ImVec2(screen_location.x + size.x * 1.5f, screen_location.y + size.y * 1.5f)
		};

		ImVec2 center = ImVec2(screen_location.x + size.x / 2, screen_location.y + size.y / 2);

		for (int i = 0; i < 8; ++i)
		{
			vertices[i] = rotate_point_2d(vertices[i], center, yaw);
		}

		draw_list->AddLine(vertices[0], vertices[1], color);
		draw_list->AddLine(vertices[1], vertices[3], color);
		draw_list->AddLine(vertices[3], vertices[2], color);
		draw_list->AddLine(vertices[2], vertices[0], color);

		draw_list->AddLine(vertices[4], vertices[5], color);
		draw_list->AddLine(vertices[5], vertices[7], color);
		draw_list->AddLine(vertices[7], vertices[6], color);
		draw_list->AddLine(vertices[6], vertices[4], color);

		draw_list->AddLine(vertices[0], vertices[4], color);
		draw_list->AddLine(vertices[1], vertices[5], color);
		draw_list->AddLine(vertices[2], vertices[6], color);
		draw_list->AddLine(vertices[3], vertices[7], color);
	}

	ImVec2 ui_manager::rotate_point_2d(const ImVec2& point, const ImVec2& center, float yaw)
	{
		float s = sinf(yaw);
		float c = cosf(yaw);

		ImVec2 translated = ImVec2(point.x - center.x, point.y - center.y);

		ImVec2 rotated = ImVec2(translated.x * c - translated.y * s, translated.x * s + translated.y * c);

		return ImVec2(rotated.x + center.x, rotated.y + center.y);
	}

}