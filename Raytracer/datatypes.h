#pragma once

#include <stdint.h>
#include <math.h>
#include <array>

class float3 {
public:
	float coords[3]; // x, y, z : y = height

	float3 normalize();
	float getLength();
	float3 operator - (const float3 &second);
	float3 operator * (const float& second);
	float3 operator + (const float3& second);
	bool operator == (const float3& second);
	float dotProduct(const float3 &secondVector);
};
