#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class PowerUp; // Forward declaration
class Obstacle;

class PlayerStates {
public:
    static const int STATE_RUNNING = 0;
    static const int STATE_JUMPING = 1;
    static const int STATE_SLIDING = 2;
};

class Player {
private:
    sf::Sprite sprite;
    sf::Texture textureRun;
    sf::Texture textureSlide;
    sf::Texture textureFly;
    
    float x, y;
    int currentLane; // 0 1 2
    int movementState;
    // Power-ups and collectibles
    bool hasShield;
    int magnetTimer; 
    int coinsCollected;
    int slideTimer; // it counts frames e.g. 60 frames = 1 second
    int doubleCoinTimer; // for double coin effect/powerup
    int jetpackTimer; // timer for jetpack powerup
    float runAnimTimer;

    // Physics
    float verticalVelocity;
    float gravity;
    float jumpHeight;
    float groundY;

public:
    Player();
    void moveLeft();
    void moveRight();
    void jump();
    void slide();
    void update();
    void draw(sf::RenderWindow& window);
    bool isJumping() const;
    bool isSliding() const;
    
    // Interactions
    void applyPowerUp(PowerUp* p); // Takes pointer to base class

    // shield logic
    void setShield(bool state);
    bool isShielded() const;

    // Magnet logic
    void setMagnet(int durationFrames);
    bool isMagnetActive() const;

    // coin logic
    void addCoin(int amount);
    int getCoins() const;

    // Double Coin logic
    void setDoubleCoin(int durationFrames);
    bool isDoubleCoinActive() const;

    // Jetpack logic
    void setJetpack(int durationFrames);
    bool isJetpackActive() const;
    
    // getters for trackmanager , needed for magnet math
    sf::FloatRect getBounds() const;
    float getX() const { return x; }
    float getY() const { return y; }
};
#endif
