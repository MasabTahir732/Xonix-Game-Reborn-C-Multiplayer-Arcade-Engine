#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
#include"Themes.h"

class LeaderboardPage {
public:
    MainMenuPage* BackToMenu;
    Themes* currentTheme;
    void Display(RenderWindow& window) {
        window.clear(currentTheme->backgroundColor);
        Font font;
        font.loadFromFile("fonts/arial.ttf");
        Text text("Leaderboard - Press B to go back", font, 24);
        text.setPosition(150, 300);
        text.setFillColor(currentTheme->textColor);
        window.draw(text);
        window.display();
    }
};
