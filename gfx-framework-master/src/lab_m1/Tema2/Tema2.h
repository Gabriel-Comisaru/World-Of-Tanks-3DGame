#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "lab_m1/Tema2/Building.h"
#include "lab_m1/Tema2/Bullet.h"
#include "lab_m1/Tema2/Enemy.h"
#include "lab_m1/Tema2/Heart.h"
#include "lab_m1/Tema2/UItext.h"

#include <random>


namespace m1
{
    enum class GameState {
        START_MENU,
        GAME,
        DEAD,
        GAME_OVER
    };


    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void ResetGame();

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int hp);



        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        int getRandomValue();
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool is_ortho;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        float rotationSpeed;
        float movementSpeed;
        glm::vec3 playerCoord;
        float rotate;
        float cannonRotate;

        int nrOfBuildings, nrOfEnemies;
        std::vector<Building*> buildings;
        std::vector<Bullet*> bullets;
        std::vector<Bullet*> playerBullets;
        std::vector<Enemy*> enemies;
        std::vector<Heart*> hearts;
        char lastKey;
        float bulletInterval;
        int playerHp;

        int random;

        GameState gameState;

        gfxc::TextRenderer* textRend;

        int score;

        bool gracePeriod;
        float time;
        float graceTime;

        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
