#pragma once
#include <raylib.h>

#include "environment.hpp"
#include "player.hpp"
#include "resource_manager.hpp"
// shoudlnt be hardcoded all of this
class EventideEngine {
 public:
  // get current state of a game
  static GameState State;
  unsigned int width, height, fps;
  static Camera2D camera;
  static PlayerObject* player;
  // move it to the environment, this will be based on where we are
  // probably good idea to move this buffer to another class, because
  // we want to render more terrain, when infinite generation introduced
  // engine will be wrapper and will check if we want to do something illegal
  // don't want so much code cluttered
  static EnvTile tiles_buff[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE],
      tree_buff[1000];
  std::vector<Vector2> placedRecently, removedRecently;
  static std::unordered_map<ItemType, Texture2D> envTextures;
  static Texture2D bgTexture[12];  // all layers
  // up to this point ^
  // mouse position, player index, tree rendering distance
  ii mpos, pidx, treeRDist;
  float countdown;
  EventideEngine(unsigned int width, unsigned int height, unsigned int fps);
  virtual ~EventideEngine();
  // engine's method
  void renderTiles();
  void loadTiles();
  // init events
  void eventideInit(unsigned int seed);
  void processInput(float dt);
  // do updates
  void update(float dt);
  void render();
  void debug();
};
// add dirt / stone / grass entity
