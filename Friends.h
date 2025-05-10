#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <cstring>
#include "login_signup.h"

using namespace sf;
using namespace std;

class FriendsPage {
    Player* allPlayers;
    int totalPlayers;
    string pendingRequests[50][2];
    int requestCount;
    Node* hashTable[101];

    int hashFunction(const string& username) {
        int sum = 0;
        for (char c : username) sum += c;
        return sum % 101;
    }

    void buildHashTable() {
        for (int i = 0; i < 101; i++) hashTable[i] = nullptr;
        Node* curr = allPlayers->returnHead();
        while (curr) {
            int idx = hashFunction(curr->Username);
            while (hashTable[idx] != nullptr) idx = (idx + 1) % 101;
            hashTable[idx] = curr;
            curr = curr->next;
        }
    }

    Node* searchPlayerByUsername(const string& uname) {
        int idx = hashFunction(uname);
        int start = idx;
        while (hashTable[idx] != nullptr) {
            if (hashTable[idx]->Username == uname) return hashTable[idx];
            idx = (idx + 1) % 101;
            if (idx == start) break;
        }
        return nullptr;
    }

    bool isAlreadyFriend(Node* player, string friendName) {
        Node* f = player->friendsHead;
        while (f) {
            if (f->Username == friendName) return true;
            f = f->next;
        }
        return false;
    }

    void addFriend(Node* player, string friendName) {
        if (!player) return;
        if (isAlreadyFriend(player, friendName)) return;
        Node* newFriend = new Node();
        newFriend->Username = friendName;
        newFriend->next = nullptr;

        if (!player->friendsHead) {
            player->friendsHead = newFriend;
        }
        else {
            Node* last = player->friendsHead;
            while (last->next) last = last->next;
            last->next = newFriend;
        }
    }

    void loadFriendships() {
        ifstream fin("Friends.txt");
        if (!fin.is_open()) return;
        string username;
        while (fin >> username) {
            string dummyColon;
            fin >> dummyColon;
            Node* player = searchPlayerByUsername(username);
            if (!player) { getline(fin, dummyColon); continue; }
            string friendName;
            while (fin.peek() != '\n' && fin >> friendName) addFriend(player, friendName);
            fin.ignore();
        }
        fin.close();
    }

    void loadRequests() {
        requestCount = 0;
        ifstream fin("Requests.txt");
        if (!fin.is_open()) return;
        string from, to;
        while (fin >> from >> to) {
            pendingRequests[requestCount][0] = from;
            pendingRequests[requestCount][1] = to;
            cout << "Loaded request: " << from << " -> " << to << endl;
            requestCount++;
        }
        fin.close();
    }

    void saveFriendships() {
        ofstream fout("Friends.txt");
        Node* temp = allPlayers->returnHead();
        while (temp) {
            fout << temp->Username << ": ";
            Node* f = temp->friendsHead;
            while (f) {
                fout << f->Username << " ";
                f = f->next;
            }
            fout << endl;
            temp = temp->next;
        }
        fout.close();
    }

    void saveRequests() {
        ofstream fout("Requests.txt");
        for (int i = 0; i < requestCount; i++) {
            if (!pendingRequests[i][0].empty() && !pendingRequests[i][1].empty())
                fout << pendingRequests[i][0] << " " << pendingRequests[i][1] << endl;
        }
        fout.close();
    }

public:
    FriendsPage(Player* players) {
        allPlayers = players;
        totalPlayers = 0;
        Node* temp = allPlayers->returnHead();
        while (temp) { totalPlayers++; temp = temp->next; }
        buildHashTable();
        loadFriendships();
        loadRequests();
    }
    bool areFriendsInFile(const string& user1, const string& user2) {
        ifstream fin("Friends.txt");
        if (!fin.is_open()) return false;
        string line;
        while (getline(fin, line)) {
            istringstream iss(line);
            string username;
            iss >> username;
            username.pop_back();
            if (username == user1) {
                string friendName;
                while (iss >> friendName) {
                    if (friendName == user2) {
                        fin.close();
                        return true;
                    }
                }
                break;
            }
        }
        fin.close();
        return false;
    }
    string sendRequest(string senderUsername, string receiverUsername) {
        if (senderUsername == receiverUsername)
            return "Cannot add yourself.";

        Node* sender = searchPlayerByUsername(senderUsername);
        Node* receiver = searchPlayerByUsername(receiverUsername);
        if (!sender || !receiver)
            return "User not found.";

        if (isAlreadyFriend(sender, receiverUsername) || areFriendsInFile(senderUsername, receiverUsername))
            return "Already friends.";

        for (int i = 0; i < requestCount; i++)
            if (pendingRequests[i][0] == senderUsername && pendingRequests[i][1] == receiverUsername)
                return "Request already sent.";

        pendingRequests[requestCount][0] = senderUsername;
        pendingRequests[requestCount][1] = receiverUsername;
        requestCount++;
        saveRequests();
        return "Friend request sent!";
    }

