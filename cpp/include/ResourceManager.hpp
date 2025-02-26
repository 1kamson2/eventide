#pragma once
#include <raylib.h>

#include <cassert>
#include <cmath>

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
