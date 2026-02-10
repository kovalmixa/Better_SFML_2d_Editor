#pragma once
#include <functional>
#include <map>
#include "ui.h"

class Controller
{
private:
	static Controller* instance_;
	using handler_t = std::function<void>;
	std::map<ButtonAction, handler_t> handlers_;
	Controller();
public:
	static Controller* get_instance();
};
