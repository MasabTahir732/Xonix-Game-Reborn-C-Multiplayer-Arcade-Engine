#pragma once

#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
using namespace std;
const int M = 25;
const int N = 40;
int grid[M][N] = { 0 };
int ts = 18;

class Game {
public:
	virtual void Display(RenderWindow& window, Player* PlayerThatisPlaying) = 0;
    struct Enemy {
        int x, y, dx, dy;
        Enemy() {
            x = y = 300;
            dx = 4 - rand() % 8;
            dy = 4 - rand() % 8;
        }
        void move() {
            x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }
            y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
        }
    };
    void drop(int y, int x) {
        if (y < 0 || y >= M || x < 0 || x >= N) return;
        if (grid[y][x] != 0) return;  // Only fill empty space (0)

        grid[y][x] = -1;  // Mark safe zone (connected to enemy)

        drop(y - 1, x);
        drop(y + 1, x);
        drop(y, x - 1);
        drop(y, x + 1);
    }

};





class SingleGame :public Game{
public:

    SingleGame() {
    }

    virtual void Display(RenderWindow& window, Player* PlayerThatisPlaying) {
        window.clear();
        window.setFramerateLimit(60);
        Texture t1, t2, t3;
        t1.loadFromFile("images/tiles.png");
        t2.loadFromFile("images/gameover.png");
        t3.loadFromFile("images/enemy.png");

        Sprite sTile(t1), sGameover(t2), sEnemy(t3);
        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(20, 20);

        int enemyCount = 4;
        Enemy a[10];
        bool Game = true;
        int x = 0, y = 0, dx = 0, dy = 0;
        float timer = 0, delay = 0.07;
        Clock clock;
        Clock powerClock;
        bool isPowerActive = false;


        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                if (i == 0 || j == 0 || i == M - 1 || j == N - 1) grid[i][j] = 1;




        bool savePressed = false;
        bool loadPressed = false;
        bool scoreSaved = false;



        while (window.isOpen()) {

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            // PlayerThatisPlaying->HighScoreUpdation(s1.getScore());
          //  PlayerThatisPlaying->updatePlayerScore(PlayerThatisPlaying->getUsername(), s1.getScore());
           //  PlayerThatisPlaying->HighScoreUpdation(s1.getScore());
             //PlayerThatisPlaying->updateTotalScore();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                    return;
            }
            if (e.type == Event::Closed ||
                (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
                if (!scoreSaved) {
                    PlayerThatisPlaying->updateTotalScore();
                    scoreSaved = true;
                }
                return;
            }

            if (Keyboard::isKeyPressed(Keyboard::A)) { dx = -1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx = 1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx = 0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx = 0; dy = 1; }
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                if (PlayerThatisPlaying->getPowerUpCount() >= 1 && !isPowerActive) {
                    isPowerActive = true;
                    powerClock.restart();
                    
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::F7)) {
                if (!savePressed) {
                    SaveGame game;
                    game.setPlayerID(PlayerThatisPlaying->getUsername());

                    for (int i = 0; i < M; ++i)
                        for (int j = 0; j < N; ++j)
                            game.setGridValue(i, j, grid[i][j]);

                    game.saveGame();
                    savePressed = true;
                }
            }

            else {
                savePressed = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::F8)) {
                if (!loadPressed) {
                    SaveGame loadedGame;
                    loadedGame.loadGame(PlayerThatisPlaying->getUsername());
                    for (int i = 0; i < 25; ++i) {
                        for (int j = 0; j < 40; ++j) {
                            grid[i][j] = loadedGame.getGridValue(i, j);

                        }
                    }
                    loadPressed = true;
                }
            }
            if (!Game) continue;

            if (timer > delay) {
                x += dx; y += dy;
                if (x < 0) x = 0; if (x > N - 1) x = N - 1;
                if (y < 0) y = 0; if (y > M - 1) y = M - 1;
                if (grid[y][x] == 2) {
                    if (!scoreSaved) {
                        PlayerThatisPlaying->updateTotalScore();
                        scoreSaved = true;
                    }
                    Game = false;
                }

                if (grid[y][x] == 0) grid[y][x] = 2;
                timer = 0;
            }

            if (isPowerActive) {
                if (powerClock.getElapsedTime().asSeconds() > 3.0f) {
                    isPowerActive = false;
                }
            }
            else {
                for (int i = 0; i < enemyCount; i++) a[i].move();
            }

