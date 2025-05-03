#pragma once
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;


class Screen {
public:
    sf::RenderWindow window;

    Screen()
        : window(sf::VideoMode(720,450 ), "Xonix Project by 23i-0572 & 23i-0006") {
        window.setFramerateLimit(60);
        
    }

    void clear() {
        window.clear(sf::Color::Black);
    }

    void display() {
        window.display();
    }

    void drawSprite(sf::Sprite sprite) {
        window.draw(sprite);
    }

    void drawRectangle(sf::RectangleShape& rectangle) {

        window.draw(rectangle);

    }

    void drawText(sf::Text text) {

        window.draw(text);

    }

    void drawLine(const sf::Vertex v1[], int a) {

        window.draw(v1, a, sf::Lines);

    }

    void close() {

        window.close();

    }

    bool isOpen() const {
        return window.isOpen();
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
};
