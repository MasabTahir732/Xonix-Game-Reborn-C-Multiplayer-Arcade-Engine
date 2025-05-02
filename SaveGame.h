#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include"Scoring.h"

using namespace std;

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

    void setSaveID() {
        string temp;
        cout << "enter save ID: ";
        cin >> temp;
        SaveID = temp;
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

    void saveGame() {
        TimeStamp = getCurrentTimestamp();
        setSaveID();

        ofstream outFile("saved_game.txt", ios::app); // append mode

        if (outFile.is_open()) {
            outFile << PlayerID << endl;
            outFile << SaveID << endl;
            outFile << "Timestamp: " << TimeStamp << endl;

            // Build a 2D array from the linked list to write it line by line
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
            cout << "Game saved!" << endl;
        }
        else {
            cout << "Error saving the game!" << endl;
        }
    }

    void loadGame(string user) {
        ifstream inFile("saved_game.txt");
        if (!inFile.is_open()) {
            cout << "Error loading the game!" << endl;
            return;
        }

        string askforsaveid;
        cout << "Enter the existing SaveID: ";
        cin >> askforsaveid;

        string line;
        while (getline(inFile, line)) {
            string filePlayerID = line;

            string fileSaveID;
            if (!getline(inFile, fileSaveID)) break;

            string timestampLine;
            if (!getline(inFile, timestampLine)) break;

            if (filePlayerID == user && fileSaveID == askforsaveid) {
                clearGrid(); // clear previous data
                for (int i = 0; i < 25; ++i) {
                    for (int j = 0; j < 40; ++j) {
                        int val;
                        inFile >> val;
                        if (val != 0) setGridValue(i, j, val);
                    }
                    inFile.ignore(); // skip newline
                }
                cout << "Game loaded!" << endl;
                inFile.close();
                return;
            }
            else {
                // skip 25 lines of grid data
                for (int i = 0; i < 25; ++i) getline(inFile, line);
            }
        }

        cout << "Save not found for user and save ID." << endl;
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
