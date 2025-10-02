#pragma once

namespace big
{
	struct Hardware
	{
		Hardware() = default;

		Hardware(std::string const& os, std::string const& cpu, std::string const& gpu, std::string const& motherboard) :
			os(os),
			cpu(cpu),
			gpu(gpu),
			motherboard(motherboard)
		{}

		Hardware(const Hardware&) = default;
		Hardware& operator=(const Hardware&) = default;
		Hardware(Hardware&&) noexcept = default;
		Hardware& operator=(Hardware&&) noexcept = default;

		std::string os;
		std::string cpu;
		std::string gpu;
		std::string motherboard;

		nlohmann::json to_json()
		{
			return *this;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Hardware, os, cpu, gpu, motherboard)
	};
}