#pragma once
#include <map>
#include <string>
#include "imgui.h"

enum class ButtonAction
{
	None,
	Circle,
	Rectangle,
	Polygon,
	ParticleEmitter,
	Pipette,
	Paint

};

class UI
{
private:
	ButtonAction currentAction = ButtonAction::None;
	std::map<std::string, ButtonAction> buttons;
	static UI* instance_;
	UI();
public:
	static UI* get_instance();

	void button_click(std::string label, ButtonAction& tool);
	void draw_button(const std::string& label, ButtonAction action, ImVec2 size = ImVec2(100, 20));
};