#include "raylib.h"
#include "rlgl.h"
#include "rlights.h"
#include "math.h"
#include <cstring>

Color wallcolor = {73, 73, 81, 255 };
Color wallcolor2 = {78, 78, 86, 255};
Color metalBase = { 74, 74, 79, 255 };
Color rustLight = { 160, 90, 44, 255 };
Color rustDark = { 107, 58, 30, 255 };
Color DARKYELLOW = { 100, 90, 0, 255 };
Color DARKERGREEN = { 0, 50, 0, 255 };
Color OTHERGRAY = { 110, 110, 110, 255 };
Color metalDirty = { 58, 58, 63, 255 };
Color blueprint = { 0, 255, 255, 12 };
Color fogColor = { 18, 18, 20, 2 };   // pale dusty fog
// Global textures (initialized in main)
Texture2D texWall;
Texture2D texRust;
Texture2D texMetal;
Texture2D texFabric;
Texture2D fogTex;
Texture2D TexTableNoise;
Texture2D texCabinet;
Texture2D texDesk;
Texture2D texComp;
Texture2D texStatic;

bool GeneratorActive = false;

float fogRadius = 20.0f;                  // how far the fog extends
Model wall;
Model TableTopModel;
Model TableTopDisk;
Model cabinetModel;
Model deskTopModel;
Model deskLegModel;
Model trapModel;
Model deskModel;






struct SmokeParticle {
    Vector3 pos;
    float size;
    float alpha;
    float riseSpeed;
    float growSpeed;
    bool active;
};

const int MAX_SMOKE = 64;
SmokeParticle smoke[MAX_SMOKE];

void SpawnSmoke(Vector3 pos)
{
    for (int i = 0; i < MAX_SMOKE; i++)
    {
        if (!smoke[i].active)
        {
            smoke[i].active = true;
            smoke[i].pos = pos;
            smoke[i].size = 0.1f;
            smoke[i].alpha = 1.0f;
            smoke[i].riseSpeed = 0.4f + (float)(rand()%100)/300.0f; // random rise
            smoke[i].growSpeed = 0.3f;
            return;
        }
    }
}

void UpdateSmoke(float dt)
{
    for (int i = 0; i < MAX_SMOKE; i++)
    {
        if (!smoke[i].active) continue;

        smoke[i].pos.y += smoke[i].riseSpeed * dt;
        smoke[i].size += smoke[i].growSpeed * dt;
        smoke[i].alpha -= dt * 0.4f;
                //🌬 🌬 🌬 🌬 🌬 🌬 🌬 🌬 🌬 🌬 🌬 🌬 🌬
        smoke[i].pos.x += ((rand() % 100) / 500000.0f - 0.01f);
        smoke[i].pos.z += ((rand() % 100) / 500000.0f - 0.01f);


        if (smoke[i].alpha <= 0.0f)
            smoke[i].active = false;
    }
}

void DrawSmoke3D()
{
    for (int i = 0; i < MAX_SMOKE; i++)
    {
        if (!smoke[i].active) continue;

        Color c = { (unsigned char)(smoke[i].alpha * 1024), (unsigned char)(smoke[i].alpha * 1024), (unsigned char)(smoke[i].alpha * 1024), 255 };
        DrawSphere(smoke[i].pos, smoke[i].size, c);
    }
}

void DrawTubeJointRotatable(
    Vector3 jointPos,
    float radius,        // tube radius AND sphere radius
    float tubeLength,
    float angleDeg,
    Vector3 axis,
    float tubeBRot,     // NEW
    float offsety1,
    float offsety2,
    float offsetx1,
    float offsetx2,
    float offsetz1,
    float offsetz2
)
{
    Color color = GRAY;
    float half = tubeLength * 0.5f;

    // Tube A: flat along -Z, touching the sphere
    rlPushMatrix();
        rlTranslatef(jointPos.x - offsetx1, jointPos.y - offsety1, jointPos.z - offsetz1);
        rlRotatef(90, 1, 0, 0);                 // make cylinder lie flat (Y → Z)
        DrawCylinder(
            (Vector3){0, -half - radius, 0},    // center so it touches sphere
            radius, radius, tubeLength,
            16, color
        );
    rlPopMatrix();

    // Joint sphere
    DrawSphere(jointPos, radius, color);

    // Tube B: rotatable, flat, touching the sphere
    rlPushMatrix();
        rlTranslatef(jointPos.x - offsetx2, jointPos.y - offsety2, jointPos.z - offsetz2);
        rlRotatef(angleDeg, axis.x, axis.y, axis.z);
        rlRotatef(90, 1, 0, 0);                 // make cylinder lie flat (Y → Z)
        rlRotatef(tubeBRot, 0, 0, 1);                 // NEW: Tube B twist
        DrawCylinder(
            (Vector3){0, half + radius, 0},     // center so it touches sphere
            radius, radius, tubeLength,
            16, color
        );
    rlPopMatrix();
}

