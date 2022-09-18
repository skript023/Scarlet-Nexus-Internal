#pragma once

#pragma pack(push, 1)

template <typename T>
struct TArray
{
	inline T& operator[](const int i) { return items[i]; }

	inline const T& operator[](const int i) const { return items[i]; }

	inline static TArray<T> init(const std::vector<T>& items)
	{
		TArray<T> ret;
		ret.item_count = ret.max_item = items.size();
		ret.items = new T[ret.item_count];
		for (int i = 0; i < ret.item_count; i++)
			ret.items[i] = items[i];
		return ret;
	}

	inline void uninit()
	{
		delete[] this->items;
	}

	inline bool valid(int i)
	{
		if (item_count > max_item || !items[i])
			return false;

		return true;
	}

	inline bool valid_ex(int i)
	{
		uintptr_t result{};
		if (!ReadProcessMemory(GetCurrentProcess(), this->items[i], &result, sizeof(this->items[i]), nullptr))
		{
			return false;
		}
		return true;
	}

	inline int count()
	{
		return item_count;
	}

	inline int max_count()
	{
		return max_item;
	}

	T* items = nullptr;
	int item_count = 0;
	int max_item = 0;
};

struct FString : public TArray<wchar_t>
{
	inline wchar_t* get_wstring() { return this->items; }

	inline std::string get_string()
	{

		int size = WideCharToMultiByte(CP_UTF8, 0, items, item_count, nullptr, 0, nullptr, nullptr);
		std::string str(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, items, item_count, &str[0], size, nullptr, nullptr);
		return str;
	}
};
#pragma pack(pop)