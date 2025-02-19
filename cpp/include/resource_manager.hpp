#pragma once
#include <raylib.h>

#include <cassert>
#include <cmath>

enum class GAMES_STATES { ACTIVE = 0, MENU = 1, DEBUGGING = 2, EXIT = -1 };
enum class BLOCKING_ID { YES = 0, NO = 1 };
enum class IS_AGENT_IDENTIFIER { YES = 0, NO = 1 };
// enum class FACES;

class ResourceManager {
 public:
  static Shader shaderInit();  // not implemented
  static Shader getShaders();  // not implemented
  static Texture2D textureInit(const char* path, float width = 16,
                               float height = 16);
  static Texture getTextures();
  static void clear();
  ResourceManager() = delete;  // might cause problem? undo delete if so
};