void DrawSmokeSphere(Vector3 pos, float radius, float alpha)
{
    Color c = { 180, 180, 190, (unsigned char)(alpha * 255) };
    DrawSphere(pos, radius, c);
}



void DrawGenerator(void)
{
    SpawnSmoke((Vector3){9.67, 1, -0.6f});
    SpawnSmoke((Vector3){9.67f, 0.9f, -0.6f});
    DrawTubeJointRotatable(
    (Vector3){9.67, 0.19, 0},
    0.19f,
    0.5f,
    90.0f,
    (Vector3){0, 1, 0},   // rotate flat
    0,
    0,
    0,
    0,
    0.43f,
    0.09f,    // rotate around z axis
    0
        );
        DrawTubeJointRotatable(
    (Vector3){10.5, 0.19, 0},
    0.19f,
    0.5f,
    180.0f,
    (Vector3){0, 1, 0},   // rotate flat
    270,
    0,
    0,
    0,
    -0.38f,
    0.09f,    // rotate around z axis
    0
        
);
DrawCylinder((Vector3){9.67, 0.33, -0.6}, 0.25, 0.19, 0.5, 16, OTHERGRAY);
DrawSphere((Vector3){10.5, 0.19, -0.6}, 0.19, GRAY);
DrawSphere((Vector3){9.67, 0.19, -0.6}, 0.19, GRAY);
}

void DrawFloorOffice(float x, float z, float rot)
{
    rlPushMatrix();
        rlTranslatef(x, 0.1f, z);
        rlRotatef(rot, 0, 1, 0);
        DrawCube((Vector3){x, 0.01f, z}, 4.0f, 0.1f, 1.0f, GRAY);
    rlPopMatrix();
}

void DrawCustomGrid(int size, float spacing, Vector3 center, Color color)
{
    int half = size / 2;

    for (int i = -half; i <= half; i++)
    {
        // Vertical lines (Z direction)
        DrawLine3D(
            (Vector3){ center.x + i * spacing, 0.0f, center.z - half * spacing },
            (Vector3){ center.x + i * spacing, 0.0f, center.z + half * spacing },
            color
        );

        // Horizontal lines (X direction)
        DrawLine3D(
            (Vector3){ center.x - half * spacing, 0.0f, center.z + i * spacing },
            (Vector3){ center.x + half * spacing, 0.0f, center.z + i * spacing },
            color
        );
    }
}

Mesh MergeMeshes(Mesh a, Mesh b)
{
    Mesh out = {0};

    out.vertexCount   = a.vertexCount + b.vertexCount;
    out.triangleCount = a.triangleCount + b.triangleCount;

    out.vertices = (float*)MemAlloc(out.vertexCount * 3 * sizeof(float));
    out.texcoords = (float*)MemAlloc(out.vertexCount * 2 * sizeof(float));
    out.indices = (unsigned short*)MemAlloc(out.triangleCount * 3 * sizeof(unsigned short));

    memcpy(out.vertices, a.vertices, a.vertexCount * 3 * sizeof(float));
    memcpy(out.texcoords, a.texcoords, a.vertexCount * 2 * sizeof(float));
    memcpy(out.indices, a.indices, a.triangleCount * 3 * sizeof(unsigned short));

    // Offset b’s indices
    for (int i = 0; i < b.triangleCount * 3; i++)
        out.indices[a.triangleCount * 3 + i] = b.indices[i] + a.vertexCount;

    // Append b’s vertices + UVs
    memcpy(out.vertices + a.vertexCount * 3, b.vertices, b.vertexCount * 3 * sizeof(float));
    memcpy(out.texcoords + a.vertexCount * 2, b.texcoords, b.vertexCount * 2 * sizeof(float));

    UploadMesh(&out, false);
    return out;
}

Mesh GenTrap(float bw, float tw, float h, float bd, float td)
{
    Mesh m = {0};
    m.vertexCount = 8;
    m.triangleCount = 12;

    m.vertices = (float*)MemAlloc(8*3*sizeof(float));
    m.texcoords = (float*)MemAlloc(8*2*sizeof(float));
    m.indices = (unsigned short*)MemAlloc(12*3*sizeof(unsigned short));

    float hb = bw/2, ht = tw/2;
    float hbd = bd/2, htd = td/2;

    float v[24] = {
    // bottom (rotated)
    -hb, -hbd, 0,
     hb, -hbd, 0,
     hb,  hbd, 0,
    -hb,  hbd, 0,

    // top (rotated)
    -ht, -htd, -h,
     ht, -htd, -h,
     ht,  htd, -h,
    -ht,  htd, -h
};

    memcpy(m.vertices, v, sizeof(v));

    float uv[16] = {
        0,0, 1,0, 1,1, 0,1,
        0,0, 1,0, 1,1, 0,1
    };
    memcpy(m.texcoords, uv, sizeof(uv));

    unsigned short ind[36] = {
    // bottom
    0,1,2,  0,2,3,

    // top
    4,6,5,  4,7,6,

    // sides
    0,4,5,  0,5,1,
    1,5,6,  1,6,2,
    2,6,7,  2,7,3,
    3,7,4,  3,4,0
    };

    memcpy(m.indices, ind, sizeof(ind));

    UploadMesh(&m, false);
    return m;
}
void DrawCornerDesk(Vector3 pos, float rot)
{
    rlPushMatrix();
        rlTranslatef(pos.x, pos.y, pos.z);
        rlRotatef(rot, 0, 1, 0);
        DrawModel(deskModel, (Vector3){0,0,0}, 1.0f, WHITE);
    rlPopMatrix();
}

