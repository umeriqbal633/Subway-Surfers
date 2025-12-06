#include "TrackManager.h"
#include "PowerUps.h"
#include <cstdlib>
#include <iostream>
using namespace std;

TrackManager::TrackManager() {
    spawnTimer = 0;
}

TrackManager::~TrackManager() {
    // Manual memory cleanup
    for(int i=0; i<obstacles.getSize(); i++) delete obstacles.get(i);
    for(int i=0; i<coins.getSize(); i++) delete coins.get(i);
    for(int i=0; i<powerups.getSize(); i++) delete powerups.get(i);
}

void TrackManager::generateObstacle() {
    float lanes[] = {220.0f, 400.0f, 600.0f}; 
    int lane = rand() % 3;
    int type = rand() % 4;

    Obstacle* obs = nullptr;
    //spawn Y -200 offscreen top
    float spawnY= -250.0f;
    if(type == 0) obs = new Train(lanes[lane], spawnY);
    else if(type == 1) obs = new Barrier(lanes[lane], spawnY);
    else if(type == 2) obs = new Cone(lanes[lane], spawnY);
    else obs = new Fence(lanes[lane], spawnY);
    
    obstacles.add(obs);
}

void TrackManager::generateCoin() {
    float lanes[] = {200.0f, 400.0f, 600.0f}; 
    int lane = rand() % 3;
    coins.add(new Coin(lanes[lane], -50));
}

void TrackManager::generatePowerUp() {
    float lanes[] = {200.0f, 400.0f, 600.0f}; 
    int lane = rand() % 3;
    int type = rand() % 4; // 0: Shield, 1: Magnet 2: Double Coin 3.jetpack


    if(type == 0){
        // Spawn a Shield
        // Note : Y =-50 it ensures that it spawns above the screen
        powerups.add(new ShieldPower(lanes[lane], -50));
        cout<<"[TRACK] Shield spawned in lane "<<lane<<endl;
    }
    else if(type == 1){
        // Spawn a Magnet
        powerups.add(new MagnetPower(lanes[lane], -50));
        cout <<"[TRACK] Magnet spawned in lane "<<lane<<endl;
    }
    else if(type == 2){
        // Spawn Double Coin
        powerups.add(new DoubleCoinPower(lanes[lane], -50));
        cout <<"[TRACK] Double Coin spawned in lane "<<lane<<endl;
    }
    else{
        // Spawn Jetpack
        powerups.add(new JetpackPower(lanes[lane], -50));
        cout <<"[TRACK] Jetpack spawned in lane "<<lane<<endl;
    }
}

void TrackManager::updateTrack(float speed, Player* p) {
    spawnTimer++;
    if(spawnTimer > 60) {
        if(rand()%100 < 50) generateObstacle();
        if(rand()%100 < 30) generateCoin();
        if(rand()%100 < 5) generatePowerUp();
        spawnTimer = 0;
    }

    // Update Obstacles
    for(int i=0; i<obstacles.getSize(); i++) {
        Obstacle* o = obstacles.get(i);
        o->update(speed);
        if(o->getY() > 800) {
            delete o;
            obstacles.remove(i);
            i--;
        }
    }

    // Update Coins ... with magnet effect
    for(int i=0; i<coins.getSize(); i++) {
        Coin* c = coins.get(i);
        c->update(speed);

        // Magnet effect
        if(p->isMagnetActive()) {
            float dx = p->getX() - c->getBounds().left;
            float dy = p->getY() - c->getBounds().top;
            // move 10% closer each frame
            c->move(dx * 0.1f, dy * 0.1f);
        }

        if(c->getY() > 800) {
            delete c;
            coins.remove(i);
            i--;
        }
    }

    // Update PowerUps
    for(int i=0; i<powerups.getSize(); i++) {
        PowerUp* p = powerups.get(i);
        p->update(speed);
        if(p->getY() > 800) {
            delete p;
            powerups.remove(i);
            i--;
        }
    }
}

bool TrackManager::checkCollisions(Player* p) {

    //1. Check Obstacles (Trains, Barriers, Cones, Fences, etc)
    for(int i=0; i<obstacles.getSize(); i++) {
        if(obstacles.get(i)->checkCollision(p)) {

            // --Jetpack god mode
            // If player has jetpack active , ignore collisions
            if(p->isJetpackActive()) {
                cout<<"[JETPACK] Collision ignored due to active jetpack."<<endl;
                return false; // game continues
            }

            // Shield logic check
            if(p->isShielded()) {
                cout<<"[SHIELD] Saved player! Shield broken."<<endl;
                p->setShield(false); // Remove shield

                // Remove obstacle so we don't hit again next frame
                delete obstacles.get(i);
                obstacles.remove(i);
                return false;  // Game continues
            }

            return true; // Game Over
        }
    }
    
    //2. Coins
    for(int i=0; i<coins.getSize(); i++) {
        if(coins.get(i)->getBounds().intersects(p->getBounds())) {
            p->addCoin(1);
            delete coins.get(i);
            coins.remove(i);
            i--;
        }
    }

    // 3. PowerUps
    for(int i=0; i<powerups.getSize(); i++) {
        PowerUp* pu = powerups.get(i);

        // Simple collision/intersection check
        if(pu->getBounds().intersects(p->getBounds())) {
            // polymorphic effect application, applyEffect does different things based on powerup type
            pu->applyEffect(p);
            //remove powerup from track after applying effect
            delete pu;
            powerups.remove(i);
            i--;
        }
    }

    return false;
}

void TrackManager::draw(sf::RenderWindow& window) {
    for(int i=0; i<obstacles.getSize(); i++) obstacles.get(i)->draw(window);
    for(int i=0; i<coins.getSize(); i++) coins.get(i)->draw(window);
    for(int i=0; i<powerups.getSize(); i++) powerups.get(i)->draw(window);
}
