
#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = false;

    nrOfBuildings = 30;
    nrOfEnemies = 10;
    lastKey = 'n'; // no key pressed
    bulletInterval = 2.0f; // 2 seconds between bullets of enemies
    playerHp = 3;
    score = 0;

    gracePeriod = true; 
    time = 0;
    graceTime = 0;

    random = 0;

    playerCoord = glm::vec3(0, 0, 3.5f);
    rotate = 0;
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 0.8, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    cannonRotate = 45.0f;

    // set game state to start menu
    gameState = GameState::START_MENU;

    textRend = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRend->Load(window->props.selfDir + "\\assets\\fonts\\Hack-Bold.ttf", 128);

    // load player's tank
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila1cover");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila1cover.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("senila2cover");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila2cover.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bazaTank");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "bazaTank.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bazaTun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "bazaTun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

  
    int startX = 1;
    int startZ = 1;
    int limitX = 24;
    int limitZ = 24;

    // spawn buildings in random positions
    for (int i = 0; i < nrOfBuildings; i++) {
        Building* building = new Building();
		building->mesh = new Mesh("bloc");
        building->mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "bloc.obj");
        // random coordinates
        if (rand() % 2 == 0)
			building->x = startX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitX - startX)));
		else
			building->x = -startX - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitX - startX)));
        if (rand() % 2 == 0)
            building->z = startZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitZ - startZ)));
        else
            building->z = -startZ - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitZ - startZ)));
        buildings.push_back(building);
    }

    // spawn enemies in random positions without spawning them in buildings
    for (int i = 0; i < nrOfEnemies; i++) {
        float x, z;
        bool ok = false;
        while (!ok) {
			ok = true;
            if (rand() % 2 == 0) {
                x = -startX - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitX - startX)));
                z = -startZ - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitZ - startZ)));
            }
            else {
                x = startX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitX - startX)));
				z = startZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitZ - startZ)));
            }
            for (int j = 0; j < nrOfBuildings; j++) {
                if (x + 0.5f >= buildings[j]->x - 1 && x - 0.5f <= buildings[j]->x + 1 &&
                    z + 0.5f >= buildings[j]->z - 1 && z - 0.5f <= buildings[j]->z + 1) {
					ok = false;
					break;
				}
			}
		}
        Enemy* enemy = new Enemy();
		enemy->mesh = new Mesh("enemy");
		enemy->position = glm::vec3(x, 0, z);
		enemy->direction = glm::vec3(0, 0, 0);
		enemy->color = glm::vec3(1, 0, 0);
        enemy->moves.push_back(1);
        enemy->moves.push_back(2);
        enemy->moves.push_back(3);
        enemy->moves.push_back(4);
        enemy->moves.push_back(5);
        enemy->moves.push_back(6);
		enemies.push_back(enemy);
    }


    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    {
        lightPosition = glm::vec3(0, 1, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

   
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f); 

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

int Tema2::getRandomValue() {
    // Set up a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a custom probability distribution
    std::discrete_distribution<> distribution({ 30, 30, 20, 20 }); // Adjust probabilities as needed

    // Generate a random value
    return distribution(gen) + 1; // Add 1 to shift the range to [1, 4]
}

void Tema2::Update(float deltaTimeSeconds)
{
    std::string scoreStr;
    switch (gameState) {
    case GameState::START_MENU:
        // change background color
        glClearColor(0.12157, 0.47843, 0.12157, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textRend->RenderText("WORLD OF TANKS", window->GetResolution().x / 12, window->GetResolution().y / 4, 1.0f, glm::vec3(0, 0, 0));
        textRend->RenderText("Press   to start the game", window->GetResolution().x / 3 - window->GetResolution().x / 15, 2 * window->GetResolution().y / 3, 0.3f, glm::vec3(0, 0, 0));
        textRend->RenderText("X", 2 * window->GetResolution().x / 5 - 30, 2 * window->GetResolution().y / 3, 0.3f, glm::vec3(1, 0, 0));

        break;
    case GameState::GAME:

        graceTime += deltaTimeSeconds;
        time += deltaTimeSeconds;
        // grace time period to not get spawn killed
        if (graceTime >= 3.0f)
            gracePeriod = false;

        // if the game ran for more than 60 seconds, game over
        if (graceTime >= 60.0f) {
            gameState = GameState::GAME_OVER;
			break;
        }
        // light is attached to player
        lightPosition = playerCoord + glm::vec3(0, 1, 0);
        bulletInterval += deltaTimeSeconds;

        // spawn hearts at randomly once every 10 seconds
        if (time >= 10.0f) {
            time = 0;
            float x, z;
            bool ok = false;
            int startX = 1;
            int startZ = 1;
            int limitX = 24;
            int limitZ = 24;
            while (!ok) {
                ok = true;
                if (rand() % 2 == 0) {
                    x = -startX - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitX - startX)));
                    z = -startZ - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitZ - startZ)));
                }
                else {
                    x = startX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitX - startX)));
                    z = startZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (limitZ - startZ)));
                }
                for (int j = 0; j < nrOfBuildings; j++) {
                    if (x + 0.5f >= buildings[j]->x - 1 && x - 0.5f <= buildings[j]->x + 1 &&
                        z + 0.5f >= buildings[j]->z - 1 && z - 0.5f <= buildings[j]->z + 1) {
                        ok = false;
                        break;
                    }
                }
            }
            Heart* heart = new Heart();
            heart->mesh = new Mesh("heart");
            heart->mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "heart.obj");
            heart->position = glm::vec3(x, 0, z);
			hearts.push_back(heart);
        }

        {
            // render hearts
            for (int i = 0; i < hearts.size(); i++) {

                // verify collision between player and hearts
                float dist = glm::distance(playerCoord, hearts[i]->position);
                if (dist < 1.2f) {
                    delete hearts[i];
                    hearts.erase(std::remove(hearts.begin(), hearts.end(), hearts[i]), hearts.end());
                    playerHp++;
                    if (playerHp < 3)
                        playerHp = 3;
                    continue;
                }
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, hearts[i]->position);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
                RenderSimpleMesh(hearts[i]->mesh, shaders["TemaShader"], modelMatrix, glm::vec3(1, 0, 0), 5);
            }
        }

        {
            // render buildings
            for (int i = 0; i < nrOfBuildings; i++) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(buildings[i]->x, 0, buildings[i]->z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
                RenderSimpleMesh(buildings[i]->mesh, shaders["TemaShader"], modelMatrix, glm::vec3(0.64314, 0.64314, 0.75686), 5);
            }

            // render enemies
            for (int i = 0; i < enemies.size(); i++) {
                enemies[i]->bulletInterval += deltaTimeSeconds;
                if (enemies[i]->isDead) {
                    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemies[i]), enemies.end());
                    score++;
                    // if all enemies are dead
                    if (score == nrOfEnemies) {
                        gameState = GameState::GAME_OVER;
                    }
                    continue;
                }
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemies[i]->position);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
                modelMatrix = glm::rotate(modelMatrix, enemies[i]->rotate, glm::vec3(0, 1, 0));
                RenderSimpleMesh(meshes["senila1"], shaders["TemaShader"], modelMatrix, glm::vec3(0.64314, 0.64314, 0.75686), enemies[i]->hp);
                RenderSimpleMesh(meshes["senila2"], shaders["TemaShader"], modelMatrix, glm::vec3(0.64314, 0.64314, 0.75686), enemies[i]->hp);
                RenderSimpleMesh(meshes["senila1cover"], shaders["TemaShader"], modelMatrix, enemies[i]->color, enemies[i]->hp);
                RenderSimpleMesh(meshes["senila2cover"], shaders["TemaShader"], modelMatrix, enemies[i]->color, enemies[i]->hp);
                RenderSimpleMesh(meshes["bazaTank"], shaders["TemaShader"], modelMatrix, enemies[i]->color, enemies[i]->hp);


                enemies[i]->cannonAngle = 0.0f;
                // verify if enemy is 10 blocks away from player
                if (glm::distance(playerCoord, enemies[i]->position) < 10) {
                    glm::vec3 direction = glm::normalize(playerCoord - enemies[i]->position);
                    enemies[i]->cannonAngle = acos(glm::dot(direction, glm::vec3(0, 0, 1)));
                    if (playerCoord.x < enemies[i]->position.x)
                        enemies[i]->cannonAngle = -enemies[i]->cannonAngle;
                    enemies[i]->inPlayerRange = true;
                }
                else {
                    enemies[i]->cannonAngle = enemies[i]->rotate;
                    enemies[i]->inPlayerRange = false;
                }



                // rotate enemy towards player if in range
                glm::mat4 modelMatrix2 = glm::mat4(1);
                modelMatrix2 = glm::translate(modelMatrix2, enemies[i]->position);
                modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.3f));
                modelMatrix2 = glm::rotate(modelMatrix2, enemies[i]->cannonAngle, glm::vec3(0, 1, 0));

                RenderSimpleMesh(meshes["bazaTun"], shaders["TemaShader"], modelMatrix2, enemies[i]->color, enemies[i]->hp);
                RenderSimpleMesh(meshes["tun"], shaders["TemaShader"], modelMatrix2, glm::vec3(0.64314, 0.64314, 0.75686), enemies[i]->hp);

                // if the enemy is at the border of the map, go into the opposite direction
                if (enemies[i]->position.x < -25 + 0.6f || enemies[i]->position.x > 25 - 0.6f || enemies[i]->position.z < -25 + 0.6f || enemies[i]->position.z > 25 - 0.6f) {
                    if (enemies[i]->random == 1)
                        enemies[i]->random = 2;
                    else if (enemies[i]->random == 2)
                        enemies[i]->random = 1;
                }
                else if (enemies[i]->timeSinceLastMove >= 4.0f) { // change the move every 4 seconds
                    enemies[i]->random = getRandomValue();
                    enemies[i]->timeSinceLastMove = 0;
                    if (enemies[i]->random == 3 || enemies[i]->random == 4) { // if the move is to rotate, rotate for just 1 second
                        enemies[i]->timeSinceLastMove = 3.0f;
                    }
                }

                enemies[i]->makeMove(enemies[i]->random, deltaTimeSeconds);
                enemies[i]->timeSinceLastMove += deltaTimeSeconds;

                // verify collision between buildings and enemies
                for (int j = 0; j < nrOfBuildings; j++) {
                    float dist = glm::distance(enemies[i]->position, glm::vec3(buildings[j]->x, 0, buildings[j]->z));
                    if (dist < 1.8f) {
                        float P = abs(1.8f - dist);
                        glm::vec3 dif = glm::vec3(buildings[j]->x, 0, buildings[j]->z) - enemies[i]->position;
                        glm::vec3 x = P * glm::normalize(dif);
                        enemies[i]->position += x * -0.5f;
                    }
                }

                // verify collision between enemies and enemies
                for (int j = 0; j < enemies.size(); j++) {
                    if (i != j) {
                        float dist = glm::distance(enemies[i]->position, enemies[j]->position);
                        if (dist < 1.2f) {
                            float P = abs(1.2f - dist);
                            glm::vec3 dif = enemies[j]->position - enemies[i]->position;
                            glm::vec3 x = P * glm::normalize(dif);
                            enemies[i]->position += x * -0.5f;
                            enemies[j]->position += x * 0.5f;
                        }
                    }
                }

                // verify collision between enemies and player
                float dist = glm::distance(playerCoord, enemies[i]->position);
                if (dist < 1.2f) {
                    float P = abs(1.2f - dist);
                    glm::vec3 dif = enemies[i]->position - playerCoord;
                    glm::vec3 x = P * glm::normalize(dif);
                    playerCoord += x * -0.5f;
                    enemies[i]->position += x * 0.5f;
                }

                // shoot player
                if (enemies[i]->inPlayerRange && enemies[i]->bulletInterval >= 2.0f) {
                    Bullet* bullet = new Bullet();
                    bullet->mesh = new Mesh("bullet");
                    bullet->mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
                    bullet->position = enemies[i]->position;
                    bullet->position.x = enemies[i]->position.x + 3.5 * 0.3f * sin(enemies[i]->cannonAngle);
                    bullet->position.y = enemies[i]->position.y - 0.025f;
                    bullet->position.z = enemies[i]->position.z + 3.5 * 0.3f * cos(enemies[i]->cannonAngle);
                    bullet->position.y += 0.3f;
                    bullet->direction = glm::vec3(sin(enemies[i]->cannonAngle), 0, cos(enemies[i]->cannonAngle));
                    bullets.push_back(bullet);
                    enemies[i]->bulletInterval = 0.0f;
                }
            }
        }

        {
            // verify collision between buildings and player
            for (int i = 0; i < nrOfBuildings; i++) {
                float dist = glm::distance(playerCoord, glm::vec3(buildings[i]->x, 0, buildings[i]->z));
                if (dist < 1.8f) {
                    float P = abs(1.8f - dist);
                    glm::vec3 dif = glm::vec3(buildings[i]->x, 0, buildings[i]->z) - playerCoord;
                    glm::vec3 x = P * glm::normalize(dif);
                    playerCoord += x * -0.5f;
                }
            }
        }


        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, playerCoord);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(180), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));

            RenderSimpleMesh(meshes["senila1"], shaders["TemaShader"], modelMatrix, glm::vec3(0.64314, 0.64314, 0.75686), playerHp);
            RenderSimpleMesh(meshes["senila2"], shaders["TemaShader"], modelMatrix, glm::vec3(0.64314, 0.64314, 0.75686), playerHp);
            RenderSimpleMesh(meshes["senila1cover"], shaders["TemaShader"], modelMatrix, glm::vec3(0.12157, 0.47843, 0.12157), playerHp);
            RenderSimpleMesh(meshes["senila2cover"], shaders["TemaShader"], modelMatrix, glm::vec3(0.12157, 0.47843, 0.12157), playerHp);
            RenderSimpleMesh(meshes["bazaTank"], shaders["TemaShader"], modelMatrix, glm::vec3(0.12157, 0.47843, 0.12157), playerHp);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, playerCoord);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90) + cannonRotate, glm::vec3(0, 1, 0)); // rotate cannon towards mouse

            RenderSimpleMesh(meshes["bazaTun"], shaders["TemaShader"], modelMatrix, glm::vec3(0.12157, 0.47843, 0.12157), playerHp);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, playerCoord);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
            modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90) + cannonRotate, glm::vec3(0, 1, 0)); // rotate cannon towards mouse

            RenderSimpleMesh(meshes["tun"], shaders["TemaShader"], modelMatrix, glm::vec3(0.64314, 0.64314, 0.75686), playerHp);
        }

        {
            // render bullets
            float maxBulletTime = 3.0f;
            float bulletSpeed = 4.0f;
            bool toSkip = false;
            for (Bullet* bullet : bullets) {

                // Update the bullet position based on direction and velocity
                bullet->position += bullet->direction * deltaTimeSeconds * bulletSpeed;
                bullet->time += deltaTimeSeconds;

                // Check for collision with buildings or other conditions and remove the bullet if necessary

                // Example: Remove bullets that have traveled a certain distance
                if (bullet->time > maxBulletTime) {
                    // Remove the bullet from the list
                    bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
                    delete bullet;
                    continue;
                }

                // collision with buildings
                for (int i = 0; i < nrOfBuildings; i++) {
                    if (bullet->position.x + 0.1f >= buildings[i]->x - 1 && bullet->position.x - 0.1f <= buildings[i]->x + 1 &&
                        bullet->position.z + 0.1f >= buildings[i]->z - 1 && bullet->position.z - 0.1f <= buildings[i]->z + 1) {
                        // Remove the bullet from the list
                        bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
                        delete bullet;
                        toSkip = true;
                        break;
                    }
                }

                // bullet collision with enemies by player
                for (int i = 0; i < enemies.size(); i++) {
                    float dist = glm::distance(bullet->position, enemies[i]->position);
                    if (dist < 0.5f) {
                        // Remove the bullet from the list
                        bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
                        delete bullet;
                        toSkip = true;
                        // if the bullet is from player, damage enemy
                        if (std::count(playerBullets.begin(), playerBullets.end(), bullet) > 0) {
                            enemies[i]->hp--;
                        }

                        if (enemies[i]->hp <= 0)
                            enemies[i]->isDead = true;
                        break;
                    }
                }

                // bullet collision with player by enemy
                float dist = glm::distance(bullet->position, playerCoord);
                if (dist < 0.5f) {
                    bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
                    delete bullet;
                    if (!gracePeriod)
                        playerHp--;
                    toSkip = true;
                    if (playerHp <= 0)
                        gameState = GameState::DEAD;
                    continue;
                }

                if (toSkip) {
                    toSkip = false;
                    continue;
                }

                // Render the bullet
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, bullet->position);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
                RenderSimpleMesh(bullet->mesh, shaders["TemaShader"], modelMatrix, glm::vec3(0, 0, 0), 5);
            }
        }




        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.15f, 0));
            RenderSimpleMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, glm::vec3(0.60000, 1.00000, 0.60000), 5);
        }

        // set the camera to follow the player
        camera->SetDependingTank(playerCoord);
        break;
    case GameState::DEAD:
        // change background color
        glClearColor(0.25098, 0.25098, 0.25098, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        textRend->RenderText("YOU DIED", window->GetResolution().x / 4, window->GetResolution().y / 4, 1.0f, glm::vec3(1, 1, 1));
        scoreStr = "Your score is " + std::to_string(score) + "/" + std::to_string(nrOfEnemies);
        textRend->RenderText(scoreStr, window->GetResolution().x / 3 - 10, window->GetResolution().y / 2, 0.3f, glm::vec3(0.00000, 0.50196, 0.00000));
        textRend->RenderText("Press SPACE to restart", window->GetResolution().x / 3 - 125, 2 * window->GetResolution().y / 3, 0.4f, glm::vec3(1, 1, 1));
        break;
    case GameState::GAME_OVER:
        // change background color
        glClearColor(0.25098, 0.25098, 0.25098, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textRend->RenderText("GAME OVER", window->GetResolution().x / 5 + 20, window->GetResolution().y / 4, 1.0f, glm::vec3(1, 1, 1));
        // save the score into a string 
        std::string scoreStr = "Your score is " + std::to_string(score) + "/" + std::to_string(nrOfEnemies);
        textRend->RenderText(scoreStr, window->GetResolution().x / 3 - 10, window->GetResolution().y / 2, 0.3f, glm::vec3(0.00000, 0.50196, 0.00000));
        textRend->RenderText("Press SPACE to restart", window->GetResolution().x / 3 - 125, 2 * window->GetResolution().y / 3, 0.4f, glm::vec3(1, 1, 1));
        break;
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int hp)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(loc_light_position, 1, glm::value_ptr(lightPosition));

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    GLint loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

    materialShininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(materialShininess, materialShininess);

    materialShininess = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(materialShininess, materialKd);

    materialShininess = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(materialShininess, materialKs);

    GLint loc_object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(loc_object_color, 1, glm::value_ptr(color));

    // color
    int light_color_location = glGetUniformLocation(shader->program, "light_color");
    glUniform3fv(light_color_location, 1, glm::value_ptr(glm::vec3(0, 1, 0)));

    // hp
    int hp_location = glGetUniformLocation(shader->program, "hp");
    glUniform1i(hp_location, hp);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    movementSpeed = 2.0f;
    rotationSpeed = 1.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
		glm::vec3 direction = glm::vec3(sin(M_PI - rotate), 0, -cos(M_PI - rotate)); // direction of movement (it may be diagonal)
        playerCoord -= glm::normalize(direction) * deltaTime * movementSpeed;
        lastKey = 'W';
	}

    if (window->KeyHold(GLFW_KEY_S)) {
        glm::vec3 direction = glm::vec3(sin(M_PI - rotate), 0, -cos(M_PI - rotate)); // direction of movement (it may be diagonal)
        playerCoord += glm::normalize(direction) * deltaTime * movementSpeed;
        lastKey = 'S';
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        float in = rotate;
        rotate += deltaTime * rotationSpeed;

        camera->RotateFirstPerson_OY(rotate - in);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
		float in = rotate;
		rotate -= deltaTime * rotationSpeed;

		camera->RotateFirstPerson_OY(rotate - in);
	}
}


