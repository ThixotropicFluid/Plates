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

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                TerrainData terrain_data;

                terrain_data.width = 10;
                terrain_data.height = 10;

                TerrainDrawSettings terrain_draw_settings;

                DrawTerrain(&terrain_data, &terrain_draw_settings);

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

    i = index * 2;

    mesh->texcoords[i + 0] = texcoord.x;
    mesh->texcoords[i + 1] = texcoord.y;
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
            float org_x = (float) x; 
            float org_y = (float) y; 
            
            //InsertTriangle(&mesh, 0, (Vector3) {0, 0.0f, 0}, (Vector3) {0, 0.0f, 1}, (Vector3) {1, 0.0f, 0}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});
            //InsertTriangle(&mesh, 3, (Vector3) {0, 0.0f, 0}, (Vector3) {0, 0.0f, 1}, (Vector3) {1, 0.0f, 0}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});
            
            //printf("tri at: %f, %f, %f # %f, %f, %f # %f, %f, %f \n", org_x, 0.0f, org_y, org_x + 1, 0.0f, org_y, org_x, 0.0f, org_y + 1);
            InsertTriangle(&mesh, i, (Vector3) {org_x, 0.0f, org_y}, (Vector3) {org_x, 0.0f, org_y + 1}, (Vector3) {org_x + 1, 0.0f, org_y}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});
            i++;
            InsertTriangle(&mesh, i, (Vector3) {org_x + 1, 0.0f, org_y + 1}, (Vector3) {org_x + 1, 0.0f, org_y}, (Vector3) {org_x, 0.0f, org_y + 1}, (Vector3) {0.0f, 1.0f, 0.0f}, (Vector2) {0, 0});
            i++;
        }
    }

    Image terrain_image;

    terrain_image.width = 2;
    terrain_image.height = 2;
    terrain_image.mipmaps = 1;
    terrain_image.format = 7;
    terrain_image.data = malloc(sizeof(char) * 4);
    char* data = (char*)terrain_image.data;
    for(int i = 0 ; i < terrain_image.width * terrain_image.height; i ++) {
        data[i] = i * 16;
    }

    


    UploadMesh(&mesh, false);
    
    
    Model terrain_model = LoadModelFromMesh(mesh);

    Texture2D texture = LoadTextureFromImage(terrain_image);

    terrain_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    DrawModel(terrain_model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);

    UnloadTexture(texture);
    UnloadModel(terrain_model);
    UnloadImage(terrain_image);

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