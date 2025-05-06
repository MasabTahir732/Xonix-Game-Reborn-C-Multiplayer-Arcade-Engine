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
#include"Player_Profile.h"
#include"Multiplayer_Page.h"

using namespace std;
using namespace sf;
//int Login = 0;
//int MainMenu = 1;
//int LeaderBoard = 2;
//int Inventory = 3;
//int SinglePlayer = 4;
//int Multiplayer = 5;
//int PlayerProfile = 6;


int main() {
    srand((time(0)));
    RenderWindow window(VideoMode(720, 450), "Xonix Game");
  
    
    //loading Themes
    Themes* theme1 = new Themes(1, Color(30, 30, 30), Color(245, 245, 245), "fonts/arial.ttf","images/1.png");
    Themes* theme2 = new Themes(2, Color(245, 245, 245), Color(255, 0, 255), "fonts/AGENCYB.ttf","images/2.png");
    Themes* theme3 = new Themes(3, Color(70, 130, 180), Color(255, 255, 255), "fonts/BERNHC.ttf","images/3.png");
    Themes* theme4 = new Themes(4, Color(34, 139, 34), Color(255, 255, 255), "fonts/COLONNA.ttf","images/4.png");
    Themes* theme5 = new Themes(5, Color(75, 0, 130), Color(240, 240, 240), "fonts/BRLNSB.ttf","images/5.png");


    themes.insert(theme1);
    themes.insert(theme2);
    themes.insert(theme3);
    themes.insert(theme4);
    themes.insert(theme5);
    LoginPage login;
    Themes* currentTheme = theme1;
    InventoryPage Inventory(&themes);
    Player* PlayerthatisPlaying = new Player;
    MainMenuPage menu;
    LeaderboardPage leader;
    MainGame game;
    PlayerProfilePage profile;
    MultiplayerPage multi;
    themes.displayInOrder();
    int currentPage = 0;
    while (window.isOpen()) {
        if (currentPage ==0) {
            *PlayerthatisPlaying = login.Display(window, currentTheme);
            int pref = PlayerthatisPlaying->getPref();
            currentTheme=themes.search(pref);
           if (currentTheme == nullptr) {
                cout << "No preffered theme found, applying default theme!" << endl;
                currentTheme = theme1;
            }
           currentPage = 1;

     }
        if (currentPage == 1) {
           currentPage=menu.Display(window, PlayerthatisPlaying, currentTheme);
        }
        if (currentPage == 3) {
            currentPage=Inventory.Display(window, PlayerthatisPlaying, currentTheme);
        }
        if (currentPage == 4) {
            game.Display(window, PlayerthatisPlaying,currentTheme);
        }
        if (currentPage == 2) {
            currentPage = leader.Display(window, currentTheme);
        }
        if (currentPage == 6) {
            currentPage = profile.Display(window, PlayerthatisPlaying, currentTheme);
        }
        if (currentPage == 5) {
            currentPage = multi.Display(window, PlayerthatisPlaying, currentTheme);
        }
    
    }
    return 0;
}
