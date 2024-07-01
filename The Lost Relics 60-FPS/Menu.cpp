#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

Menu::Menu(float width, float height) {
	font.loadFromFile("./assets/font/HeadlinerNo.45 DEMO.ttf");

	//light blue ======== Color{ 115,158,170 }
	//dark blue ========= Color{ 4,0,33 }
	//light dark blue === Color{ 0,22,63 }
	//orange yello ====== Color{ 255,204,0 }
	//dark red ========== Color{ 138,1,1 }
	//dark green ======== Color{ 41,64,53 }

	int fontSize = 56;
	float fontOutlineSize = 0.19;

	mainmenu[0].setFont(font);
	mainmenu[0].setFillColor(Color{ 115,158,170 });
	mainmenu[0].setOutlineColor(Color::Black);
	mainmenu[0].setOutlineThickness(fontOutlineSize);
	mainmenu[0].setString("New Game");
	mainmenu[0].setCharacterSize(fontSize);
	mainmenu[0].setPosition(120, 458 - 37);
	
	mainmenu[1].setFont(font);
	mainmenu[1].setFillColor(Color::White);
	mainmenu[1].setOutlineColor(Color::Black);
	mainmenu[1].setOutlineThickness(fontOutlineSize);
	mainmenu[1].setString("Load Game");
	mainmenu[1].setCharacterSize(fontSize);
	mainmenu[1].setPosition(120, 458 + 120 - 37);

	mainmenu[2].setFont(font);
	mainmenu[2].setFillColor(Color::White);
	mainmenu[2].setOutlineColor(Color::Black);
	mainmenu[2].setOutlineThickness(fontOutlineSize);
	mainmenu[2].setString("Controls");
	mainmenu[2].setCharacterSize(fontSize);
	mainmenu[2].setPosition(120, 458 + 120 + 120 - 37);

	mainmenu[3].setFont(font);
	mainmenu[3].setFillColor(Color::White);
	mainmenu[3].setOutlineColor(Color::Black);
	mainmenu[3].setOutlineThickness(fontOutlineSize);
	mainmenu[3].setString("Options");
	mainmenu[3].setCharacterSize(fontSize);
	mainmenu[3].setPosition(120, 458 + 120 + 120 + 120 - 37);

	mainmenu[4].setFont(font);
	mainmenu[4].setFillColor(Color::White);
	mainmenu[4].setOutlineColor(Color::Black);
	mainmenu[4].setOutlineThickness(fontOutlineSize);
	mainmenu[4].setString("Exit");
	mainmenu[4].setCharacterSize(fontSize);
	mainmenu[4].setPosition(120, 458 + 120 + 120 + 120 + 120 - 37);

	selected = 0;
}

Menu::~Menu() {}

void Menu::setSelected(int n) {
	selected = n;
}

void Menu::draw(RenderWindow& window) {
	for (int i = 0; i < 5; i++) {
		window.draw(mainmenu[i]);
	}
}

void Menu::MoveDown() {
	if (selected + 1 <= 5 && !isMouse) // Not "Exit"
	{
		mainmenu[selected].setFillColor(Color::White);
		selected++;
		if (selected == 5)
		{
			selected = 0;
		}
		mainmenu[selected].setFillColor(Color{ 115,158,170 });
		click.openFromFile("./assets/sounds/click.mp3");
		click.setVolume(50);
		click.play();
	}
}

void Menu::MoveUp() {
	if (selected - 1 >= -1 && !isMouse) // Not "Play"
	{
		mainmenu[selected].setFillColor(Color::White);
		selected--;
		if (selected == -1)
		{
			selected = 4;
		}
		mainmenu[selected].setFillColor(Color{ 115,158,170 });
		click.openFromFile("./assets/sounds/click.mp3");
		click.setVolume(50);
		click.play();
	}
}

void Menu::Mouse(RenderWindow& window) {
	mouse.setSize(Vector2f(15,20));
	Vector2i mousePosition = Mouse::getPosition(window);
	Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
	mouse.setPosition(worldMousePosition);
	if (mouse.getGlobalBounds().intersects(mainmenu[0].getGlobalBounds())) {
		selected = 0;
		mainmenu[0].setFillColor(Color{ 115,158,170 });
		mainmenu[1].setFillColor(Color::White);
		mainmenu[2].setFillColor(Color::White);
		mainmenu[3].setFillColor(Color::White);
		mainmenu[4].setFillColor(Color::White);
		isMouse = true;
	}
	else if (mouse.getGlobalBounds().intersects(mainmenu[1].getGlobalBounds())) {
		selected = 1;
		mainmenu[1].setFillColor(Color{ 115,158,170 });
		mainmenu[0].setFillColor(Color::White);
		mainmenu[2].setFillColor(Color::White);
		mainmenu[3].setFillColor(Color::White);
		mainmenu[4].setFillColor(Color::White);
		isMouse = true;
	}
	else if (mouse.getGlobalBounds().intersects(mainmenu[2].getGlobalBounds())) {
		selected = 2;
		mainmenu[2].setFillColor(Color{ 115,158,170 });
		mainmenu[0].setFillColor(Color::White);
		mainmenu[1].setFillColor(Color::White);
		mainmenu[3].setFillColor(Color::White);
		mainmenu[4].setFillColor(Color::White);
		isMouse = true;
	}
	else if (mouse.getGlobalBounds().intersects(mainmenu[3].getGlobalBounds())) {
		selected = 3;
		mainmenu[3].setFillColor(Color{ 115,158,170 });
		mainmenu[4].setFillColor(Color::White);
		mainmenu[0].setFillColor(Color::White);
		mainmenu[1].setFillColor(Color::White);
		mainmenu[2].setFillColor(Color::White);
		isMouse = true;
	}
	else if (mouse.getGlobalBounds().intersects(mainmenu[4].getGlobalBounds())) {
		selected = 4;
		mainmenu[4].setFillColor(Color{ 115,158,170 });
		mainmenu[3].setFillColor(Color::White);
		mainmenu[0].setFillColor(Color::White);
		mainmenu[1].setFillColor(Color::White);
		mainmenu[2].setFillColor(Color::White);
		isMouse = true;
	}
	else {
		isMouse = false;
	}
}