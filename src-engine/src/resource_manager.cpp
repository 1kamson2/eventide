#include "resource_manager.hpp"

#include <raylib.h>

#include "engine.hpp"

Texture2D ResourceManager::textureInit(const char* path, bool isTile = false) {
  Image img = LoadImage(path);
  if (img.data == NULL) {
    std::cerr << "Invalid image, image data is null." << std::endl;
    exit(0);
  }
  if (isTile) {
    ImageResize(&img, TILE_SZ, TILE_SZ);
  }
  Texture2D texture = LoadTextureFromImage(img);
  UnloadImage(img);
  return texture;
}
