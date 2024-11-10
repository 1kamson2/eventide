#pragma once
#include <cmath>
#include <iostream>
#include <unordered_map>
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
#define PLAYER_SPEED_X 100.0f
#define PLAYER_SPEED_Y 300.0f
#define PLAYER_JUMP TILE_SZ
#define MAX_HEALTH_TEXT 100
#define INVENTORY_ROWS 8
#define INVENTORY_COLS 3
#define GRASS_DESTROY_TIME 120  // 120 frames
#define DIRT_DESTROY_TIME 120   // 120 frames
#define STONE_DESTROY_TIME 300  // 300 frames

class EventideEngine {
 public:
  GameState State;
  Camera2D camera;
  Player* player;
  unsigned int width, height, fps;
  EnvTile tiles_buff[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE], tree_buff[1000];
  std::vector<Vector2> placedRecently, removedRecently;
  std::unordered_map<ItemType, Texture2D> envTextures;
  Texture2D bgTexture[12];  // all layers
  ii mouseCoords, playerIndex, treeRDist;
  float countdown;
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();

  void renderTiles();
  void loadTiles();
  // init events
  void eventideInit(unsigned int seed);
  void updatePlayer(float dt);
  void processInput(float dt);
  ii mouseWorldCoords();
  ii findEnvPositions();
  void tileInit(const int& x, const int& y, const float& depth,
                const Color& color, const ItemType& it, const CanBlock& cb);
  void treeInit(const int& pos, const Color& color, const CanBlock& cb);
  void tileUpdate(const int& x, const int& y, const Color& color,
                  const ItemType& it, const CanBlock& cb);
  // check conditions
  bool checkCollision(const EnvTile& tempET);
  bool canModifyEnv();
  // do updates
  void update(float dt);
  void render();
  void debug();
};
// add dirt / stone / grass entity
