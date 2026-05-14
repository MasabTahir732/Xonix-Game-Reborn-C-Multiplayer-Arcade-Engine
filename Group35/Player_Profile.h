#pragma once

//Masab Tahir 23i-0006
//Muhammad Arslan 23i-0572
//Xonix Game
#include<iostream>
#include<SFML/Graphics.hpp>
#include<string>
#include"Themes.h"
#include"login_signup.h"
using namespace std;
using namespace sf;

class PlayerProfilePage {
public:
    int Display(RenderWindow& window, Player* currentPlayer, Themes* currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Failed to load font!" << endl;
            return 1;
        }

        Node* playerNode = currentPlayer->returnHead();
        if (!playerNode) {
            cerr << "No player data found!" << endl;
            return 1;
        }

        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "cannot load theme!" << endl;
            }
            Sprite B(image);
            B.setPosition(0, 0);
            window.draw(B);

            Text title("Player Profile", font, 36);
            title.setPosition(100, 50);
            title.setFillColor(currentTheme->textColor);
            window.draw(title);

       
            string usernameStr = "Username:      " + playerNode->Username;
            Text usernameText(usernameStr, font, 26);
            usernameText.setPosition(100, 150);
            usernameText.setFillColor(currentTheme->textColor);
            window.draw(usernameText);

            string totalScoreStr = "Total Score:   " + to_string(playerNode->TotalScore);
            Text totalScoreText(totalScoreStr, font, 26);
            totalScoreText.setPosition(100, 200);
            totalScoreText.setFillColor(currentTheme->textColor);
            window.draw(totalScoreText);

          
            string currentScoreStr = "Current Score: " + to_string(playerNode->CurrentScore);
            Text currentScoreText(currentScoreStr, font, 26);
            currentScoreText.setPosition(100, 250);
            currentScoreText.setFillColor(currentTheme->textColor);
            window.draw(currentScoreText);

           
            string winStr = "Win %:         " + to_string(playerNode->WinPercentage) + "%";
            Text winText(winStr, font, 26);
            winText.setPosition(100, 300);
            winText.setFillColor(currentTheme->textColor);
            window.draw(winText);

         
            string rewardsStr = "Rewards:       " + to_string(playerNode->RewardCounter);
            Text rewardsText(rewardsStr, font, 26);
            rewardsText.setPosition(100, 350);
            rewardsText.setFillColor(currentTheme->textColor);
            window.draw(rewardsText);

           
            string powerUpStr = "Power-Ups:     " + to_string(playerNode->PowerUpCount);
            Text powerUpText(powerUpStr, font, 26);
            powerUpText.setPosition(100, 400);
            powerUpText.setFillColor(currentTheme->textColor);
            window.draw(powerUpText);

            Text instructions("Press ESC to return to Main Menu", font, 20);
            instructions.setPosition(100, 500);
            instructions.setFillColor(currentTheme->textColor);
            window.draw(instructions);

            window.display();

           
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                    return 1;  
            }
        }
        return 1;
    }
};
