#pragma once

#include <vector>
#include "renderer.h"
#include "datatypes.h"
#include "objects.h"
#include <corecrt_math_defines.h>
#include <math.h>

#include <iostream>

namespace GameScene {
	class GameScene {
	private:
		//change to vertices when changing
		std::vector<Sphere> objects; //aus verschiedener gründe ist int.maxValue = maxNumberOfObjectsInScene
		float3 cameraLocation;
		float3 cameraDirection;
		std::vector<float3> directionGrid; // old was voidpointer new is std::vector if changed back please make free() handling
		float maxDistance; // make public
		uint32_t defaultColor; // make public

		//begin use of https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
		//Matrix44f;
		float cLens;
		float imageAspectRatio;

		void getAllDirections(int height, int width);
		//end of use
		
		int depth;

		uint32_t getRayColor(float3 dir, float3 origin); //new Origin because of recursive depth method
		float getObjectDistance(float3 dir, float3 orig, float3 *normal, int objectIndex); //Checks the Distance of object to camera , Change normal pointer to reference when needed


	public:

		float fov;

		GameScene(int height, int width, int dep = 3, float nFov = 160, float maxDist = 2000000, float3 cDirection = { 0, 0, -1 }, float3 cOrigin = { 0, 0, 0 });
		~GameScene();

		void resizeBuffer(int height, int width);

		void updateObjects(std::vector<Sphere> newObjects);
		void updateCamera(float3 cameraPos, float3 cameraDir);
		
		void rewriteScreen(renderer::window& windowHandler);
	};
}

/*
TODO:
normalize function
define cLens, fov, imageAspectRatio
write All directions to directiongrid
write getObectDistance function
Check if it works ;)
*/