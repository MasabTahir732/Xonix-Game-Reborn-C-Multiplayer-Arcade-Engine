#pragma once

//Masab Tahir 23i-0006
//Muhammad Arslan 23i-0572
//Xonix Game
#include<iostream>
#include<fstream>
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;

struct LeaderboardEntry {
    string Username;
    int TotalScore;
};

class LeaderboardPage {
    LeaderboardEntry heap[10];
    int heapSize;

    void heapifyDown(int idx) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < heapSize && heap[left].TotalScore < heap[smallest].TotalScore)
            smallest = left;
        if (right < heapSize && heap[right].TotalScore < heap[smallest].TotalScore)
            smallest = right;

        if (smallest != idx) {
            swap(heap[idx], heap[smallest]);
            heapifyDown(smallest);
        }
    }

    void buildHeap() {
        for (int i = heapSize / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    void insert(const LeaderboardEntry& entry) {
        if (heapSize < 10) {
            heap[heapSize++] = entry;
            if (heapSize == 10) buildHeap();
        }
        else {
            if (entry.TotalScore > heap[0].TotalScore) {
                heap[0] = entry;
                heapifyDown(0);
            }
        }
    }

    void loadPlayers() {
        ifstream file("Players.txt");
        if (!file.is_open()) {
            cerr << "Failed to open Players.txt" << endl;
            return;
        }

        string username, password;
        int totalScore;
        while (file >> username >> password >> totalScore) {
            LeaderboardEntry entry;
            entry.Username = username;
            entry.TotalScore = totalScore;
            insert(entry);
            string restOfLine;
            getline(file, restOfLine);
        }
        file.close();
    }

public:
    LeaderboardPage() {
        heapSize = 0;
        loadPlayers();
    }

    int Display(RenderWindow& window, Themes* currentTheme) {
        Font font;
        if (!font.loadFromFile(currentTheme->font)) {
            cerr << "Failed to load font!" << endl;
            return 1;
        }

        LeaderboardEntry sortedHeap[10];
        int n = heapSize;
        for (int i = 0; i < n; i++) {
            sortedHeap[i] = heap[i];
        }

        for (int i = 0; i < n - 1; i++) {
            int maxIdx = i;
            for (int j = i + 1; j < n; j++) {
                if (sortedHeap[j].TotalScore > sortedHeap[maxIdx].TotalScore) {
                    maxIdx = j;
                }
            }
            if (maxIdx != i) {
                swap(sortedHeap[i], sortedHeap[maxIdx]);
            }
        }

        while (window.isOpen()) {
            window.clear(currentTheme->backgroundColor);
            Texture image;
            if (!image.loadFromFile(currentTheme->image)) {
                cerr << "cannot load theme!" << endl;
            }
            Sprite B(image);
            B.setPosition(0, 0);
            window.draw(B);

            Text title("Leaderboard - Top 10 Players", font, 30);
            title.setPosition(100, 50);
            title.setFillColor(currentTheme->textColor);
            window.draw(title);

            for (int i = 0; i < n; i++) {
                string entryStr = to_string(i + 1) + ". " + sortedHeap[i].Username + " - " + to_string(sortedHeap[i].TotalScore);
                Text entryText(entryStr, font, 24);
                entryText.setPosition(100, 120 + i * 40);
                entryText.setFillColor(currentTheme->textColor);
                window.draw(entryText);
            }

            Text instructions("Press ESC to return to Main Menu", font, 20);
            instructions.setPosition(100, 600);
            instructions.setFillColor(currentTheme->textColor);
            window.draw(instructions);

            window.display();

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                    return 1;
            }
        }
        return 1;
    }
};
