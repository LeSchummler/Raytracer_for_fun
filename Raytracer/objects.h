#pragma once

#include "datatypes.h"

struct Material {
	uint32_t color;
	//float roughness;
	//bool metallic;
	char reflection;
};

struct Sphere {
	float3 coords;
	Material material;
	float radius;
};
/*
struct faces {
	float3 normal;
	float3 pointIndex[3];
	
};


struct Object {
	Material mat;
	float3 coords;
	
	float3 points[];
	faces faces[];
};
*/