#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "TrackManager.h"
#include "ScoreManager.h"

class GameEngine {
private:
    sf::RenderWindow window;
    Player* player;
    TrackManager* trackManager;
    ScoreManager* scoreManager;

    // --- STATE CONSTANTS (Replaces Enum) ---
    static const int STATE_MENU = 0;
    static const int STATE_GAME = 1;
    static const int STATE_GAMEOVER = 2;
    
    // Tracks current state (0, 1, or 2)
    int currentState;
    
    bool isRunning;
    float gameSpeed;
    int scoreTimer;
    // UI elements
    sf::Font font;
    sf::Text scoreText;

    // Menu UI
    sf::Text titleText;
    sf::Text instructionsText;
    sf::Text nameInputText;
    sf::Text highScoreDisplay;
    sf::Text gameOverText;
    
    // Input Buffer for Name
    std::string userInputName;

    // PAUSE SYSYTEM
    bool isPaused;
    sf::Text pauseText;
    sf::RectangleShape pauseOverlay;

    // --scroling background variables
    sf::Texture bgTexture;
    sf::Sprite bgSprite1; //1st cpy
    sf::Sprite bgSprite2; //2nd starts above the first
    float bgY;

    sf::Texture inspectorTexture;
    sf::Sprite inspectorSprite;
    float inspectorTimer;       // controls how long inspxtr chases

    // AUDIO
    sf::Music bgMusic;

    // helper to reset game variables
    void resetGame();
public:
    GameEngine();
    ~GameEngine();
    
    void run();
    void handleInput();
    void update();
    void render();
    void gameOver();
};
#endif
