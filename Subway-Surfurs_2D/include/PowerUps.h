#ifndef POWERUPS_H
#define POWERUPS_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <string>
using namespace std;

//-------------------------------
// PowerUp Base Class 
//-------------------------------
class PowerUp {
protected:
    float x, y;
    float duration;
    sf::Sprite sprite;  //change from circleshape
    sf::Texture texture;
    string type;

public:
    PowerUp(float sx, float sy,float dur , string textureFile,string typeName);
    virtual ~PowerUp() {}

    virtual void applyEffect(Player* p) = 0;
    virtual void update(float speed); 
    void draw(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    float getY() const { return y; }
    string getType() const {return type; }
    float getDuration() const { return duration; }
};

//-------------------------------
// Derived PowerUp Classes
//-------------------------------
class ShieldPower : public PowerUp {
public:
    ShieldPower(float sx, float sy);
    void applyEffect(Player* p) override;
};

//-------------------------------
// Derived: Magnet PowerUp Class
//-------------------------------
class MagnetPower : public PowerUp {
public:
    MagnetPower(float sx, float sy);
    void applyEffect(Player* p) override;
};

class DoubleCoinPower : public PowerUp {
public:
    DoubleCoinPower(float sx, float sy);
    void applyEffect(Player* p) override;
};
class JetpackPower : public PowerUp {
public:
    JetpackPower(float sx, float sy);
    void applyEffect(Player* p) override;
};

// Add other PowerUps similarly...
#endif