#pragma once

namespace memory
{
	class pattern_hash
	{
	public:
		std::uint64_t m_hash;

		constexpr pattern_hash() :
			m_hash(0xD84A917A64D4D016ULL)
		{
		}

		constexpr pattern_hash(std::uint64_t hash) :
			m_hash(hash)
		{
		}

		constexpr pattern_hash(const pattern_hash& other) :
			m_hash(other.m_hash)
		{
		}

		constexpr pattern_hash(pattern_hash&& other) :
			m_hash(other.m_hash)
		{
		}

		constexpr void operator=(const pattern_hash& other)
		{
			m_hash = other.m_hash;
		}

		constexpr pattern_hash update(char data) const
		{
			auto hash = m_hash;
			hash += (static_cast<std::uint64_t>(data));
			hash += hash << 10;
			hash ^= (hash >> 6);
			return pattern_hash(hash);
		}

		constexpr pattern_hash update(int data) const
		{
			auto hash = m_hash;
			hash ^= (data >> 8);
			hash += hash << 10;
			hash ^= (hash >> 7);
			return pattern_hash(hash);
		}

		constexpr pattern_hash update(std::uint64_t data) const
		{
			auto hash = m_hash;
			hash ^= (data * 0xA5C38736C426FCB8ULL);
			hash += hash << 15;
			hash ^= (hash >> 5);
			return pattern_hash(hash);
		}

		constexpr std::uint64_t get_hash() const
		{
			return m_hash;
		}
	};
}