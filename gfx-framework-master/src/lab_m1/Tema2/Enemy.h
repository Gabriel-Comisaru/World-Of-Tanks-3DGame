#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace m1
{
	class Enemy
	{
	public:
		Mesh* mesh;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float rotate;
		float movementSpeed;
		float rotationSpeed;
		std::vector<int> moves;
		bool isDead;
		float timeSinceLastMove;
		int random;
		bool inPlayerRange;
		float bulletInterval;
		float cannonAngle;
		int hp;
		Enemy() {
				this->mesh = NULL;
				this->position = glm::vec3(0, 0, 0);
				this->direction = glm::vec3(0, 0, 0);
				this->color = glm::vec3(0, 0, 0);
				this->rotate = 0;
				this->movementSpeed = 1.0f;
				this->rotationSpeed = 1.0f;
				this->isDead = false;
				this->timeSinceLastMove = 4.0f;
				this->inPlayerRange = false;
				this->bulletInterval = 2.0f;
				this->cannonAngle = 0.0f;
				this->hp = 3;
			}

		~Enemy() {
				if (mesh != NULL)
					delete mesh;
			}

		void makeMove(int move, float deltaTime) {
			switch (move)
			{
			case 1:
				moveForward(deltaTime);
				break;
			case 2:
				moveBackward(deltaTime);
				break;
			case 3:
				rotateLeft(deltaTime);
				break;
			case 4:
				rotateRight(deltaTime);
				break;
			default:
				break;
			}
		}

		void moveForward(float deltaTime) {
			glm::vec3 direction = glm::vec3(sin(M_PI - this->rotate), 0, -cos(M_PI - this->rotate));
			this->position -= glm::normalize(direction) * deltaTime * this->movementSpeed;
		}

		void moveBackward(float deltaTime) {
			glm::vec3 direction = glm::vec3(sin(M_PI - this->rotate), 0, -cos(M_PI - this->rotate));
			this->position += glm::normalize(direction) * deltaTime * this->movementSpeed;
		}

		void rotateLeft(float deltaTime) {
			float in = this->rotate;
			this->rotate += deltaTime * this->movementSpeed;
		}

		void rotateRight(float deltaTime) {
			float in = this->rotate;
			this->rotate -= deltaTime * this->movementSpeed;
		}

	};
}