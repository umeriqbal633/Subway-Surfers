#ifndef COIN_H
#define COIN_H
#include <SFML/Graphics.hpp>
#include <cmath>

class Coin {
private:
    float x, y;
    // sf::CircleShape shape;
    sf::Sprite sprite;
    sf::Texture texture;
    float animTimer; //animation timer

public:
    Coin(float sx, float sy) : x(sx), y(sy) {
        if(!texture.loadFromFile("assets/coin.png")){
            // fallback if missing 
            sf::Image img; img.create(30,30,sf::Color::Yellow);
            texture.loadFromImage(img);
        }
        sprite.setTexture(texture);
        sprite.setPosition(x,y);

        // scale based on width
        sf::Vector2u size = texture.getSize();
        //calculate X and Y scale independently  
        sprite.setOrigin(size.x/2.0f,size.y/2.0f);

        // intial scale logic
        float scale=30.0f/size.x;
        sprite.setScale(scale,scale);

        animTimer= (float) (rand()%100); //random start not sync
    }

    void update(float speed) { 
        y += speed; 

        //SPIN ANIMATION
        animTimer+=0.01f; //spin speed

        //get currrent base scale approx 30px
        float baseScale= 30.0f / texture.getSize().x;

        // modify X scale using SIN wave
        sprite.setScale(baseScale*sin(animTimer),baseScale); // this make it look fliping
        sprite.setPosition(x,y);
    }

    // for magnet effect
    void move(float dx, float dy) {
        x += dx;
        y += dy;
        sprite.setPosition(x, y);
    }
    
    void draw(sf::RenderWindow& w) { w.draw(sprite); }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    float getY() const { return y; }
};
#endif
