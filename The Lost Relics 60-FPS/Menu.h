#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Menu
{
public:
	Text mainmenu[5];
	Music click;
	RectangleShape mouse;
	Menu(float width, float height);
	bool isMouse;
	void draw(RenderWindow& window);
	void MoveUp();
	void MoveDown();
	void Mouse(RenderWindow& window);
	void setSelected(int n);
	int pressed() {
		return selected;
	}
	~Menu();

private:
	int selected;
	Font font;
};

