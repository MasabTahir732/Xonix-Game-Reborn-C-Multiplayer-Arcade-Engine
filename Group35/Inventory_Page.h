#pragma once

//Masab Tahir 23i-0006
//Muhammad Arslan 23i-0572
//Xonix Game
#include<iostream>
#include<SFML/Graphics.hpp>
#include<string>
#include<sstream>
#include"Themes.h"
#include"login_signup.h"

using namespace std;
using namespace sf;

class InventoryPage {
    AVLTree* themesTree;
    Themes** themeArray;
    int themeCount;
    int currentIndex;

    void fillThemeArray(Themes* node, Themes** arr, int& index) {
        if (!node) return;
        fillThemeArray(node->llink, arr, index);
        arr[index++] = node;
        fillThemeArray(node->rlink, arr, index);
    }

    int SearchPage(RenderWindow& window, Font& font, Themes* currentTheme) {
        string searchInput = "";
        string message = "";

        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);

            Text searchText("Enter Theme ID to search: " + searchInput, font, 24);
            searchText.setPosition(100, 150);
            searchText.setFillColor(currentTheme->textColor);
            window.draw(searchText);

            if (!message.empty()) {
                Text msgText(message, font, 20);
                msgText.setPosition(100, 220);
                msgText.setFillColor(Color::Red);
                window.draw(msgText);
            }

            Text instructions("Enter: Search | Esc: Back to Inventory", font, 20);
            instructions.setPosition(100, 300);
            instructions.setFillColor(currentTheme->textColor);
            window.draw(instructions);

            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();

                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::Escape) {
                        return -1;  
                    }
                    else if (e.key.code == Keyboard::Enter) {
                        if (!searchInput.empty()) {
                            int idToSearch = stoi(searchInput);
                            Themes* found = themesTree->search(idToSearch);
                            if (found) {
                               
                                for (int i = 0; i < themeCount; i++) {
                                    if (themeArray[i]->id == idToSearch) {
                                        currentIndex = i;
                                        break;
                                    }
                                }
                                message = "Theme found and previewed!";
                            }
                            else {
                                message = "Theme not found.";
                            }
                        }
                    }
                }
                else if (e.type == Event::TextEntered) {
                    if (e.text.unicode >= '0' && e.text.unicode <= '9') {
                        searchInput += static_cast<char>(e.text.unicode);
                    }
                    else if (e.text.unicode == '\b' && !searchInput.empty()) {
                        searchInput.pop_back();
                    }
                }
            }
        }
        return -1;  
    }

public:
    InventoryPage(AVLTree* tree)
        : themesTree(tree) {
        themeCount = themesTree->countThemes();
        themeArray = new Themes * [themeCount];
        int idx = 0;
        fillThemeArray(tree->getRoot(), themeArray, idx);
        currentIndex = 0;
    }

    ~InventoryPage() {
        delete[] themeArray;
    }

    int Display(RenderWindow& window, Player* PlayerThatisPlaying, Themes*& currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Cannot load font from file!" << endl;
        }

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

            Texture previewTexture;
            if (!previewTexture.loadFromFile(themeArray[currentIndex]->image)) {
                cerr << "Cannot load preview image: " << themeArray[currentIndex]->image << endl;
                RectangleShape preview;
                preview.setSize(sf::Vector2f(300, 200));
                preview.setPosition(100, 100);
                preview.setFillColor(themeArray[currentIndex]->backgroundColor);
                window.draw(preview);
            }
            else {
                Sprite previewSprite(previewTexture);
                sf::Vector2u texSize = previewTexture.getSize();
                previewSprite.setScale(300.f / texSize.x, 200.f / texSize.y);
                previewSprite.setPosition(100, 100);
                window.draw(previewSprite);
            }

            Text themeName("Theme: " + to_string(themeArray[currentIndex]->id), font, 24);
            themeName.setPosition(100, 320);
            themeName.setFillColor(themeArray[currentIndex]->textColor);
            window.draw(themeName);

            Text desc("Font: " + themeArray[currentIndex]->font, font, 20);
            desc.setPosition(100, 360);
            desc.setFillColor(themeArray[currentIndex]->textColor);
            window.draw(desc);

            Text instructions("W/S: Browse | Enter: Apply Temporarily | F: Search by ID | P: Save as Preferred | Esc: Back", font, 20);
            instructions.setPosition(0, 420);
            instructions.setFillColor(themeArray[currentIndex]->textColor);
            window.draw(instructions);

            if (!message.empty()) {
                Text msgText(message, font, 20);
                msgText.setPosition(100, 460);
                msgText.setFillColor(Color::Red);
                window.draw(msgText);
            }

            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();

                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::W) {
                        currentIndex = (currentIndex - 1 + themeCount) % themeCount;
                        message = "";
                    }
                    else if (e.key.code == Keyboard::S) {
                        currentIndex = (currentIndex + 1) % themeCount;
                        message = "";
                    }
                    else if (e.key.code == Keyboard::Enter) {
                        currentTheme = themeArray[currentIndex];
                        message = "Theme applied successfully! Returning to main menu...";

                     
                        window.clear(currentTheme->backgroundColor);
                        Text successText(message, font, 24);
                        successText.setFillColor(Color::Green);
                        successText.setPosition(100, 200);
                        window.draw(successText);
                        window.display();

                        sf::sleep(sf::seconds(1));  
                        return 1;  
                    }
                    else if (e.key.code == Keyboard::P) {
                        currentTheme = themeArray[currentIndex];
                        PlayerThatisPlaying->updatePrefTheme(currentTheme->id);
                        message = "Preferred theme saved!";
                    }
                    else if (e.key.code == Keyboard::F) {
                        int result = SearchPage(window, font, currentTheme);
                        if (result == 0) {
                            return 0;  
                        }
                      
                    }
                    else if (e.key.code == Keyboard::Escape) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
};
