#pragma once
#include <cmath>

#include "raylib.h"
#include "resource_manager.hpp"
using ii = std::pair<int, int>;
#define TILE_SZ 16
#define RENDER_DISTANCE 32
#define SMALL_WORLD_START -2048
#define SMALL_WORLD_END 2048
#define SMALL_WORLD_DEPTH 96
#define SMALL_WORLD_WIDTH 256
#define PLAYER_HORIZONTAL_SPEED 250
#define PLAYER_VERTICAL_SPEED 250

class EventideEngine {
 public:
  GameState State;
  Camera2D camera;
  Player* player;
  unsigned int width, height, fps;
  EnvTile tiles_buff[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE];
  EnvTile tree_buff[1000];
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();

  ii getPlayerIndex() const;
  ii renderTrees() const;
  void renderTiles();

  void eventideInit(unsigned int seed);
  void updatePlayer(float dt);
  ii mouseWorldCoords();
  bool checkCollision(const EnvTile& tempET);
  void update(float dt);
  void render();
  void debug();
};
