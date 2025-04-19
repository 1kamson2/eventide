#include "Voxel/Voxel.hpp"

using namespace voxel;
Voxel::Voxel(Vector3 position, Color col) : position(position), color(col) {
  center = (Vector3){position.x + LENGTH / 2, position.y - LENGTH / 2,
                     position.z + LENGTH / 2};
  extents =
      (Vector3){position.x + LENGTH, position.y - LENGTH, position.z + LENGTH};
}

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

Vector3 Voxel::GetMidPoint() const { return center; }

Vector3 Voxel::GetExtentsPoint() const { return center; }

Vector3 Voxel::GetPosition() const { return position; }

bool Voxel::IsBlank() {
  Color col2 = BLANK;
  return color.a == col2.a && color.b == col2.b && color.g == col2.g &&
         color.r == col2.r;
}
