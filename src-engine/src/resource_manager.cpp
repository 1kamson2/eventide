#include "resource_manager.hpp"

#include <raylib.h>

#include "engine.hpp"

Texture2D ResourceManager::textureInit(const char* path, bool isTile = false) {
  Image img = LoadImage(path);
  if (img.data == NULL) {
    std::cerr << "[WARNING] Image data is null! This error occurred for:"
              << std::endl;
    std::cerr << path << std::endl;
    exit(0);
  }
  if (isTile) {
    ImageResize(&img, TILE_SZ, TILE_SZ);
  } else {
    ImageResize(&img, 200, 200);
  }
  Texture2D texture = LoadTextureFromImage(img);
  UnloadImage(img);
  return texture;
}
