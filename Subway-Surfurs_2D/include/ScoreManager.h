#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <string>
using namespace std;

class ScoreManager {
private:
    int currentScore;
    int highScore;
    string playerName;
    string filename;

public:
    ScoreManager();

    void addScore(int amount);
    void resetScore();

    // file I/O
    void saveHighScore();
    void loadHighScore();
    
    // getters
    int getScore() const;
    int getHighScore() const;
    string getPlayerName() const;
};

#endif