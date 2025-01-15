#pragma once
enum class OptionAction
{
	LeftPress,
	RightPress,
	EnterPress
};

enum class OptionFlag
{
	Horizontal = (1 << 0),
	Enterable = (1 << 1),
	Toggle = (1 << 2),
	BoolSliderInt = (1 << 3),
	BoolSliderFloat = (1 << 4),
};

class abstract_option
{
public:
	virtual ~abstract_option() noexcept = default;

	virtual const char* get_left_text() = 0;
	virtual const char* get_right_text() = 0;
	virtual int get_integer() = 0;
	virtual int get_min_integer() = 0;
	virtual int get_max_integer() = 0;
	virtual float get_float() = 0;
	virtual float get_min_float() = 0;
	virtual float get_max_float() = 0;
	virtual const char* get_description() = 0;
	virtual void handle_action(OptionAction action) = 0;
	virtual bool get_flag(OptionFlag flag) = 0;
protected:
	explicit abstract_option() = default;

	abstract_option(abstract_option const&) = default;
	abstract_option& operator=(abstract_option const&) = default;

	abstract_option(abstract_option&&) = default;
	abstract_option& operator=(abstract_option&&) = default;
};