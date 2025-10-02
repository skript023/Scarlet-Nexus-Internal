#pragma once
namespace big
{
	enum class NotificationType {
		INFO,
		SUCCESS,
		PROTECTED,
		WARNING,
		DANGER,
	};

	struct notification_data
	{
		NotificationType type;
		const std::string title;
		const std::string message;
		const std::chrono::time_point<std::chrono::system_clock> created_on;
		const float destroy_in;
		float alpha;
	};

	class notification final
	{
		std::unordered_map<std::size_t, notification_data> notifications;

		notification() = default;
		virtual ~notification() noexcept = default;

		void push(notification_data);
		void push(std::string, std::string);
		void push_warning(std::string, std::string);
		void push_error(std::string, std::string);
		void push_success(std::string, std::string);

		std::vector<notification_data> get_impl();

		std::map<NotificationType, ImVec4> notification_colors = {
			{NotificationType::INFO, ImVec4(0.80f, 0.80f, 0.83f, 1.00f)},
			{NotificationType::PROTECTED, ImVec4(0.29f, 0.34f, 0.69f, 1.00f)},
			{NotificationType::SUCCESS, ImVec4(0.29f, 0.69f, 0.34f, 1.00f)},
			{NotificationType::WARNING, ImVec4(0.69f ,0.49f, 0.29f, 1.00f) },
			{NotificationType::DANGER, ImVec4(0.69f, 0.29f , 0.29f, 1.00f)},
		};

		static notification& instance()
		{
			static notification instance;
			return instance;
		}
	public:
		static void info(std::string, std::string);
		static void success(std::string, std::string);
		static void warning(std::string, std::string);
		static void error(std::string, std::string);

		static void protection(std::string);
		static void report(std::string);

		static ImVec4 get_color(NotificationType type)
		{
			return instance().notification_colors[type];
		}
		static std::vector<notification_data> get() { return instance().get_impl(); };
	};
}