#pragma once
#include "uobject.hpp"

class UKismetTextLibrary : public UObject
{
public:
	static UClass* static_class()
	{
		if (!m_kismet_text_library)
			m_kismet_text_library = UObject::find_class("Class Engine.KismetTextLibrary");

		return m_kismet_text_library;
	}

	FString convert_text_to_string(const FText& InText)
	{
		struct {
			FText m_in_text;
			FString m_return_value;
		}params;
		static auto fn = UObject::find_object<UFunction*>("Function Engine.KismetTextLibrary.Conv_TextToString");

		params.m_in_text = InText;

		this->process_event(fn, &params);

		return params.m_return_value;
	}
private:
	inline static UClass* m_kismet_text_library{nullptr};
};