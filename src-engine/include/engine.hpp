#pragma once
#include <cmath>
#include <iostream>
#include <vector>

#include "raylib.h"
#include "resource_manager.hpp"
using ii = std::pair<int, int>;
#define TILE_SZ (float)16.0
#define RENDER_DISTANCE 32
#define SMALL_WORLD_START (float)-2048.0
#define SMALL_WORLD_END (float)2048.0
#define SMALL_WORLD_DEPTH 96
#define SMALL_WORLD_WIDTH 256
#define PLAYER_HORIZONTAL_SPEED TILE_SZ
#define PLAYER_VERTICAL_SPEED 3 * TILE_SZ

class EventideEngine {
 public:
  GameState State;
  Camera2D camera;
  Player* player;
  unsigned int width, height, fps;
  EnvTile tiles_buff[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE], tree_buff[1000];
  std::vector<Vector2> placedRecently, removedRecently;
  Texture2D grassTexture, stoneTexture, dirtTexture, oakTexture, pineTexture;
  ii mouseCoords, playerIndex, treeRDist;
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();

  void renderTiles();
  void loadTiles();

  void eventideInit(unsigned int seed);
  void updatePlayer(float dt);
  ii mouseWorldCoords();
  bool checkCollision(const EnvTile& tempET);
  void update(float dt);
  void render();
  void debug();
};
// add dirt / stone / grass entity
