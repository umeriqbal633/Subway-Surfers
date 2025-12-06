#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

#include "GameList.h"
#include "Obstacles.h"
#include "PowerUps.h"
#include "Coin.h"

// Forward declaration
class Player;

class TrackManager {
private:
    GameList<Obstacle*> obstacles;
    GameList<Coin*> coins;
    GameList<PowerUp*> powerups;
    
    float spawnTimer;

public:
    TrackManager();
    ~TrackManager();
    
    void updateTrack(float speed, Player* p);

    void draw(sf::RenderWindow& window);
    bool checkCollisions(Player* p); // Returns true if collision ended game
    
    void generateObstacle();
    void generateCoin();
    void generatePowerUp();
};
#endif
