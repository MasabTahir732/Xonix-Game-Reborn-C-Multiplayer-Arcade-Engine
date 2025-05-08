#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include<string>
#include"login_signup.h"
#include"Themes.h"

using namespace std;
using namespace sf;

class MatchMaking {
    Node* queue[50];
    int queueSize;

    void insertPlayer(Node* player) {
        if (queueSize >= 50) {
            cout << "Queue full!" << endl;
            return;
        }

        int i = queueSize - 1;
        while (i >= 0 && queue[i]->TotalScore < player->TotalScore) {
            queue[i + 1] = queue[i];
            i--;
        }
        queue[i + 1] = player;
        queueSize++;
    }

    Node* dequeuePlayer() {
        if (queueSize == 0) return nullptr;
        Node* top = queue[0];
        for (int i = 1; i < queueSize; i++) {
            queue[i - 1] = queue[i];
        }
        queueSize--;
        return top;
    }

public:
    MatchMaking() {
        queueSize = 0;
    }

    void loadPlayers(Player* allPlayers) {
        Node* temp = allPlayers->returnHead();
        while (temp) {
            insertPlayer(temp);
            temp = temp->next;
        }
    }

    int Display(RenderWindow& window, Player* PlayerthatisPlaying, Themes* currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Cannot load font!" << endl;
            return 1;
        }
        Player* AllPlayers = new Player;
        AllPlayers->loadPlayersFromFile();
        loadPlayers(AllPlayers); 

        string message = "";

        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "cannot load theme!" << endl;
            }
            Sprite B(image);
            B.setPosition(0, 0);
            window.draw(B);

            Text title("Matchmaking Queue", font, 30);
            title.setFillColor(currentTheme->textColor);
            title.setPosition(100, 50);
            window.draw(title);

    
            for (int i = 0; i < queueSize; i++) {
                string displayText = to_string(i + 1) + ". " + queue[i]->Username + " (" + to_string(queue[i]->TotalScore) + ")";
                Text playerText(displayText, font, 20);
                playerText.setFillColor(currentTheme->textColor);
                playerText.setPosition(100, 120 + i * 30);
                window.draw(playerText);
            }

            if (!message.empty()) {
                Text msg(message, font, 24);
                msg.setFillColor(Color::Green);
                msg.setPosition(100, 350);
                window.draw(msg);
            }

            Text instructions("Press M to Match players | Esc to return", font, 20);
            instructions.setFillColor(currentTheme->textColor);
            instructions.setPosition(100, 400);
            window.draw(instructions);

            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::Escape)
                        return 5;
                    if (e.key.code == Keyboard::M) {
                        if (queueSize >= 2) {
                            Node* p1 = dequeuePlayer();
                            Node* p2 = dequeuePlayer();
                            message = "Matched: " + p1->Username + " vs " + p2->Username;

                            //Run Multiplayer here

                        }
                        else {
                            message = "Not enough players to match!";
                        }
                    }
                }
            }
        }
        return 1;
    }
};
