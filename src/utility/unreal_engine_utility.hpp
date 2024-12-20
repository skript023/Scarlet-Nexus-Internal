#pragma once
#include "class/engine_sdk.hpp"
#include <pointers.hpp>

namespace big::unreal_engine
{
	inline UserParamManager* get_user_params()
	{
		if (auto m_unreal_engine = *g_pointers->m_engine)
		{
			if (auto m_instance = m_unreal_engine->m_game)
			{
				return m_instance->m_manager[0x10];
			}
		}

		return nullptr;
	}

	inline Character* get_character_base()
	{
		if (auto m_unreal_engine = *g_pointers->m_engine)
		{
			if (auto unreal_engine_game = m_unreal_engine->m_game)
			{
				if (auto local_player = unreal_engine_game->m_character_base[0])
				{
					if (auto param_base = local_player->m_get_character)
					{
						if (auto character = param_base->m_character)
						{
							return character;
						}
					}
				}
			}
		}

		return nullptr;
	}

	inline bool is_key_pressed(std::uint16_t key)
	{
		if (GetForegroundWindow() == g_pointers->m_hwnd)
		{
			if (GetAsyncKeyState(key) & 0x8000)
			{
				return true;
			}
		}

		return false;
	}

	inline bool is_controller_pressed(std::uint16_t button)
	{
		XINPUT_STATE state;
		// Zero out the state structure
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller (controller 0)
		if (XInputGetState(0, &state) == ERROR_SUCCESS)
		{
			// Check if the specific button is pressed
			return (state.Gamepad.wButtons & button) != 0;
		}

		// Controller is not connected
		return false;
	}
}