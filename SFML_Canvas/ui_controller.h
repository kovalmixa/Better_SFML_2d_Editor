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

class UIController
{
private:
	std::map<std::string, ButtonAction> buttons;
	static UIController* instance_;
	UIController() = default;

	static sf::Color current_color_;
	ButtonAction current_button_action = ButtonAction::None;
	int current_polygon_points = 3;
public:
	static UIController* get_instance();

	void button_click(std::string label, ButtonAction& tool);

	void set_current_color(const sf::Color& color);
	sf::Color get_current_color() const;

	void set_button_action(ButtonAction button_action);
	ButtonAction get_button_action();

	void set_polygon_points(const int num_points);
	int get_polygon_points();

	void draw_button(const std::string& label, ButtonAction action, ImVec2 size = ImVec2(100, 20));
};