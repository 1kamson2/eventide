#include "resource_manager.hpp"

#include <raylib.h>

Texture2D ResourceManager::textureInit(const char* path, float width,
                                       float height) {
  Image img = LoadImage(path);
  if (img.data == NULL) {
    std::cerr << "[WARNING] Image data is null! This error occurred for:"
              << std::endl;
    std::cerr << path << std::endl;
    exit(0);
  }
  std::cout << " " << path << std::endl;
  printf(
      "[Image info]:\nLoading Image: %s\nWIDTH: %.3f\nHEIGHT: %.3f\nASPECT "
      "RATIO: %.3f",
      path, width, height, width / height);
  ImageResize(&img, width, height);
  Texture2D texture = LoadTextureFromImage(img);
  UnloadImage(img);
  return texture;
}
