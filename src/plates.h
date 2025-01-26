typedef struct {
    int seed;
} TerrainGenerationSettings;

typedef struct {
    int width;
    int height;
    int image_count;
    Image* images;
    Texture2D textures;
    int* hightmap;
    TerrainGenerationSettings terrain_settings;
} TerrainData;

void GenerateTerrain(TerrainData* data);