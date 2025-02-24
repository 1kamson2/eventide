#pragma once
#include <raylib.h>

#include <iostream>

#include "Voxels.hpp"
/*using Voxel = EnvironmentObject;*/
/*using VoxelNode = EnvironmentNode<Voxel>;*/
/**/
namespace engvars {
extern const int MAX_OBJECTS_IN_AREA;
extern const int CHUNK_SIZE;
extern const int TICKRATE;
/* RENDER_DISTANCE unit: chunks */
extern const int RENDER_DISTANCE;
extern const double EDGE_LENGTH;
extern const double CURSOR_LENGTH;
}  // namespace engvars

enum class EnvironmentState {
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

namespace camvars {
/* This namespace includes all variables used for camera */
extern const double CAMERA_DEFAULT_POSITION_X;
extern const double CAMERA_DEFAULT_POSITION_Y;
extern const double CAMERA_DEFAULT_POSITION_Z;
extern const Vector3 CAMERA_DEFAULT_TARGET;
extern const Vector3 CAMERA_DEFAULT_ROTATION;
extern const double CAMERA_DEFAULT_FOV;
extern const double CAMERA_DEFAULT_PERSPECTIVE;
extern const double CAMERA_DEFAULT_MODE;
extern const double CAMERA_DEFAULT_X_FACTOR;
extern const double CAMERA_DEFAULT_ROTATE_FACTOR;
extern const double CAMERA_DEFAULT_MAX_SPEED;
}  // namespace camvars
