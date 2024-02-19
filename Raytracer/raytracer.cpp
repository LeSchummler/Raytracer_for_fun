#include "raytracer.h"

#include "objects.h"

// !!! Put somewhere else
bool solveQuadratic(float a, float b, float c, float &root1, float &root2) {
	if (b * b - 4. * a * c > 0) {
		root1 = (-b + sqrt(b * b - 4. * a * c)) / (2. * a);
		root2 = (-b - sqrt(b * b - 4. * a * c)) / (2. * a);
		return true;
	}
	return false;
}

bool solveQuadraticO(float a, float b, float c, float& root1, float& root2) {
	b = b / a * 0.5f;
	c = c / a;
	if ((c = b * b - c) < 0) {
		return false;
	}
	root1 = (-b + sqrtf(c));
	root2 = (-b - sqrtf(c));
	return true;
}


namespace GameScene {
	GameScene::GameScene(int height, int width, int dep, float nFov, float maxDist, float3 cDirection, float3 cOrigin) {
		depth = dep;
		maxDistance = maxDist;
		cameraDirection = cDirection;
		cameraLocation = cOrigin;
		fov = nFov;
		cLens = tan((fov * 0.5)* M_PI/180); // tan(deg2rad(fov*0.5))
		resizeBuffer(height, width);
	}
	GameScene::~GameScene() {
	}

	void GameScene::resizeBuffer(int height, int width) {
		getAllDirections(height, width);
		imageAspectRatio = width / (float)height;
	}

	//begin use of https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html

	void GameScene::getAllDirections(int height, int width) {

		int floatSize = sizeof(float3);
		// float3* base = (float3*)directionGrid; // +i * sizeof(float3) + j * width * sizeof(float3);
		float3 dir;
		directionGrid.resize(height * width);

		for (uint16_t j = 0; j < height; ++j) {
			for (uint16_t i = 0; i < width; ++i) {
				float x = (2 * (i + 0.5) / (float)width - 1) * imageAspectRatio * cLens;
				float y = (1 - 2 * (j + 0.5) / (float)height) * cLens;
				dir.coords[0] = x;
				dir.coords[1] = y;
				dir.coords[2] = -1.0f;
				dir.normalize();
				directionGrid[j * width + i] = dir;
			}
		}
		return;
		for (int i = 0; i < directionGrid.size(); i++) {
			std::cout << "(" << directionGrid[i].coords[0] << " | " << directionGrid[i].coords[1] << " | " << directionGrid[i].coords[2] << "), ";
			if (i % height == 0) {
				std::cout << std::endl << i << std::endl;
			}
		}

	}

	//end of use

	void GameScene::updateObjects(std::vector<Sphere> newObjects) {
		objects.resize(newObjects.size());
		for (int i = newObjects.size()-1; i >= 0; i--) {
			objects[i] = newObjects[i];
		}
	}

	void GameScene::updateCamera(float3 cameraPos, float3 cameraDir) {
		cameraLocation = cameraPos;
		cameraDirection = cameraDir;
	}

	uint32_t GameScene::getRayColor(float3 dir, float3 origin) {
		float nearestDistance = maxDistance; // Initialize with a large value.
		int nearestObjectIndex = -1;
		float3 normal = {0, 0, 0};
		char color[4] = {0, 0, 0, 255};
		uint32_t ObjectColor;

		for (int j = 0; j < depth; j++) {
			nearestObjectIndex = -1;
			for (int i = objects.size() - 1; i >= 0; i--) {
				float distance = getObjectDistance(dir, origin, &normal, i);
				if (distance >= 0 && distance < nearestDistance) {
					nearestDistance = distance;
					nearestObjectIndex = i;
				}
			}
			
			if (nearestObjectIndex >= 0) { // if any Object hit
				ObjectColor = objects[nearestObjectIndex].material.color;
			}
			else {
				ObjectColor = defaultColor;
			}
			
			color[0] = ((ObjectColor >> 16) & 0xff) * ((float)color[3] / 255) + color[0] * (1.0f - (float)color[3] / 255);
			color[1] = ((ObjectColor >> 8) & 0xff) * ((float)color[3] / 255) + color[1] * (1.0f - (float)color[3] / 255);
			color[2] = ((ObjectColor) & 0xff) * ((float)color[3] / 255) + color[2] * (1.0f - (float)color[3] / 255);
			
			if (nearestObjectIndex < 0) {
				break;
			}
			
			color[3] = objects[nearestObjectIndex].material.reflection; // save reflection for next iteration

			// set bounce variables
			origin.coords[0] = origin.coords[0] + nearestDistance * dir.coords[0];
			origin.coords[1] = origin.coords[1] + nearestDistance * dir.coords[1];
			origin.coords[2] = origin.coords[2] + nearestDistance * dir.coords[2];

			// same
			if (dir == (normal * (-1))) {
				dir = normal;
			}
			else {
				dir = dir - normal * (dir.dotProduct(normal)) * 2.0f;
			}
			
		}
		//ObjectColor = (color[0] << 16 | color[1] << 8 | color[2]);
		return ObjectColor;
	}

	float GameScene::getObjectDistance(float3 dir, float3 orig, float3* normal, int i) { // !!!calc radius^2 in previous

		float t0, t1;

		// geometric solution
		float3 L = objects[i].coords - orig;
		float tca = L.dotProduct(dir);
		// if (tca < 0) return -1;
		float d2 = L.dotProduct(L) - tca * tca;
		if (d2 > objects[i].radius*objects[i].radius) return -2;
		float thc = sqrt(objects[i].radius * objects[i].radius - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		/* ANALYTIC APPROACH currently not working*/
		/*
		

		float3 L = objects[i].coords - orig; // maybe orig - center
		float a = dir.dotProduct(dir);
		float b = 2 * dir.dotProduct(L);
		float c = L.dotProduct(L) - (objects[i].radius*objects[i].radius);
		if (!solveQuadraticO(a, b, c, t0, t1)) return -2;
		*/
		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead
			if (t0 < 0) return -3; // both t0 and t1 are negative
		}

		// PointHit = RayOrigin + t0 * RayDirection
		// *normal = ((orig + dir * t0) - objects[i].coords).normalize(); // set normal pointer

		return t0;
	}

	void GameScene::rewriteScreen(renderer::window &windowHandler) {
		for (int i = windowHandler.height-1; i >= 0; i--) {
			for (int j = windowHandler.width - 1; j >= 0; j--) {
				windowHandler.setPixel(
					j, i, getRayColor(
					directionGrid[i*windowHandler.width + j],
					cameraLocation));
				//std::cout << j << " | " << i << std::endl;
			}
		}
	}
}
