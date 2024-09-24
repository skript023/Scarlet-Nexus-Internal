#pragma once
#include "base_option.hpp"

namespace big
{
	class reguler_option : public base_option<reguler_option>
	{
	public:
		explicit reguler_option() = default;
		explicit reguler_option(const char* text, const char* description = nullptr, std::function<void()> action = [] {})
		{
			set_left_text(text);
			if (description)
				set_description(description);
			set_action(std::move(action));
		}

		~reguler_option() noexcept = default;
		reguler_option(reguler_option const&) = default;
		reguler_option& operator=(reguler_option const&) = default;
		reguler_option(reguler_option&&) = default;
		reguler_option& operator=(reguler_option&&) = default;
	};
}