/*void DrawCornerDesk(Vector3 pos, float rotationDeg)
{


    float topW = 1.5f;   // width of each wing
    float topD = 0.6f;   // depth of each wing
    float topH = 1.0f;   // height above ground

    float legH = 0.5f;

    float h = 0.85f; // table height

    Vector3 unit = { 1.0f/4.0f, 1.0f, 1.0f/2.0f };

    // A: top-left tile
    DrawModelEx(
        deskTopModel,
        (Vector3){pos.x + 0.0f, pos.y + h, pos.z + 0.0f},
        (Vector3){0,1,0},
        0.0f,
        unit,
        WHITE
    );

    // B: top-right tile
    DrawModelEx(
        deskTopModel,
        (Vector3){pos.x + 0.75f, pos.y + h, pos.z + 0.0f},
        (Vector3){0,1,0},
        0.0f,
        unit,
        WHITE
    );

    // C: bottom-right tile
    DrawModelEx(
        deskTopModel,
        (Vector3){pos.x + 0.75f, pos.y + h, pos.z + 0.75f},
        (Vector3){0,1,0},
        0.0f,
        unit,
        WHITE
    );


    // -------------------------
    // LEGS
    // -------------------------

    // A bottom-left corner
DrawModel(deskLegModel,
          (Vector3){pos.x - 0.3f, pos.y + legH/2, pos.z - 0.3f},
          1.0f,
          GRAY);

// A top-left corner
DrawModel(deskLegModel,
          (Vector3){pos.x - 0.3f, pos.y + legH/2, pos.z + 0.3f},
          1.0f,
          GRAY);

// B top-right corner
DrawModel(deskLegModel,
          (Vector3){pos.x + 0.45f, pos.y + legH/2, pos.z + 1.05f},
          1.0f,
          GRAY);

// C bottom-right corner
DrawModel(deskLegModel,
          (Vector3){pos.x + 1.05f, pos.y + legH/2, pos.z + 1.05f},
          1.0f,
          GRAY);

}*/



void DrawBox(Vector3 pos, Vector3 rotDeg, Vector3 size, Color tint)
{
    // Load the textured cube model once
    static Model boxModel = {0};
    static bool loaded = false;

    if (!loaded)
    {
        Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        boxModel = LoadModelFromMesh(cube);

        Texture2D tex = LoadTexture("textures/box.png");
        boxModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;

        loaded = true;
    }

    rlPushMatrix();

        // Apply the -5 shift on X
        rlTranslatef(pos.x - 5.0f, pos.y, pos.z);

        // Apply rotations
        rlRotatef(rotDeg.x, 1, 0, 0);
        rlRotatef(rotDeg.y, 0, 1, 0);
        rlRotatef(rotDeg.z, 0, 0, 1);

        // Apply scaling
        rlScalef(size.x, size.y, size.z);

        // Draw the textured cube
        DrawModel(boxModel, (Vector3){0,0,0}, 1.0f, tint);

    rlPopMatrix();
}

// The computer
void DrawComputer(Vector3 pos, float scale, float rotation)
{
    // --- SCREEN ---
    Mesh screenMesh = GenMeshCube(0.40f, 0.30f, 0.02f);
    Model screenModel = LoadModelFromMesh(screenMesh);
    screenModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall; // placeholder

    Mesh staticMesh = GenMeshCube(0.40f, 0.30f, 0.0f);
    Model staticModel = LoadModelFromMesh(staticMesh);
    staticModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texStatic;

    // --- TRAPEZOID BACK ---
    extern Model trapModel; // your trapezoid model

    // --- STAND ---
    Mesh standMesh = GenMeshCube(0.10f, 0.20f, 0.10f);
    Model standModel = LoadModelFromMesh(standMesh);
    standModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texMetal;

    // --- DRAW ---
    // --- ROTATED COMPUTER BODY ---
    rlPushMatrix();
        rlTranslatef(pos.x, pos.y, pos.z);
        rlRotatef(rotation, 0, 1, 0);   // <--- ROTATION HERE
        rlScalef(scale, scale, scale);

        // Screen
        DrawModel(screenModel, (Vector3){0, 0.25f, 0}, 1.0f, WHITE);
        DrawModel(staticModel, (Vector3){0, 0.25f, 0.015f}, 1.0f, WHITE);

        // Stand
        DrawModel(standModel, (Vector3){0, 0.10f, 0}, 1.0f, WHITE);

        // Trapezoid back (now rotates with the rest)
        DrawModel(trapModel, (Vector3){0, 0.25f, -0.02f}, 0.75f, WHITE);

    rlPopMatrix();
}



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

