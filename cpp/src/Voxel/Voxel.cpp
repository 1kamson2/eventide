#include "Voxel/Voxel.hpp"

Voxel::Voxel(Vector3 position, Color col) : position(position), color(col) {}

float Voxel::GetAxis(const int& dim) const {
  float axis_pos;
  switch (dim) {
    case 0:
      axis_pos = position.x;
      break;
    case 1:
      axis_pos = position.y;
      break;
    case 2:
      axis_pos = position.z;
      break;
  }
  return axis_pos;
}

Vector3 Voxel::GetPosition() const { return position; }

bool Voxel::IsBlank() {
  Color col2 = BLANK;
  return color.a == col2.a && color.b == col2.b && color.g == col2.g &&
         color.r == col2.r;
}
