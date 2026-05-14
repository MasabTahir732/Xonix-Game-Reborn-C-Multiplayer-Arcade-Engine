#pragma once

//Masab Tahir 23i-0006
//Muhammad Arslan 23i-0572
//Xonix Game
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
#include"SaveGame.h"
#include"Themes.h"
AVLTree themes;
const int M = 25;
const int N = 40;
int grid[M][N] = { 0 };
int ts = 18;
struct Enemy {
    int x, y, dx, dy;
    Enemy() {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }
    void move() {
        x += dx; if (grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 3||grid[y / ts][x / ts] == 5) { dx = -dx; x += dx; }
        y += dy; if (grid[y / ts][x / ts] == 1|| grid[y / ts][x / ts] == 3|| grid[y / ts][x / ts] == 5) { dy = -dy; y += dy; }
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
int PauseMenu(RenderWindow& window, Themes* currentTheme) {
    Font font;
    if (!font.loadFromFile(currentTheme->font)) {
        cerr << "Failed to load font!" << endl;
        return 0; 
    }

    const int numOptions = 2;
    string options[numOptions] = { "Resume Game", "Return to Main Menu" };

    Text menuItems[numOptions];
    for (int i = 0; i < numOptions; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setString(options[i]);
        menuItems[i].setCharacterSize(36);
        menuItems[i].setPosition(200, 150 + i * 60);
        menuItems[i].setFillColor(currentTheme->textColor);
    }

    int selectedItem = 0;

    while (window.isOpen()) {
        window.clear(currentTheme->backgroundColor);

   
        Texture image;
        if (image.loadFromFile(currentTheme->image)) {
            Sprite background(image);
            background.setPosition(0, 0);
            window.draw(background);
        }


        for (int i = 0; i < numOptions; i++) {
            if (i == selectedItem)
                menuItems[i].setFillColor(Color::Yellow);  
            else
                menuItems[i].setFillColor(currentTheme->textColor);
            window.draw(menuItems[i]);
        }

        window.display();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::W) {
                    selectedItem--;
                    if (selectedItem < 0) selectedItem = numOptions - 1;
                }
                if (event.key.code == Keyboard::S) {
                    selectedItem++;
                    if (selectedItem >= numOptions) selectedItem = 0;
                }
                if (event.key.code == Keyboard::Enter) {
                    if (selectedItem == 0) return 0;
                    else if (selectedItem == 1) return 1; 
                }
            }
        }
    }

    return 0; 
}
int GameOverMenu(sf::RenderWindow& window, Themes* currentTheme, Player* player) {
    Font font;
    if (!font.loadFromFile(currentTheme->font)) {
        cerr << "Failed to load font!" << endl;
    }

    const int numOptions = 2;
    string options[numOptions] = { "Return to Main Menu", "Quit Game" };

    Text menuItems[numOptions];
    for (int i = 0; i < numOptions; ++i) {
        menuItems[i].setFont(font);
        menuItems[i].setString(options[i]);
        menuItems[i].setCharacterSize(36);
        menuItems[i].setPosition(200, 250 + i * 60);
        menuItems[i].setFillColor(currentTheme->textColor);
    }

   Text title("GAME OVER", font, 50);
    title.setFillColor(sf::Color::Red);
    title.setPosition(200, 50);


   string scoreTextStr = "Final Score: " + to_string(player->returnHead()->CurrentScore);
   Text scoreText(scoreTextStr, font, 30);
    scoreText.setFillColor(currentTheme->textColor);
    scoreText.setPosition(200, 150);

    int selectedItem = 0;

    while (window.isOpen()) {
        window.clear(currentTheme->backgroundColor);


        Texture image;
        if (image.loadFromFile(currentTheme->image)) {
            Sprite background(image);
            background.setPosition(0, 0);
            window.draw(background);
        }

        window.draw(title);
        window.draw(scoreText);

        for (int i = 0; i < numOptions; ++i) {
            if (i == selectedItem)
                menuItems[i].setFillColor(Color::Yellow);
            else
                menuItems[i].setFillColor(currentTheme->textColor);
            window.draw(menuItems[i]);
        }

        window.display();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::W) {
                    selectedItem--;
                    if (selectedItem < 0) selectedItem = numOptions - 1;
                }
                if (event.key.code == Keyboard::S) {
                    selectedItem++;
                    if (selectedItem >= numOptions) selectedItem = 0;
                }
                if (event.key.code == Keyboard::Enter) {
                    if (selectedItem == 0) return 1; 
                    else if (selectedItem == 1) return -1;  
                }
            }
        }
    }

    return 0;  
}



