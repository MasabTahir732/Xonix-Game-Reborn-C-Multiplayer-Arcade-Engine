#include<iostream>
#include <SFML/Graphics.hpp>

#include <time.h>
#include<string>
#include <fstream>
#include"Main Menu.h"
#include"SaveGame.h"
#include"Login_signup.h"
using namespace std;
using namespace sf;


int main() {
    srand((time(0)));
    RenderWindow window(VideoMode(720, 450), "Xonix Game");
    LoginPage login;
    MainMenuPage menu;
    LeaderboardPage leaderboard;
    MainGame game;
    login.MainMenuNext = &menu;
    menu.GamePageNext = &game;
    menu.LeaderBoardNext = &leaderboard;
    menu.BackToLogin = &login;
    leaderboard.BackToMenu = &menu;
    game.BackToMenu = &menu;
    int currentPage = 0;
    while (window.isOpen()) {
        if (currentPage == 0) {
            login.Display(window);
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) window.close();
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
                    currentPage = 1;
            }
        }
        else if (currentPage == 1) {
            menu.Display(window);
            Event e; 
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) window.close();
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::Num1) currentPage = 2;
                    if (e.key.code == Keyboard::Num2) currentPage = 3;
                    if (e.key.code == Keyboard::Escape) currentPage = 0;
                }
            }
        }
        else if (currentPage == 2) {
            game.Display(window);
            currentPage = 1;
        }
        else if (currentPage == 3) {
            leaderboard.Display(window);
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) window.close();
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::B) currentPage = 1;
            }
        }
    }
    return 0;
}
