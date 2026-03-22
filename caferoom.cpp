#include "raylib.h"
#include "rlgl.h"
#include "rlights.h"
#include "math.h"


Color wallcolor = {73, 73, 81, 255 };
Color wallcolor2 = {78, 78, 86, 255};
Color metalBase = { 74, 74, 79, 255 };
Color rustLight = { 160, 90, 44, 255 };
Color rustDark = { 107, 58, 30, 255 };
Color metalDirty = { 58, 58, 63, 255 };
Color fogColor = { 180, 180, 200, 80 };   // pale dusty fog
// Global textures (initialized in main)
Texture2D texRust;
Texture2D texMetal;
Texture2D texFabric;

float fogRadius = 20.0f;                  // how far the fog extends



Vector3 ForwardFromCamera(Camera3D cam)
{
    // Compute forward vector manually (Pi‑safe)
    Vector3 f;
    f.x = cam.target.x - cam.position.x;
    f.y = cam.target.y - cam.position.y;
    f.z = cam.target.z - cam.position.z;

    // Normalize
    float mag = sqrtf(f.x*f.x + f.y*f.y + f.z*f.z);
    if (mag > 0)
    {
        f.x /= mag;
        f.y /= mag;
        f.z /= mag;
    }
    return f;
}

// ------------------------------------------------------------
// Draw cafe chair
// ------------------------------------------------------------
void DrawCafeChair(void)
{
    // -----------------------------
    // CHAIR DIMENSIONS
    // -----------------------------
    float seatW = 0.45f;
    float seatH = 0.05f;

    float legR = 0.03f;
    float legH = 0.28f;

    float backW = seatW;
    float backH = 0.35f;
    float backT = 0.05f;

    // -----------------------------
    // SEAT (single cube)
    // -----------------------------
    Vector3 seatCenter = {
        0.0f,
        legH + seatH/2.0f,
        0.0f
    };

    Color seatColor = (Color){120, 70, 50, 255};
    Mesh seatMesh = GenMeshCube(seatW, seatH, seatW);
    Model seatModel = LoadModelFromMesh(seatMesh);
    seatModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texFabric;

    DrawModel(seatModel, seatCenter, 1.0f, WHITE);


    // -----------------------------
    // BACKREST (single cube)
    // -----------------------------
    float backOffset = 0.04f;

    Vector3 backCenter = {
        0.0f,
        legH + seatH + backH/2.0f,
        -seatW/2.0f + backT/2.0f + backOffset
    };

    Color backColor = (Color){100, 60, 45, 255};
    Mesh backMesh = GenMeshCube(backW, backH, backT);
    Model backModel = LoadModelFromMesh(backMesh);
    backModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texFabric;

    DrawModel(backModel, backCenter, 1.0f, WHITE);


    // -----------------------------
    // LEGS (single cylinder each)
    // -----------------------------
    Color legColor = (Color){80, 60, 50, 255};

    float o = seatW/3.0f - legR;
    Vector3 legPos[4] = {
        { +o, 10.0f, +o },
        { -o, 10.0f, +o },
        { +o, 10.0f, -o },
        { -o, 10.0f, -o }
    };

    // Create a reusable textured leg model (created once per program run)
    static Model legModel = {0};
    static bool legModelReady = false;
    if (!legModelReady)
    {
        Mesh legMesh = GenMeshCylinder(legR, legH, 16);
        legModel = LoadModelFromMesh(legMesh);
        legModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texMetal;
        legModelReady = true;
    }

    for (int i = 0; i < 4; i++)
    {
        Vector3 legCenter = { legPos[i].x, legH/2.0f, legPos[i].z };
        DrawModel(legModel, legCenter, 1.0f, WHITE);
    }
}

// ------------------------------------------------------------
// Draw round cafe table
// ------------------------------------------------------------
void DrawRoundTable(void)
{
    // Table dimensions
    float topRadius = 0.40f;     // tabletop radius
    float topThickness = 0.04f;  // tabletop thickness

    float poleRadius = 0.05f;    // center support column
    float poleHeight = 0.25f;    // height from floor to tabletop

    float baseRadius = 0.30f;    // round base
    float baseThickness = 0.03f; // base thickness

    // Colors (adjust as you like)
    Color topColor  = (Color){180, 180, 180, 255};  // light gray tabletop
    Color poleColor = (Color){120, 120, 120, 255};  // darker gray pole
    Color baseColor = (Color){100, 100, 100, 255};  // dark gray base

    // -----------------------------
    // DRAW BASE
    // -----------------------------
    DrawCylinder(
        (Vector3){0.0f, baseThickness/2.0f, 0.0f},
        baseRadius, baseRadius,
        baseThickness,
        32,
        baseColor
    );

    // -----------------------------
    // DRAW POLE
    // -----------------------------
    DrawCylinder(
        (Vector3){0.0f, baseThickness - 0.12f + poleHeight/2.0f, 0.0f},
        poleRadius, poleRadius,
        poleHeight,
        32,
        poleColor
    );

    // -----------------------------
    // DRAW TABLETOP
    // -----------------------------
    DrawCylinder(
        (Vector3){0.0f, baseThickness - 0.12f + poleHeight + 0.12f + topThickness/2.0f, 0.0f},
        topRadius, topRadius,
        topThickness,
        32,
        topColor
    );
}



