#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera.h"

#include <vector>
#include <string>
#include <iostream>


namespace m1
{
	class Heart
	{

		public:
		Mesh* mesh;
		glm::vec3 position;
			Heart() {
				this->mesh = NULL;
				this->position = glm::vec3(0, 0, 0);
			}

			~Heart() {
				if (mesh != NULL)
					delete mesh;
			}

	};
}