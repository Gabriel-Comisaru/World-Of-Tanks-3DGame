#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera.h"

#include <vector>
#include <string>
#include <iostream>


namespace m1
{
	class Building
	{
	public:
		Mesh* mesh;
		float x;
		float y;
		float z;
		Building() {
			this->mesh = NULL;
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		~Building() {
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