// ------------------------------------------------------------
// Draw central block
// ------------------------------------------------------------
void DrawCentralBlock(Color outerColor, Color innerColor, Color frontColor)
{
    float w = 1.0f;   // width
    float h = 1.2f;   // height
    float d = 1.0f;   // depth
    float t = 0.05f;  // wall thickness

    float cx = 0;
    float cy = 0.6f;
    float cz = 0;

    Vector3 pos;

    float innerDepth = d - t*2;   // NEW: shortened depth for side walls
    float innerWidth = w - t*2;   // NEW: shortened width for front/back walls

    // LEFT WALL (shortened)
    pos.x = cx - w/2 + t/2;
    pos.y = cy;
    pos.z = cz;
    DrawCube(pos, t, h, innerDepth, innerColor);

    // RIGHT WALL (shortened)
    pos.x = cx + w/2 - t/2;
    pos.y = cy;
    pos.z = cz;
    DrawCube(pos, t, h, innerDepth, innerColor);

    // FRONT WALL — split into two pieces to create a hole
    float holeHeight = 0.5f;              // height of the opening
    float topPiece = (h - holeHeight) / 2.0f;
    float bottomPiece = topPiece;

    // TOP FRONT PIECE
    pos.x = cx;
    pos.y = cy + (holeHeight/2.0f + topPiece/2.0f);
    pos.z = cz + d/2 + t/2;               // front side
    DrawCube(pos, innerWidth, topPiece, t, wallcolor2);

    // BOTTOM FRONT PIECE
    pos.x = cx;
    pos.y = cy - (holeHeight/2.0f + bottomPiece/2.0f);
    pos.z = cz + d/2 + t/2;               // front side
    DrawCube(pos, innerWidth, bottomPiece, t, wallcolor2);




    // BACK WALL — now using the old FRONT wall position
    pos.x = cx;
    pos.y = cy;
    pos.z = cz - d/2 + t/2;   // swapped
    DrawCube(pos, innerWidth, h, t, outerColor);


    // TOP (inner color)
    pos.x = cx;
    pos.y = cy + h/2 - t/2;
    pos.z = cz;
    DrawCube(pos, w, t, d, outerColor);

}

// ------------------------------------------------------------
// Draw room
// ------------------------------------------------------------
void DrawRoom()
{
    DrawCube((Vector3){0, -0.01f, 0}, 4.0f, 0.02f, 4.0f, DARKGRAY);

    DrawCube((Vector3){0,    1.25f, -2.0f}, 4.0f, 2.5f, 0.1f, wallcolor);
    DrawCube((Vector3){-2.0f, 1.25f,  0   }, 0.1f, 2.5f, 4.0f, wallcolor);
    DrawCube((Vector3){ 2.0f, 1.25f,  0   }, 0.1f, 2.5f, 4.0f, wallcolor);
    // Back wall behind the camera
    DrawCube((Vector3){0, 1.25f, 2.00f}, 4.0f, 2.5f, 0.1f, wallcolor);
    // Roof (sideways wall)
    DrawCube((Vector3){0, 2.5f, 0}, 4.0f, 0.1f, 4.0f, DARKGRAY);


}

