#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
#include"SaveGame.h"
#include"Login_signup.h"
#include"Themes.h"
#include"LeaderBoardPage.h"

class Menu {
public:

    sf::Texture menutexture;
    sf::Sprite menusprite;
    bool flag = false;

public:
    Menu() {
        displayMainMenu();
    }
    sf::Sprite displayMainMenu() {
        if (!menutexture.loadFromFile("page1.png")) {
        }
        menusprite.setTexture(menutexture);
        return menusprite;
    }
    sf::Sprite displayLoginSignupMenu() {
        if (!menutexture.loadFromFile("page1.png")) {
        }
        menusprite.setTexture(menutexture);
        return menusprite;
    }

    sf::Sprite displayGameMenu() {
        if (!menutexture.loadFromFile("page2.png")) {
        }
        menusprite.setTexture(menutexture);
        return menusprite;
    }
};
/*
class MainMenuPage {
public:
    Menu menu;
    LoginPage login;
    MainGame* GamePageNext;
    LeaderboardPage* LeaderBoardNext;
    Themes* currentTheme;
    MainMenuPage() {
        GamePageNext = nullptr;
        LeaderBoardNext = nullptr;
        currentTheme = nullptr;
    }
    void run() {
        menu.displayMainMenu();
    }
    void Display(RenderWindow& window, Player* PlayerThatisPlaying) {
        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Font font;
            font.loadFromFile("fonts/arial.ttf");
            Text text("Main Menu - 1: Start 2: Leaderboard Esc: Back", font, 24);
            text.setPosition(100, 300);
            text.setFillColor(currentTheme->textColor);
            window.draw(text);
            window.display();
            bool wait = true;
            while (wait && window.isOpen()) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed)


                        window.close();
                    if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
                        wait = false;
                }
            }
            GamePageNext->Display(window, PlayerThatisPlaying);


        }
    }
};
*/







