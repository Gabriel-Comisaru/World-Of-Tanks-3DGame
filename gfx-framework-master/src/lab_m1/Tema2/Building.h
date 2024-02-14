#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera.h"

#include <vector>
#include <string>
#include <iostream>


namespace m1
{
	class Bullet
	{
		public:
		Mesh* mesh;
		float x;
		float y;
		float z;
		glm::vec3 position;
		glm::vec3 direction;
		float time;
			Bullet() {
				this->mesh = NULL;
				this->x = 0;
				this->y = 0;
				this->z = 0;
				this->position = glm::vec3(0, 0, 0);
				this->direction = glm::vec3(0, 0, 0);
				this->time = 0;
			}

			~Bullet() {
				if (mesh != NULL)
					delete mesh;
			}

			// get mesh
			Mesh* getMesh() {
				return mesh;
			}

			float getX() {
				return x;
			}

			float getY() {
				return y;
			}

			float getZ() {
				return z;
			}

			void setX(float x) {
				this->x = x;
			}

			void setY(float y) {
				this->y = y;
			}

			void setZ(float z) {
				this->z = z;
			}
			
	};
}