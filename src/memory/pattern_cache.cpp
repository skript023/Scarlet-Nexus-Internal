#include "pattern_cache.hpp"
#include "file_manager.hpp"

namespace memory
{
	pattern_cache::pattern_cache(const std::string_view name): m_initialized(false), m_name(name)
	{
		auto file = big::g_file_manager.get_project_file(std::format("./{}.bin", m_name));
		if (file.exists())
		{
			std::ifstream stream(file.get_path(), std::ios_base::binary);
			while (!stream.eof())
			{
				std::uint64_t hash;
				int offset;

				stream.read(reinterpret_cast<char*>(&hash), sizeof(hash));
				stream.read(reinterpret_cast<char*>(&offset), sizeof(offset));

				m_data.emplace(hash, offset);
			}
		}

		m_initialized = true;
	}
	void pattern_cache::update()
	{
		auto file = big::g_file_manager.get_project_file(std::format("./{}.bin", m_name));
		std::ofstream stream(file.get_path(), std::ios_base::binary);

		for (auto& [h, offset] : m_data)
		{
			auto hash = h;
			stream.write(reinterpret_cast<char*>(&hash), sizeof(hash));
			stream.write(reinterpret_cast<char*>(&offset), sizeof(offset));
		}
	}
	std::optional<int> pattern_cache::get_cached_offset(pattern_hash hash)
	{
		if (auto it = m_data.find(hash.get_hash()); it != m_data.end())
			return it->second;

		return std::nullopt;
	}
	void pattern_cache::update_cached_offset(pattern_hash hash, int offset)
	{
		m_data[hash.get_hash()] = offset;
	}
}