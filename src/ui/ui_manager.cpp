#include "ui_manager.hpp"
#include "renderer.hpp"
#include "pointers.hpp"
#include "utility/unreal_engine_utility.hpp"

namespace big
{
	ui_manager::ui_manager()
	{
		g_ui_manager = this;
	}

	ui_manager::~ui_manager() noexcept
	{
		g_ui_manager = nullptr;
	}

	void ui_manager::tick()
	{
		std::lock_guard lock(m_Mutex);

		if (m_opened)
		{
			m_draw_base_y = m_pos.y;
			draw_header();
			if (!m_submenu_stack.empty())
			{
				auto sub = m_submenu_stack.top();
				sub->reset();
				sub->execute();

				draw_submenu_bar(sub);
				if (sub->get_num_option() != 0)
				{
					draw_rect(
						m_pos.x,
						m_pos.y + m_header_height + (m_submenu_bar_height * 1.5f),
						m_width,
						m_option_height * (sub->get_num_option() > m_option_per_page ? m_option_per_page : sub->get_num_option()),
						m_option_unselected_background_color);

					std::size_t startPoint = 0;
					std::size_t endPoint = sub->get_num_option() > m_option_per_page ? m_option_per_page : sub->get_num_option();
					if (sub->get_num_option() > m_option_per_page && sub->get_selected_option() >= m_option_per_page)
					{
						startPoint = sub->get_selected_option() - m_option_per_page + 1;
						endPoint = sub->get_selected_option() + 1;
					}

					for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
					{
						if (i == sub->get_selected_option())
						{
							m_current_coord = lerp(m_current_coord, m_draw_base_y + (m_option_height / 2.f), 0.2f);
							draw_rect(
								m_pos.x,
								m_current_coord,
								m_width,
								m_option_height,
								m_option_selected_background_color);
						}

						draw_option(sub->get_option(i), i == sub->get_selected_option());
					}
				}
			}

			draw_footer();
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

		m_open_key_pressed = unreal_engine::is_key_pressed(VK_INSERT) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_BACK);
		m_back_key_pressed = unreal_engine::is_key_pressed(VK_NUMPAD0) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_B);
		m_enter_key_pressed = unreal_engine::is_key_pressed(VK_NUMPAD5) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_A);
		m_up_key_pressed = unreal_engine::is_key_pressed(VK_NUMPAD8) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_UP);
		m_down_key_pressed = unreal_engine::is_key_pressed(VK_NUMPAD2) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_DOWN);
		m_left_key_pressed = unreal_engine::is_key_pressed(VK_NUMPAD4) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_LEFT);
		m_right_key_pressed = unreal_engine::is_key_pressed(VK_NUMPAD6) || unreal_engine::is_controller_pressed(XINPUT_GAMEPAD_DPAD_RIGHT);
	}

	void ui_manager::handle_input()
	{
		static Timer openTimer(0ms);
		openTimer.SetDelay(std::chrono::milliseconds(m_open_delay));
		if (m_open_key_pressed && openTimer.Update())
		{
			m_opened ^= true;

			if (m_sounds)
				play_sound(m_opened ? "SELECT" : "BACK");
		}

		static Timer backTimer(0ms);
		backTimer.SetDelay(std::chrono::milliseconds(m_back_delay));
		if (m_opened && m_back_key_pressed && backTimer.Update())
		{
			if (m_sounds)
				play_sound("BACK");

			if (m_submenu_stack.size() <= 1)
				m_opened = false;
			else
				m_submenu_stack.pop();
		}

		if (m_opened && !m_submenu_stack.empty())
		{
			auto sub = m_submenu_stack.top();

			static Timer enterTimer(0ms);
			enterTimer.SetDelay(std::chrono::milliseconds(m_enter_delay));
			if (m_enter_key_pressed && sub->get_num_option() != 0 && enterTimer.Update())
			{
				if (m_sounds)
					play_sound("SELECT");

				if (const auto opt = sub->get_option(sub->get_selected_option()))
					opt->handle_action(OptionAction::EnterPress);
			}

			static Timer upTimer(0ms);
			upTimer.SetDelay(std::chrono::milliseconds(m_vectical_delay));
			if (m_up_key_pressed && sub->get_num_option() != 0 && upTimer.Update())
			{
				if (m_sounds)
					play_sound("NAV_UP_DOWN");

				sub->ScrollBackward();
			}

			static Timer downTimer(0ms);
			downTimer.SetDelay(std::chrono::milliseconds(m_vectical_delay));
			if (m_down_key_pressed && sub->get_num_option() != 0 && downTimer.Update())
			{
				if (m_sounds)
					play_sound("NAV_UP_DOWN");

				sub->ScrollForward();
			}

			static Timer leftTimer(0ms);
			leftTimer.SetDelay(std::chrono::milliseconds(m_horizontal_delay));
			if (m_left_key_pressed && sub->get_num_option() != 0 && leftTimer.Update())
			{
				if (m_sounds)
					play_sound("NAV_LEFT_RIGHT");

				if (const auto opt = sub->get_option(sub->get_selected_option()))
					opt->handle_action(OptionAction::LeftPress);
			}

			static Timer rightTimer(0ms);
			rightTimer.SetDelay(std::chrono::milliseconds(m_horizontal_delay));
			if (m_right_key_pressed && sub->get_num_option() != 0 && rightTimer.Update())
			{
				if (m_sounds)
					play_sound("NAV_LEFT_RIGHT");

				if (const auto opt = sub->get_option(sub->get_selected_option()))
					opt->handle_action(OptionAction::RightPress);
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
			m_pos.x,
			m_draw_base_y + (m_submenu_bar_height / 2.f),
			m_width,
			m_header_height,
			m_header_background_color);

		m_draw_base_y += m_header_height;
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
			m_pos.x,
			m_draw_base_y + (m_submenu_bar_height / 2.f),
			m_width, m_submenu_bar_height,
			m_submenu_bar_background_color);
		draw_left_text(
			&leftText[0],
			m_pos.x + m_padding.x,
			m_draw_base_y + (m_submenu_bar_height / 2.f) + m_padding.y,
			m_submenu_bar_text_color,
			g_renderer->m_ui_manager_font);
		draw_centered_text(
			&centerText[0],
			m_pos.x + (m_width / 2.f),
			m_draw_base_y + (m_submenu_bar_height / 2.f) + m_padding.y,
			m_submenu_bar_text_color,
			g_renderer->m_ui_manager_font);
		draw_right_text(
			&rightText[0],
			m_pos.x + (m_width - m_padding.x),
			m_draw_base_y + (m_submenu_bar_height / 2.f) + m_padding.y,
			m_submenu_bar_text_color,
			g_renderer->m_ui_manager_font);

		m_draw_base_y += m_submenu_bar_height;
	}

	void ui_manager::draw_option(abstract_option* opt, bool selected)
	{
		draw_left_text(
			opt->get_left_text(),
			m_pos.x + m_padding.x,
			m_draw_base_y + (m_option_height / 2.f) + m_padding.y,
			selected ? m_option_selected_text_color : m_option_unselected_text_color,
			g_renderer->m_ui_manager_font);
		draw_right_text(
			opt->get_right_text(),
			m_pos.x + (m_width - m_padding.x),
			m_draw_base_y + (m_option_height / 2.f) + m_padding.y,
			selected ? m_option_selected_text_color : m_option_unselected_text_color,
			g_renderer->m_ui_manager_font);

		if (opt->get_flag(OptionFlag::Enterable))
		{
			draw_rect(
				m_pos.x + (m_width - m_submenu_rect_width),
				m_draw_base_y + (m_option_height / 2.f),
				m_submenu_rect_width,
				m_option_height,
				m_submenu_rect_color,
				ImGui::GetForegroundDrawList());
		}

		if (opt->get_flag(OptionFlag::Toggle))
		{
			draw_sprite(
				g_renderer->m_toggle,
				m_pos.x + (m_width - (m_padding.x * 5.f)),
				m_draw_base_y + (m_option_height / 2.f) + (m_padding.y / 2.f),
				32.f,
				32.f,
				m_bool_option ? m_toggle_on_color : m_toggle_off_color);
		}

		m_draw_base_y += m_option_height;
	}

	void ui_manager::draw_footer()
	{
		draw_rect(
			m_pos.x,
			m_draw_base_y + (m_FooterHeight / 2.f),
			m_width,
			m_FooterHeight,
			m_FooterBackgroundColor);

		m_draw_base_y += m_FooterHeight;
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
		//GRAPHICS::GET_ACTUAL_SCREEN_RESOLUTION(&x, &y);
		Vector2 sz = { (static_cast<float>(y) / static_cast<float>(x)) * size, size };

		LOG(HACKER) << "Size: " << sz.x << "x" << sz.y << "y";

		return sz; 
	}

	ImRect ui_manager::get_rect(ImVec2 pos, ImVec2 size)
	{
		const auto ItemSize = ImGui::CalcItemSize(ImVec2(size.x, size.y), 0.0f, 0.0f);
		return ImRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + ItemSize.x, pos.y + ItemSize.y));
	}

	void ui_manager::play_sound(const char* name)
	{
		// g_fiber_pool->queue_job([name] {
		// 	AUDIO::PLAY_SOUND_FRONTEND(-1, name, "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		// });
	}
}