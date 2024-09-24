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
	Toggle = (1 << 2)
};

class abstract_option
{
public:
	virtual ~abstract_option() noexcept = default;

	virtual const char* get_left_text() = 0;
	virtual const char* get_right_text() = 0;
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