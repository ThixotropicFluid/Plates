

#include "plates.h"
#include "raylib.h"
#include "stdlib.h"
#include "perlin.h"
#include "stdio.h"

void GenerateTerrain(TerrainData* data) {
    // alloc memory
    data->height_map = MemAlloc(sizeof(float) * (data->width + 1) * (data->height + 1));
    data->image_count = 1; // currenly no texture data, 
    data->images = MemAlloc(sizeof(Image) * data->image_count);
    data->textures = MemAlloc(sizeof(Texture2D) * data->image_count);


    setPerlinSeed(04);
    for(int i = 0; i < data->width + 1; i ++) {
        for(int j = 0; j < data->height + 1; j++) {
            float res = (perlin((float)i/10, (float)j/10, 0.5)- 0.5f);
            data->height_map[i * (data->width + 1) + j] = res;
            //data->height_map[i * (data->width + 1) + j] = 0;
        }
        
    }

    data->images[0] = GenImageChecked(data->width, data->height, 1, 1, PINK, ORANGE);
    data->textures[0] = LoadTextureFromImage(data->images[0]);

}

void UnloadTerrain(TerrainData* data) {
    MemFree(data->height_map);
    for(int i = 0; i < data->image_count; i ++) {
        UnloadImage(data->images[i]);
        UnloadTexture(data->textures[i]);
    }
    MemFree(data->images);
    MemFree(data->textures);
}
