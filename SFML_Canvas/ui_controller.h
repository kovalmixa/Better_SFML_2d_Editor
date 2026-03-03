#pragma once
#include <map>
#include <string>

#include "particle_system.h"
#include "imgui.h"

enum class ButtonAction
{
	None,
	Ellipse,
	Rectangle,
	Polygon,
	Pipette,
	Paint,
	AddEffect
};

class UIController
{
private:
	std::map<std::string, ButtonAction> buttons;
	std::map<std::string, EmitterMode> effects;
	static UIController* instance_;
	UIController();

	static sf::Color current_color_;
	ButtonAction current_button_action = ButtonAction::None;
	EmitterMode current_effect = EmitterMode::Explosion;
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

	void set_effect_mode(std::string effect_name);
	EmitterMode get_effect_mode();

	void draw_button(const std::string& label, ButtonAction action, ImVec2 size = ImVec2(100, 20));
};