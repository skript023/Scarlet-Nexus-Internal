#pragma once
#include "class/engine_sdk.hpp"
#include <pointers.hpp>

namespace big::unreal_engine
{
	inline UserParams* get_user_params()
	{
		if (auto m_unreal_engine = *g_pointers->m_engine)
		{
			if (auto unreal_engine_game = m_unreal_engine->m_game)
			{
				if (auto user_params = unreal_engine_game->m_user_params)
				{
					if (auto param_base = user_params->m_param_base)
					{
						return param_base;
					}
				}
			}
		}

		return nullptr;
	}

	inline CharacterParams* get_character_base()
	{
		if (auto m_unreal_engine = *g_pointers->m_engine)
		{
			if (auto unreal_engine_game = m_unreal_engine->m_game)
			{
				if (auto character = unreal_engine_game->m_character_base)
				{
					if (auto param_base = character->m_param_base)
					{
						if (auto get_character = param_base->m_get_character)
						{
							if (auto character = get_character->m_character)
							{
								return character;
							}
						}
					}
				}
			}
		}

		return nullptr;
	}


}