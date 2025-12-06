#include "Obstacles.h"
#include <iostream>
using namespace std;

// Base Class Constructor
Obstacle::Obstacle(float startX, float startY, float w, float h, string texturePath, string typeName)
    : x(startX), y(startY), width(w), height(h), type(typeName) {

    if (!texture.loadFromFile(texturePath)) {
        // Fallback: Red Square
        sf::Image img; 
        img.create(w, h, sf::Color::Red);
        texture.loadFromImage(img);
    }
    sprite.setTexture(texture);
    
    // Scale Logic: Force sprite to fit the hitbox (w, h)
    sf::Vector2u size = texture.getSize();
    sprite.setOrigin(size.x/2.0f, size.y/2.0f); // CENTER ORIGIN

    sprite.setPosition(x,y);
    sprite.setScale(w / size.x, h / size.y);
}

// Base Collision Check is in header file
bool Obstacle::checkCollision(Player* p) {
    if (sprite.getGlobalBounds().intersects(p->getBounds())) {
        // if object is jumpable and player is jumping, no collision (safe jump)
        if(this->isJumpable() && p->isJumping()) {
            return false; // No collision if obstacle is jumpable and player is jumping
        }
        // check slide pass
        if(this->isSlideable() && p->isSliding()){
            return false;
        }
        cout << "[Collision] with " << this->type << " detected!" << endl;
        return true;
    }
    return false; // No collision
}
//-------------------------------
// --- Train Implementation ---
// -------------------------------
Train::Train(float sx, float sy) 
   // height=120,(  tall! ) width=60, color=red
    : Obstacle(sx, sy, 255.0f/*width*/, 270.0f/*height*/, "assets/train.png", "Train") {}

void Train::update(float speed) {
    y += speed * 1.5; // Trains move faster
    sprite.setPosition(x, y);
}

void Train::draw(sf::RenderWindow& w) {
    w.draw(sprite);
}

//-------------------------------
// --- Barrier Implementation ---
// -------------------------------
Barrier::Barrier(float sx, float sy) 
    // height=50, width=100(jumpable), color=orange
    : Obstacle(sx, sy, 100.0f, 50.0f, "assets/barrier.png", "Barrier") {} 
void Barrier::update(float speed) { 
    y += speed; 
    sprite.setPosition(x, y); 
}
void Barrier::draw(sf::RenderWindow& w) { 
    w.draw(sprite); 
}

//-------------------------------
// --- Cone Implementation ---      
// -------------------------------
Cone::Cone(float sx, float sy) 
    // small (30x30), color=orange/gold
    : Obstacle(sx, sy, 30.0f, 30.0f, "assets/cone.png", "Cone") {} 
void Cone::update(float speed) { 
    y += speed; 
    sprite.setPosition(x, y); 
}
void Cone::draw(sf::RenderWindow& w) { 
    w.draw(sprite); 
}   

//-------------------------------
// --- Fence Implementation ---
// -------------------------------
Fence::Fence(float sx, float sy) 
    // wide (120) and short (20), color=brown
    : Obstacle(sx, sy, 120.0f, 20.0f, "assets/fence.png", "Fence") {} 
void Fence::update(float speed) { 
    y += speed; 
    sprite.setPosition(x, y); 
}
void Fence::draw(sf::RenderWindow& w) { 
    w.draw(sprite); 
}
