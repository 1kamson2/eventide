#pragma once
#include <cmath>
#include <fstream>
#include <iostream>

#include "raylib.h"
using ii = std::pair<int, int>;
#define CHUNK_SZ 16
#define SMALL_WORLD_STRIDE 1536
#define TILE_SZ 16
#define RENDER_DISTANCE 32

#define SMALL_WORLD_START -2048
#define SMALL_WORLD_END 2048
#define SMALL_WORLD_WIDTH 256
#define SMALL_WORLD_DEPTH 12  // in chunks
#define AIR 0
#define GRASS 1
#define DIRT 2
#define STONE 3

enum class GameState { GAME_ACTIVE = 1, GAME_MENU = 0, GAME_WIN = 2 };
struct PlayerObject {
  Rectangle hitbox{0, -40, 40, 40};
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
  Tile tiles_buf[RENDER_DISTANCE * 4];
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();

  int getPlayerIndex() const;
  ii renderTrees() const;
  ii renderTiles() const;

  void eventideInit(unsigned int seed);
  void processInput(float dt);
  void update(float dt);
  void render();
};
