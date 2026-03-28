#include "raylib.h"
#include "rlgl.h"

Texture2D texCabinet;
Model cabinetModel;



void InitCabinetTest(void)
{
    texCabinet = LoadTexture("textures/cabinet.png");

    Mesh m = GenMeshCube(0.6f, 0.6f, 0.5f);
    cabinetModel = LoadModelFromMesh(m);

    cabinetModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texCabinet;
}

void DrawPaper(Vector3 pos, float rotationDeg, float scale)
{
    float width  = 0.21f * scale;
    float height = 0.297f * scale;
    float thick  = 0.01f;

    Vector3 axis = {0.0f, 1.0f, 0.0f};

    rlPushMatrix();
        rlTranslatef(pos.x + 5.0f, pos.y, pos.z);   // SHIFTED +5
        rlRotatef(rotationDeg, axis.x, axis.y, axis.z);
        DrawCube((Vector3){0,0,0}, width, thick, height, RAYWHITE);
        DrawCubeWires((Vector3){0,0,0}, width, thick, height, LIGHTGRAY);
    rlPopMatrix();
}

void DrawCabinetTest(void)
{
    Vector3 corners[4] = {
        { -2.0f + 5.0f, 0.3f, -2.0f },   // SHIFTED
        {  2.0f + 5.0f, 0.3f, -2.0f },   // SHIFTED
        { -2.0f + 5.0f, 0.3f,  2.0f },   // SHIFTED
        {  2.0f + 5.0f, 0.3f,  2.0f }    // SHIFTED
    };

    for (int i = 0; i < 4; i++)
    {
        Vector3 base = corners[i];

        DrawModel(cabinetModel, base, 1.0f, WHITE);

        DrawModel(cabinetModel,
                  (Vector3){ base.x, base.y + 0.6f, base.z },
                  1.0f,
                  WHITE);
    }
}

void DrawDesk(void)
{
    DrawCube((Vector3){5.0f, 1.0f, 0.0f}, 3.0f, 0.2f, 1.5f, BROWN);          // SHIFTED
    DrawCubeWires((Vector3){5.0f, 1.0f, 0.0f}, 3.0f, 0.2f, 1.5f, BLACK);     // SHIFTED

    float lx = 1.3f;
    float lz = 0.6f;

    DrawCube((Vector3){ 5.0f + lx, 0.5f,  lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
    DrawCube((Vector3){ 5.0f - lx, 0.5f,  lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
    DrawCube((Vector3){ 5.0f + lx, 0.5f, -lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
    DrawCube((Vector3){ 5.0f - lx, 0.5f, -lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
}



int main(void)
{
    InitWindow(1280, 720, "Cabinet Texture Test");

    Camera3D cam = {0};
    cam.position = (Vector3){0.0f, 2.0f, 7.0f };
    cam.target   = (Vector3){0.0f, 0.6f, 0.0f};
    cam.up       = (Vector3){0.0f, 1.0f, 0.0f};
    cam.fovy     = 60.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    InitCabinetTest();

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateCamera(&cam, CAMERA_FREE);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(cam);
        DrawGrid(10, 1.0f);

        DrawCabinetTest();
        DrawDesk();

        // ALL PAPERS SHIFTED BY +5 ON X
        DrawPaper((Vector3){0.0f, 1.1f, 0.0f}, 45.0f, 1.0f);
        DrawPaper((Vector3){-0.5f, 0.1f, 0.5f}, -30.0f, 0.8f);
        DrawPaper((Vector3){0.5f, 0.1f, -0.5f}, 30.0f, 0.8f);
        DrawPaper((Vector3){-0.8f, 0.1f, -0.2f}, -30.0f, 0.8f);
        DrawPaper((Vector3){0.8f, 0.1f, 0.2f}, 30.0f, 0.8f);
        DrawPaper((Vector3){0.1f, 0.1f, 0.8f}, 45.0f, 0.8f);
        DrawPaper((Vector3){-0.1f, 0.1f, -1.2f}, -45.0f, 0.8f);
        DrawPaper((Vector3){1.3f, 0.1f, -0.8f}, 60.0f, 0.8f);
        DrawPaper((Vector3){-1.3f, 0.1f, 0.8f}, -60.0f, 0.8f);
        DrawPaper((Vector3){-2.0f, 1.2f, -2.0f}, 15.0f, 0.8f);
        DrawPaper((Vector3){2.0f, 1.2f, 2.0f}, -15.0f, 0.8f);
        DrawPaper((Vector3){-2.0f, 1.2f, 2.0f}, 30.0f, 0.8f);
        DrawPaper((Vector3){2.0f, 1.2f, -2.0f}, -30.0f, 0.8f);
        DrawPaper((Vector3){0.0f, 0.1f, -2.0f}, 45.0f, 0.8f);
        DrawPaper((Vector3){0.0f, 0.1f, 2.0f}, -45.0f, 0.8f);
        DrawPaper((Vector3){2.0f, 0.1f, 0.0f}, -60.0f, 0.8f);

        // Paper stack
        for (float y = 1.1f; y <= 1.4f; y += 0.01f)
            DrawPaper((Vector3){-0.3f, y, -0.01f}, 15.0f, 0.8f);

        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(cabinetModel);
    UnloadTexture(texCabinet);
    CloseWindow();
    return 0;
}
