#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <manager/manager.hpp>
#include <entities/entity.hpp>
#include <entities/spaceship.hpp>

// Constructor definition
GameManager::GameManager(sf::RenderWindow *gameWindow)
    : gameWindow(gameWindow)
{
    // Initialise enemy spawner
    spawner = std::make_unique<Spawner>();
    font.loadFromFile("assets/8bit.ttf");

    // Initialise health bar
    struct FrameData frameInfo = {
        .framesPerUpdate = 6,
        .numFrames = 14,
        .xdim = 32,
        .ydim = 32};
    healthBar = std::make_unique<Animated>(728, 0, "assets/green_bar_v2.png", &frameInfo);

    // Initialise score text
    scoreText.setFont(font);
    scoreText.setPosition(10, 10);
    scoreText.setCharacterSize(24);
    // scoreText.setFillColor(sf::Color::White);

    // Initialise player
    int width = 220;
    int height = 70;
    startButton = std::make_unique<Button>(400 - (width / 2), 380 - (height / 2), width, height, *gameWindow, "Start", "assets/8bit.ttf", 50, 5, 48);

    width = 160;
    height = 60;
    quitButton = std::make_unique<Button>(400 - (width / 2), 480 - (height / 2), width, height, *gameWindow, "Quit", "assets/8bit.ttf", 40, 6, 40);

    frameInfo = {
        .framesPerUpdate = 6,
        .numFrames = 4,
        .xdim = 32,
        .ydim = 32};
    player = std::make_unique<Spaceship>(10, 500, "assets/animated_big_spaceship_v3.png", &frameInfo);
    std::cout << "GameManager initialized with window pointer" << std::endl;

    // Initialise background
    background.loadFromFile("assets/starry_background.png");
    backgroundSprite.setTexture(background);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(4.0f, 4.0f);
}

GameManager::~GameManager()
{
    std::cout << "Deleting GameManager";
}

// Update function definition
void GameManager::update()
{
    // If game hasn't started yet
    if (!started)
    {

        if (startButton->update())
        {
            started = true;
        }
        if (quitButton->update())
        {
            quit = true;
        }
        return;
    }

    if (ended)
    {
        if (quitButton->update())
        {
            quit = true;
        }
        return;
    }

    handleMovement();
    handleUpdates();
}

void GameManager::handleMovement()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
    {
        // left key is pressed: move our character
        // puts("Moving left");
        player->move(-1, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
    {
        // right key is pressed: move our character
        // puts("Moving right");
        player->move(1, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
    {
        // left key is pressed: move our character
        // puts("Moving up");
        player->move(0, -1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
    {
        // right key is pressed: move our character
        // puts("Moving down");
        player->move(0, 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
    {
        // space is pressed: shoot
        puts("Spacebar pressed");
        player->shoot();
    }
}

void GameManager::handleUpdates()
{
    // Update game logic here
    player->update();

    // Update spawner
    spawner->update(*player);

    // Updating player projectiles
    int len = player->fired.size();
    for (int i = 0; i < len; i++)
    {
        if (player->fired[i]->y < -32 || spawner->checkProjectileCollision(&(*player->fired[i])))
        {
            player->fired.erase(player->fired.begin() + i);
            // Since erasing vector component shifts whole vector reduce counting index and length of vector
            i--;
            len--;
        }
        else
        {
            player->fired[i]->update();
        }
    }

    // Update health bar
    healthBar->scaleRects(32, 32, (float)player->health / (float)player->maxHealth);
    healthBar->animate();

    // Update score
    scoreText.setString("Score: " + std::to_string(spawner->score));

    // Check if game is over
    if (player->health <= 0) {
        ended = true;
    }
}

// Draw function definition
void GameManager::draw()
{
    // Draw sprites to the window here
    gameWindow->clear(sf::Color::Black);
    gameWindow->draw(backgroundSprite);

    if (!started)
    {
        drawStart();
    }

    else if (ended)
    {
        drawEnd();
    } else {
        drawGame();
    }

    gameWindow->display();
}

void GameManager::drawStart()
{
    startButton->draw();
    quitButton->draw();
}
void GameManager::drawGame()
{
    // Draw projectiles
    int firedLen = player->fired.size();
    for (int i = 0; i < firedLen; i++)
    {
        gameWindow->draw(player->fired[i]->sprite);
    }

    // Draw player
    gameWindow->draw(player->sprite);

    // Draw projectiles
    int projLen = spawner->enemyProjectiles.size();
    for (int i = 0; i < projLen; i++)
    {
        gameWindow->draw(spawner->enemyProjectiles[i]->sprite);
    }

    // Draw enemies
    int enemyLen = spawner->enemies.size();
    for (int i = 0; i < enemyLen; i++)
    {
        gameWindow->draw(spawner->enemies[i]->sprite);
    }

    // Draw score
    gameWindow->draw(scoreText);

    // Draw health
    gameWindow->draw(healthBar->sprite);
}

void GameManager::drawEnd(){
    // Draw end screen
    quitButton->draw();

    // Draw score
    gameWindow->draw(scoreText);
}