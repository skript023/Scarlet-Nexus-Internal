#pragma once
#include "class/engine_sdk.hpp"
#include <pointers.hpp>

namespace big::unreal_engine
{
	inline UserParamManager* get_user_params()
	{
		if (auto m_unreal_engine = *g_pointers->m_engine)
		{
			if (auto unreal_engine_game = m_unreal_engine->m_game)
			{
				return unreal_engine_game->m_manager[0x80];
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


}