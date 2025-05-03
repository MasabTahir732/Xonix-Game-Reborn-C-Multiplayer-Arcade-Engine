#pragma once
#include"Screen.h"
#include"Menu.h"
#include"Login_Page.h"
#include"Game.h"
#include<iostream>

using namespace std;

class Gameboy {
private:
    Screen screen;
    Menu menu;
  //  LoginPage loginsignup;
    Player Player1;
    Player* optionalplayer;
    Game* g1;   //polymorphism implementation
    int currentpage;
    int currentoption;
public:
    Gameboy() {
        g1 = nullptr;
        currentpage = 1;
        currentoption = 0;
        Player1.loadPlayersFromFile();

    }

    void run() {
        while (screen.isOpen()) {

            sf::Event event;
            while (screen.window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    screen.window.close();
                }
                if (Keyboard::isKeyPressed(Keyboard::Num1)&& currentpage==1) {  //signup
                    currentoption = 1;
                    currentpage = 2;
                }
                if (Keyboard::isKeyPressed(Keyboard::Num2)&&currentpage==1) {//login
                    currentoption = 2;
                    currentpage = 2;
                }
                if (Keyboard::isKeyPressed(Keyboard::Num1) && currentpage == 3) {  //single player
                    currentoption = 1;
                }
                if (Keyboard::isKeyPressed(Keyboard::Num2) && currentpage == 3) {  //multiplayer
                    currentoption = 2;
                   
                }if (Keyboard::isKeyPressed(Keyboard::Num3) && currentpage == 3) {  //leaderboard
                    currentoption = 3;
                }

              
            }

            screen.handleEvents();
            screen.clear();
           // cin >> currentpage;

            if (currentpage == 1) {                           //page1 is menu
                screen.window.draw(menu.displayMainMenu()); // Draw the menu sprite
            }
            else if (currentpage == 2) {                        //page2 is empty page
                screen.window.draw(menu.displayLoginSignupMenu());
                if (currentoption == 1) { 
                    cout << "Enter the name and Password";
                    string temp1, temp2;
                    cin >> temp1 >> temp2;
                    Player1.createAccount(temp1,temp2);
                    currentpage = 3;
                    currentoption = -1;
                }
                else if (currentoption == 2) {                   //page2 is empty page
                    cout << "Enter the name and Password";
                    string temp1, temp2;
                    cin >> temp1 >> temp2;
                    Player1.Login(temp1,temp2); 
                    currentpage = 3;
                    currentoption = -1;
                    
                
                }

            }
            else if (currentpage==3) {            //page3 is singleplayer,multiplayer etc;
                screen.window.draw(menu.displayGameMenu()); //draw the sprite of game menu (check screen class)
                if (currentoption == 1) {                //option 1(single player selected)
                    g1 = new SingleGame;
                    g1->Display(screen.window, &Player1);
                }
                if (currentoption == 2) {                //option 2 selected (multiplayer)
                    cout << "Enter the name and Password for the second player";
                    //string temp1, temp2;
                    //cin >> temp1 >> temp2;
                    //optionalplayer = new Player;
                   // optionalplayer->createAccount(temp1, temp2);
                    //g1 = new MultiPlayer;
                }
                if (currentoption == 3) {               // option leaderboard 
                    g1->Display(screen.window, &Player1);
                }

            }


            screen.display();
        }
    }



};