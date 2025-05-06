#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
#include"SaveGame.h"
#include"Login_signup.h"
#include"Themes.h"
#include"Main_GamePage.h"
#include"LeaderBoardPage.h"
#include"Inventory_Page.h"
#include"Multiplayer_Page.h"

class MainMenuPage {
public:
 
 
    bool isRunning;

    MainMenuPage() {
 
   
        isRunning = true;
    }

    int Display(RenderWindow& window, Player* PlayerThatisPlaying,Themes *currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font));

        const int numOptions = 6;
        std::string options[numOptions] = {
            "Start Game",
            "Leaderboard",
            "Multiplayer Mode",
            "Inventory",
            "Player Profile",
            "Exit"
        };

        Text menuItems[numOptions];
        for (int i = 0; i < numOptions; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setString(options[i]);
            menuItems[i].setCharacterSize(36);
            menuItems[i].setPosition(200, 50 + i * 60);
            menuItems[i].setFillColor(currentTheme->textColor);
        }

        int selectedItem = 0;
        while (window.isOpen() && isRunning) {
            window.clear(currentTheme->backgroundColor);
            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "cannot load theme!" << endl;
            }
            Sprite B(image);
            B.setPosition(0, 0);
            window.draw(B);
     
          
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
                      return  handleSelection(selectedItem, window, PlayerThatisPlaying,currentTheme);
                    }
                }
            }
        }
    }

    int handleSelection(int selectedItem, RenderWindow& window, Player* PlayerThatisPlaying,Themes* currentTheme) {
        switch (selectedItem) {
        case 0: 
            return 4;
            break;
        case 1:  
            return 2;
            break;
        case 2: 
            return 5;
            break;
            
        case 3:  
            return 3;
            break;
        case 4:  
            return 6;
            break;
        case 5:
            isRunning = false;
            window.close();
            break;
        default:
            break;
        }
    }
};







