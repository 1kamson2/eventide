#include "engine.hpp"

#include <raylib.h>

#include <iostream>

EventideEngine::EventideEngine(unsigned int width, unsigned int height,
                               unsigned int fps)
    : width{width},
      height{height},
      fps{fps},
      player{new Player},
      State{GameState::GAME_ACTIVE} {
  player->pos = (Vector2){SMALL_WORLD_START + 100, -200};
  player->speed = 0;
  player->canJump = false;
  player->hitbox.rect =
      (Rectangle){player->pos.x, player->pos.y, TILE_SZ, 3 * TILE_SZ};
  player->hitbox.type = ItemType::PLAYER;
  player->hitbox.blocking = CanBlock::YES;
  player->hitbox.color = (Color){RED};
  this->camera = {0};
  camera.target = (Vector2){player->pos.x + 20.0f, player->pos.y + 20.0f};
  camera.offset = (Vector2){this->width / 2.0f, this->height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;
}

void EventideEngine::eventideInit(unsigned int seed) {
  SetRandomSeed(seed);
  int depth{0};
  // SMALL_WORLD_DEPTH
  // SMALL_WORLD_STRIDE
  for (int y = 0; y < 3 * RENDER_DISTANCE; ++y) {
    depth = y * TILE_SZ;
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      if (depth == 0) {
        // GRASS ENUM
        this->tiles_buff[y][x].rect =
            (Rectangle){(float)SMALL_WORLD_START + x * TILE_SZ, (float)depth,
                        TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].color = (Color){GREEN};
        this->tiles_buff[y][x].type = ItemType::GRASS;
        this->tiles_buff[y][x].blocking = CanBlock::YES;
      } else if (depth > 0 && depth <= 60) {
        // DIRT ENUM
        this->tiles_buff[y][x].rect =
            (Rectangle){(float)SMALL_WORLD_START + x * TILE_SZ, (float)depth,
                        TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].color = (Color){BROWN};
        this->tiles_buff[y][x].type = ItemType::DIRT;
        this->tiles_buff[y][x].blocking = CanBlock::YES;
      } else if (depth > 60) {
        int isStone = GetRandomValue(0, 100);
        this->tiles_buff[y][x].rect =
            (Rectangle){(float)SMALL_WORLD_START + x * TILE_SZ, (float)depth,
                        TILE_SZ, TILE_SZ};
        this->tiles_buff[y][x].blocking = CanBlock::YES;
        if (isStone >= 0 && isStone <= 77) {
          this->tiles_buff[y][x].color = (Color){GRAY};
          this->tiles_buff[y][x].type = ItemType::STONE;
        } else if (isStone > 77 && isStone <= 100) {
          this->tiles_buff[y][x].color = (Color){BROWN};
          this->tiles_buff[y][x].type = ItemType::GRASS;
        }
      }
    }
  }
  int pos = 0;
  while (pos < 1000) {
    pos += GetRandomValue(0, TILE_SZ);
    tree_buff[pos].rect.width = (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    tree_buff[pos].rect.height =
        (float)GetRandomValue(5 * TILE_SZ, 15 * TILE_SZ);
    tree_buff[pos].rect.y =
        -100.0f + (float)GetRandomValue(TILE_SZ, 2 * TILE_SZ);
    tree_buff[pos].rect.x = SMALL_WORLD_START + pos * TILE_SZ;
    tree_buff[pos].color =
        (Color){(unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200), 255};
    pos += 2 * TILE_SZ;
  }
}

bool EventideEngine::checkCollision(const EnvTile& tempET) {
  bool collisionX =
      player->pos.x + player->hitbox.rect.width >= tempET.rect.x &&
      tempET.rect.x + tempET.rect.width >= player->pos.x;
  bool collisionY =
      player->pos.y + player->hitbox.rect.height >= tempET.rect.y &&
      tempET.rect.y + tempET.rect.height >= player->pos.y;
  return collisionX && collisionY;
}

ii EventideEngine::mouseWorldCoords() {
  // mouse_world_coords = player position + mouse_pos_on_screen + WORLD_OFFSET
  int mosX = GetMouseX();
  int mosY = GetMouseY();
  return ii(this->player->pos.x + mosX - 300, this->player->pos.y + mosY - 285);
}

void EventideEngine::updatePlayer(float dt) {
  if (IsKeyDown(KEY_RIGHT)) {
    player->pos.x += PLAYER_HORIZONTAL_SPEED * dt;
  } else if (IsKeyDown(KEY_LEFT)) {
    player->pos.x -= PLAYER_HORIZONTAL_SPEED * dt;
  } else if (IsKeyDown(KEY_DOWN) && this->player->canGoFaster) {
    player->pos.y += PLAYER_VERTICAL_SPEED * dt;
  } else if (IsKeyDown(KEY_UP) && this->player->canJump) {
    player->pos.y -= PLAYER_VERTICAL_SPEED * dt;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    // bad cast is probable
    ii mosPos = this->mouseWorldCoords();
    if (mosPos.first - this->player->pos.x < 5 * TILE_SZ ||
        mosPos.first - this->player->pos.y > -5 * TILE_SZ) {
      int arrX = std::max(0, (SMALL_WORLD_END + mosPos.first) / TILE_SZ);
      int arrY = std::max(0, (45 + mosPos.second) / TILE_SZ);
      this->tiles_buff[arrY][arrX].type = ItemType::AIR;
      this->tiles_buff[arrY][arrX].blocking = CanBlock::NO;
      this->tiles_buff[arrY][arrX].color = (Color){WHITE};
    }
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    ii mosPos = this->mouseWorldCoords();
    if (mosPos.first - this->player->pos.x < 5 * TILE_SZ ||
        mosPos.first - this->player->pos.y > -5 * TILE_SZ) {
      int arrX = std::max(0, (SMALL_WORLD_END + mosPos.first) / TILE_SZ);
      int arrY = std::max(0, (45 + mosPos.second) / TILE_SZ);
      this->tiles_buff[arrY][arrX].type = ItemType::DIRT;
      this->tiles_buff[arrY][arrX].blocking = CanBlock::YES;
      this->tiles_buff[arrY][arrX].color = (Color){BROWN};
    }
  }
  player->hitbox.rect.y = player->pos.y;
  player->hitbox.rect.x = player->pos.x;
  camera.target = (Vector2){player->pos.x + player->hitbox.rect.width / 2,
                            player->pos.y + player->hitbox.rect.height / 2};
  bool hitObstacle = false;
  for (int y = 0; y < 3 * RENDER_DISTANCE; y++) {
    for (int x = 0; x < 3 * RENDER_DISTANCE; ++x) {
      EnvTile tempET = this->tiles_buff[y][x];
      Vector2 playerPos = player->pos;
      if (tempET.blocking == CanBlock::YES && this->checkCollision(tempET)) {
        std::cout << "Collision handled." << std::endl;
        hitObstacle = true;
        player->speed = 0.0f;
        playerPos.y = tempET.rect.y;
        break;
      }
    }
  }
  // jumping works weird...
  if (!hitObstacle) {
    player->speed += PLAYER_VERTICAL_SPEED * dt;
    player->pos.y += PLAYER_HORIZONTAL_SPEED * dt;
    player->canJump = false;
    player->canGoFaster = true;
  } else {
    player->canJump = true;
    player->canGoFaster = false;
  }
}

void EventideEngine::update(float dt) {
  this->updatePlayer(dt);
  // this->renderTiles();
}
ii EventideEngine::getPlayerIndex() const {
  return ii(ceil((player->pos.x - SMALL_WORLD_START) / TILE_SZ),
            ceil(player->pos.y / TILE_SZ));
}
ii EventideEngine::renderTrees() const {
  int currIdx = this->getPlayerIndex().first;
  return ii(std::max((int)(currIdx - RENDER_DISTANCE / 2), 0),
            std::min(1000, (int)(currIdx + RENDER_DISTANCE / 2)));
}

void EventideEngine::renderTiles() { std::cout << "Do nothing" << std::endl; }
void EventideEngine::debug() {
  std::cout << "[DEBUG] SCREEN SIZE [W, H]: " << this->width << " "
            << this->height << std::endl;
  std::cout << "[DEBUG] FPS: " << GetFPS() << std::endl;
  std::cout << "[DEBUG] PLAYER POS [X, Y]: " << this->player->pos.x << " "
            << this->player->pos.y << std::endl;
}

EventideEngine::~EventideEngine() {
  delete player;
  player = nullptr;
}

// put getPlayerIndex into update function, and as class attribute
