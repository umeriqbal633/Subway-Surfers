#include "Player.h"
#include <iostream>
#include <cmath>
using namespace std;

Player::Player() {
    currentLane = 1; 
    movementState = PlayerStates::STATE_RUNNING;
    hasShield = false;
    coinsCollected = 0;
    magnetTimer = 0;
    doubleCoinTimer = 0;
    jetpackTimer = 0;
    runAnimTimer=0.0f;

    x = 400.0f; 
    y = 500.0f;
    groundY = 500.0f;

    // --- LOAD ALL TEXTURES ---
    // 1. Run (Normal)
    if(!textureRun.loadFromFile("assets/player.png")){
        sf::Image img; img.create(50,50,sf::Color::Green);
        textureRun.loadFromImage(img);
    }
    
    // 2. Slide
    if(!textureSlide.loadFromFile("assets/player_slide.png")){
        // Fallback to normal if missing
        textureSlide = textureRun; 
    }

    // 3. Fly (Jetpack)
    if(!textureFly.loadFromFile("assets/player_jetpack.png")){
        // Fallback to normal if missing
        textureFly = textureRun;
    }
    sprite.setTexture(textureRun);

    // Center origin
    sf::Vector2u size = textureRun.getSize();
    sprite.setOrigin(size.x/2.0f, size.y/2.0f);
    sprite.setPosition(x,y);

    // scale logic: force player to approx 50px wide
    // height will scale automatically
    float scale = 50.0f /size.x;
    sprite.setScale(scale,scale);


    this->verticalVelocity = 0.0f;
    this->gravity = 0.8f;
    this->jumpHeight = -15.0f;
    this->slideTimer = 0;
}

void Player::moveLeft() {
    // now move left anytime, as long as player not at edge 
    if (currentLane > 0 ) {
        currentLane--;
        x -= 200.0f;
        sprite.setPosition(x, y);
        cout<<"[DEBUG] Moved Left to lane "<<currentLane<<endl;
    }
}

void Player::moveRight() {
    // now move right anytime, as long as player not at edge
    if (currentLane < 2 ) {
        currentLane++;
        x += 200.0f;
        sprite.setPosition(x, y);
        cout<<"[DEBUG] Moved Right to lane "<<currentLane<<endl;
    }
}

void Player::jump() {
    // Only jump if currently running
    if (this->movementState == PlayerStates::STATE_RUNNING) {
        cout << "[DEBUG] Jump taken." << endl;
        //change state so dont jump again mid-air instantly
        this->movementState = PlayerStates::STATE_JUMPING;
        // applying instantaneous upward velocity
        this->verticalVelocity = jumpHeight;
    }
}

void Player::slide() {
    // only slide when running not when jumping
    if (this->movementState == PlayerStates::STATE_RUNNING) {
        cout << "[DEBUG] Slide started." << endl;
        this->movementState = PlayerStates::STATE_SLIDING;
        this->slideTimer = 60; // 60 frames @60fps  means 1 second of sliding

        // Simple visual feedback for slide
        // visual to shrink the player
        //sf::Vector2u size = textureSlide.getSize();
        // width stays 30, height becomes 30
        // sprite.setScale(30.0f/size.x, 30.0f/size.y);

        //sprite.setScale(size.x, size.y);
        // move Y down so feet stay on ground
        //this->y = this->groundY + 12.5f;
        //this->sprite.setPosition(this->x, this->y);
    }
}

