#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <optional>
#include <vector>
#include "ui.h"

static std::atomic running(true);
static std::mutex eventMutex;
static std::vector<sf::Event> event_queue;
static sf::Clock delta_clock;
static ButtonAction current_action = ButtonAction::None;

void logicThread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void renderThread(sf::RenderWindow* window)
{
    window->setActive(true);
    ImGui::SFML::Init(*window);

    while (running)
    {
		{
            std::lock_guard<std::mutex> lock(eventMutex);
            for (auto& event : event_queue)
            {
                ImGui::SFML::ProcessEvent(*window, event);
                if (event.is<sf::Event::Closed>())
                {
                    running = false;
                    window->close();
                }
            }
            event_queue.clear();
		}

        ImGui::SFML::Update(*window, delta_clock.restart());
        if (ImGui::BeginMainMenuBar())
        {
            ImGui::BeginGroup();
			auto ui = UI::get_instance();
            ui->draw_button("Circle", ButtonAction::Circle);
            ImGui::SameLine();
            ui->draw_button("Rectangle", ButtonAction::Rectangle);
            ui->draw_button("Polygon", ButtonAction::Rectangle);
            ui->draw_button("Particle emitter", ButtonAction::Rectangle, ImVec2(125, 20));
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
        window->clear(sf::Color(30, 30, 30));

        ImGui::SFML::Render(*window);
        window->display();
    }
    ImGui::SFML::Shutdown();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML MT");
    window.setActive(false);

    std::thread logic(logicThread);
    std::thread render(renderThread, &window);
    while (running)
    {
        while (const std::optional event = window.pollEvent())
        {
            std::lock_guard<std::mutex> lock(eventMutex);
            event_queue.push_back(*event);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    if (logic.joinable()) logic.join();
    if (render.joinable()) render.join();
    return 0;
}
