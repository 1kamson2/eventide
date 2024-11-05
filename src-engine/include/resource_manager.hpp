#pragma once
#include <cassert>
#include <iostream>

#include "raylib.h"
#define GRASS_TEXTURE \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/grass-tile.png"
#define STONE_TEXTURE \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/stone-tile.png"
#define DIRT_TEXTURE \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/dirt-tile.png"
#define PINE_TEXTURE_PATH \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/tree-1.png"
#define OAK_TEXTURE_PATH \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/tree-2.png"
enum class GameState {
  GAME_ACTIVE = 1,
  GAME_MENU = 0,
  GAME_WIN = 2,
  DEBUGGING = 3
};
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
  static Shader shaderInit();  // not implemented
  static Shader getShaders();  // not implemented
  static Texture2D textureInit(const char* path, bool isTile);
  static Texture getTextures();
  static EnvTile tileInit();
  static Player playerInit();
  static void Clear();
  ResourceManager() = delete;  // might cause problem? undo delete if so
};
