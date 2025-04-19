#pragma once
#include <math.h>
#include <raylib.h>

#include <iostream>

namespace vec_operations {

inline float VectorLength(const Vector3& vec) {
  return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline void Normalize(Vector3& vec) {
  float length = VectorLength(vec);
  vec.x = vec.x / length;
  vec.y = vec.y / length;
  vec.z = vec.z / length;
}

inline Vector3 operator*(float& a, Vector3& vec) {
  return (Vector3){a * vec.x, a * vec.y, a * vec.z};
}

inline Vector3 operator*(Vector3& vec, float& a) {
  return (Vector3){a * vec.x, a * vec.y, a * vec.z};
}

inline Vector3 operator+(Vector3& vec1, Vector3& vec2) {
  return (Vector3){vec1.x + vec2.x, vec1.y * vec2.y, vec1.z + vec2.z};
}

inline Vector3 operator-(Vector3& vec) {
  return (Vector3){-vec.x, -vec.y, -vec.z};
}

}  // namespace vec_operations