void Player::update() {

    // --- TEXTURE SWAPPING LOGIC ---
    // Decides which image to show based on state
    
    if (jetpackTimer > 0) {
        // 1. JETPACK STATE
        if (sprite.getTexture() != &textureFly) {
            sprite.setTexture(textureFly);
            // Re-center origin for new image
            sf::Vector2u size = textureFly.getSize();
            sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
            // Re-scale (Force 50px width)
            float scale = 50.0f / size.x;
            sprite.setScale(scale, scale);
        }
    } 
    else if (movementState == PlayerStates::STATE_SLIDING) {
        // 2. SLIDING STATE
        if (sprite.getTexture() != &textureSlide) {
            sprite.setTexture(textureSlide);
            sf::Vector2u size = textureSlide.getSize();
            sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
            float scale = 80.0f / size.x;
            sprite.setScale(scale, scale);
            
            // Adjust Y down slightly for slide center
            this->y = this->groundY + 25.0f; 
        }
    } 
    else {
        // 3. RUNNING / JUMPING STATE
        if (sprite.getTexture() != &textureRun) {
            sprite.setTexture(textureRun);
            sf::Vector2u size = textureRun.getSize();
            sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
            float scale = 50.0f / size.x;
            sprite.setScale(scale, scale);
            
            // Reset Y to ground
            this->y = this->groundY;
        }
    }



    // --Physics and State logic--

    // 1. --JEtpack Logic-- it overrides gravity and jumping
    if(jetpackTimer > 0) {
        jetpackTimer--;

        // fly up to Y=100 i.e top of screen area
        // smooth transition, move 10pixels per frame upwards until hit 100
        if(this->y > 100.0f) {
            this->y -= 10.0f;
        } else{
            this->y = 100.0f; // clamp to 100
        }

        //visual; for now it is cyan when flying
        sprite.setColor(sf::Color::Cyan);

        if(jetpackTimer == 0) {
            cout << "[DEBUG] Jetpack Deactivated fuel empty..." << endl;
            // when ending ensure player falls naturally
            verticalVelocity = 0.0f;
            movementState = PlayerStates::STATE_JUMPING; //it will trigger gravity next frame  
             
            sprite.setColor(sf::Color::White); // reset color
        }
    }
    //---- Normal physics Only when jetpack is off-----
    // 2. Handle jumping physics
    else if(this->movementState == PlayerStates::STATE_JUMPING ) {
        //B. move player vertically
        this->y += verticalVelocity;
        //A. apply gravity
        this->verticalVelocity += gravity;
        //c. CHECKING GROUND COLLISION
        if (this->y >= this->groundY) {
            cout<< "[DEBUG] Player Landed on ground." << endl;
            this->y = this->groundY; // reset to ground level
            verticalVelocity = 0.0f; //stop falling
            this->movementState = PlayerStates::STATE_RUNNING;
        }   
    }
    // 3. -- SLIDING LOGIC-- only when jetpack is off
    else if(this->movementState == PlayerStates::STATE_SLIDING) {
        this->slideTimer--; 
        // if times up return to running stand up
        if (this->slideTimer <= 0) {
            cout << "[DEBUG] Slide ended.(Standingg..,)" << endl;
            this->movementState = PlayerStates::STATE_RUNNING;

            // reset size and position
            sf::Vector2u size = textureSlide.getSize();
            float scale =50.0/size.x;
            sprite.setScale(scale, scale);
            // reset y position original position 
            this->y = this->groundY;
        }
    }
    
    // 4. Magnet logic
    if(magnetTimer > 0) {
        magnetTimer--;
        if(magnetTimer == 0) {
            cout << "[DEBUG] Magnet Deactivated." << endl;
        }
    }
    // 5. Double Coin logic
    if(doubleCoinTimer > 0) {
        doubleCoinTimer--;
        if(doubleCoinTimer == 0) {
            cout << "[DEBUG] Double Coin Deactivated." << endl;
        }
    }

    // 6. Shield and Magnet outline logic
    // Shield Visual
    if(hasShield){
        sprite.setColor(sf::Color(100,100,255)); // blue tint
    }
    else if(magnetTimer > 0){
        sprite.setColor(sf::Color(255,100,255)); // magenta
    }
    else if(doubleCoinTimer>0){
        sprite.setColor(sf::Color(255,255,100)); // yellow
    }
    else{
        sprite.setColor(sf::Color::White); // No Tint (Normal Image)
    }

    sprite.setPosition(x, y);

    // --- RUNNING ANIMATION (BOBBING) ---
    // Only bob if running on ground (not jumping, not sliding, not jetpack)
    if (movementState == PlayerStates::STATE_RUNNING && jetpackTimer <= 0) {
        runAnimTimer += 0.2f; // Speed of animation
        
        // Calculate bounce: moves up/down by 3 pixels
        float bounceY = sin(runAnimTimer) * 3.0f; 
        
        // Apply visual offset (Does not affect physics Y)
        sprite.setPosition(x, y + bounceY);
    } else {
        // Reset position if flying/jumping
        sprite.setPosition(x, y);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::setShield(bool state) { 
    hasShield = state;
    if(state) cout << "[DEBUG] Shield Activated." << endl;
    else cout << "[DEBUG] Shield Deactivated." << endl;
}
void Player::setMagnet(int durationFrames) { 
    magnetTimer = durationFrames; 
    cout << "[DEBUG] Magnet Activated for " << durationFrames/60.0f << " seconds." << endl;
}

bool Player::isMagnetActive() const { 
    return magnetTimer > 0; 
}

void Player::addCoin(int amount) { 
    // if double coin powerup active , double the amount
    if(doubleCoinTimer > 0) {
        amount *= 2;
        cout << "[DEBUG] Double Coin Active! Coins doubled to " << amount << endl;
    }
    coinsCollected += amount; 
    cout << "[DEBUG] Coin Collected " << amount << " coins. Total: " << coinsCollected << endl;
}

void Player::setJetpack(int durationFrames) {
    jetpackTimer = durationFrames;
    // set state to something safe so we don;t acccidently "land" mid-air
    movementState = PlayerStates::STATE_JUMPING;
    cout << "[DEBUG] Jetpack Activated!" << endl;
}

bool Player::isJetpackActive() const{
    return jetpackTimer > 0;
}

int Player::getCoins() const { return coinsCollected; }
bool Player::isShielded() const { return hasShield; }

sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }
void Player::applyPowerUp(PowerUp* p) { /* Logic handled in PowerUp classes usually, or here */ }

bool Player::isJumping() const { // checking if player is in jumping state
    return this->movementState == PlayerStates::STATE_JUMPING;
}

void Player::setDoubleCoin(int durationFrames) {
    doubleCoinTimer = durationFrames;
    cout << "[DEBUG] Double Coin Activated for " << durationFrames/60.0f << " seconds." << endl;
}

bool Player::isDoubleCoinActive() const {
    return doubleCoinTimer > 0;
}

bool Player::isSliding() const {
    return this->movementState == PlayerStates::STATE_SLIDING;
}