class MainGame {
public:


    
    int Display(RenderWindow& window, Player* PlayerThatisPlaying, Themes* currentTheme) {
        PlayerThatisPlaying->returnHead()->CurrentScore = 0;

        window.clear();

        window.setFramerateLimit(70);
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


        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (i == 0 || j == 0 || i == M - 1 || j == N - 1) { grid[i][j] = 1; }
                else {
                    grid[i][j] = 0;
                }
            }
        }

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
                    return 1;

                if (e.type == Event::Closed ||
                    (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
                    if (!scoreSaved) {
                        PlayerThatisPlaying->updateTotalScore();
                        scoreSaved = true;
                    }
                    return 1;
                }
                if (Keyboard::isKeyPressed(Keyboard::P)) {
                    int choice;
                    choice = PauseMenu(window, currentTheme);
                    if (choice == 1) {
                        return 1;
                    }

                }
            }
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx = -1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx = 1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx = 0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx = 0; dy = 1; }
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                if (PlayerThatisPlaying->getPowerUpCount() == 1 && !isPowerActive) {
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

                    game.saveGame(window,currentTheme);
                    savePressed = true;
                }
            }

            else {
                savePressed = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::F8)) {
                if (!loadPressed) {
                    SaveGame loadedGame;
                    loadedGame.loadGame(window,currentTheme,PlayerThatisPlaying->getUsername());
                    for (int i = 0; i < 25; ++i) {
                        for (int j = 0; j < 40; ++j) {
                            grid[i][j] = loadedGame.getGridValue(i, j);

                        }
                    }
                    loadPressed = true;
                }
            }


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
                    PlayerThatisPlaying->displayScore(window, currentTheme);
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



            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "cannot load theme!" << endl;
            }
            Sprite B(image);
            B.setPosition(0, 0);
            window.draw(B);
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
            PlayerThatisPlaying->displayScore(window, currentTheme);
            if (!Game) {
                int c = GameOverMenu(window, currentTheme, PlayerThatisPlaying);
                return c;
            }
            window.display();

        }
    }
   
};




























class Multiplayer {
public:
    bool player1IsMoving = false;
    bool player2IsMoving = false;

