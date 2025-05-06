#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
#include"Themes.h"
class MultiplayerPage {
public:
    int Display(RenderWindow& window, Player* PlayerThatisPlaying, Themes* currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Cannot load font!" << endl;
            return 1;
        }

        const int numOptions = 3;
        std::string options[numOptions] = {
            "Friends",
            "Matchmaking",
            "Game Room"
        };

        Text menuItems[numOptions];
        for (int i = 0; i < numOptions; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setString(options[i]);
            menuItems[i].setCharacterSize(36);
            menuItems[i].setPosition(200, 100 + i * 60);
            menuItems[i].setFillColor(currentTheme->textColor);
        }

        int selectedItem = 0;
        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);

       
            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "Cannot load theme image!" << endl;
            }
            else {
                Sprite bg(image);
                bg.setPosition(0, 0);
                window.draw(bg);
            }

            for (int i = 0; i < numOptions; i++) {
                if (i == selectedItem)
                    menuItems[i].setFillColor(Color::Yellow);
                else
                    menuItems[i].setFillColor(currentTheme->textColor);
                window.draw(menuItems[i]);
            }

            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();

                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::W) {
                        selectedItem--;
                        if (selectedItem < 0)
                            selectedItem = numOptions - 1;
                    }
                    if (e.key.code == Keyboard::S) {
                        selectedItem++;
                        if (selectedItem >= numOptions)
                            selectedItem = 0;
                    }
                    if (e.key.code == Keyboard::Enter) {
                        return handleMultiplayerSelection(selectedItem, window, PlayerThatisPlaying, currentTheme);
                    }
                    if (e.key.code == Keyboard::Escape) {
                        return 1;  
                    }
                }
            }
        }

        return 1;
    }

    int handleMultiplayerSelection(int selectedItem, RenderWindow& window, Player* PlayerThatisPlaying, Themes* currentTheme) {
        switch (selectedItem) {
        case 0:
            cout << "Friends selected" << endl;
            return 7;  
        case 1:
            cout << "Matchmaking selected" << endl;
            return 8;
        case 2:
            cout << "Game Room selected" << endl;
            return 9;
        default:
            return 1; 
        }
    }
};