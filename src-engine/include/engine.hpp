#pragma once
#include <cmath>
#include <fstream>
#include <iostream>

#include "raylib.h"
using ii = std::pair<int, int>;
using pis = std::pair<int, std::string>;
#define CHUNK_SZ 16
#define SMALL_WORLD_STRIDE 4096
#define TILE_SZ 16
#define RENDER_DISTANCE 32

#define SMALL_WORLD_START -2048
#define SMALL_WORLD_END 2048
#define SMALL_WORLD_DEPTH 96
#define SMALL_WORLD_WIDTH 256  // <-- ignore
// #define SMALL_WORLD_DEPTH 12   // <-- ignore
#define AIR 0
#define GRASS 1
#define DIRT 2
#define STONE 3

enum class GameState { GAME_ACTIVE = 1, GAME_MENU = 0, GAME_WIN = 2 };
struct PlayerObject {
  Rectangle hitbox{SMALL_WORLD_START + 3 * TILE_SZ, -80, 40, 40};
};

struct WorldSeed {
  Rectangle trees[1000] = {0};  // temp
  Color treeColors[1000] = {0};
};

struct Tile {
  Rectangle tile{0, 0, TILE_SZ, TILE_SZ};
  Color col{0, 0, 0};
};

class EventideEngine {
 public:
  GameState State;
  Camera2D camera;
  PlayerObject* player;
  WorldSeed* wseed;
  unsigned int width, height, fps;
  Tile tiles_buf[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE];
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();

  ii getPlayerIndex() const;
  ii renderTrees() const;
  void renderTiles();

  void eventideInit(unsigned int seed);
  void processInput(float dt);
  void update(float dt);
  void render();
};
