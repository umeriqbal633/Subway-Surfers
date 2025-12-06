#include "PowerUps.h"
#include <iostream>
using namespace std;

//--- Base Class Implementation ---
PowerUp::PowerUp(float startX, float startY,float dur, string textureFile,string typeName)
    : x(startX), y(startY),duration(dur) ,type(typeName) {

    if(!texture.loadFromFile(textureFile)){
        //fall back color logic if image missing
        sf::Image img;
        img.create(40,40,sf::Color::Magenta);
        texture.loadFromImage(img);
    }

    sprite.setTexture(texture);
    sprite.setPosition(x,y);

    //scale to 40x40
    sf::Vector2u size = texture.getSize();
    sprite.setScale(40.0f/size.x , 40.0f/size.y);
}

void PowerUp::update(float speed) {
    y += speed;
    sprite.setPosition(x, y);
}

void PowerUp::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}

// -- SHield PowerUp Implementation ---
ShieldPower::ShieldPower(float sx, float sy)
    : PowerUp(sx, sy, 0.0f, "assets/shield.png","Shield") {}

void ShieldPower::applyEffect(Player* p) {
    cout << "[PowerUp] Shield Collected!" << endl;
    p->setShield(true);   
}

// -- Magnet PowerUp Implementation ---
MagnetPower::MagnetPower(float sx, float sy)
    : PowerUp(sx, sy, 600.0f, "assets/magnet.png","Magnet") {}

void MagnetPower::applyEffect(Player* p) {
    cout << "[PowerUp] Magnet Collected!" << endl;
    // its duration is inherited from base class about 10 seconds (600 frames)
    p->setMagnet((int)this->duration); 
}

// -- Double Coin PowerUp Implementation ---
DoubleCoinPower::DoubleCoinPower(float sx, float sy)
    :PowerUp(sx, sy, 600.0f, "assets/doublecoin.png", "DoubleCoin") {}

void DoubleCoinPower::applyEffect(Player* p) {
    cout << "[PowerUp] Double Coin Collected!" << endl;
    p->setDoubleCoin((int)this->duration);
}

JetpackPower::JetpackPower(float sx, float sy)
    :PowerUp(sx, sy, 600.0f, "assets/jetpack.png", "Jetpack") {}

void JetpackPower::applyEffect(Player* p) {
    cout << "[PowerUp] Jetpack Collected! Flying..." << endl;
    p->setJetpack((int)this->duration);
}