/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera free
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "stdlib.h"
#include "plates.h"
#include "stdio.h"


typedef struct {
    int texture_select;
    float scale;
} TerrainDrawSettings;




void GetTerrain(TerrainData* data);

void DrawTerrain(TerrainData* data, TerrainDrawSettings* draw_settings);

void DrawReferanceMesh(int slices, float spaceing, Color color);

Mesh CreateDebugTerrainMesh();

Vector3 crossProduct(Vector3 a, Vector3 b);

Vector3 subtract(Vector3 a, Vector3 b);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 2.0f, 2.0f, 2.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    TerrainData terrain_data;

    terrain_data.width = 100;
    terrain_data.height = 100;

    TerrainDrawSettings terrain_draw_settings;
    terrain_draw_settings.texture_select = 1;
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

            BeginMode3D(camera);
                

                GenerateTerrain(&terrain_data);

                if(IsKeyPressed(KEY_C)) {
                    terrain_draw_settings.texture_select += 1;
                    if(terrain_draw_settings.texture_select >= terrain_data.image_count) {
                        terrain_draw_settings.texture_select = -1;
                    }
                }


                
                DrawTerrain(&terrain_data, &terrain_draw_settings);
                UnloadTerrain(&terrain_data);

                DrawReferanceMesh(20, 0.1f, RED);

            EndMode3D();

            DrawFPS(10, 20);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InsertTriangle(Mesh* mesh, int index, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 normal, Vector2 texcoord) {
    int i = index * 9;
    mesh->vertices[i + 0] = v1.x;
    mesh->vertices[i + 1] = v1.y;
    mesh->vertices[i + 2] = v1.z;

    
    mesh->vertices[i + 3] = v2.x;
    mesh->vertices[i + 4] = v2.y;
    mesh->vertices[i + 5] = v2.z;

    
    mesh->vertices[i + 6] = v3.x;
    mesh->vertices[i + 7] = v3.y;
    mesh->vertices[i + 8] = v3.z;

    i = index * 3;

    mesh->normals[i + 0] = normal.x;
    mesh->normals[i + 1] = normal.y;
    mesh->normals[i + 2] = normal.z;

    i = index * 6;

    mesh->texcoords[i + 0] = texcoord.x;
    mesh->texcoords[i + 1] = texcoord.y;

    mesh->texcoords[i + 2] = texcoord.x;
    mesh->texcoords[i + 3] = texcoord.y;

    mesh->texcoords[i + 4] = texcoord.x;
    mesh->texcoords[i + 5] = texcoord.y;
}

void DrawTerrain(TerrainData* terrain_data, TerrainDrawSettings* draw_settings) {
    Mesh mesh = {0};
    mesh.triangleCount = (terrain_data->width * terrain_data->height) * 2;
    mesh.vertexCount = mesh.triangleCount * 3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount * 2 * sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    int i = 0;
    for (int x = 0; x < terrain_data->width; x ++) {
        for (int y = 0; y < terrain_data->height; y ++) {
            float org_x = ((float) x - (float)terrain_data->width/2.0f) / (float)terrain_data->width; 
            float org_y = ((float) y - (float)terrain_data->height/2.0f) / (float)terrain_data->height;

            float x_scl =  1 / (float)terrain_data->width;
            float y_scl =  1 / (float)terrain_data->height;

            float vert_scl = 0.1f;
            
            Vector3 v1 = (Vector3) {org_x + 0, terrain_data->height_map[(x+0) * terrain_data->width + (y + 0)] * vert_scl, org_y + 0};
            Vector3 v2 = (Vector3) {org_x + x_scl, terrain_data->height_map[(x+1) * terrain_data->width + (y + 0)] * vert_scl, org_y + 0};
            Vector3 v3 = (Vector3) {org_x + 0, terrain_data->height_map[(x+0) * terrain_data->width + (y + 1)] * vert_scl, org_y + y_scl};
            Vector3 v4 = (Vector3) {org_x + x_scl, terrain_data->height_map[(x+1) * terrain_data->width + (y + 1)] * vert_scl, org_y + y_scl};

            Vector3 norm = crossProduct(subtract(v3, v1), subtract(v2, v1)) ;

            
            Vector2 texcoord = (Vector2) {(float)x / (float)terrain_data->width, (float)y / (float)terrain_data->height};

            //InsertTriangle(&mesh, 0, (Vector3) {0, 0.0f, 0}, (Vector3) {0, 0.0f, 1}, (Vector3) {1, 0.0f, 0}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});
            //InsertTriangle(&mesh, 3, (Vector3) {0, 0.0f, 0}, (Vector3) {0, 0.0f, 1}, (Vector3) {1, 0.0f, 0}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});
            
            //printf("tri at: %f, %f, %f # %f, %f, %f # %f, %f, %f \n", org_x, 0.0f, org_y, org_x + 1, 0.0f, org_y, org_x, 0.0f, org_y + 1);
            InsertTriangle(&mesh, i, v1, v3, v2, norm, texcoord);
            i++;
            InsertTriangle(&mesh, i, v2, v3, v4, norm, texcoord);
            i++;
        }
    }

    //load in textures

    Texture2D texture;

    if (draw_settings->texture_select < 0 || draw_settings->texture_select >= terrain_data->image_count) {
        Image checked = GenImageChecked(terrain_data->width, terrain_data->height, 1, 1, RED, GREEN);
        texture = LoadTextureFromImage(checked);
        UnloadImage(checked);
    } else {
        texture = terrain_data->textures[draw_settings->texture_select];
    }


    UploadMesh(&mesh, false);
    
    
    Model terrain_model = LoadModelFromMesh(mesh);

    terrain_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    DrawModel(terrain_model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);

    if (draw_settings->texture_select < 0 || draw_settings->texture_select >= terrain_data->image_count) {
        UnloadTexture(texture);    
    } // stupid
    UnloadModel(terrain_model);

}

void DrawReferanceMesh(int slices, float spaceing, Color color) {

    float size = (float) slices * spaceing;
    Vector3 start = {-size/2, 0, -size/2};
    Vector3 stop = {-size/2, 0, size/2};
    for(int i = 0; i <= slices; i++) {

        DrawLine3D(start, stop, color);
        start.x += spaceing;
        stop.x += spaceing;
    }

    start = (Vector3){-size/2, 0, -size/2};
    stop = (Vector3){size/2, 0, -size/2};
    for(int i = 0; i <= slices; i++) {

        DrawLine3D(start, stop, color);
        start.z += spaceing;
        stop.z += spaceing;
    }

}

Mesh CreateDebugTerrainMesh() {
    Mesh mesh = { 0 };
    mesh.triangleCount = 2;
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    InsertTriangle(&mesh, 3, (Vector3) {0, 0.0f, 0}, (Vector3) {0, 0.0f, 1}, (Vector3) {1, 0.0f, 0}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}

Vector3 subtract(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

// Function to compute the cross product of two vectors
Vector3 crossProduct(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}