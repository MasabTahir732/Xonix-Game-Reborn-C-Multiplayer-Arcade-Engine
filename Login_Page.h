#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
#include"Main Menu.h"
#include"SaveGame.h"
#include"login_signup.h"
#include"Themes.h"
class LoginPage {
public:
    Player playerList;
    LoginPage() {
        playerList.loadPlayersFromFile();
    }

    Player Display(RenderWindow& window, Themes* currentTheme) {
        string username = "";
        string password = "";
        bool enteringPassword = false;
        bool signupMode = false;
        string errorMessage = "";

        Font font;
        font.loadFromFile("fonts/arial.ttf");

        Text prompt("", font, 24);
        Text inputText("", font, 24);
        Text modeText("", font, 20);
        Text errorText("", font, 20);

        prompt.setFillColor(currentTheme->textColor);
        inputText.setFillColor(currentTheme->textColor);
        modeText.setFillColor(currentTheme->textColor);
        errorText.setFillColor(Color::Red);
        Texture Back;
        if (!Back.loadFromFile("images/login.png")) {
            cerr << "Cannot load login photo!" << endl;
        }
        Sprite BackGround(Back);
        BackGround.setPosition(0, 0);
        string* activeField = &username;

        while (window.isOpen()) {
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();

                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::Enter) {
                        if (!enteringPassword) {
                            enteringPassword = true;
                            activeField = &password;
                        }
                        else {
                            if (!signupMode) {
                                Node* result = playerList.Login(username, password);
                                if (result) {
                                    window.clear(currentTheme->backgroundColor);
                                    window.draw(BackGround);
                                    Text successText("Login successful! Press Enter to continue.", font, 24);
                                    successText.setFillColor(Color::Green);
                                    successText.setPosition(100, 200);
                                    window.draw(successText);
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

                                 
                                    return Player(result);
                                }
                                else if (playerList.usernameExists(username)) {
                                    errorMessage = "Wrong Password. Please try again.";
                                }
                                else {
                                    errorMessage = "Account doesn't exist. Switching to Signup mode.";
                                    signupMode = true;
                                    enteringPassword = false;
                                    activeField = &username;
                                    username = "";
                                    password = "";
                                }
                            }
                            else {
                                if (!playerList.usernameExists(username)) {
                                    int created = playerList.createAccount(username, password);
                                    if (created) {
                                        window.clear(currentTheme->backgroundColor);
                                        Text createdText("Account Created! Press Enter to continue.", font, 24);
                                        window.draw(BackGround);
                                        createdText.setFillColor(Color::Green);
                                        createdText.setPosition(100, 200);
                                        window.draw(createdText);
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

                                        Node* newNode = playerList.findNodeByUsername(username);
                                        return Player(newNode);  
                                    }
                                }
                                else {
                                    errorMessage = "Username already exists. Press enter to login!.";
                                    window.clear(currentTheme->backgroundColor);
                                    window.draw(BackGround);
                                    Text successText(errorMessage, font, 24);
                                    successText.setFillColor(Color::Green);
                                    successText.setPosition(100, 200);
                                    window.draw(successText);
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
                                    signupMode = false; 
                                    enteringPassword = false;
                                    username = "";
                                    password = "";
                                    activeField = &username;
                                }
                            }
                        }
                    }

                    if (e.key.code == Keyboard::BackSpace && !activeField->empty()) {
                        activeField->pop_back();
                    }

                    // A-Z
                    for (int k = Keyboard::A; k <= Keyboard::Z; ++k) {
                        if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(k))) {
                            bool shift = Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::RShift);
                            char ch = (shift ? 'A' : 'a') + (k - Keyboard::A);
                            activeField->push_back(ch);
                            break;
                        }
                    }

                    // 0-9
                    for (int k = Keyboard::Num0; k <= Keyboard::Num9; ++k) {
                        if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(k))) {
                            char ch = '0' + (k - Keyboard::Num0);
                            activeField->push_back(ch);
                            break;
                        }
                    }
                }
            }

            window.clear(currentTheme->backgroundColor);
            window.draw(BackGround);
            string modeStr = signupMode ? "SIGNUP MODE" : "LOGIN MODE";
            modeText.setString(modeStr);
            modeText.setPosition(100, 50);

            prompt.setString(enteringPassword ? "Enter Password:" : "Enter Username:");
            prompt.setPosition(100, 100);

            string shownText = enteringPassword ? string(password.length(), '*') : username;
            inputText.setString(shownText);
            inputText.setPosition(100, 150);

            window.draw(modeText);
            window.draw(prompt);
            window.draw(inputText);

            if (!errorMessage.empty()) {
                errorText.setString(errorMessage);
                errorText.setPosition(100, 200);
                window.draw(errorText);
            }

            window.display();
        }

  
        return Player();
    }

};

