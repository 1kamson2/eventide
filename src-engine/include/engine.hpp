#pragma once
#include <cmath>

#include "raylib.h"
using ii = std::pair<int, int>;
#define CHUNK_SZ 16
#define SMALL_WORLD_STRIDE 4096
#define TILE_SZ 16
#define RENDER_DISTANCE 32

#define SMALL_WORLD_START -2048
#define SMALL_WORLD_END 2048
#define SMALL_WORLD_DEPTH 96
#define SMALL_WORLD_WIDTH 256  // <-- ignore
#define PLAYER_HORIZONTAL_SPEED 250
#define PLAYER_VERTICAL_SPEED 1000
enum class GameState { GAME_ACTIVE = 1, GAME_MENU = 0, GAME_WIN = 2 };
enum class ItemType { AIR = 0, GRASS = 1, DIRT = 2, STONE = 3, PLAYER = 99 };
enum class CanBlock { YES = 1, NO = 0 };

struct EnvTile {
  Rectangle rect;
  ItemType type;
  CanBlock blocking;
  Color color;
};

struct Player {
  Vector2 pos;
  float speed{0};
  bool canJump{0};
  bool canGoFaster{0};
  EnvTile hitbox;
};

struct WorldSeed {
  Rectangle trees[1000] = {0};  // temp
  Color treeColors[1000] = {0};
};

class EventideEngine {
 public:
  GameState State;
  Camera2D camera;
  Player* player;
  WorldSeed* wseed;
  unsigned int width, height, fps;
  EnvTile tiles_buf[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE];
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();

  ii getPlayerIndex() const;
  ii renderTrees() const;
  void renderTiles();

  void eventideInit(unsigned int seed);
  void updatePlayer(float dt);
  bool checkCollision(const EnvTile& tempET);
  void update(float dt);
  void render();
};
