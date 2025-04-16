#pragma once

namespace biomes {
#include <raylib.h>
constexpr Color WATER = {0, 0, 255, 255};
constexpr Color GRASS = {11, 102, 34, 255};
constexpr Color MOUNTAIN = {160, 160, 160, 255};
constexpr Color SAND = {255, 255, 153, 255};
constexpr Color FOREST = {34, 139, 34, 255};
constexpr Color SNOW = {204, 255, 255, 255};
constexpr Color DIRT = {139, 69, 19, 255};
constexpr Color DESERT = {163, 135, 34, 255};

constexpr float WATER_ELEVATION = 0.1f;
constexpr float DIRT_ELEVATION = 0.2f;
constexpr float GRASS_ELEVATION = 0.3f;
constexpr float FOREST_ELEVATION = 0.5f;
constexpr float MOUNTAIN_ELEVATION = 0.7f;
constexpr float SNOW_ELEVATION = 0.8f;
constexpr float DESERT_ELEVATION = 0.9f;
constexpr float AIR_ELEVATION = 0.1f;
constexpr float ELEVATION_NOT_IMPORTANT = -2147483647;

bool Water(const float& elevation, const float& y_min, const float& y_max,
           const float& y);

bool Dirt(const float& elevation, const float& y_min, const float& y_max,
          const float& y);

bool Grass(const float& elevation, const float& y_min, const float& y_max,
           const float& y);

bool Forest(const float& elevation, const float& y_min, const float& y_max,
            const float& y);

bool Mountain(const float& elevation, const float& y_min, const float& y_max,
              const float& y);

bool Snow(const float& elevation, const float& y_min, const float& y_max,
          const float& y);

bool Desert(const float& elevation, const float& y_min, const float& y_max,
            const float& y);

bool Air(const float& elevation, const float& y_min, const float& y_max,
         const float& y);
}  // namespace biomes

inline bool biomes::Water(const float& elevation, const float& y_min,
                          const float& y_max, const float& y) {
  return elevation <= biomes::WATER_ELEVATION && y >= y_min && y <= y_max;
}

inline bool biomes::Dirt(const float& elevation, const float& y_min,
                         const float& y_max, const float& y) {
  return elevation <= biomes::DIRT_ELEVATION && y >= y_min && y <= y_max;
}
inline bool biomes::Grass(const float& elevation, const float& y_min,
                          const float& y_max, const float& y) {
  return elevation <= biomes::GRASS_ELEVATION && y >= y_min && y <= y_max;
}

inline bool biomes::Forest(const float& elevation, const float& y_min,
                           const float& y_max, const float& y) {
  return elevation <= biomes::FOREST_ELEVATION && y >= y_min && y <= y_max;
}

inline bool biomes::Mountain(const float& elevation, const float& y_min,
                             const float& y_max, const float& y) {
  return elevation <= biomes::MOUNTAIN_ELEVATION && y >= y_min && y <= y_max;
}
inline bool biomes::Snow(const float& elevation, const float& y_min,
                         const float& y_max, const float& y) {
  return elevation <= biomes::SNOW_ELEVATION && y >= y_min && y <= y_max;
}
inline bool biomes::Desert(const float& elevation, const float& y_min,
                           const float& y_max, const float& y) {
  return elevation <= biomes::DESERT_ELEVATION && y >= y_min && y <= y_max;
}
inline bool biomes::Air(const float& elevation, const float& y_min,
                        const float& y_max, const float& y) {
  return elevation <= biomes::AIR_ELEVATION && y >= y_min && y <= y_max;
}
