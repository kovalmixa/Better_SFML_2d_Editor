#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <mutex>
#include <optional>
#include <vector>
#include <random>

#include "basic_functions.h"
#include "logic_controller.h"
#include "ui_controller.h"
#include "selection_controller.h"

static std::atomic running(true);
static std::mutex event_mutex;
static std::vector<sf::Event> event_queue;
static sf::Clock delta_clock;
static int x = 0;

void logic_pipeline(sf::RenderWindow* window, const std::vector<sf::Event>& events)
{
    auto logic_controller = LogicController::get_instance();
    auto ui_controller = UIController::get_instance();
    std::map<sf::Keyboard::Key, bool> keys;
    sf::Vector2f mouse_position;

    for (auto& event : events)
    {
        ImGui::SFML::ProcessEvent(*window, event);
        mouse_position = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

        // Mouse pressed
        if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
        {
            sf::Vector2f mouse_position = window->mapPixelToCoords(mouseEvent->position);
            bool overUI = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
            if (!overUI)
            {
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    if (ui_controller->get_button_action() == ButtonAction::None &&
                        SelectionController::get_instance()->is_point_on_selection(mouse_position))
                    { logic_controller->begin_drag(mouse_position); }
                    else logic_controller->execute_action(ui_controller->get_button_action(), mouse_position);
                }
                else if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>())
                    if (mouseEvent->button == sf::Mouse::Button::Left) logic_controller->end_drag();
                else if (mouseEvent->button == sf::Mouse::Button::Right && 
                    SelectionController::get_instance()->is_point_on_selection(mouse_position))
                {

                }
            }
            if (mouseEvent->button == sf::Mouse::Button::Right) logic_controller->remove_actions();
        }

		//Mouse released
        if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>())
            if (mouseEvent->button == sf::Mouse::Button::Left)
                logic_controller->end_drag();

        logic_controller->keyboard_action_process(event, mouse_position);

        if (event.is<sf::Event::Closed>())
        {
            running = false;
            window->close();
        }
        if (const auto* resized = event.getIf<sf::Event::Resized>()) {
            sf::FloatRect visibleArea({ 0.f, 0.f }, { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
            window->setView(sf::View(visibleArea));
        }
    }

    // Mouse drag handling
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
        logic_controller->update_drag(ui_controller->get_button_action(), worldPos);
    }

    event_queue.clear();
}

void rendering_pipeline(sf::RenderWindow* window){
    ImGui::SFML::Update(*window, delta_clock.restart());

    if (ImGui::BeginMainMenuBar())
    {
        #pragma region shapes picker
        ImGui::BeginGroup();
        auto ui = UIController::get_instance();
        ui->draw_button("Ellipse", ButtonAction::Ellipse);
        ImGui::SameLine();
        ui->draw_button("Rectangle", ButtonAction::Rectangle);
        ui->draw_button("Polygon", ButtonAction::Polygon);
        const char* items[] = { "3", "5", "6", "8", "10", "12" };
        static int selectedItem = 0;
        ImGui::SetNextItemWidth(50.0f);
        if (ImGui::Combo("", &selectedItem, items, IM_ARRAYSIZE(items)))
            ui->set_polygon_points(std::stoi(items[selectedItem]));
        ImGui::EndGroup();
        #pragma endregion

        #pragma region color picker
        ImGui::BeginGroup();
        ui->draw_button("Pipette", ButtonAction::Pipette);
        ImGui::Text("Color picker:");
        #pragma region color_pallete_picker_code
        sf::Color current_color = ui->get_current_color();
        float color_array[4];
        color_array[0] = (float)current_color.r / 255.0f;
        color_array[1] = (float)current_color.g / 255.0f;
        color_array[2] = (float)current_color.b / 255.0f;
        color_array[3] = (float)current_color.a / 255.0f;

        if (ImGui::ColorEdit4("##ColorPicker", color_array,
            ImGuiColorEditFlags_PickerHueWheel |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_NoLabel)
            )
        {
            ui->set_current_color(sf::Color(
                static_cast<uint8_t>(color_array[0] * 255),
                static_cast<uint8_t>(color_array[1] * 255),
                static_cast<uint8_t>(color_array[2] * 255),
                static_cast<uint8_t>(color_array[3] * 255)
            ));
        }

        #pragma endregion
        ui->draw_button("Paint", ButtonAction::Paint, ImVec2(50, 20));
        ImGui::EndGroup();
        #pragma endregion

        #pragma region color_pallete_picker_code
        ImGui::BeginGroup();
        ui->draw_button("Pipette", ButtonAction::Pipette);
        ui->draw_button("Pipette", ButtonAction::Pipette);
        ui->draw_button("Pipette", ButtonAction::Pipette);
        ImGui::EndGroup();
        #pragma endregion



        ImGui::EndMainMenuBar();
    }

    window->clear(sf::Color(30, 30, 30));
    //window->clear(rainbow_function(x));

    LogicController::get_instance()->render_shapes(*window);
    ImGui::SFML::Render(*window);

    window->display();
}

void window_render_thread(sf::RenderWindow* window)
{
    window->setActive(true);
    ImGui::SFML::Init(*window);
    while (running)
    {
		x = x >= 1023 ? 0 : ++x;
        std::vector<sf::Event> current_frame_events;
        {
            std::lock_guard<std::mutex> lock(event_mutex);
            current_frame_events.swap(event_queue);
        }
        logic_pipeline(window, current_frame_events);
	    rendering_pipeline(window);
    }
    ImGui::SFML::Shutdown();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1000, 800 }), "SFML MT");
    window.setActive(false);
    window.setFramerateLimit(60);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::thread render(window_render_thread, &window);

    while (running)
    {
        while (const std::optional event = window.pollEvent())
        {
            std::lock_guard<std::mutex> lock(event_mutex);
            event_queue.push_back(*event);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    if (render.joinable()) render.join();

    return 0;
}
