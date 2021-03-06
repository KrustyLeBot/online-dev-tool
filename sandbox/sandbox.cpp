//Standard libs
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>

//Dear ImGui
#include "imgui.h"
#include "imgui-SFML.h"

//SFML
#include "SFML/Graphics.hpp"

//Custom class
#include "JsonSizeAnalyser/JsonSizeAnalyser.h"


#define TARGET_WINDOW_NAME "Online Dev Tool"
#define TARGET_WINDOW_LENGTH 1920
#define TARGET_WINDOW_HEIGHT 1080
#define TARGET_FRAMERATE 60

INT32 APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT32 nCmdShow)
{
	OutputDebugString("Start application\n");

	sf::RenderWindow window(sf::VideoMode(TARGET_WINDOW_LENGTH, TARGET_WINDOW_HEIGHT), TARGET_WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(TARGET_FRAMERATE);

	ImGui::SFML::Init(window);

	sf::Clock deltaClock;

	std::vector<JsonSizeAnalyser> analysers;
	analysers.push_back(JsonSizeAnalyser(analysers.size()));
	analysers.push_back(JsonSizeAnalyser(analysers.size()));

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_AlwaysUseWindowPadding |
		ImGuiWindowFlags_MenuBar;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		//Draw ImGui
		ImGui::SFML::Update(window, deltaClock.restart());

		/////////////////////////////////////////////////////////////
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		if (ImGui::Begin("StatusBar", nullptr, flags))
		{
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("Json Size Analyser"))
				{
					if (ImGui::MenuItem("Open new analyser", NULL))
					{
						analysers.push_back(JsonSizeAnalyser(analysers.size()));
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		//Handle Json size analyser
		int i = 0;
		std::vector<int> toDelete;
		for (auto& analyser : analysers)
		{
			if (!analyser.Draw())
			{
				toDelete.push_back(i);
			}
			i++;
		}

		for (const auto& index : toDelete)
		{
			analysers.erase(std::next(analysers.begin(), index));
		}
		/////////////////////////////////////////////////////////////

		//Clear old frame
		window.clear();

		//Render ImGui and draw the frame
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}