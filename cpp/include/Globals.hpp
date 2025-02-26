#pragma once
#include <raylib.h>

#include <cstdint>
#include <iostream>

namespace engvars {
/* Return boundaries <TopLeftFront, BottomRightBack> */
using Boundaries = std::tuple<Vector3, Vector3>;
extern const int MAX_OBJECTS_IN_AREA;
extern const int CHUNK_SIZE;
extern const int TICKRATE;
/* RENDER_DISTANCE unit: chunks */
extern const int RENDER_DISTANCE;
extern const double EDGE_LENGTH;
extern const double CURSOR_LENGTH;
extern const int RED_COLOR_MASK;
extern const int GREEN_COLOR_MASK;
extern const int BLUE_COLOR_MASK;
extern const int ALPHA;
extern const int X_COORD_MASK;
extern const int Y_COORD_MASK;
extern const int Z_COORD_MASK;
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
enum class GAMES_STATES { ACTIVE = 0, MENU = 1, DEBUGGING = 2, EXIT = -1 };
enum class BLOCK_ID { YES = 0, NO = 1 };
enum class AGENT_ID { YES = 0, NO = 1 };

namespace octree {
extern const uint32_t BOTTOM_LEFT_FRONT;
extern const uint32_t BOTTOM_RIGHT_FRONT;
extern const uint32_t BOTTOM_LEFT_BACK;
extern const uint32_t BOTTOM_RIGHT_BACK;
extern const uint32_t TOP_LEFT_FRONT;
extern const uint32_t TOP_RIGHT_FRONT;
extern const uint32_t TOP_LEFT_BACK;
extern const uint32_t TOP_RIGHT_BACK;
}  // namespace octree
