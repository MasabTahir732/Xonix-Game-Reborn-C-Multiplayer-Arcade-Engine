#include<iostream>
#include <SFML/Graphics.hpp>

#include <time.h>
#include<string>
#include <fstream>
#include"Main Menu.h";
#include"SaveGame.h";
#include"Login_signup.h";
#include"Themes.h";
#include"Login_Page.h"
#include"LeaderBoardPage.h"
#include"Main_GamePage.h"


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
    //leaderboard.BackToMenu = &menu;
    game.BackToMenu = &menu;
    
    //loading Themes
    Themes* theme1 = new Themes(1, Color(30, 30, 30), Color::White, "fonts/arial.ttf"); // dark theme
    Themes* theme2 = new Themes(2, Color(255, 255, 255), Color::Black, "fonts/ARCADECLASSIC.ttf"); // light theme
    Themes* theme3 = new Themes(3, Color(70, 130, 180), Color::Yellow, "fonts/BERNHC.ttf"); //  blue + yellow
    Themes* theme4 = new Themes(4, Color(34, 139, 34), Color(255, 255, 0), "fonts/COLONNA.ttf"); // green + yellow
    Themes* theme5 = new Themes(5, Color(75, 0, 130), Color::Cyan, "fonts/BRLNSB.ttf"); // indigo + cyan

    themes.insert(theme1);
    themes.insert(theme2);
    themes.insert(theme3);
    themes.insert(theme4);
    themes.insert(theme5);
    Themes* selectedTheme = themes.search(1);
    login.currentTheme = selectedTheme;
    menu.currentTheme = selectedTheme;
  
    leaderboard.currentTheme = selectedTheme;

    while (window.isOpen()) {
        login.Display(window);
    }
    return 0;
}