    void dropTilesBasedOnPlayerMovement() {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                if (grid[i][j] == 1) { 
                    if (player1IsMoving) {
                        grid[i][j] = 2;  
                    }
                    else if (player2IsMoving) {
                        grid[i][j] = 3; 
                    }
                }
                else if (grid[i][j] == 2) {
                    if (player1IsMoving) {
                        grid[i][j] = 1;  
                    }
                }
                else if (grid[i][j] == 3) {
                    if (player2IsMoving) {
                        grid[i][j] = 4;  
                    }
                }
            }
        }
    }


    int Display(RenderWindow& window, Player* PlayerThatisPlaying, Player* Player2, Themes* currentTheme) {
        PlayerThatisPlaying->returnHead()->CurrentScore = 0;
        Player2->returnHead()->CurrentScore=0;
        sf::Text scoreTextPlayer2;
        int player2Score = 0;
        sf::Text scoreTextPlayer1;
        int player1Score = 0;
        

        sf::Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Failed to load font in displayScore!\n";
            return 1;
        }
        sf::Text powerupText;
        powerupText.setFont(font);
        powerupText.setCharacterSize(17);
        powerupText.setFillColor(sf::Color::Red);
        powerupText.setString("PowerUp available (Press Space to Use)");
        powerupText.setPosition(250, 30);
       // if (Player2->PowerUpCount != 0) {
       //     window.draw(powerupText);
       // }
        scoreTextPlayer1.setFont(font);
        scoreTextPlayer1.setCharacterSize(20);
        scoreTextPlayer1.setFillColor(currentTheme->textColor);
        scoreTextPlayer1.setString("Score of player2: " + to_string(player2Score));
        scoreTextPlayer1.setPosition(250, 10);

        scoreTextPlayer2.setFont(font);
        scoreTextPlayer2.setCharacterSize(20);
        scoreTextPlayer2.setFillColor(currentTheme->textColor);
        scoreTextPlayer2.setString("Score of player2: " + to_string(player2Score));
        scoreTextPlayer2.setPosition(450, 10);

        window.clear();

        window.setFramerateLimit(70);
        Texture t1, t2, t3;
        t1.loadFromFile("images/tiles.png");
        t2.loadFromFile("images/gameover.png");
        t3.loadFromFile("images/enemy.png");
        sf::Texture t4;
        t4.loadFromFile("images/Player2.png");
        Sprite sTile(t1), sGameover(t2), sEnemy(t3);
        sf::Sprite Player2Tile(t4);
        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(20, 20);

        int enemyCount = 4;
        Enemy a[10];
        bool Game = true;
        int x0 = 0, y0 = 0, dx0 = 0, dy0 = 0;
        int x1 = M - 1, y1 = N - 1, dx1 = 0, dy1 = 0;
        float timer = 0, delay = 0.07;
        Clock clock;
        Clock powerClock;
        bool isPowerActive = false;


        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (i == 0 || j == 0 || i == M - 1 || j == N - 1) { grid[i][j] = 1; }
                else {
                    grid[i][j] = 0;
                }
            }
        }



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
                    return 1;

                if (e.type == Event::Closed ||
                    (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
                    if (!scoreSaved) {
                        PlayerThatisPlaying->updateTotalScore();
                        scoreSaved = true;
                    }
                    return 1;
                }
                if (Keyboard::isKeyPressed(Keyboard::P)) {
                    int choice;
                    choice = PauseMenu(window, currentTheme);
                    if (choice == 1) {
                        return 1;
                    }

                }
            }
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx0 = -1; dy0 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx0 = 1; dy0 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx0 = 0; dy0 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx0 = 0; dy0 = 1; }
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                if (PlayerThatisPlaying->getPowerUpCount() == 1 && !isPowerActive) {
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

                    game.saveGame(window, currentTheme);
                    savePressed = true;
                }
            }

            else {
                savePressed = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::F8)) {
                if (!loadPressed) {
                    SaveGame loadedGame;
                    loadedGame.loadGame(window, currentTheme, PlayerThatisPlaying->getUsername());
                    for (int i = 0; i < 25; ++i) {
                        for (int j = 0; j < 40; ++j) {
                            grid[i][j] = loadedGame.getGridValue(i, j);

                        }
                    }
                    loadPressed = true;
                }
            }
            ///////////////////////////////////////////////////////////////////////////////////
        /*for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (grid[i][j] == 0) { cout << "."<<" "; }
                    else {
                        cout << grid[i][j] << ' ';
                    }
                }
                cout << '\n';
            }
         */
         ////////////////////////////////////////////////////////////////////////////////////////

            if (timer > delay) {
                x0 += dx0; y0 += dy0;
                x1 += dx1; y1 += dy1;
                if (x0 < 0) x0 = 0; if (x0 > N - 1) x0 = N - 1;
                if (y0 < 0) y0 = 0; if (y0 > M - 1) y0 = M - 1;
                if (x1 < 0) x1 = 0; if (x1 > N - 1) x1 = N - 1;
                if (y1 < 0) y1 = 0; if (y1 > M - 1) y1 = M - 1;

                if (grid[y0][x0] == 2) {
                    if (!scoreSaved) {
                        PlayerThatisPlaying->updateTotalScore();
                        scoreSaved = true;
                    }
                    Game = false;
                }
              //  if (grid[y1][x1] == 3||grid[y1][x1]==4) {
              //      if (!scoreSaved) {
              //          Player2->updateTotalScore();
              //          scoreSaved = true;
              //      }
              //      Game = false;
              //  }

               

                if (grid[y0][x0] == 0) grid[y0][x0] = 2;
                if (grid[y1][x1] == 0) grid[y1][x1] = 4;

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

            dropTilesBasedOnPlayerMovement();

            if (grid[y0][x0] == 1 ) {
                dx0 = dy0 = 0;

                if (grid[y1][x1] == 1) {
                    dx1 = dy1 = 0;
                }

                for (int i = 0; i < enemyCount; i++) {
                    drop(a[i].y / ts, a[i].x / ts);
                }

                int tilesBeforeP1 = 0, tilesBeforeP2 = 0;
                for (int i = 1; i < M - 1; ++i) {
                    for (int j = 1; j < N - 1; ++j) {
                        if (grid[i][j] == 5) tilesBeforeP1++;
                        if (grid[i][j] == 3) tilesBeforeP2++;
                    }
                }

                for (int i = 1; i < M - 1; ++i) {
                    for (int j = 1; j < N - 1; ++j) {
                        if (grid[i][j] == -1) {
                            grid[i][j] = 0; 
                        }
                        else if (grid[i][j] == 2) {
                            grid[i][j] = 5; 
                        }
                        else if (grid[i][j] == 3) {
                            grid[i][j] = 4;
                        }
                        else if (grid[i][j] == 0) {
                            bool nearP2 = false;

                            if (grid[i + 1][j] == 3 || grid[i + 1][j] == 4 ||
                                grid[i - 1][j] == 3 || grid[i - 1][j] == 4 ||
                                grid[i][j + 1] == 3 || grid[i][j - 1] == 4 ||
                                grid[i][j - 1] == 3 || grid[i][j + 1] == 4) {
                                nearP2 = true;
                            }

                            if (nearP2)
                                grid[i][j] = 4; 
                            else
                                grid[i][j] = 5; 
                        }

                    }
                }

                int tilesAfterP1 = 0, tilesAfterP2 = 0;
                for (int i = 1; i < M - 1; ++i) {
                    for (int j = 1; j < N - 1; ++j) {
                        if (grid[i][j] == 5) tilesAfterP1++;
                        if (grid[i][j] == 4) tilesAfterP2++;
                    }
                }

                int newTilesP1 = tilesAfterP1 - tilesBeforeP1;
                int newTilesP2 = tilesAfterP2 - tilesBeforeP2;
                cout << "tiles before: " << tilesBeforeP2 << " " << tilesAfterP2 << " " << newTilesP2 << endl;

                if (newTilesP1 > 0) {
                    PlayerThatisPlaying->updateScore(newTilesP1);
                    PlayerThatisPlaying->displayScore(window,currentTheme);
                }
                if (newTilesP2 > 0) {
                    player2Score = newTilesP2;
                    int scoreGain = newTilesP2;

                    if (Player2->returnHead()->RewardCounter > 5 && newTilesP2 >= 5) {
                        scoreGain = newTilesP2 * 4;
                        Player2->returnHead()->RewardCounter++;
                    }
                    else if (Player2->returnHead()->RewardCounter > 3 && newTilesP2 >= 5) {
                        scoreGain = newTilesP2 * 2;
                        Player2->returnHead()->RewardCounter++;
                    }
                    else if (newTilesP2 >= 10) {
                        scoreGain = newTilesP2 * 2;
                        Player2->returnHead()->RewardCounter++;
                    }

                    Player2->returnHead()->CurrentScore += scoreGain;
                    player2Score = scoreGain;


                    if (Player2->returnHead()->CurrentScore > 50 ||player2Score>50) {
                        Player2->returnHead()->PowerUpCount++;
                        std::cout << "Press SPACE to use power-up.\n";
                    }

                    scoreTextPlayer2.setString("Score: " + std::to_string(tilesAfterP2));
                    window.draw(scoreTextPlayer2);
                    //Player2->displayScore(window, currentTheme);
//                    cout << "hello";

                   // Player2->updateScore(10);
                   // Player2->displayScore(window, currentTheme);
                   
                }

            }




            for (int i = 0; i < enemyCount; i++) {

                if (grid[a[i].y / ts][a[i].x / ts] == 2 ) {
                    if (!scoreSaved) {
                        PlayerThatisPlaying->updateTotalScore();
                        scoreSaved = true;
                        cout << "Player1 LOOOOOOOOOOOSE";

                    }
                    Game = false;
                }
            }
            for (int i = 0; i < enemyCount; i++) {

                if ( grid[a[i].y / ts][a[i].x / ts] == 4 || grid[a[i].y / ts][a[i].x / ts] == 3) {
                    if (!scoreSaved) {
                        Player2->updateTotalScore();
                        scoreSaved = true;
                        cout << "PLayer 2 LOOOOOOOSE";

                    }
                    Game = false;
                }
            }


            for (int i = 1; i < M - 1; ++i) {
                for (int j = 1; j < N - 1; ++j) {
                    if (grid[i][j] == 4) {
                        for (int k = j; grid[i][k] == 4 || grid[i][k] == 1; ++k) {
                            grid[i][k] = 3;
                        }
                    }
                }
            }
            for (int i = 1; i < M - 1; ++i) {
                for (int j = 1; j < N - 1; ++j) {
                    if (grid[i][j] == 1) {
                        grid[i][j] = 5;

                    }
                }
            }

            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "cannot load theme!" << endl;
            }
            Sprite B(image);
            B.setPosition(0, 0);
            window.draw(B);
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == 0) continue;

                    if (grid[i][j] == 1 || grid[i][j] == 5) {
                        sTile.setTextureRect(IntRect(0, 0, ts, ts)); 
                        sTile.setPosition(j * ts, i * ts);
                        window.draw(sTile);
                    }
                    else if (grid[i][j] == 2) {
                        sTile.setTextureRect(IntRect(54, 0, ts, ts)); 
                        sTile.setPosition(j * ts, i * ts);
                        window.draw(sTile);
                    }
                    else if (grid[i][j] == 3) {
                        Player2Tile.setPosition(j * ts, i * ts); 
                        window.draw(Player2Tile);
                    }
                    else if (grid[i][j] == 4) {
                        Player2Tile.setPosition(j * ts, i * ts); 
                        window.draw(Player2Tile);
                    }
                }
            }



            
            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x0 * ts, y0 * ts);
            Player2Tile.setPosition(x1 * ts, y1 * ts);
            window.draw(sTile);
            window.draw(Player2Tile);


            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }
            PlayerThatisPlaying->displayScore(window, currentTheme);
            if (!Game) {
                int c = GameOverMenu(window, currentTheme, PlayerThatisPlaying);
                return c;
            }
            window.draw(scoreTextPlayer2);

           
            window.display();

        }
    }

};

