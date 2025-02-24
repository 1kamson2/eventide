#include "Globals.hpp"
enum class GAMES_STATES { ACTIVE = 0, MENU = 1, DEBUGGING = 2, EXIT = -1 };
enum class BLOCK_ID { YES = 0, NO = 1 };
enum class AGENT_ID { YES = 0, NO = 1 };
namespace camvars {
const double CAMERA_DEFAULT_POSITION_X = 0.0f;
const double CAMERA_DEFAULT_POSITION_Y = 2.0f;
const double CAMERA_DEFAULT_POSITION_Z = 4.0f;
const Vector3 CAMERA_DEFAULT_TARGET = (Vector3){0.0f, 2.0f, 0.0f};
const Vector3 CAMERA_DEFAULT_ROTATION = (Vector3){0.0f, 1.0f, 0.0f};
const double CAMERA_DEFAULT_FOV = 90.0f;
const double CAMERA_DEFAULT_PERSPECTIVE = CAMERA_PERSPECTIVE;
const double CAMERA_DEFAULT_MODE = CAMERA_FIRST_PERSON;
const double CAMERA_DEFAULT_X_FACTOR = 0.1f;
const double CAMERA_DEFAULT_ROTATE_FACTOR = 0.05f;
const double CAMERA_DEFAULT_MAX_SPEED = 1.0f;
}  // namespace camvars

namespace engvars {
const int MAX_OBJECTS_IN_AREA = 1 << 18;
const int CHUNK_SIZE = 16;
const int TICKRATE = 20;

/* RENDER_DISTANCE unit: chunks */
const int RENDER_DISTANCE = 16;
const double EDGE_LENGTH = 1.0f;
const double CURSOR_LENGTH = 0.1f;
}  // namespace engvars