    int Display(RenderWindow& window, Player* currentPlayer, Themes* currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Font load error!" << endl;
            return 1;
        }

        const int numOptions = 4;
        string options[numOptions] = { "View Friends", "View Requests", "Add Friend", "Back to Multiplayer Menu" };
        Text menuItems[numOptions];
        for (int i = 0; i < numOptions; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setString(options[i]);
            menuItems[i].setCharacterSize(28);
            menuItems[i].setPosition(100, 100 + i * 50);
            menuItems[i].setFillColor(currentTheme->textColor);
        }
        int selectedItem = 0;

        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Texture bg;
            if (bg.loadFromFile(currentTheme->image)) {
                Sprite background(bg);
                window.draw(background);
            }

            for (int i = 0; i < numOptions; i++) {
                if (i == selectedItem) menuItems[i].setFillColor(Color::Yellow);
                else menuItems[i].setFillColor(currentTheme->textColor);
                window.draw(menuItems[i]);
            }
            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) window.close();
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::W) { selectedItem--; if (selectedItem < 0) selectedItem = numOptions - 1; }
                    if (e.key.code == Keyboard::S) { selectedItem++; if (selectedItem >= numOptions) selectedItem = 0; }
                    if (e.key.code == Keyboard::Enter) {
                        if (selectedItem == 0) { showFriends(window, currentPlayer, currentTheme); }
                        else if (selectedItem == 1) { handleRequests(window, currentPlayer, currentTheme); }
                        else if (selectedItem == 2) {
                            string input = "";
                            string message = "";
                            while (window.isOpen()) {
                                window.clear(currentTheme->backgroundColor);
                                if (bg.loadFromFile(currentTheme->image)) {
                                    Sprite background(bg);
                                    window.draw(background);
                                }
                                Text prompt("Enter Username to Add: " + input, font, 24);
                                prompt.setFillColor(currentTheme->textColor);
                                prompt.setPosition(100, 150);
                                window.draw(prompt);

                                if (!message.empty()) {
                                    Text msg(message, font, 20);
                                    msg.setFillColor(Color::Green);
                                    msg.setPosition(100, 200);
                                    window.draw(msg);
                                }

                                Text instr("Press ESC to cancel", font, 20);
                                instr.setFillColor(Color::Yellow);
                                instr.setPosition(100, 250);
                                window.draw(instr);

                                window.display();

                                Event inputEvent;
                                while (window.pollEvent(inputEvent)) {
                                    if (inputEvent.type == Event::Closed) window.close();
                                    if (inputEvent.type == Event::TextEntered) {
                                        if (inputEvent.text.unicode == '\b' && !input.empty()) input.pop_back();
                                        else if (inputEvent.text.unicode >= 32 && inputEvent.text.unicode <= 126) input += static_cast<char>(inputEvent.text.unicode);
                                    }
                                    if (inputEvent.type == Event::KeyPressed) {
                                        if (inputEvent.key.code == Keyboard::Enter) {
                                            if (input == currentPlayer->getUsername()) message = "Cannot add yourself.";
                                            else {
                                                Node* found = searchPlayerByUsername(input);
                                                if (!found) message = "Player not found.";
                                                else if (isAlreadyFriend(currentPlayer->returnHead(), input)) message = "Already friends.";
                                                else {
                                                    message = sendRequest(currentPlayer->getUsername(), input);
                                                }

                                            }
                                        }
                                        if (inputEvent.key.code == Keyboard::Escape) goto backToMainMenu;
                                    }
                                }
                            }
                        }
                        else if (selectedItem == 3) { return 5; }
                    }
                }
            }
        }
    backToMainMenu:
        return 1;
    }

    void showFriends(RenderWindow& window, Player* currentPlayer, Themes* currentTheme) {
        Font font;
        font.loadFromFile(currentTheme->font);

        string currentUsername = currentPlayer->getUsername();
        string friendNames[20];  // FIX: using array instead of vector
        int friendCount = 0;

        ifstream fin("Friends.txt");
        if (fin.is_open()) {
            string line;
            while (getline(fin, line)) {
                istringstream iss(line);
                string username;
                iss >> username;
                if (!username.empty() && username[username.size() - 1] == ':')
                    username.pop_back();  // remove colon

                if (username == currentUsername) {
                    string friendName;
                    while (iss >> friendName && friendCount < 20) {
                        friendNames[friendCount++] = friendName;
                    }
                    break;
                }
            }
            fin.close();
        }

        Text friendsList[20];
        for (int i = 0; i < friendCount; i++) {
            friendsList[i].setFont(font);
            friendsList[i].setString(friendNames[i]);
            friendsList[i].setCharacterSize(26);
            friendsList[i].setFillColor(currentTheme->textColor);
            friendsList[i].setPosition(100, 100 + i * 40);
        }

        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Texture bg;
            if (bg.loadFromFile(currentTheme->image)) {
                Sprite background(bg);
                window.draw(background);
            }

            for (int i = 0; i < friendCount; i++) window.draw(friendsList[i]);

            Text instr("Press ESC to go back", font, 20);
            instr.setFillColor(Color::Yellow);
            instr.setPosition(100, 400);
            window.draw(instr);

            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) return;
                if (e.type == Event::Closed) window.close();
            }
        }
    }


    void handleRequests(RenderWindow& window, Player* currentPlayer, Themes* currentTheme) {
        Font font;
        font.loadFromFile(currentTheme->font);
        int currentIndex = 0;
        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Texture bg;
            if (bg.loadFromFile(currentTheme->image)) {
                Sprite background(bg);
                window.draw(background);
            }
            int displayCount = 0;
            for (int i = 0; i < requestCount; i++) {
                if (pendingRequests[i][1] == currentPlayer->getUsername()) {
                    string reqText = "From: " + pendingRequests[i][0];
                    Text t(reqText, font, 24);
                    t.setFillColor(displayCount == currentIndex ? Color::Yellow : currentTheme->textColor);
                    t.setPosition(100, 100 + displayCount * 40);
                    window.draw(t);
                    displayCount++;
                }
            }
            Text instr("A: Accept | R: Reject | Esc: Back", font, 20);
            instr.setFillColor(Color::White);
            instr.setPosition(100, 400);
            window.draw(instr);
            window.display();
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) window.close();
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::W) { currentIndex--; if (currentIndex < 0) currentIndex = displayCount - 1; }
                    if (e.key.code == Keyboard::S) { currentIndex++; if (currentIndex >= displayCount) currentIndex = 0; }
                    if (e.key.code == Keyboard::A) {
                        int realIdx = -1, found = 0;
                        for (int i = 0; i < requestCount; i++) {
                            if (pendingRequests[i][1] == currentPlayer->getUsername()) {
                                if (found == currentIndex) { realIdx = i; break; }
                                found++;
                            }
                        }
                        if (realIdx != -1) {
                            string senderName = pendingRequests[realIdx][0];
                            string receiverName = pendingRequests[realIdx][1];

    
                            ifstream fin("Friends.txt");
                            ofstream fout("Friends_temp.txt");
                            string line;
                            bool senderFound = false, receiverFound = false;
                            while (getline(fin, line)) {
                                istringstream iss(line);
                                string username;
                                iss >> username;
                                if (!username.empty() && username.back() == ':')
                                    username.pop_back();

                                string updatedLine = line;  

                                if (username == senderName && !senderFound) {
                                    updatedLine += " " + receiverName;
                                    senderFound = true;
                                }
                                if (username == receiverName && !receiverFound) {
                                    updatedLine += " " + senderName;
                                    receiverFound = true;
                                }

                                fout << updatedLine << endl;
                            }
                            if (!senderFound) fout << senderName << ": " << receiverName << endl;
                            if (!receiverFound) fout << receiverName << ": " << senderName << endl;
                            fin.close();
                            fout.close();
                            remove("Friends.txt");
                            rename("Friends_temp.txt", "Friends.txt");

                            pendingRequests[realIdx][0] = "";
                            pendingRequests[realIdx][1] = "";
                            saveRequests();
                        }
                    }
                    if (e.key.code == Keyboard::R) {
                        int realIdx = -1, found = 0;
                        for (int i = 0; i < requestCount; i++) {
                            if (pendingRequests[i][1] == currentPlayer->getUsername()) {
                                if (found == currentIndex) { realIdx = i; break; }
                                found++;
                            }
                        }
                        if (realIdx != -1) {
                            pendingRequests[realIdx][0] = "";
                            pendingRequests[realIdx][1] = "";
                            saveRequests();
                        }
                    }
                    if (e.key.code == Keyboard::Escape) return;
                }
            }
        }
    }

};
