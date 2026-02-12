#pragma once
#include <map>
#include <string>
#include "imgui.h"

enum class ButtonAction
{
	None,
	Ellipse,
	Rectangle,
	Polygon,
	Pipette,
	Paint

};

class UI
{
private:
	std::map<std::string, ButtonAction> buttons;
	static UI* instance_;
	UI() = default;
public:
	static UI* get_instance();
	ButtonAction current_action = ButtonAction::None;

	void button_click(std::string label, ButtonAction& tool);
	void draw_button(const std::string& label, ButtonAction action, ImVec2 size = ImVec2(100, 20));
};