//prepare cabinet 
void InitCabinetTest(void)
{
    texCabinet = LoadTexture("textures/cabinet.png");

    Mesh m = GenMeshCube(0.6f, 0.6f, 0.5f);
    cabinetModel = LoadModelFromMesh(m);

    cabinetModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texCabinet;
}
//paper drawing function
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
//the star of the show, the cabinet
void DrawCabinetTest(void)
{
    Vector3 corners[4] = 
    {
        { 5.0f - 1.5f, 0.3f, -1.5f },
        { 5.0f + 1.5f, 0.3f, -1.5f },
        { 5.0f - 1.5f, 0.3f,  1.5f },
        { 5.0f + 1.5f, 0.3f,  1.5f }
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
//a nice centerpiece, the desk
void DrawDesk(void)
{
    // Desk top
    DrawModel(deskTopModel, (Vector3){5.0f, 1.0f, 0.0f}, 1.0f, WHITE);

    float lx = 1.3f;
    float lz = 0.6f;

    // Legs
    DrawModel(deskLegModel, (Vector3){5.0f + lx, 0.5f,  lz}, 1.0f, WHITE);
    DrawModel(deskLegModel, (Vector3){5.0f - lx, 0.5f,  lz}, 1.0f, WHITE);
    DrawModel(deskLegModel, (Vector3){5.0f + lx, 0.5f, -lz}, 1.0f, WHITE);
    DrawModel(deskLegModel, (Vector3){5.0f - lx, 0.5f, -lz}, 1.0f, WHITE);
}

// ------------------------------------------------------------
// Draw cafe chair, another star
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
    float backOffset = 0.0f;

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
        { +o, 0.0f, +o },
        { -o, 0.0f, +o },
        { +o, 0.0f, -o },
        { -o, 0.0f, -o }
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
        Vector3 legCenter = { legPos[i].x, legH/10.0f, legPos[i].z };
        DrawModel(legModel, legCenter, 1.0f, BROWN);
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
    // DRAW POLE
    // -----------------------------
    Color polewhite = {0xfe, 0xfe, 0xfe, 0xff};
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
    Color otherwhite = {0xfd, 0xfd, 0xfd, 0xff};
    Vector3 topPos = {
    0.0f,
    baseThickness - 0.12f + poleHeight + 0.12f + topThickness/2.0f,
    0.0f
    };

    Vector3 diskPos = {
    0.0f,
    baseThickness - 0.12f + poleHeight + 0.12f + topThickness,
    0.0f
    };

    diskPos.y += 0.025f;

    DrawModel(TableTopModel, topPos, 1.0f, otherwhite);

    DrawModelEx(
    TableTopDisk,
    diskPos,
    (Vector3){1,0,0},   // rotate flat
    00.0f,
    (Vector3){1,1,1},
    otherwhite
    );

    }



// ------------------------------------------------------------
// Draw central block
// ------------------------------------------------------------
void DrawCentralBlock(Color outerColor, Color innerColor, Color frontColor, Model wallModel, Model upModel, Model downModel)
{
    float w = 1.0f;
    float h = 1.2f;
    float d = 1.0f;
    float t = 0.05f;

    float cx = 0;
    float cy = 0.6f;
    float cz = 0;

    Vector3 pos;

    float innerDepth = d - t*2;
    float innerWidth = w - t*2;

    // LEFT WALL
    pos = { cx - w/2 + t/2, cy, cz };
    DrawModelEx(wallModel, pos, {0,1,0}, 0.0f, {t, h, innerDepth}, WHITE);

    // RIGHT WALL
    pos = { cx + w/2 - t/2, cy, cz };
    DrawModelEx(wallModel, pos, {0,1,0}, 0.0f, {t, h, innerDepth}, WHITE);

    // FRONT WALL (top)
    float holeHeight = 0.5f;
    float topPiece = (h - holeHeight) / 2.0f;
    float bottomPiece = topPiece;

    pos = { cx, cy + (holeHeight/2.0f + topPiece/2.0f), cz + d/2 + t/2 };
    DrawModelEx(downModel, pos, {0,1,0}, 0.0f, {innerWidth, topPiece, t}, WHITE);

    // FRONT WALL (bottom)
    pos = { cx, cy - (holeHeight/2.0f + bottomPiece/2.0f), cz + d/2 + t/2 };
    DrawModelEx(upModel, pos, {0,1,0}, 0.0f, {innerWidth, bottomPiece, t}, WHITE);

    // BACK WALL
    pos = { cx, cy, cz - d/2 + t/2 };
    DrawModelEx(wallModel, pos, {0,1,0}, 0.0f, {innerWidth, h, t}, WHITE);

    // TOP
    pos = { cx, cy + h/2 - t/2, cz };
    DrawModelEx(wallModel, pos, {0,1,0}, 0.0f, {w, t, d}, WHITE);
}

// ------------------------------------------------------------
// Draw room
// ------------------------------------------------------------
void DrawRoom()
{
    static Model wallCubeModel;
{
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);   // unit cube
    wallCubeModel = LoadModelFromMesh(cubeMesh);
    wallCubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall;
}


    rlDisableBackfaceCulling();
    DrawModel(wall, (Vector3){0, -0.01f, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();

    DrawModelEx(
    wallCubeModel,
    (Vector3){0, 0.0f, 0},
    (Vector3){1,0,0}, 00.0f,
    (Vector3){4.0f, 0.1f, 4.0f},
    WHITE
    );


    DrawModelEx(wallCubeModel,
    (Vector3){0, 1.25f, -2.0f},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){4.0f, 2.5f, 0.1f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){-2.0f, 1.25f, 0},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){0.1f, 2.5f, 4.0f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){2.0f, 1.25f, 0},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){0.1f, 2.5f, 4.0f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){0, 1.25f, 2.0f},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){4.0f, 2.5f, 0.1f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){0, 2.5f, 0},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){4.0f, 0.1f, 4.0f},
    WHITE);


}
// ------------------------------------------------------------
// ROOM 2
// ------------------------------------------------------------
void DrawRoom2()
{
    static Model wallCubeModel;
    {
        Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
        wallCubeModel = LoadModelFromMesh(cubeMesh);
        wallCubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall;
    }

    float X = 5.0f;   // archives room offset

    rlDisableBackfaceCulling();
    DrawModel(wall, (Vector3){X, -0.01f, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();

    DrawModelEx(
        wallCubeModel,
        (Vector3){X, 0.0f, 0},
        (Vector3){1,0,0}, 0.0f,
        (Vector3){4.0f, 0.1f, 4.0f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){X, 1.25f, -2.0f},
        (Vector3){0,1,0}, 0.0f,
        (Vector3){4.0f, 2.5f, 0.1f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){X - 2.0f, 1.25f, 0},   // left wall
        (Vector3){0,1,0}, 0.0f,
        (Vector3){0.1f, 2.5f, 4.0f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){X + 2.0f, 1.25f, 0},   // right wall
        (Vector3){0,1,0}, 0.0f,
        (Vector3){0.1f, 2.5f, 4.0f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){X, 1.25f, 2.0f},
        (Vector3){0,1,0}, 0.0f,
        (Vector3){4.0f, 2.5f, 0.1f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){X, 2.5f, 0},
        (Vector3){0,1,0}, 0.0f,
        (Vector3){4.0f, 0.1f, 4.0f},
        WHITE
    );
}
// ------------------------------------------------------------
// ROOM 3
// ------------------------------------------------------------
void DrawRoom3()
{
    static Model wallCubeModel;
    {
        Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);   // unit cube
        wallCubeModel = LoadModelFromMesh(cubeMesh);
        wallCubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall;
    }

    rlDisableBackfaceCulling();
    DrawModel(wall, (Vector3){-5.0f, -0.01f, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();

    DrawModelEx(
        wallCubeModel,
        (Vector3){-5.0f, 0.0f, 0},
        (Vector3){1,0,0}, 0.0f,
        (Vector3){4.0f, 0.1f, 4.0f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){-5.0f, 1.25f, -2.0f},
        (Vector3){0,1,0}, 0.0f,
        (Vector3){4.0f, 2.5f, 0.1f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){-7.0f, 1.25f, 0},   // -5 - 2
        (Vector3){0,1,0}, 0.0f,
        (Vector3){0.1f, 2.5f, 4.0f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){-3.0f, 1.25f, 0},   // -5 + 2
        (Vector3){0,1,0}, 0.0f,
        (Vector3){0.1f, 2.5f, 4.0f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){-5.0f, 1.25f, 2.0f},
        (Vector3){0,1,0}, 0.0f,
        (Vector3){4.0f, 2.5f, 0.1f},
        WHITE
    );

    DrawModelEx(
        wallCubeModel,
        (Vector3){-5.0f, 2.5f, 0},
        (Vector3){0,1,0}, 0.0f,
        (Vector3){4.0f, 0.1f, 4.0f},
        WHITE
    );
}
// ------------------------------------------------------------
// Draw Room 4
// ------------------------------------------------------------
void DrawRoom4()
{
   rlPushMatrix();
    rlTranslatef(0, 0, 5.0f);
    static Model wallCubeModel;
{
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);   // unit cube
    wallCubeModel = LoadModelFromMesh(cubeMesh);
    wallCubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall;
}


    rlDisableBackfaceCulling();
    DrawModel(wall, (Vector3){0, -0.01f, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();

    DrawModelEx(
    wallCubeModel,
    (Vector3){0, 0.0f, 0},
    (Vector3){1,0,0}, 00.0f,
    (Vector3){4.0f, 0.1f, 4.0f},
    WHITE
    );


    DrawModelEx(wallCubeModel,
    (Vector3){0, 1.25f, -2.0f},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){4.0f, 2.5f, 0.1f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){-2.0f, 1.25f, 0},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){0.1f, 2.5f, 4.0f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){2.0f, 1.25f, 0},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){0.1f, 2.5f, 4.0f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){0, 1.25f, 2.0f},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){4.0f, 2.5f, 0.1f},
    WHITE);

    DrawModelEx(wallCubeModel,
    (Vector3){0, 2.5f, 0},
    (Vector3){0,1,0}, 0.0f,
    (Vector3){4.0f, 0.1f, 4.0f},
    WHITE);
   
rlPopMatrix();

}
// ------------------------------------------------------------
// MAIN PROGRAM
// ------------------------------------------------------------
int main(void)
{
    InitWindow(1280, 720, "Cafeteria for What Lives Below");

    float fogSize = 100.0f;   // or whatever size you want


// ------------------------------------------------------------
// GENERATE PROCEDURAL TEXTURES
// ------------------------------------------------------------
//early load textures so we can use them in our procedural materials
texDesk = LoadTexture("textures/TableTex.png");
// -----------------------------
// BUILD DESK TOP TILES
// -----------------------------
Mesh A = GenMeshCube(0.75f, 0.2f, 0.75f);
Mesh B = GenMeshCube(0.75f, 0.2f, 0.75f);
Mesh C = GenMeshCube(0.75f, 0.2f, 0.75f);

float raiseTop = 0.85f;

for (int i = 0; i < A.vertexCount; i++)
    A.vertices[i*3 + 1] += raiseTop;

for (int i = 0; i < B.vertexCount; i++)
    B.vertices[i*3 + 1] += raiseTop;

for (int i = 0; i < C.vertexCount; i++)
    C.vertices[i*3 + 1] += raiseTop;


// Move B right by 0.75
for (int i = 0; i < B.vertexCount; i++)
    B.vertices[i*3 + 0] += 0.75f;

// Move C right + down
for (int i = 0; i < C.vertexCount; i++)
{
    C.vertices[i*3 + 0] += 0.75f;
    C.vertices[i*3 + 2] += 0.75f;
}

// Merge tops
Mesh deskMesh = MergeMeshes(A, B);
deskMesh = MergeMeshes(deskMesh, C);

// -----------------------------
// BUILD LEGS
// -----------------------------
Mesh L1 = GenMeshCube(0.2f, 1.0f, 0.2f);
Mesh L2 = GenMeshCube(0.2f, 1.0f, 0.2f);
Mesh L3 = GenMeshCube(0.2f, 1.0f, 0.2f);
Mesh L4 = GenMeshCube(0.2f, 1.0f, 0.2f);

// Offsets (same as your DrawCornerDesk)
float legH = 0.5f;

for (int i = 0; i < L1.vertexCount; i++)
{
    L1.vertices[i*3 + 0] += -0.3f;
    L1.vertices[i*3 + 1] += legH/2;
    L1.vertices[i*3 + 2] += -0.3f;
}

for (int i = 0; i < L2.vertexCount; i++)
{
    L2.vertices[i*3 + 0] += -0.3f;
    L2.vertices[i*3 + 1] += legH/2;
    L2.vertices[i*3 + 2] +=  0.3f;
}

for (int i = 0; i < L3.vertexCount; i++)
{
    L3.vertices[i*3 + 0] += 0.45f;
    L3.vertices[i*3 + 1] += legH/2;
    L3.vertices[i*3 + 2] += 1.05f;
}

for (int i = 0; i < L4.vertexCount; i++)
{
    L4.vertices[i*3 + 0] += 1.05f;
    L4.vertices[i*3 + 1] += legH/2;
    L4.vertices[i*3 + 2] += 1.05f;
}

// Merge legs into desk
deskMesh = MergeMeshes(deskMesh, L1);
deskMesh = MergeMeshes(deskMesh, L2);
deskMesh = MergeMeshes(deskMesh, L3);
deskMesh = MergeMeshes(deskMesh, L4);

// -----------------------------
// LOAD FINAL DESK MODEL
// -----------------------------
deskModel = LoadModelFromMesh(deskMesh);
deskModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texDesk;
deskModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = (Color){120, 70, 50, 255};

//wall texture

// Load your PNG
TexTableNoise = LoadTexture("textures/TableTop.png");
texWall = LoadTexture("textures/TexWall.png");
Texture2D TexWallUpHalf = LoadTexture("textures/TexWallUpHalf.png");
Texture2D TexWallDownHalf = LoadTexture("textures/TexWalldownhalf.png");
texComp = LoadTexture("textures/texComp.png");
texStatic = LoadTexture("textures/Static.png");


// Build the wall model using the same pattern as the legs
Mesh meshTop = GenMeshCylinder(0.40f, 0.04f, 32);  // radius, height, slices
TableTopModel = LoadModelFromMesh(meshTop);
TableTopModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TexTableNoise;


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

Image fog = GenImagePerlinNoise(256, 256, 0, 0, 8.0f);

// Make it soft gray instead of white
ImageColorTint(&fog, (Color){80, 80, 90, 255});

// Add transparency
for (int y = 0; y < fog.height; y++)
{
    for (int x = 0; x < fog.width; x++)
    {
        Color c = GetImageColor(fog, x, y);
        c.a = 120; // soft alpha
        ImageDrawPixel(&fog, x, y, c);
    }
}

fogTex = LoadTextureFromImage(fog);

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

    // texwall
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model cubeModel = LoadModelFromMesh(cubeMesh);
    cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall;

    // texwall up and down half
    // TOP HALF MODEL
    Model cubeModelUp = LoadModelFromMesh(cubeMesh);
    cubeModelUp.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TexWallUpHalf;

    // BOTTOM HALF MODEL
    Model cubeModelDown = LoadModelFromMesh(cubeMesh);
    cubeModelDown.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TexWallDownHalf;

    //table top model
    TableTopModel = LoadModelFromMesh(meshTop);
    TableTopModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = TexTableNoise;
    // top textures
    Mesh meshDisk = GenMeshCylinder(0.40f, 0.01f, 32);  // height = 0 → flat disk


    // Desk top model
    Mesh deskTopMesh = GenMeshCube(3.0f, 0.2f, 1.5f);
    deskTopModel = LoadModelFromMesh(deskTopMesh);
    deskTopModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texDesk;

    // Desk leg model
    Mesh deskLegMesh = GenMeshCube(0.2f, 1.0f, 0.2f);
    deskLegModel = LoadModelFromMesh(deskLegMesh);
    deskLegModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texDesk;

    // Computer monitor model
    Mesh trapMesh = GenTrap(
    0.5f,   // bottom width
    0.35f,   // top width
    0.05f,   // height
    0.4f,   // bottom depth
    0.2f    // top depth
);

    trapModel = LoadModelFromMesh(trapMesh);
    trapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texComp;



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

    InitCabinetTest();

    while (!IsKeyPressed(KEY_ESCAPE))
    {
        UpdateCamera(&cam, CAMERA_FREE);

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
        ClearBackground(RAYWHITE);ClearBackground((Color){10, 10, 12, 255});   // dead industrial gray
        BeginMode3D(cam);
        
        float globalDim = 0.35f;   // 00.01% brightness
        Color coldTint = { 180, 190, 220, 255 };  // bluish-gray

        
        
        // Apply cold tint + dimming to everything
        rlColor4f(
            (coldTint.r / 255.0f) * globalDim,
            (coldTint.g / 255.0f) * globalDim,
            (coldTint.b / 255.0f) * globalDim,
            1.0f
        );

        //DrawGrid(100, 3.0f, 3.0f, fogColor);
        DrawCustomGrid(100, 1.0f, (Vector3){3.0f, 0.0f, 3.0f}, SKYBLUE);


        DrawCabinetTest();

        DrawComputer((Vector3){1.25f, 0.9f, 5.7f}, 1.0f, 0.0f);//fine
        DrawCornerDesk((Vector3){0.85f, 0.0f, 6.55f}, 90.0f); //fine
        rlPushMatrix();
            rlTranslatef(1.7f, 0.0f, 6.55f);
            rlRotatef(180.0f, 0, 1, 0);
            rlScalef(1.5, 2.0, 1.5);
            DrawCafeChair();
        rlPopMatrix();



        DrawComputer((Vector3){-1.25f, 0.9f, 4.25f}, 1.0f, 180.0f);
        DrawCornerDesk((Vector3){-0.85f, 0.0f, 3.4f}, 270.0f); //fine
        rlPushMatrix();
            rlTranslatef(1.7f, 0.0f, 3.55f);
            rlRotatef(0.0f, 0, 1, 0);
            rlScalef(1.5, 2.0, 1.5);
            DrawCafeChair();
        rlPopMatrix();
        
        DrawComputer((Vector3){1.25f, 0.9f, 4.25f}, 1.0f, 180.0f);
        DrawCornerDesk((Vector3){1.6f, 0.0f, 4.15f}, 180.0f); //fine
        rlPushMatrix();
            rlTranslatef(-1.7f, 0.0f, 3.55f);
            rlRotatef(0.0f, 0, 1, 0);
            rlScalef(1.5, 2.0, 1.5);
            DrawCafeChair();
        rlPopMatrix();
        
        
        DrawComputer((Vector3){-1.25f, 0.9f, 5.7f}, 1.0f, 0.0f); //fine
        DrawCornerDesk((Vector3){-1.55f, 0.0f, 5.85f}, 0.0f); //fine
        rlPushMatrix();
            rlTranslatef(-1.7f, 0.0f, 6.55f);
            rlRotatef(180.0f, 0, 1, 0);
            rlScalef(1.5, 2.0, 1.5);
            DrawCafeChair();
        rlPopMatrix();


if (GetRandomValue(0, 10) == 0)
{
    SpawnSmoke((Vector3){2,  1 + 1.0f, 2});
    }
UpdateSmoke(GetFrameTime());
//if (GeneratorActive)   
//{  
DrawSmoke3D();
//}
DrawGenerator();

DrawCube((Vector3){10, 0, -0.25}, 0.9, 1, 0.7, OTHERGRAY);

        DrawDesk();
        DrawFloorOffice(0.0f, 2.5f, 0.0f);
        DrawFloorOffice(-2.5f, 2.5f, 90.0f);
        DrawBox((Vector3){0.0f, 0.25f, 0.0f}, (Vector3){0, 45, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKERGREEN);
        DrawBox((Vector3){0.0f, 0.25f, 0.6f}, (Vector3){0, 0, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKYELLOW);
        DrawBox((Vector3){0.0f, 0.75f, 0.0f}, (Vector3){0, 30, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKERGREEN);
        DrawBox((Vector3){0.0f, 0.75f, 0.6f}, (Vector3){0, 60, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKYELLOW);
        DrawBox((Vector3){0.0f, 1.25f, 0.0f}, (Vector3){0, 90, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKERGREEN);
        DrawBox((Vector3){1.75f, 0.25f, 1.75f}, (Vector3){0, 0, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKGRAY);
        DrawBox((Vector3){1.75f, 0.25f, -1.75f}, (Vector3){0, 0, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKGRAY);
        DrawBox((Vector3){-1.75f, 0.25f, 1.75f}, (Vector3){0, 0, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKGRAY);
        DrawBox((Vector3){-1.75f, 0.25f, -1.75f}, (Vector3){0, 0, 0}, (Vector3){0.5f, 0.5f, 0.5f}, DARKGRAY);

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
        DrawPaper((Vector3){-1.5f, 1.2f, -1.5f}, 15.0f, 0.8f);
        DrawPaper((Vector3){1.5f, 1.2f, 1.5f}, -15.0f, 0.8f);
        DrawPaper((Vector3){-1.5f, 1.2f, 1.5f}, 30.0f, 0.8f);
        DrawPaper((Vector3){1.5f, 1.2f, -1.5f}, -30.0f, 0.8f);
        DrawPaper((Vector3){0.0f, 0.1f, -2.0f}, 45.0f, 0.8f);
        DrawPaper((Vector3){0.0f, 0.1f, 2.0f}, -45.0f, 0.8f);
        DrawPaper((Vector3){2.0f, 0.1f, 0.0f}, -60.0f, 0.8f);

        // Paper stack
        for (float y = 1.1f; y <= 1.4f; y += 0.01f)
            DrawPaper((Vector3){-0.3f, y, -0.01f}, 15.0f, 0.8f);

        DrawRoom();
        DrawRoom2();
        DrawRoom3();
        DrawRoom4();
        DrawCentralBlock(DARKGRAY, GRAY, wallcolor, cubeModel, cubeModelUp, cubeModelDown);

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
                rlTranslatef(p.x + 0.8f, 0.2f, -0.3f + p.z);
                rlRotatef(0, 0, 1, 0);      // face sideways
                rlRotatef(90, 0, 0, 1);      // tilt sideways (lean)
                DrawCafeChair();
            rlPopMatrix();


            // 118° CHAIR
            rlPushMatrix();
                rlTranslatef(p.x - 0.8f, 0.3f, p.z);
                rlRotatef(90, 0, 1, 0);
                rlRotatef(131, 90, 0, 1);
                DrawCafeChair();
            rlPopMatrix();
        }

// ---- Dusty fog billboard ----
Vector3 forward = ForwardFromCamera(cam);

Vector3 fogPos = {
    cam.position.x + forward.x * 1.0f,
    cam.position.y + forward.y * 1.0f,
    cam.position.z + forward.z * 1.0f
};

Color fogColor = { 10, 10, 11, 60 };

rlSetBlendMode(RL_BLEND_MULTIPLIED);
DrawBillboard(cam, fogTex, fogPos, fogSize, fogColor);
rlSetBlendMode(RL_BLEND_ALPHA);




 



        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }
    
    UnloadTexture(texRust);
    UnloadTexture(texMetal);
    UnloadTexture(texFabric);
    UnloadTexture(texWall);
    UnloadTexture(TexTableNoise);
    UnloadImage(fog);
    UnloadModel(cabinetModel);
    UnloadTexture(texCabinet);
    UnloadModel(TableTopModel);


    CloseWindow();
    return 0;
}
