#include "ui.h"
#include <imgui-sfml.h>
#include <imgui.h>

UI* UI::instance_ = nullptr;

UI::UI()
{
}

UI* UI::get_instance()
{
	return instance_ ? instance_ : (instance_ = new UI());
}

void UI::button_click(std::string label, ButtonAction& selected_action)
{
	auto it = buttons.find(label);
	if (it == buttons.end()) return;
    if (currentAction == ButtonAction::None || currentAction != selected_action)
    {
        currentAction = it->second;
        selected_action = currentAction;
    }
    else if (currentAction == selected_action) {
        currentAction = ButtonAction::None;
        selected_action = currentAction;
	}
}

void UI::draw_button(const std::string& label, ButtonAction action = ButtonAction::None, ImVec2 size)
{
    bool active = (action == currentAction && action != ButtonAction::None);

    if (active)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
    }
    if (ImGui::Button(label.c_str(), size)) button_click(label, action);
    if (active)
    {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
}