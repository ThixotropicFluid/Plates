#ifndef PLATES_H
#define PLATES_H

#include "raylib.h"

typedef struct {
    int seed;
} TerrainGenerationSettings;

typedef struct {
    int width;
    int height;
    int image_count;
    Image* images;
    Texture2D* textures;
    float* height_map;
    TerrainGenerationSettings terrain_settings;
} TerrainData;

void GenerateTerrain(TerrainData* data);
void UnloadTerrain(TerrainData* data);

#endif