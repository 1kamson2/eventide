#pragma once
#include <string>
#include <unordered_map>

#include "raylib.h"

class ResourceManager {
 public:
  static std::unordered_map<std::string, Shader>
      shaders;  // <-- second type is a placeholder
  static std::unordered_map<std::string, Texture>
      textures;                  // <-- second type is a placeholder
  static Shader loadShader();    // <-- placeholder
  static Shader getShader();     // <-- placeholder
  static Texture loadTexture();  // <-- placeholder
  static Texture getTexture();   // <-- placeholder
 private:
  ResourceManager() {}
  static Shader loadShaderFromFile();
  static Texture loadTextureFromFile();
};
