#include "engine.hpp"

EventideEngine::EventideEngine(unsigned int width, unsigned int height,
                               unsigned int fps)
    : width{width},
      height{height},
      fps{fps},
      player{new PlayerObject},
      wseed{new WorldSeed},
      State{GameState::GAME_ACTIVE} {
  this->camera = {0};
  camera.target = (Vector2){player->hitbox.x + 20.0f, player->hitbox.y + 20.0f};
  camera.offset = (Vector2){this->width / 2.0f, this->height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

void EventideEngine::eventideInit(unsigned int seed) {
  SetRandomSeed(seed);
  for (unsigned int chunk = 0; chunk < CHUNK_LEN; ++chunk) {
    std::cout << "Generating chunk: " << chunk << std::endl;
    for (unsigned int y = 0; y < CHUNK_HEIGHT; ++y) {
      for (unsigned int x = 0; x < CHUNK_WIDTH; ++x) {
        unsigned int idx = chunk * STRIDE + y * CHUNK_WIDTH + x;

        this->tiles[idx] = (Rectangle){
            (float)(x + chunk * CHUNK_WIDTH) * TILE_SZ,
            height - 6 * TILE_SZ + (float)y * TILE_SZ, TILE_SZ, TILE_SZ};
      }
    }
  }
  int idx = 0;
  while (idx < 1000) {
    // real position is -6000 + idx * TILE_SZ
    idx += GetRandomValue(0, TILE_SZ);

    wseed->trees[idx].width = (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    wseed->trees[idx].height = (float)GetRandomValue(5 * TILE_SZ, 10 * TILE_SZ);
    wseed->trees[idx].y = height + 2 * TILE_SZ -
                          wseed->trees[idx].height;  // <-- -2 if doesnt work
    wseed->trees[idx].x = -6000.0f + idx * TILE_SZ;
    wseed->treeColors[idx] =
        (Color){(unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200), 255};
    // add atleast 2 * TILE_SZ to next
    idx += 2 * TILE_SZ;
  }
}

void EventideEngine::processInput(float dt) {
  if (IsKeyDown(KEY_RIGHT)) {
    player->hitbox.x += 100;
  } else if (IsKeyDown(KEY_LEFT)) {
    player->hitbox.x -= 100;

  } else if (IsKeyDown(KEY_DOWN)) {
    player->hitbox.y += 100;
  } else if (IsKeyDown(KEY_UP)) {
    player->hitbox.y -= 100;
  }
}
void EventideEngine::update(float dt) {
  camera.target = (Vector2){player->hitbox.x + 20, player->hitbox.y + 20};
}

ii EventideEngine::renderDistance() {
  // cap to i > 0 and i < 1000, -6000 is start of the world, 1000 because a 1000
  // trees

  int currIdx = floor((player->hitbox.x + 6000) / TILE_SZ);
  return ii(std::max((int)(currIdx - RENDER_DISTANCE), 0),
            std::min(1000, (int)(currIdx + RENDER_DISTANCE)));
}

EventideEngine::~EventideEngine() {
  delete player;
  delete wseed;
  player = nullptr;
  wseed = nullptr;
}
