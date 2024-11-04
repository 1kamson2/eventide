#pragma once
#include <cassert>
#include <string>
#include <unordered_map>

#include "raylib.h"
enum class GameState { GAME_ACTIVE = 1, GAME_MENU = 0, GAME_WIN = 2 };
enum class ItemType { AIR = 0, GRASS = 1, DIRT = 2, STONE = 3, PLAYER = 99 };
enum class CanBlock { YES = 1, NO = 0 };

struct EnvTile {
  Rectangle rect;
  ItemType type;
  CanBlock blocking;
  Color color;
  Texture2D texture;
};

struct Player {
  Vector2 pos;
  float speed{0};
  bool canJump{0};
  bool canGoFaster{0};
  EnvTile hitbox;
  Texture2D texture;
};

class ResourceManager {
 public:
  static std::unordered_map<std::string, Shader> shaders;
  static std::unordered_map<std::string, Texture> textures;
  static Shader shaderInit();  // not implemented
  static Shader getShaders();  // not implemented
  static void textureInit();
  static Texture getTextures();
  static EnvTile tileInit();
  static Player playerInit();
  static void Clear();
  ResourceManager() = delete;  // might cause problem? undo delete if so
};
