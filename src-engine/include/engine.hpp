#pragma once
#include <cmath>
#include <iostream>

#include "raylib.h"
#define CHUNK_LEN 100
#define CHUNK_HEIGHT 16
#define CHUNK_WIDTH 16
#define STRIDE 255
#define TILE_SZ 16
#define RENDER_DISTANCE 32

using ii = std::pair<int, int>;
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
  Tile tiles[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LEN];
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
