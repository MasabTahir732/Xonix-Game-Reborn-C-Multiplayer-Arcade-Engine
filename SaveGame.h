#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include"Scoring.h"
#include"Themes.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct GridNode {
    int row, col;
    int value;
    GridNode* next;

    GridNode(int r, int c, int val) {
        row = r;
        col = c;
        value = val;
        next = nullptr;
    }
};

class SaveGame {
private:
    string PlayerID;
    string TimeStamp;
    GridNode* head;
    string SaveID;

public:
    SaveGame() {
        SaveID = "";
        PlayerID = "";
        TimeStamp = "";
        head = nullptr;
    }

    ~SaveGame() {
        clearGrid();
    }

    void setPlayerID(string id) {
        PlayerID = id;
    }

    void setSaveID(RenderWindow& window, Themes* currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Cannot load font!" << endl;
            return;
        }

        string inputText = "";
        Text prompt("Enter Save ID: ", font, 32);
        prompt.setFillColor(currentTheme->textColor);
        prompt.setPosition(100, 150);

        Text inputDisplay("", font, 32);
        inputDisplay.setFillColor(currentTheme->textColor);
        inputDisplay.setPosition(100, 200);

        while (window.isOpen()) {
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    window.close();
                    return;
                }
                if (e.type == Event::TextEntered) {
                    if (e.text.unicode >= 32 && e.text.unicode <= 126) {
                        inputText += static_cast<char>(e.text.unicode);
                    }
                    else if (e.text.unicode == 8 && !inputText.empty()) {
                        inputText.pop_back();
                    }
                }
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter) {
                    SaveID = inputText;
                    return;
                }
            }

            window.clear(currentTheme->backgroundColor);

            Texture bg;
            if (bg.loadFromFile(currentTheme->image)) {
                Sprite background(bg);
                window.draw(background);
            }

            inputDisplay.setString(inputText);

            window.draw(prompt);
            window.draw(inputDisplay);

            window.display();
        }
    }


    void setGridValue(int row, int col, int value) {
        GridNode* current = head;
        while (current != nullptr) {
            if (current->row == row && current->col == col) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        GridNode* newNode = new GridNode(row, col, value);
        newNode->next = head;
        head = newNode;
    }

    int getGridValue(int row, int col) {
        GridNode* current = head;
        while (current != nullptr) {
            if (current->row == row && current->col == col)
                return current->value;
            current = current->next;
        }
        return 0; // default value
    }

    void clearGrid() {
        while (head) {
            GridNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void saveGame(RenderWindow& window, Themes* currentTheme) {
        TimeStamp = getCurrentTimestamp();
        setSaveID(window,currentTheme);

        ofstream outFile("saved_game.txt", ios::app);
        if (outFile.is_open()) {
            outFile << PlayerID << endl;
            outFile << SaveID << endl;
            outFile << "Timestamp: " << TimeStamp << endl;

            int tempGrid[25][40] = { 0 };
            GridNode* current = head;
            while (current != nullptr) {
                tempGrid[current->row][current->col] = current->value;
                current = current->next;
            }

            for (int i = 0; i < 25; ++i) {
                for (int j = 0; j < 40; ++j) {
                    outFile << tempGrid[i][j] << " ";
                }
                outFile << endl;
            }
            outFile.close();

            Font font;
            if (!font.loadFromFile(currentTheme->font)) {
                cerr << "Cannot load font!" << endl;
                return;
            }

            Text message;
            message.setFont(font);
            message.setCharacterSize(36);
            message.setString("Game Saved!\nPress Enter to continue.");
            message.setFillColor(currentTheme->textColor);

            FloatRect textBounds = message.getLocalBounds();
            message.setOrigin(textBounds.width / 2, textBounds.height / 2);
            message.setPosition(720 / 2, 450 / 2);

            Texture bgTexture;
            bool hasBackground = bgTexture.loadFromFile(currentTheme->image);
            Sprite background(bgTexture);

            while (window.isOpen()) {
                window.clear(currentTheme->backgroundColor);
                if (hasBackground) window.draw(background);
                window.draw(message);
                window.display();

                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed)
                        window.close();
                    if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
                        return;
                }
            }
        }
        else {
            cout << "Error saving the game!" << endl;
        }
    }


    void loadGame(RenderWindow& window, Themes* currentTheme, string user) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Cannot load font!" << endl;
            return;
        }

        string inputText = "";
        Text prompt("Enter Save ID to load: ", font, 32);
        prompt.setFillColor(currentTheme->textColor);
        prompt.setPosition(100, 150);

        Text inputDisplay("", font, 32);
        inputDisplay.setFillColor(currentTheme->textColor);
        inputDisplay.setPosition(100, 200);

        bool idEntered = false;
        string askforsaveid = "";

        while (window.isOpen() && !idEntered) {
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    window.close();
                    return;
                }
                if (e.type == Event::TextEntered) {
                    if (e.text.unicode >= 32 && e.text.unicode <= 126) {
                        inputText += static_cast<char>(e.text.unicode);
                    }
                    else if (e.text.unicode == 8 && !inputText.empty()) {
                        inputText.pop_back();
                    }
                }
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter) {
                    askforsaveid = inputText;
                    idEntered = true;
                }
            }

            window.clear(currentTheme->backgroundColor);

            Texture bg;
            if (bg.loadFromFile(currentTheme->image)) {
                Sprite background(bg);
                window.draw(background);
            }

            inputDisplay.setString(inputText);

            window.draw(prompt);
            window.draw(inputDisplay);

            window.display();
        }

        ifstream inFile("saved_game.txt");
        if (!inFile.is_open()) {
            cerr << "Error loading the game!" << endl;
            return;
        }

        string line;
        while (getline(inFile, line)) {
            string filePlayerID = line;
            string fileSaveID;
            if (!getline(inFile, fileSaveID)) break;
            string timestampLine;
            if (!getline(inFile, timestampLine)) break;

            if (filePlayerID == user && fileSaveID == askforsaveid) {
                clearGrid();
                for (int i = 0; i < 25; ++i) {
                    for (int j = 0; j < 40; ++j) {
                        int val;
                        inFile >> val;
                        if (val != 0) setGridValue(i, j, val);
                    }
                    inFile.ignore();
                }
                inFile.close();
                return;
            }
            else {
                for (int i = 0; i < 25; ++i) getline(inFile, line);
            }
        }

        inFile.close();
    }


    string getCurrentTimestamp() {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);

        char buffer[30];
        sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + ltm.tm_year,
            1 + ltm.tm_mon,
            ltm.tm_mday,
            ltm.tm_hour,
            ltm.tm_min,
            ltm.tm_sec);

        return string(buffer);
    }
};
