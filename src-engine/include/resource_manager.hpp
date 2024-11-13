#pragma once
#include <raylib.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using ii = std::pair<int, int>;
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
#define COAL_TEXTURE_PATH \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/coal-tile.png"
#define IRON_TEXTURE_PATH \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/iron-tile.png"
#define DIAMOND_TEXTURE_PATH \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/diamond-ore.png"
#define LAYER0 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0000_9.png"
#define LAYER1 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0001_8.png"
#define LAYER2 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0002_7.png"
#define LAYER3 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0003_6.png"
#define LAYER4                                              \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/" \
  "Layer_0004_Lights.png"
#define LAYER5 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0005_5.png"
#define LAYER6 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0006_4.png"
#define LAYER7                                              \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/" \
  "Layer_0007_Lights.png"
#define LAYER8 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0008_3.png"
#define LAYER9 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0009_2.png"
#define LAYER10 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0010_1.png"
#define LAYER11 \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/Layer_0011_0.png"
#define PLAYER_TEXTURE \
  "/home/kums0n-desktop/Dev/Eventide/src-engine/resources/crawler.png"
#define TILE_SZ (float)16.0
#define RENDER_DISTANCE 32
#define SMALL_WORLD_START (float)-2048.0
#define SMALL_WORLD_END (float)2048.0
#define SMALL_WORLD_DEPTH 96
#define SMALL_WORLD_WIDTH 256
#define PLAYER_SPEED_X 100.0f
#define PLAYER_SPEED_Y 300.0f
#define PLAYER_JUMP TILE_SZ
#define MAX_HEALTH_TEXT 100
#define INVENTORY_ROWS 8
#define INVENTORY_COLS 3
#define GRASS_DESTROY_TIME 120  // 120 frames
#define DIRT_DESTROY_TIME 120   // 120 frames
#define STONE_DESTROY_TIME 300  // 300 frames
#define BACKGROUND_WIDTH 256
#define BACKGROUND_HEIGHT 256
#define SCROLL_BACK 0.3f
#define SCROLL_MID 0.5f
#define SCROLL_FORE 1.0f
enum class GameState { ACTIVE = 1, MENU = 2, DEBUGGING = 3, EXIT = -1 };
enum class ItemType {
  AIR = 0,
  GRASS = 1,
  DIRT = 2,
  STONE = 3,
  OAK_TREE = 4,
  PINE_TREE = 5,
  COAL = 6,
  IRON = 7,
  DIAMOND = 8,
  PLAYER = 99
};
enum class CanBlock { YES = 1, NO = 0 };

struct EnvTile {
  Rectangle rect = (Rectangle){0, 0, 0, 0};
  ItemType type{ItemType::AIR};
  CanBlock blocking{CanBlock::NO};
  Color color{BLANK};
  Texture2D texture = (Texture2D){0};
  // BE CAREFUL - WILL RESULT IN OVERFLOW, MORE CHECKING NEEDED.
  uint8_t quantity{1};
};

class ResourceManager {
 public:
  static Shader shaderInit();  // not implemented
  static Shader getShaders();  // not implemented
  static Texture2D textureInit(const char* path, float width = 16,
                               float height = 16);
  static Texture getTextures();
  static EnvTile tileInit();
  static void clear();
  ResourceManager() = delete;  // might cause problem? undo delete if so
};
