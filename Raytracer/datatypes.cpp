#include "datatypes.h"

float3 float3::normalize() {
	float length = getLength();
	coords[0] = coords[0] / length;
	coords[1] = coords[1] / length;
	coords[2] = coords[2] / length;
	return *this;
}

float float3::getLength() {
	return sqrt(coords[0]*coords[0] + coords[1] * coords[1] + coords[2] * coords[2]);
}

float3 float3::operator -(const float3& second) {
	return float3({
		coords[0] - second.coords[0],
		coords[1] - second.coords[1],
		coords[2] - second.coords[2],
	});
}

float float3::dotProduct(const float3 &secondVector) {
	//ax × bx + ay × by
	return coords[0] * secondVector.coords[0] + coords[1] * secondVector.coords[1] + coords[2] * secondVector.coords[2];
}

float3 float3::operator * (const float& second) {
	return float3({
		coords[0] * second,
		coords[1] * second,
		coords[2] * second,
		});
}

float3 float3::operator+(const float3& second) {
	return float3({
		coords[0] * second.coords[0],
		coords[1] * second.coords[1],
		coords[2] * second.coords[2]
		});
}

bool float3::operator==(const float3& second) {
	return (
		coords[0] == second.coords[0] &&
		coords[1] == second.coords[1] &&
		coords[2] == second.coords[2]
		);
}
