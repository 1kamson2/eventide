#pragma once
#include <raylib.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
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
  Rectangle rect;
  ItemType type;
  CanBlock blocking;
  Color color;
  Texture2D texture;
  // BE CAREFUL - WILL RESULT IN OVERFLOW, MORE CHECKING NEEDED.
  uint8_t quantity{1};
};

struct Player {
  float speedY{0.0f};
  float speedX{0.0f};
  bool canJump{false};
  bool canGoFaster{false};
  bool eastCollision{false};
  bool westCollision{false};
  Vector2 pos;
  Texture2D texture;
  EnvTile hitbox;
  EnvTile hotbar[9];
  EnvTile inventory[3][8];
  EnvTile health;
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
