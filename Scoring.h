#pragma once
#pragma once
#include<iostream>
using namespace std;

class Score {
    int currentScore;
public:
    Score() {
        currentScore = 0;
    }

    void updateScore(int tilesCaptured) {
        if (tilesCaptured >= 10) {
            currentScore += tilesCaptured * 2;
        }
        else {
            currentScore += tilesCaptured;
        }
    }

    void displayScore() const {
        cout << "Current Score: " << currentScore << endl;
    }

    int getScore() const {
        return currentScore;
    }
};