            if (grid[y][x] == 1) {
                dx = dy = 0;

                for (int i = 0; i < enemyCount; i++)
                    drop(a[i].y / ts, a[i].x / ts);

                int tilesBefore = 0;
                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j)
                        if (grid[i][j] == 1)
                            tilesBefore++;

                for (int i = 0; i < M; ++i) {
                    for (int j = 0; j < N; ++j) {
                        if (grid[i][j] == -1)
                            grid[i][j] = 0;
                        else if (grid[i][j] == 2)
                            grid[i][j] = 1;
                        else if (grid[i][j] == 0)
                            grid[i][j] = 1;
                    }
                }

                int tilesAfter = 0;
                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j)
                        if (grid[i][j] == 1)
                            tilesAfter++;

                int newTiles = tilesAfter - tilesBefore;
                if (newTiles > 0) {
                    PlayerThatisPlaying->updateScore(newTiles);
                    PlayerThatisPlaying->displayScore();
                }
            }



            for (int i = 0; i < enemyCount; i++) {
                if (grid[a[i].y / ts][a[i].x / ts] == 2) {
                    if (!scoreSaved) {
                        PlayerThatisPlaying->updateTotalScore();
                        scoreSaved = true;
                    }
                    Game = false;
                }
            }


            window.clear();
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == 0) continue;
                    if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    sTile.setPosition(j * ts, i * ts);
                    window.draw(sTile);
                }

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            window.draw(sTile);

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }

            if (!Game) window.draw(sGameover);
            window.display();
        }
    }

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inside MultiPlayer class
class MultiPlayer : public Game {
    
    void Display(RenderWindow& window, Player* PlayerThatisPlaying) {
        window.clear();
        window.setFramerateLimit(60);

        Texture t1, t2, t3, t4;
        t1.loadFromFile("images/tiles.png");       // Normal tile
        t2.loadFromFile("images/gameover.png");    // Game over image
        t3.loadFromFile("images/enemy.png");       // Enemy sprite
        t4.loadFromFile("images/player2tiles.png"); // Player 2 tile

        Sprite sTile(t1), sTileP2(t4), sGameover(t2), sEnemy(t3);
        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(20, 20);

        int enemyCount = 4;
        Enemy a[10];
        bool Game = true;
        int x1 = 1, y1 = 1, dx1 = 0, dy1 = 0;       // Player 1
        int x2 = N - 2, y2 = M - 2, dx2 = 0, dy2 = 0; // Player 2

        float timer = 0, delay = 0.07;
        Clock clock;

        // Initialize borders
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

        while (window.isOpen()) {
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape))
                    return;
            }

            // Controls for Player 1 (Arrow Keys)
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }

            // Controls for Player 2 (WASD)
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }

            if (timer > delay) {
                // Player 1 movement
                x1 += dx1; y1 += dy1;
                if (grid[y1][x1] == 0) grid[y1][x1] = 2;

                // Player 2 movement
                x2 += dx2; y2 += dy2;
                if (grid[y2][x2] == 0) grid[y2][x2] = 3;

                timer = 0;
            }

            // Draw everything
            window.clear();
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == 0) continue;
                    if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));         // solid tile
                    if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));        // P1 trail
                    if (grid[i][j] == 3) sTileP2.setTextureRect(IntRect(0, 0, ts, ts));       // P2 tile

                    Sprite& currentTile = (grid[i][j] == 3) ? sTileP2 : sTile;
                    currentTile.setPosition(j * ts, i * ts);
                    window.draw(currentTile);
                }
            }

            // Draw current positions
            sTile.setTextureRect(IntRect(36, 0, ts, ts));  // P1 current position
            sTile.setPosition(x1 * ts, y1 * ts);
            window.draw(sTile);

            sTileP2.setTextureRect(IntRect(0, 0, ts, ts));  // P2 current position
            sTileP2.setPosition(x2 * ts, y2 * ts);
            window.draw(sTileP2);

            // Draw enemies
            for (int i = 0; i < enemyCount; i++) {
                a[i].move();
                sEnemy.setPosition(a[i].x, a[i].y);
                sEnemy.rotate(10);
                window.draw(sEnemy);
            }

            window.display();
        }
    }

};