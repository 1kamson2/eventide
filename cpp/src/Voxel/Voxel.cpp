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
std::shared_ptr<Voxel>& Voxel::FindVoxelOnY(
    float& y, std::shared_ptr<Voxel>& curr_voxel) {
  // begin from root
  std::shared_ptr<Voxel> null = nullptr;
  if (curr_voxel == nullptr) {
    return null;
  }

  if (curr_voxel->GetPosition().y == y) {
    return curr_voxel;
  }

  std::shared_ptr<Voxel> left_vox = FindVoxelOnY(y, curr_voxel->left);
  if (left_vox) {
    return left_vox;
  }
  std::shared_ptr<Voxel> right_vox = FindVoxelOnY(y, curr_voxel->right);

  if (right_vox) {
    return right_vox;
  }

  return null;
}
