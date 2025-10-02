#include "notification_service.hpp"
#include "fonts/font_list.hpp"
#include "fonts/icon_list.hpp"

namespace big
{
	void notification::push(notification_data n)
	{
		this->notifications.emplace(std::hash<std::string>{}(n.message + n.title), n);
	}

	void notification::push(std::string title, std::string message)
	{
		this->push({ NotificationType::INFO, ICON_FA_INFO_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f });
	}

	void notification::push_warning(std::string title, std::string message)
	{
		this->push({ NotificationType::WARNING, ICON_FA_EXCLAMATION_TRIANGLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f });
	}

	void notification::push_error(std::string title, std::string message)
	{
		this->push({ NotificationType::DANGER, ICON_FA_TIMES_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f });
	}

	void notification::push_success(std::string title, std::string message)
	{
		this->push({ NotificationType::SUCCESS, ICON_FA_CHECK_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f });
	}

	void notification::info(std::string title, std::string message)
	{
		instance().push({NotificationType::INFO, ICON_FA_INFO_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f});
	}

	void notification::success(std::string, std::string)
	{
	}

	void notification::warning(std::string title, std::string message)
	{
		instance().push({ NotificationType::WARNING, ICON_FA_EXCLAMATION_TRIANGLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f });
	}

	void notification::error(std::string title, std::string message)
	{
		instance().push({ NotificationType::DANGER, ICON_FA_TIMES_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f , 1.f });
	}

	void notification::protection(std::string message)
	{
		instance().push({ NotificationType::PROTECTED, ICON_FA_SHIELD_ALT " Illegal Event Protection", message, std::chrono::system_clock::now(), 3000.f, 1.f });
	}

	void notification::report(std::string message)
	{
		instance().push({ NotificationType::PROTECTED, ICON_FA_USER_SHIELD " Report Protection", message, std::chrono::system_clock::now(), 3000.f, 1.f });
	}

	std::vector<notification_data> notification::get_impl()
	{
		std::vector<notification_data> notifications_to_sent;
		std::vector<std::size_t> to_remove;
		for (auto& n : this->notifications)
		{
			std::chrono::time_point<std::chrono::system_clock> curTime = std::chrono::system_clock::now();
			const float time_diff = (float)std::chrono::duration_cast<std::chrono::milliseconds>(curTime - n.second.created_on).count();
			n.second.alpha = 1;
			if (n.second.destroy_in <= time_diff) {
				n.second.alpha = 1.f - ((time_diff - n.second.destroy_in) / 600);
				n.second.alpha = n.second.alpha < 0.f ? 0.f : n.second.alpha;
			}

			if (n.second.alpha > 0.f)
				notifications_to_sent.push_back(n.second);
			else to_remove.push_back(n.first);
		}
		for (std::size_t k : to_remove)
			this->notifications.erase(k);

		return notifications_to_sent;
	}
}