#include "GameEngine.h"
#include <iostream>
using namespace std;

GameEngine::GameEngine() {
    window.create(sf::VideoMode(800, 600), "Subway Surfers OOP");
    window.setFramerateLimit(60);
    
    player = new Player();
    trackManager = new TrackManager();
    scoreManager = new ScoreManager();
    scoreTimer = 0;

    // Init Variables
    currentState = STATE_MENU; // Start in Menu
    userInputName = "Player";  // Default Name
    isRunning = true;
    gameSpeed = 3.0f;

    // -----PASUE UI---
    isPaused=false;
    pauseOverlay.setSize(sf::Vector2f(800,600));
    pauseOverlay.setFillColor(sf::Color(0,0,0,150));

    // Pause text
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOutlineColor(sf::Color::Black);
    pauseText.setOutlineThickness(3);

    // center the pause text
    pauseText.setPosition(300,250);
    
    // 1. Load Font (arial.ttf)
    if(!font.loadFromFile("assets/arial.ttf")) {
        cout<< "[ERROR] Could not load assets/aerial.ttf" <<endl;
    }
    // ---Score TEXT
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);     //make test readable
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(10,10);

    // ---Title Text - menu
    titleText.setFont(font);
    titleText.setString("SUBWAY SURFERS 2D");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(4);
    titleText.setPosition(150, 50);

    // Instructions - menu
    instructionsText.setFont(font);
    instructionsText.setString("CONTROLS:\nW - Jump\nS - Slide\nA/D - Move\n\nPress ENTER to Start\nPress ESC to Quit");
    instructionsText.setCharacterSize(20);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setOutlineColor(sf::Color::Black);
    instructionsText.setOutlineThickness(2);
    instructionsText.setPosition(250, 300);

    // Name Input - menu
    nameInputText.setFont(font);
    nameInputText.setCharacterSize(30);
    nameInputText.setFillColor(sf::Color::Cyan);
    nameInputText.setOutlineColor(sf::Color::Black);
    nameInputText.setOutlineThickness(2);
    nameInputText.setPosition(200, 200);

    // Game Over Text
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(3);
    gameOverText.setPosition(150, 200);

    // 2. load Background
    if(!bgTexture.loadFromFile("assets/background.png")){
        cout<<"[ERROR] Could not load background.png"<<endl;
    }

    //setup bg sprite1
    bgSprite1.setTexture(bgTexture);
    // scale to window size
    sf::Vector2u bgSize = bgTexture.getSize();
    bgSprite1.setScale(800.0f/bgSize.x, 600.0f / bgSize.y);
    bgSprite1.setPosition(0,0);

    // setup bg sprite2
    bgSprite2.setTexture(bgTexture);
    bgSprite2.setScale(800.0f / bgSize.x, 600.0f / bgSize.y);
    bgSprite2.setPosition(0, -600.0f); // Starts off-screen top

    bgY = 0.0f;

    // 3. Load Inspector
    if(inspectorTexture.loadFromFile("assets/inspector.png")){
        inspectorSprite.setTexture(inspectorTexture);
        sf::Vector2u iSize = inspectorTexture.getSize();
        inspectorSprite.setScale(70.0f/ iSize.x, 70.0f/iSize.y); //Slightly bigger than player
    }
    inspectorTimer=0.0f;

    // MUSIC SETUP
    if (!bgMusic.openFromFile("assets/bgm.wav")) {
        cout << "[ERROR] Could not load bgm.wav" << endl;
    } else {
        bgMusic.setLoop(true);   // Loop forever
        bgMusic.setVolume(50);   // 50% Volume (Adjust as needed)
        bgMusic.play();          // Start playing immediately
    }
}

GameEngine::~GameEngine() {
    delete player;
    delete trackManager;
    delete scoreManager;
}

// Reset everything when restarting
void GameEngine::resetGame() {
    delete player;
    delete trackManager;
    player = new Player();
    trackManager = new TrackManager();
    
    scoreManager->resetScore();
    gameSpeed = 4.0f;
    scoreTimer = 0;
    inspectorTimer = 0;
}

void GameEngine::run() {
    while (window.isOpen() && isRunning) {
        handleInput();
        update();
        render();
    }
}

void GameEngine::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // --- MENU INPUT (Typing Name) ---
            if (currentState == STATE_MENU) {
                if (event.type == sf::Event::TextEntered) {
                    // Handle ASCII characters
                    if (event.text.unicode == 8) { // Backspace
                        if (!userInputName.empty()) 
                            userInputName.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode > 31) {
                        // Limit name length
                        if(userInputName.length() < 12)
                            userInputName += static_cast<char>(event.text.unicode);
                    }
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        currentState = STATE_GAME; // START GAME
                        resetGame(); // Ensure fresh start
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close(); // EXIT
                    }
                }
            }
            
            // --- GAME INPUT ---
            else if (currentState == STATE_GAME) {
                if (event.type == sf::Event::KeyPressed) {
                    // Toggle Pause & Music
                    if (event.key.code == sf::Keyboard::P) {
                        isPaused = !isPaused;
                        
                        if(isPaused) {
                            bgMusic.pause();
                        } else {
                            bgMusic.play();
                        }
                    }
                    //move only when not paused
                    if(!isPaused){
                        if (event.key.code == sf::Keyboard::A) player->moveLeft();
                        if (event.key.code == sf::Keyboard::D) player->moveRight();
                        if (event.key.code == sf::Keyboard::W) player->jump();
                        if (event.key.code == sf::Keyboard::S) player->slide();
                    }

                    // Quit to menu
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentState = STATE_MENU;
                        bgMusic.stop();
                    }
                }
            }

            // --- GAME OVER INPUT ---
            else if (currentState == STATE_GAMEOVER) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) {
                        currentState = STATE_MENU; // Go back to menu
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
            }

    }
}

