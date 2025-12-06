#include "ScoreManager.h"
#include <fstream>
#include <iostream>
using namespace std;

ScoreManager::ScoreManager() {
    currentScore = 0;
    highScore = 0;
    playerName = "Surfer1";  // Default name
    filename = "highscore.txt";

    // load data immmediately when game starts
    loadHighScore();
}

void ScoreManager::addScore(int amount) {
    currentScore += amount;
}

void ScoreManager::resetScore() {
    currentScore = 0;
}

// --File I/O Implementations --
void ScoreManager::saveHighScore() {
    //only save if current score is higher than previous high score
    if(currentScore > highScore) {
        highScore = currentScore;
        cout << "[SCORE] New High Score! Saving to file..." << highScore << " by " << playerName << endl;
        
        ofstream file(filename);
        if(file.is_open()) {
            file << playerName << " " << highScore;
            file.close();
        }
        else{
            cout << "[ERROR] Unable to open score file to save!" << endl;
        }
    }
}

void ScoreManager::loadHighScore() {
    ifstream file(filename);
    if(file.is_open()) {
        file >> playerName >> highScore;
        file.close();
        cout << "[SCORE] Loaded High Score: " << highScore << " by " << playerName << endl;
    }
    else{
        cout << "[SCORE] NO high score file found. Starting fresh." << endl;
        highScore = 0;
    }
}

int ScoreManager::getScore() const { return currentScore; }
int ScoreManager::getHighScore() const { return highScore; }
string ScoreManager::getPlayerName() const { return playerName; }
