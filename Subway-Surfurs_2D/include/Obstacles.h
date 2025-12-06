#ifndef OBSTACLES_H
#define OBSTACLES_H

#include <SFML/Graphics.hpp>
#include "Player.h"
using namespace std;

// Abstract Base Class
class Obstacle {
protected:
    float x, y;
    float width, height;
    // Change sprite
    sf::Sprite sprite;
    sf::Texture texture;

    string type; // for debuging ,e.g. "Train", "Barrier"

public:
    Obstacle(float startX, float startY, float w, float h, string texturePath,string typeName);
    
    virtual ~Obstacle() {}

    virtual void update(float speed) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Polymorphic collision check
    virtual bool checkCollision(Player* p);
    virtual bool isJumpable() const { return false; }
    virtual bool isSlideable() const { return false; }

    float getY() const { return y; }
    string getType() const { return type; }
};

// Derived Classes
class Train : public Obstacle {
public:
    Train(float sx, float sy); 
    void update(float speed) override ;
    void draw(sf::RenderWindow& w) override;
};

class Barrier : public Obstacle {
public:
    Barrier(float sx, float sy); 
    void update(float speed) override;
    void draw(sf::RenderWindow& w) override;
    bool isJumpable() const override { return true; } // barrier is short so jumpable
    bool isSlideable() const override { return true; }
};

class Cone : public Obstacle {
public:
    Cone(float sx, float sy);
    void update(float speed) override;
    void draw(sf::RenderWindow& w) override;
    bool isJumpable() const override { return true; } // cone is short so jumpable
};

class Fence : public Obstacle {
public:
    Fence(float sx, float sy); 
    void update(float speed) override;
    void draw(sf::RenderWindow& w) override;
    bool isJumpable() const override { return true; } // fence is short so jumpable
};

#endif
