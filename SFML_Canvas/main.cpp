#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <mutex>
#include <optional>
#include <vector>

#include "basic_functions.h"
#include "controller.h"
#include "ui.h"

static std::atomic running(true);
static std::mutex event_mutex;
static std::vector<sf::Event> event_queue;
static sf::Clock delta_clock;
static int x = 0;

void logic_pipeline(sf::RenderWindow* window, const std::vector<sf::Event>& events)
{
    for (auto& event : events)
    {
        ImGui::SFML::ProcessEvent(*window, event);
        if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                bool overUI = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
                if (!overUI)
                {
                    std::cout << "Logic: Canvas Click!" << std::endl;
                    Controller::get_instance()->execute_action(UI::get_instance()->current_action);
                }
            }
        }
        if (event.is<sf::Event::Closed>())
        {
            running = false;
            window->close();
        }
    }
    event_queue.clear();
}

void rendering_pipeline(sf::RenderWindow* window, ImGuiIO* io){
    ImGui::SFML::Update(*window, delta_clock.restart());
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::BeginGroup();
        auto ui = UI::get_instance();
        ui->draw_button("Ellipse", ButtonAction::Ellipse);
        ImGui::SameLine();
        ui->draw_button("Rectangle", ButtonAction::Rectangle);
        ui->draw_button("Polygon", ButtonAction::Rectangle);
        ImGui::EndGroup();

        ImGui::BeginGroup();
        ui->draw_button("Pipette", ButtonAction::Pipette);
        ImGui::Text("Color picker:");
        static float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
        ImGui::SetNextItemWidth(100);
        ImGui::ColorEdit4("##ColorPicker", color,
            ImGuiColorEditFlags_PickerHueWheel |
            ImGuiColorEditFlags_NoInputs |
            ImGuiColorEditFlags_NoLabel);
        ui->draw_button("Paint", ButtonAction::Rectangle, ImVec2(50, 20));
        ImGui::EndGroup();
        ImGui::EndMainMenuBar();
    }
    //window->clear(sf::Color(30, 30, 30));
    window->clear(rainbow_function(x));

    ImGui::SFML::Render(*window);
    window->display();
}

void window_render_thread(sf::RenderWindow* window, ImGuiIO* io)
{
    window->setActive(true);
    while (running)
    {
		x = x >= 1023 ? 0 : ++x;
        std::vector<sf::Event> current_frame_events;
        {
            std::lock_guard<std::mutex> lock(event_mutex);
            current_frame_events.swap(event_queue);
        }
        logic_pipeline(window, current_frame_events);
	    rendering_pipeline(window, io);
    }
    ImGui::SFML::Shutdown();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML MT");
    window.setActive(false);
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();

    std::thread render(window_render_thread, &window, &io);

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