void Tema2::OnKeyPress(int key, int mods)
{
    // start the game
    if (key == GLFW_KEY_X && gameState == GameState::START_MENU) {
		gameState = GameState::GAME;
	}

    // restart game on space
    if (key == GLFW_KEY_SPACE && gameState != GameState::START_MENU) {
        ResetGame();
        gameState = GameState::START_MENU;
    }

}

void Tema2::ResetGame()
{
    // Reset game state and variables to their initial values
    playerCoord = glm::vec3(0, 0, 3.5f);
    rotate = 0;
    cannonRotate = 45.0f;
    playerHp = 3;
    score = 0;
    bulletInterval = 2.0f;
    gameState = GameState::GAME;

    // Clear existing enemies, bullets and hearts
    for (auto& enemy : enemies)
        delete enemy;
    enemies.clear();

    for (auto& bullet : bullets)
        delete bullet;
    bullets.clear();

    for (auto& heart : hearts)
		delete heart;
    hearts.clear();

    Init();

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    // if right click is pressed, rotate the camera around the tank
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        if (window->GetSpecialKeyState() == 0) {
            // rotate vertically and horizontally; if needed we can comment the first line and move it only horizontally
            camera->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);
        }
    }
    else {
        int mouseOffsetY = window->GetResolution().y - mouseY;

        // center of cannon
        int centerX = window->GetResolution().x / 2;
        int centerY = 2 * window->GetResolution().y / 3;

        // float rotationSensitivity = 0.01f;

        // cannon rotation
        cannonRotate = atan2((mouseOffsetY - centerY) * 2, mouseX - centerX);

    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // shoot bullets with 1 second interval between them
    if (button == GLFW_MOUSE_BUTTON_2 && bulletInterval >= 1.0f) {
        Bullet* bullet = new Bullet();
        bullet->mesh = new Mesh("bullet");
        bullet->mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        bullet->position = playerCoord;
        bullet->position.x = playerCoord.x + 3.5 * 0.3f * sin(cannonRotate + RADIANS(90) + rotate);
        bullet->position.y = playerCoord.y - 0.025f;
        bullet->position.z = playerCoord.z + 3.5 * 0.3f * cos(cannonRotate + rotate + RADIANS(90));
        bullet->position.y += 0.3f;
        bullet->direction = glm::vec3(sin(cannonRotate + RADIANS(90) + rotate), 0, cos(cannonRotate + rotate + RADIANS(90)));
        playerBullets.push_back(bullet);
        bullets.push_back(bullet);
        bulletInterval = 0;
    }
    
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

