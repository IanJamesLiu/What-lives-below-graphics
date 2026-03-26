#include "raylib.h"
#include "rlgl.h"

Texture2D texCabinet;
Model cabinetModel;

void InitCabinetTest(void)
{
    texCabinet = LoadTexture("textures/cabinet.png");

    Mesh m = GenMeshCube(0.6f, 0.6f, 0.5f);
    cabinetModel = LoadModelFromMesh(m);

    // If your Raylib version supports DIFFUSE, use it.
    // If not, switch to ALBEDO.
    cabinetModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texCabinet;
}

void DrawPaper(Vector3 pos, float rotationDeg, float scale)
{
    // A4 ratio: 1 : 1.414
    float width  = 0.21f * scale;   // X
    float height = 0.297f * scale;  // Z
    float thick  = 0.01f;           // Y (paper thickness)

    // Rotation axis: Y (flat on desk)
    Vector3 axis = {0.0f, 1.0f, 0.0f};

    // Apply rotation
    rlPushMatrix();
        rlTranslatef(pos.x, pos.y, pos.z);
        rlRotatef(rotationDeg, axis.x, axis.y, axis.z);
        DrawCube((Vector3){0,0,0}, width, thick, height, RAYWHITE);
        DrawCubeWires((Vector3){0,0,0}, width, thick, height, LIGHTGRAY);
    rlPopMatrix();
}

void DrawCabinetTest(void)
{
    Vector3 corners[4] = {
        { -2.0f, 0.3f, -2.0f },   // top-left
        {  2.0f, 0.3f, -2.0f },   // top-right
        { -2.0f, 0.3f,  2.0f },   // bottom-left
        {  2.0f, 0.3f,  2.0f }    // bottom-right
    };

    for (int i = 0; i < 4; i++)
    {
        Vector3 base = corners[i];

        // bottom cube
        DrawModel(cabinetModel, base, 1.0f, WHITE);

        // top cube (0.6 higher)
        DrawModel(cabinetModel,
                  (Vector3){ base.x, base.y + 0.6f, base.z },
                  1.0f,
                  WHITE);
    }
}
void DrawDesk(void)
{
    // Desk top
    DrawCube((Vector3){0.0f, 1.0f, 0.0f}, 3.0f, 0.2f, 1.5f, BROWN);
    DrawCubeWires((Vector3){0.0f, 1.0f, 0.0f}, 3.0f, 0.2f, 1.5f, BLACK);

    // Legs (4 corners)
    float lx = 1.3f;  // leg X offset
    float lz = 0.6f;  // leg Z offset

    DrawCube((Vector3){ lx, 0.5f,  lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
    DrawCube((Vector3){-lx, 0.5f,  lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
    DrawCube((Vector3){ lx, 0.5f, -lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
    DrawCube((Vector3){-lx, 0.5f, -lz}, 0.2f, 1.0f, 0.2f, DARKBROWN);
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

    DisableCursor();  // you wanted this

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateCamera(&cam, CAMERA_FIRST_PERSON);
        // You asked for this — here it is.
        /*UpdateCameraPro(&cam,
        (Vector3){ 0, 0, 0 },  // no movement (WASD disabled)
        (Vector3){
        GetMouseDelta().x * 0.05f,   // yaw (left/right look)
        GetMouseDelta().y * 0.05f,   // pitch (up/down look)
        0.0f                          // roll
        },
        0.0f
    );*/

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(cam);
        DrawGrid(10, 1.0f);
        DrawCabinetTest();
        DrawDesk();
        DrawPaper((Vector3){0.0f, 1.2f, 0.0f}, 45.0f, 1.0f); // paper on desk
        DrawPaper((Vector3){-0.5f, 0.1f, 0.5f}, -30.0f, 0.8f); // another paper
        DrawPaper((Vector3){0.5f, 0.1f, -0.5f}, 30.0f, 0.8f); // yet another paper
        DrawPaper((Vector3){-0.8f, 0.1f, -0.2f}, -30.0f, 0.8f); // and another paper
        DrawPaper((Vector3){0.8f, 0.1f, 0.2f}, 30.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){0.1f, 0.1f, 0.8f}, 45.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.1f, 0.1f, -1.2f}, -45.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){1.3f, 0.1f, -0.8f}, 60.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-1.3f, 0.1f, 0.8f}, -60.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-2.0f, 1.2f, -2.0f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){2.0f, 1.2f, 2.0f}, -15.0f, 0.8f); // this because the world needs more paper
        DrawPaper((Vector3){-2.0f, 1.2f, 2.0f}, 30.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){2.0f, 1.2f, -2.0f}, -30.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){0.0f, 0.1f, -2.0f}, 45.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){0.0f, 0.1f, 2.0f}, -45.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-2.0f, 0.1f, 0.0f}, 60.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){2.0f, 0.1f, 0.0f}, -60.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.2f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.21f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.22f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.23f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.24f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.25f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.26f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.27f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.28f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.29f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.3f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.31f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.32f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.33f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.34f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.35f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.36f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.37f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.38f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.39f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        DrawPaper((Vector3){-0.3f, 1.4f, -0.01f}, 15.0f, 0.8f); // and yet another paper
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(cabinetModel);
    UnloadTexture(texCabinet);
    CloseWindow();
    return 0;
}