void GameEngine::update() {

    // 1. MENU STATE: Just update visual text
    if (currentState == STATE_MENU) {
        nameInputText.setString("Enter Name: " + userInputName + "_");
        return; 
    }

    // 2. GAME OVER STATE: Freeze
    if (currentState == STATE_GAMEOVER) {
        return;
    }


    // Pause check
    if(isPaused){
        return;     // Stop, so donot update positions, no spawning
    }

    player->update();
    trackManager->updateTrack(gameSpeed,player);

    // --- BACKGROUND SCROLLING LOGIC ---
    // Move background down based on game speed (creating forward movement illusion)
    float scrollSpeed = gameSpeed ; // Multiplier to make it visible
    
    bgSprite1.move(0, scrollSpeed);
    bgSprite2.move(0, scrollSpeed);

    // now this will keep the images glued together, so no overshoots
    // If Sprite 1 goes off screen bottom, move to top
    if (bgSprite1.getPosition().y >= 600) {
        bgSprite1.setPosition(0, bgSprite1.getPosition().y-1200);
    }

    // If Sprite 2 goes off screen bottom, move to top
    if (bgSprite2.getPosition().y >= 600) {
        bgSprite2.setPosition(0, bgSprite2.getPosition().y-1200);
    }

    // Update Inspector timer 60frames= 1s
    if(inspectorTimer<420.0f){  // 7s * 60fps
        inspectorTimer+=1;
        // Inspector chases player X, but stays lower Y(behind)
        float chaseX = player->getX();
        float chaseY = player->getY()+50;
        inspectorSprite.setPosition(chaseX, chaseY);

    }

    // --DYNAMIC SPEED INCREASE
    if(gameSpeed<15.0f){ //max speed caped to prevent unplayable
        gameSpeed+=0.0005f;
    }

    //--Score  Logic--
    // 1. Timer based scoring (10 points per second)
    scoreTimer++;
    if(scoreTimer >= 60) { // assuming 60 FPS = 1 sec
        scoreManager->addScore(10);
        scoreTimer = 0;

        cout<<"Score: "<<scoreManager->getScore()<<endl;
    }

    // Collision Check -> Trigger Game Over State
    if(trackManager->checkCollisions(player)) {
        currentState = STATE_GAMEOVER; // Switch State (Do NOT close window)
        
        scoreManager->saveHighScore(); // Save Data
        
        // Update Game Over Text with final score
        gameOverText.setString("GAME OVER\n\nScore: " + std::to_string(scoreManager->getScore()) + 
                               "\nHigh Score: " + std::to_string(scoreManager->getHighScore()) + 
                               "\n\nPress R to Menu\nPress ESC to Quit");
        
        // Center text visually
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        gameOverText.setPosition(400, 300);
    }
    
    // Simple Speed Scaling
    gameSpeed += 0.0005f; // increase speed gradually
}

void GameEngine::render() {
    window.clear();

    // 1. Draw BG
    window.draw(bgSprite1);
    window.draw(bgSprite2);

    if (currentState == STATE_MENU) {
        // Draw Menu Overlay
        sf::RectangleShape overlay(sf::Vector2f(800, 600));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        window.draw(titleText);
        window.draw(nameInputText);
        window.draw(instructionsText);
        
        // Show High Score on Menu
        sf::Text highText;
        highText.setFont(font);
        highText.setCharacterSize(20);
        highText.setFillColor(sf::Color::Green);
        highText.setPosition(280, 500);
        highText.setString("Current High Score: " + std::to_string(scoreManager->getHighScore()));
        window.draw(highText);
    }
    else if (currentState == STATE_GAME) {
        if(inspectorTimer < 420.0f) window.draw(inspectorSprite);
        trackManager->draw(window);
        player->draw(window);
        
        // UI Box
        sf::RectangleShape scoreBox(sf::Vector2f(200.0f,100.0f));
        scoreBox.setFillColor(sf::Color(0,0,0,150));
        window.draw(scoreBox);
        
        scoreText.setString("Player: " + userInputName + "\nScore: " + std::to_string(scoreManager->getScore()));
        window.draw(scoreText);
    }
    else if (currentState == STATE_GAMEOVER) {
        // Draw the last frame of the game behind the text
        trackManager->draw(window);
        player->draw(window);
        
        sf::RectangleShape overlay(sf::Vector2f(800, 600));
        overlay.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(overlay);
        
        window.draw(gameOverText);
    }

    // PAUSE------------
    if(isPaused){
        window.draw(pauseOverlay);
        window.draw(pauseText);
    }

    window.display();
}

void GameEngine::gameOver() {
    cout<<"------------------------------------"<<endl;
    cout << "GAME OVER!" << endl;
    cout << "Final Score: " << scoreManager->getScore() << endl;
    cout << "High Score: " << scoreManager->getHighScore() << endl;
    cout<<"------------------------------------"<<endl;

    // Requirement: Save to file
    scoreManager->saveHighScore();
    
    isRunning = false;
    window.close();
}
