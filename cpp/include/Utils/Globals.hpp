#pragma once

#include <raylib.h>
#include <rcamera.h>

#include <memory>

#include "Chunk/Chunk.hpp"
#include "Voxel/Voxel.hpp"
enum class Action {
  MOVE_X_NORTH = 0,
  MOVE_X_SOUTH = 1,
  MOVE_X_EAST = 2,
  MOVE_X_WEST = 3,
  ROLL = 4,
  PROJECTION = 5,
  IDLE = 6,
  TRY_TO_DESTROY = 100,
  TRY_TO_CREATE = 101,
};

namespace voxel_definitions {
using VoxelNode = std::shared_ptr<Voxel>;
constexpr float LENGTH = 1.0f;

}  // namespace voxel_definitions

namespace chunk_definitions {
using ChunkNode = std::shared_ptr<Chunk>;
constexpr float SIZE = 16.0f;
}  // namespace chunk_definitions

namespace cam_definitions {
constexpr Vector3 DEFAULT_TGT = (Vector3){0.0f, 2.0f, 0.0f};
constexpr Vector3 DEFAULT_ROT = (Vector3){0.0f, 1.0f, 0.0f};
constexpr float DEFAULT_FOV = 60.0f;
constexpr float DEFAULT_PROJ = CAMERA_PERSPECTIVE;
constexpr int DEFAULT_MODE = CAMERA_FIRST_PERSON;
}  // namespace cam_definitions
