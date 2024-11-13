#include "engine.hpp"

#include "environment.hpp"
#include "player.hpp"
#include "resource_manager.hpp"
Camera2D EventideEngine::camera;
GameState EventideEngine::State;
PlayerObject* EventideEngine::player;
EnvTile EventideEngine::tiles_buff[3 * RENDER_DISTANCE][3 * RENDER_DISTANCE],
    EventideEngine::tree_buff[1000];
std::unordered_map<ItemType, Texture2D> EventideEngine::envTextures;
Texture2D EventideEngine::bgTexture[12];

EventideEngine::EventideEngine(unsigned int width, unsigned int height,
                               unsigned int fps)
    : width{width}, height{height}, fps{fps} {
  // make another function to initialize player
  State = GameState::ACTIVE;
  player = new PlayerObject();
  envTextures = {
      {ItemType::GRASS, ResourceManager::textureInit(GRASS_TEXTURE)},
      {ItemType::STONE, ResourceManager::textureInit(STONE_TEXTURE)},
      {ItemType::DIRT, ResourceManager::textureInit(DIRT_TEXTURE)},
      {ItemType::OAK_TREE,
       ResourceManager::textureInit(OAK_TEXTURE_PATH, 120, 120)},
      {ItemType::PINE_TREE,
       ResourceManager::textureInit(PINE_TEXTURE_PATH, 120, 120)},
      {ItemType::COAL, ResourceManager::textureInit(COAL_TEXTURE_PATH)},
      {ItemType::IRON, ResourceManager::textureInit(IRON_TEXTURE_PATH)},
      {ItemType::DIAMOND, ResourceManager::textureInit(DIAMOND_TEXTURE_PATH)}};
  this->bgTexture[0] =
      ResourceManager::textureInit(LAYER0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[1] =
      ResourceManager::textureInit(LAYER1, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[2] =
      ResourceManager::textureInit(LAYER2, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[3] =
      ResourceManager::textureInit(LAYER3, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[4] =
      ResourceManager::textureInit(LAYER4, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[5] =
      ResourceManager::textureInit(LAYER5, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[6] =
      ResourceManager::textureInit(LAYER6, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[7] =
      ResourceManager::textureInit(LAYER7, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[8] =
      ResourceManager::textureInit(LAYER8, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[9] =
      ResourceManager::textureInit(LAYER9, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
  this->bgTexture[10] = ResourceManager::textureInit(LAYER10, BACKGROUND_WIDTH,
                                                     BACKGROUND_HEIGHT);
  this->bgTexture[11] = ResourceManager::textureInit(LAYER11, BACKGROUND_WIDTH,
                                                     BACKGROUND_HEIGHT);
  this->camera = {0};
  camera.target = (Vector2){player->pos.x + 20.0f, player->pos.y + 20.0f};
  camera.offset = (Vector2){this->width / 2.0f, this->height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;
}

void EventideEngine::eventideInit(unsigned int seed) {
  SetRandomSeed(seed);
  Environment::envInit();
  Environment::treesInit();
}
void EventideEngine::processInput(float dt) {
  if (IsKeyDown(KEY_RIGHT)) {
    if (!this->player->eastCollision) {
      this->player->lastPressed = KEY_RIGHT;
      player->pos.x += PLAYER_SPEED_X * dt;
    }
  } else if (IsKeyDown(KEY_LEFT)) {
    if (!this->player->westCollision) {
      this->player->lastPressed = KEY_LEFT;
      player->pos.x -= PLAYER_SPEED_X * dt;
    }
  } else if (IsKeyDown(KEY_DOWN) && this->player->canGoFaster) {
    this->player->lastPressed = KEY_DOWN;
    player->pos.y += PLAYER_SPEED_Y * dt;
  } else if (IsKeyDown(KEY_UP) && this->player->canJump) {
    this->player->lastPressed = KEY_UP;
    player->pos.y = -PLAYER_JUMP;
    player->canJump = false;
  } else if (IsKeyReleased(KEY_TAB)) {
    switch (this->State) {
      case GameState::ACTIVE:
        this->State = GameState::MENU;
        std::cout << "[DEBUG] Game state is MENU" << std::endl;
        break;
      case GameState::MENU:
        this->State = GameState::ACTIVE;
        std::cout << "[DEBUG] Game state is ACTIVE" << std::endl;
        break;
      default:
        this->State = GameState::EXIT;
        break;
    }
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    // bad cast is probable
    if (Environment::canModifyEnv()) {
      // add also if positions changed
      ii pos = Environment::findEnvPos();
      bool canDestroy = false;
      ItemType tempET = this->tiles_buff[pos.second][pos.first].type;

      if (tempET != ItemType::AIR) {
        if (tempET == ItemType::DIRT && this->countdown >= DIRT_DESTROY_TIME) {
          canDestroy = true;
        } else if (tempET == ItemType::GRASS &&
                   this->countdown >= GRASS_DESTROY_TIME) {
          canDestroy = true;
        } else if (tempET == ItemType::STONE &&
                   this->countdown >= STONE_DESTROY_TIME) {
          canDestroy = true;
        } else {
          countdown += dt;
        }
        if (canDestroy) {
          canDestroy = false;
          this->countdown = 0;
          Environment::tileUpdate(pos.first, pos.second, BLANK, ItemType::AIR,
                                  CanBlock::NO);
          std::cout << "Block destroyed. " << std::endl;
          if (this->removedRecently.size() >= 10) {
            this->removedRecently.clear();
          }
          this->removedRecently.push_back(
              (Vector2){(float)pos.first, (float)pos.second});
        }
      }
    }
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    if (Environment::canModifyEnv()) {
      ii pos = Environment::findEnvPos();
      if (this->tiles_buff[pos.second][pos.first].type == ItemType::AIR) {
        Environment::tileUpdate(pos.first, pos.second, BROWN, ItemType::DIRT,
                                CanBlock::YES);
        std::cout << "Block placed. " << std::endl;
        if (this->placedRecently.size() >= 10) {
          this->placedRecently.clear();
        }
        this->placedRecently.push_back(
            (Vector2){(float)pos.first, (float)pos.second});
      }
    }
  }
}

void EventideEngine::update(float dt) {
  this->processInput(dt);
  this->player->updatePlayerState(dt);
  // render related = engine
  this->treeRDist = ii(std::max((int)(this->pidx.first), 0),
                       std::min(3 * RENDER_DISTANCE, (int)(this->pidx.first)));
}

void EventideEngine::renderTiles() {
  std::cout << "Not implemented, this function will tell how to render tiles"
            << std::endl;
}
void EventideEngine::debug() {
  std::cout << "[DEBUG] SCREEN SIZE [W, H]: " << this->width << " "
            << this->height << std::endl;
  std::cout << "[DEBUG] FPS: " << GetFPS() << std::endl;
  std::cout << "[DEBUG] PLAYER POS [X, Y]: " << this->player->pos.x << " "
            << this->player->pos.y << std::endl;
  std::cout << "[DEBUG] MOUSE POS: " << this->mpos.first << " "
            << this->mpos.second << std::endl;
  std::cout << "[DEBUG] Recently placed blocks: " << std::endl;
  std::cout << "[DEBUG] ARRAY[y][x] REAL COORDS: [x, y] " << std::endl;
  for (const Vector2 el : this->placedRecently) {
    std::cout << "[" << el.x << ", " << el.y << "]";
    std::cout << " [" << el.x * TILE_SZ + SMALL_WORLD_START << ", "
              << el.y * TILE_SZ << "]" << std::endl;
  }
  std::cout << "[DEBUG] Recently removed blocks: " << std::endl;
  std::cout << "[DEBUG] ARRAY[y][x] REAL COORDS: [x, y] " << std::endl;
  for (const Vector2 el : this->removedRecently) {
    std::cout << "[" << el.x << ", " << el.y << "]";
    std::cout << " [" << el.x * TILE_SZ + SMALL_WORLD_START << ", "
              << el.y * TILE_SZ << "]" << std::endl;
  }
}

EventideEngine::~EventideEngine() {
  std::cout << "Eventide Engine: Cleaning up..." << std::endl;
  delete player;
  player = nullptr;
}
