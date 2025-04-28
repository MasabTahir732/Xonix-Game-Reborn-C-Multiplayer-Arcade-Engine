#include <SFML/Graphics.hpp>
#include"D:\SEM 4\Data Project\DataStructures Project\login_signup.h"
#include <time.h>
#include<iostream>
#include <fstream> 
using namespace sf;

const int M = 25;
const int N = 40;

int grid[M][N] = { 0 };
int ts = 18; //tile size

struct Enemy
{
    int x, y, dx, dy;

    Enemy()
    {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }

    void move()
    {
        x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }
        y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
    }
};

void drop(int y, int x)
{
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}
void loadPlayersFromFile(Player* playerList, int& currentPlayers, const string& filename) {
    ifstream fin(filename); 
    if (!fin) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string username, password;
    currentPlayers = -1; 

    while (fin >> username >> password) {
        ++currentPlayers;
        playerList[currentPlayers].createAccount(username, password);
    }

    fin.close();
}
void savePlayersToFile(Player* playerList, int currentPlayers, const string& filename) {
    ofstream fout(filename); 
    if (!fout) {
        cout << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (int i = 0; i <= currentPlayers; ++i) {
        fout << playerList[i].getUsername() << " " << playerList[i].getPassword() << endl;
    }

    fout.close(); 
}
int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    int enemyCount = 5;
    Enemy a[10];

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;
    float timer = 0, delay = 0.07;
    Clock clock;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)  grid[i][j] = 1;

    Player* playerlist=new Player[5];
    int currplayer = -1;
    const string playerinformationtextfile = "Players.txt";
    loadPlayersFromFile(playerlist, currplayer, playerinformationtextfile);


    cout << "enter 1 if you want to login\n";
    cout<<"enter 2 if you wanna signup\n";
    int choice = 0;
    cin >> choice;
    if (choice == 1) {
        cout << "enter your Existing username and password\n";
        string username, password;
        cout << "enter username: ";
        cin >> username;
        cout << "enter password: ";
        cin >> password;
        bool found = false;
        found=playerlist->check(username,password,playerlist, currplayer);
        if (found == false) {
            cout << "enter new username and password\n";
            string username, password;
            cout << "enter username: ";
            cin >> username;
            cout << "enter password: ";
            cin>> password;
            playerlist[++currplayer].createAccount(username, password);
            savePlayersToFile(playerlist, currplayer, playerinformationtextfile );
            cout<<playerlist[currplayer].getUsername()<<endl;
            cout << playerlist[currplayer].getPassword() << endl;
        }
    }
    else if (choice == 2) {
        cout << "enter new username and password\n";
        string username, password;
        cout << "enter username: ";
        cin >> username;
        cout << "enter password: ";
        cin>> password;
        playerlist[++currplayer].createAccount(username, password);
        cout << playerlist[currplayer].getUsername() << endl;
        cout << playerlist[currplayer].getPassword() << endl;
    }

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;

                    x = 10; y = 0;
                    Game = true;
                }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; };
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; };

        if (!Game) continue;

        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0) x = 0; if (x > N - 1) x = N - 1;
            if (y < 0) y = 0; if (y > M - 1) y = M - 1;

            if (grid[y][x] == 2) Game = false;
            if (grid[y][x] == 0) grid[y][x] = 2;
            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++) a[i].move();

        if (grid[y][x] == 1)
        {
            dx = dy = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1) grid[i][j] = 0;
                    else grid[i][j] = 1;
        }

        for (int i = 0; i < enemyCount; i++)
            if (grid[a[i].y / ts][a[i].x / ts] == 2) Game = false;

        /////////draw//////////
       window.clear();

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
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
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }

        if (!Game) window.draw(sGameover);

        window.display();
    }

    return 0;
}