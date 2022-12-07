#include "common.hpp"
#include "detour_hook.hpp"
#include "logger.hpp"
#include "memory/handle.hpp"

namespace big
{
	detour_hook::detour_hook(std::string name, void* target, void* detour) :
		m_name(std::move(name)),
		m_target(target),
		m_detour(detour),
		m_polyhook(std::make_unique<PLH::x64Detour>(reinterpret_cast<uintptr_t>(m_target), reinterpret_cast<uintptr_t>(m_detour), reinterpret_cast<uintptr_t*>(&m_original)))
	{
		
	}

	detour_hook::~detour_hook() noexcept
	{
		if (m_target)
		{
			m_polyhook.reset();
		}

		LOG(INFO) << "Removed hook '" << m_name << "'.";
	}

	void detour_hook::enable()
	{
		if (m_polyhook->hook())
		{
			LOG(INFO_TO_FILE) << "Enabled hook '" << m_name << "'.";
		}
		else
		{
			throw std::runtime_error(std::format("Failed to enable hook 0x{:X} ({})", reinterpret_cast<std::uintptr_t>(m_target), m_name));
		}
	}

	void detour_hook::disable()
	{
		if (m_polyhook->unHook())
		{
			LOG(INFO_TO_FILE) << "Disabled hook '" << m_name << "'.";
		}
		else
		{
			LOG(WARNING) << "Failed to disable hook '" << m_name << "'.";
		}
	}

	DWORD exp_handler(PEXCEPTION_POINTERS exp, std::string const& name)
	{
		return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION
			? EXCEPTION_EXECUTE_HANDLER
			: EXCEPTION_CONTINUE_SEARCH;
	}

	void detour_hook::fix_hook_address()
	{
		__try
		{
			auto ptr = memory::handle(m_target);
			while (ptr.as<std::uint8_t&>() == 0xE9)
			{
				ptr = ptr.add(1).rip();
			}

			m_target = ptr.as<void*>();
		}
		__except (exp_handler(GetExceptionInformation(), m_name))
		{
			[this]()
			{
				throw std::runtime_error(std::format("Failed to fix hook address for '{}'", m_name));
			}();
		}
	}
}
