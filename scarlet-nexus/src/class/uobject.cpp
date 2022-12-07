#include "uobject.hpp"
#include <return_address.hpp>
#include <pointers.hpp>

namespace big
{
	TUObjectArray* UObject::get_global_object()
	{
		return &big::g_pointers->m_object_array->m_obj_objects;
	}

	std::string UObject::get_name() const
	{
		return this->m_name.get_name();
	}

	std::string UObject::get_fullname() const
	{
		std::string name;

		if (m_class)
		{
			if (m_outer)
			{
				for (auto p = m_outer; p; p = p->m_outer)
				{
					name = p->get_name() + "." + name;
				}
				name = m_class->get_name() + " " + name + this->get_name();
			}
			else if (!m_outer)
			{
				name = m_class->get_name() + " " + this->get_name();
			}
		}

		return name;
	}

	bool UObject::IsA(UClass* cmp) const
	{
		{

			for (auto super = this->m_class; super; super = static_cast<UClass*>(super->SuperField))
			{
				if (super == cmp)
				{
					return true;
				}
			}
			return false;

		}
	}

	void UObject::process_event(UFunction* function, void* parms)
	{
		return return_address::spoof_call(g_pointers->m_return_address, GetVFunction<void(*)(UObject*, UFunction*, void*)>(this, 0x43), this, function, parms);
	}

	UObject* FUObjectItem::get_valid_object()
	{
		if (IsBadReadPtr(this, sizeof(this)) || IsBadReadPtr(this->m_object, sizeof(this->m_object)))
		{
			return nullptr;
		}
		return this->m_object;
	}
}