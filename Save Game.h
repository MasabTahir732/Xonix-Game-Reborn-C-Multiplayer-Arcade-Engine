#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;

class SaveGame {
private:
    string PlayerID;
    string TimeStamp;  
    int TilesOccupied;
    int CurrentEnemies;
    int* enemypositions;

public:
    SaveGame() {
        PlayerID = "";
        TimeStamp = "";
        TilesOccupied = 0;
        CurrentEnemies = 0;
        enemypositions = nullptr;
    }

    // Function to save game and set timestamp
    void Savegame() {
        TimeStamp = getCurrentTimestamp();  // Save the timestamp when the game is saved
    }

    // Function to generate and return the current timestamp
    string getCurrentTimestamp() {
        time_t now = time(0);  // Get current time
        tm ltm;                // Struct to hold the local time
        localtime_s(&ltm, &now); // Securely convert to local time struct

        char buffer[30];
        // Safe sprintf using sprintf_s
        sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + ltm.tm_year,   // Year
            1 + ltm.tm_mon,       // Month
            ltm.tm_mday,          // Day
            ltm.tm_hour,          // Hour
            ltm.tm_min,           // Minute
            ltm.tm_sec);          // Second

        cout << "Game saved at: " << string(buffer) << endl; // Display the timestamp in console
        return string(buffer);  // Return the timestamp as a string
    }

    // Getter and Setter for PlayerID
    void setPlayerID(const string& id) {
        PlayerID = id;
    }

    string getPlayerID() const {
        return PlayerID;
    }

    // Getter and Setter for TilesOccupied
    void setTilesOccupied(int tiles) {
        TilesOccupied = tiles;
    }

    int getTilesOccupied() const {
        return TilesOccupied;
    }

    // Getter and Setter for CurrentEnemies
    void setCurrentEnemies(int enemies) {
        CurrentEnemies = enemies;
    }

    int getCurrentEnemies() const {
        return CurrentEnemies;
    }

    // Function to set enemy positions
    void setEnemyPositions(int* positions, int count) {
        if (enemypositions != nullptr) {
            delete[] enemypositions;  // Free previously allocated memory
        }
        enemypositions = new int[count];  // Allocate new memory for enemy positions
        for (int i = 0; i < count; ++i) {
            enemypositions[i] = positions[i]; // Copy enemy positions
        }
    }

    // Function to get enemy positions (returns pointer to array)
    int* getEnemyPositions() const {
        return enemypositions;
    }

    // Destructor to ensure proper memory cleanup
    ~SaveGame() {
        delete[] enemypositions;  // Free dynamically allocated memory
    }
};
