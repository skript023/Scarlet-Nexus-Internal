#pragma once

#include "common.hpp"

namespace big
{
    struct Color
    {
        Color(std::uint8_t r = 0, std::uint8_t g = 0, std::uint8_t b = 0, std::uint8_t a = 255) :
            r(r), g(g), b(b), a(a)
        {}

        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Color, r, g, b, a)
    };
    /**
     * \brief A stream buffer that uses a char[] buffer
     */
    class MemoryStreamBuf : public std::streambuf
    {
    public:
        MemoryStreamBuf(char* array, std::size_t size)
        {
            std::fill_n(array, size, '\0');
            std::streambuf::setp(array, array + size - 1);
        }

        template <std::size_t size>
        MemoryStreamBuf(char(&array)[size]) :
            MemoryStreamBuf(&array[0], size)
        {
        }
    };

    /**
     * \brief An input/output stream that uses a char[] buffer
     */
    class MemoryStringStream : virtual MemoryStreamBuf, public std::iostream
    {
    public:
        MemoryStringStream(char* array, std::size_t size) :
            MemoryStreamBuf(array, size),
            std::iostream(static_cast<MemoryStreamBuf*>(this))
        {
        }

        template <std::size_t size>
        MemoryStringStream(char(&array)[size]) :
            MemoryStreamBuf(array),
            std::iostream(static_cast<MemoryStreamBuf*>(this))
        {
        }
    };

    /**
     * \brief A MemoryStringStream with a built in buffer
     */
    template <std::size_t N>
    class MemoryStringStreamWithBuffer : public MemoryStringStream
    {
    public:
        MemoryStringStreamWithBuffer():
            MemoryStringStream(m_Buffer)
        {}
    private:
        char m_Buffer[N];
    };

    inline constexpr char ToLower(const char c)
    {
        return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    }

    /**
     * \brief Calculates JOAAT hash of string at compile time
     */
}