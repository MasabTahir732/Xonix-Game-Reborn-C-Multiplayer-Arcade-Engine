#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SaveGame {
private:
    string PlayerID;
    string TimeStamp;
    int grid[25][40];  // Game grid (25 rows and 40 columns)

public:
    SaveGame() {
        PlayerID = "";
        TimeStamp = "";
        // Initialize the grid with zeros (empty spaces)
        for (int i = 0; i < 25; ++i) {
            for (int j = 0; j < 40; ++j) {
                grid[i][j] = 0;  // Initialize all cells to 0 (empty)
            }
        }
    }

    // Set Player ID
    void setPlayerID(string id) {
        PlayerID = id;
    }

    // Save the game grid to a file
    void saveGame() {
        TimeStamp = getCurrentTimestamp();  // Get the current timestamp for saving

        ofstream outFile("saved_game.txt");

        if (outFile.is_open()) {
            // Save PlayerID and Timestamp to the file first
            outFile << PlayerID << endl;
            outFile << "Timestamp: " << TimeStamp << endl;

            // Save the grid to the file
            for (int i = 0; i < 25; ++i) {
                for (int j = 0; j < 40; ++j) {
                    outFile << grid[i][j] << " ";  // Save each cell's value
                }
                outFile << endl;  // New line for the next row of the grid
            }
            outFile.close();
            cout << "Game saved!" << endl;
        }
        else {
            cout << "Error saving the game!" << endl;
        }
    }

    // Load the game grid from a file
    void loadGame(string user) {
        ifstream inFile("saved_game.txt");
        
        if (inFile.is_open()) {
            string line;
            getline(inFile, line);// Read PlayerID line (not used here but can be used if needed)
            if (line != user) {
                cout << "used doesnt belong to this";
                return;
            }
            getline(inFile, line);  // Read Timestamp line (can be used if needed)

            // Load the grid from the file
            for (int i = 0; i < 25; ++i) {
                for (int j = 0; j < 40; ++j) {
                    inFile >> grid[i][j];  // Read each cell's value
                }
            }
            inFile.close();
            cout << "Game loaded!" << endl;
        }
        else {
            cout << "Error loading the game!" << endl;
        }
    }

    // Function to get the current timestamp
    string getCurrentTimestamp() {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);  // secure replacement

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

    // Getters for grid and other details (if needed)
    int getGridValue(int row, int col) {
        return grid[row][col];
    }

    void setGridValue(int row, int col, int value) {
        grid[row][col] = value;
    }
};