// ------------------------------------------------------------
// MAIN PROGRAM
// ------------------------------------------------------------
int main(void)
{
    InitWindow(1280, 720, "Café Room — Chairs Example");
    
    // ------------------------------------------------------------
// GENERATE PROCEDURAL TEXTURES
// ------------------------------------------------------------

// ---- RUST TEXTURE ----
Image rustImg = GenImagePerlinNoise(64, 64, 0, 0, 6.0f);
ImageColorTint(&rustImg, (Color){180, 90, 40, 255}); // rusty brown

for (int y = 0; y < rustImg.height; y++)
{
    for (int x = 0; x < rustImg.width; x++)
    {
        Color c = GetImageColor(rustImg, x, y);

        // Dark corrosion
        if ((x + y + (rand() % 20)) % 17 == 0)
        {
            c.r *= 0.5f;
            c.g *= 0.5f;
            c.b *= 0.5f;
        }

        // Orange rust flakes
        if ((x * y + rand() % 50) % 113 == 0)
        {
            c.r = 220;
            c.g = 110;
            c.b = 40;
        }

        ImageDrawPixel(&rustImg, x, y, c);
    }
}

texRust = LoadTextureFromImage(rustImg);
UnloadImage(rustImg);


// ---- METAL TEXTURE ----
Image metalImg = GenImageWhiteNoise(64, 64, 0.15f);
ImageColorTint(&metalImg, (Color){150, 150, 160, 255});
texMetal = LoadTextureFromImage(metalImg);
UnloadImage(metalImg);


// ---- FABRIC TEXTURE ----
Image fabricImg = GenImagePerlinNoise(64, 64, 0, 0, 8.0f);
ImageColorTint(&fabricImg, (Color){120, 120, 130, 255});
texFabric = LoadTextureFromImage(fabricImg);
UnloadImage(fabricImg);

    
    Image img = GenImageColor(1, 1, WHITE);
    Texture2D fogTex = LoadTextureFromImage(img);
    UnloadImage(img);


    float globalDim = 0.55f;   // 55% brightness
    Color coldTint = { 180, 190, 220, 255 };  // bluish-gray


    Camera cam = {0};
    cam.position = (Vector3){0.0f, 0.75f, 1.8f};   // Your chosen camera
    cam.target   = (Vector3){0.0f, 1.0f, 0.0f};
    cam.up       = (Vector3){0.0f, 1.0f, 0.0f};
    cam.fovy     = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    DisableCursor();  // Mouse look
    SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2);  // virtual lock

    SetTargetFPS(60);

    while (!IsKeyPressed(KEY_ESCAPE))
    {
        UpdateCamera(&cam, CAMERA_FREE);  // old Raylib API

        BeginDrawing();
        ClearBackground(RAYWHITE);ClearBackground((Color){10, 10, 12, 255});   // dead industrial gray
        BeginMode3D(cam);
        
        float globalDim = 0.55f;   // 55% brightness
        Color coldTint = { 180, 190, 220, 255 };  // bluish-gray

        
        
        // Apply cold tint + dimming to everything
        rlColor4f(
        (coldTint.r / 255.0f) * globalDim,
        (coldTint.g / 255.0f) * globalDim,
        (coldTint.b / 255.0f) * globalDim,
        1.0f
    );

        DrawRoom();
        DrawCentralBlock(DARKGRAY, GRAY, wallcolor);

        // -------------------------------
        // TABLE POSITIONS
        // -------------------------------
        Vector3 tablePos[4] = {
            { 1.0f, 0.0f,  1.0f },
            { 1.0f, 0.0f, -1.0f },
            {-1.0f, 0.0f,  1.0f },
            {-1.0f, 0.0f, -1.0f }
        };

        // -------------------------------
        // DRAW TABLES
        // -------------------------------
        for (int i = 0; i < 4; i++)
        {
            rlPushMatrix();
                rlTranslatef(tablePos[i].x, tablePos[i].y, tablePos[i].z);
                DrawRoundTable();
            rlPopMatrix();
        }

        // -------------------------------
        // DRAW CHAIRS AROUND TABLES
        // -------------------------------
        for (int i = 0; i < 4; i++)
        {
            Vector3 p = tablePos[i];

            rlPushMatrix();
                rlTranslatef(p.x + -0.2f, 0.3f, p.z);
                rlRotatef(0, 0, 1, 0);      // face sideways
                rlRotatef(90, 0, 0, 1);      // tilt sideways (lean)
                DrawCafeChair();
            rlPopMatrix();


            // 118° CHAIR
            rlPushMatrix();
                rlTranslatef(p.x - 0.8f, 0.3f, p.z);
                rlRotatef(0, 0, 1, 0);
                rlRotatef(131, 90, 0, 1);
                DrawCafeChair();
            rlPopMatrix();
        }
        // ---- Dusty fog billboard ----

// ---- Dusty fog billboard ----
Vector3 forward = ForwardFromCamera(cam);

Vector3 fogPos = {
    cam.position.x + forward.x * 1.0f,
    cam.position.y + forward.y * 1.0f,
    cam.position.z + forward.z * 1.0f
};

Color fogColor = { 200, 200, 220, 60 };

DrawBillboard(cam, fogTex, fogPos, 4.0f, fogColor);



        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }
    
    UnloadTexture(texRust);
    UnloadTexture(texMetal);
    UnloadTexture(texFabric);


    CloseWindow();
    return 0;
}
