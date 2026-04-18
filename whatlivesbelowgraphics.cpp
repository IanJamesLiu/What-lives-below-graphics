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
Color OTHERERGRAY = { 90, 90, 90, 255 };
Color MOREGRAY = { 130, 130, 130, 255 };
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
Texture2D pullstartTex;
Texture2D ivStandTex;


bool GeneratorActive = false;
float dripEndY = 0.1f;   // example height

    float doorW = 0.04f;   // thickness
    float doorH = 1.8f;    // height
    float doorD = 0.55f;   // depth

float fogRadius = 20.0f;                  // how far the fog extends
Model wall;
Model TableTopModel;
Model TableTopDisk;
Model cabinetModel;
Model deskTopModel;
Model deskLegModel;
Model trapModel;
Model deskModel;
Model pullstartModel;
Model lockerDoorModel;
Model towelModel;
Model screenModel;
Model staticModel;
Model cubeModel;
Model standModel;
Model seatModel;
Model backModel;

Mesh m;




struct DripW {
    Vector3 pos;
    float speed;
    float alpha;
    bool active;
};



const int MAX_DRIPSW = 2;
DripW dripsW[MAX_DRIPSW];

void SpawnDripW(Vector3 start) {
    for (int i = 0; i < MAX_DRIPSW; i++) {
        if (!dripsW[i].active) {
            dripsW[i].active = true;
            dripsW[i].pos = start;
            dripsW[i].speed = 0.6f;   // slow, eerie fall
            dripsW[i].alpha = 1.0f;
            return;
        }
    }
}

void UpdateDripsW(float dt) {
    for (int i = 0; i < MAX_DRIPSW; i++) {
        if (!dripsW[i].active) continue;

        dripsW[i].pos.y -= dripsW[i].speed * dt;  // falling
        dripsW[i].alpha -= dt * 0.5f;            // fade a bit

        if (dripsW[i].pos.y <= dripEndY || dripsW[i].alpha <= 0.0f)
            dripsW[i].active = false;
    }
}

void DrawDripsW() {
    for (int i = 0; i < MAX_DRIPSW; i++) {
        if (!dripsW[i].active) continue;

        Color c = {50, 50, 200, (unsigned char)(dripsW[i].alpha * 255)};
        DrawSphere(dripsW[i].pos, 0.03f, c);
    }
}

struct Drip {
    Vector3 pos;
    float speed;
    float alpha;
    bool active;
};

struct SmokeParticle {
    Vector3 pos;
    float size;
    float alpha;
    float riseSpeed;
    float growSpeed;
    bool active;
};


const int MAX_DRIPS = 32;
Drip drips[MAX_DRIPS];

void SpawnDrip(Vector3 start) {
    for (int i = 0; i < MAX_DRIPS; i++) {
        if (!drips[i].active) {
            drips[i].active = true;
            drips[i].pos = start;
            drips[i].speed = 0.6f;   // slow, eerie fall
            drips[i].alpha = 1.0f;
            return;
        }
    }
}

void UpdateDrips(float dt) {
    for (int i = 0; i < MAX_DRIPS; i++) {
        if (!drips[i].active) continue;

        drips[i].pos.y -= drips[i].speed * dt;  // falling
        drips[i].alpha -= dt * 0.5f;            // fade a bit

        if (drips[i].pos.y <= dripEndY || drips[i].alpha <= 0.0f)
            drips[i].active = false;
    }
}

void DrawDrips() {
    for (int i = 0; i < MAX_DRIPS; i++) {
        if (!drips[i].active) continue;

        Color c = {200, 50, 50, (unsigned char)(drips[i].alpha * 255)};
        DrawSphere(drips[i].pos, 0.03f, c);
    }
}



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
        // Prevent smoke from ever spawning at the origin
if (pos.x == 0 && pos.y == 0 && pos.z == 0)
    return;

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

void DrawHalfSphereUpper(Vector3 pos, float radius, Color color)
{
    int rings = 16;     // vertical slices
    int slices = 32;    // horizontal slices

    // Draw only the upper hemisphere (0° → 90°)
    for (int i = 0; i < rings/2; i++)
    {
        float lat0 = (PI * i) / rings;
        float lat1 = (PI * (i + 1)) / rings;

        for (int j = 0; j < slices; j++)
        {
            float lon0 = (2 * PI * j) / slices;
            float lon1 = (2 * PI * (j + 1)) / slices;

            Vector3 v1 = {
                pos.x + radius * sinf(lat0) * cosf(lon0),
                pos.y + radius * cosf(lat0),
                pos.z + radius * sinf(lat0) * sinf(lon0)
            };
            Vector3 v2 = {
                pos.x + radius * sinf(lat1) * cosf(lon0),
                pos.y + radius * cosf(lat1),
                pos.z + radius * sinf(lat1) * sinf(lon0)
            };
            Vector3 v3 = {
                pos.x + radius * sinf(lat1) * cosf(lon1),
                pos.y + radius * cosf(lat1),
                pos.z + radius * sinf(lat1) * sinf(lon1)
            };
            Vector3 v4 = {
                pos.x + radius * sinf(lat0) * cosf(lon1),
                pos.y + radius * cosf(lat0),
                pos.z + radius * sinf(lat0) * sinf(lon1)
            };

            DrawTriangle3D(v1, v2, v3, color);
            DrawTriangle3D(v1, v3, v4, color);
        }
    }
}
void initall(void)
{

    Texture2D towelTex = LoadTexture("textures/Towel.png");

    Mesh towelMesh = GenMeshCube(0.02f, 2, 1);
    towelModel = LoadModelFromMesh(towelMesh);
    towelModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = towelTex;

    Texture2D tex = LoadTexture("textures/Locker.png");

    m = GenMeshCube(doorW, doorH, doorD);
    lockerDoorModel = LoadModelFromMesh(m);
    lockerDoorModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;

   Image img = LoadImage("textures/biohazard.png");
   ivStandTex = LoadTextureFromImage(img);
   UnloadImage(img); // we only needed the flipped image for the texture, can free now

   
   Mesh cubeMesh = GenMeshCube(0.125f, 0.125f, 0.0f);
   cubeModel = LoadModelFromMesh(cubeMesh);
   cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ivStandTex;

   pullstartTex = LoadTexture("textures/pullstart.png");

   Mesh m = GenMeshCylinder(0.45f, 0.12f, 32);
   pullstartModel = LoadModelFromMesh(m);
   pullstartModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = pullstartTex;

   // --- SCREEN ---
   Mesh screenMesh = GenMeshCube(0.40f, 0.30f, 0.02f);
   screenModel = LoadModelFromMesh(screenMesh);
   screenModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texWall; // placeholder

   Mesh staticMesh = GenMeshCube(0.40f, 0.30f, 0.0f);
   staticModel = LoadModelFromMesh(staticMesh);
   staticModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texStatic;
      // --- STAND ---
   Mesh standMesh = GenMeshCube(0.10f, 0.20f, 0.10f);
   standModel = LoadModelFromMesh(standMesh);
   standModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texMetal;

    float seatW = 0.45f;
    float seatH = 0.05f;

    float legR = 0.03f;
    float legH = 0.28f;

    float backW = seatW;
    float backH = 0.35f;
    float backT = 0.05f;

    Color seatColor = (Color){120, 70, 50, 255};
    Mesh seatMesh = GenMeshCube(seatW, seatH, seatW);
    seatModel = LoadModelFromMesh(seatMesh);
    seatModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texFabric;

    Color backColor = (Color){100, 60, 45, 255};
    Mesh backMesh = GenMeshCube(backW, backH, backT);
    backModel = LoadModelFromMesh(backMesh);
    backModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texFabric;
}
void DrawTowel(Vector3 towelPos)
{
    Color AIR = { 255, 255, 255, 0 };


    float width = 1.0f;      // total bar width
    float barRadius = 0.03f; // thickness of the bar
    float mountSize = 0.12f; // square wall mounts


    // Left and right mount positions
    Vector3 leftMount  = { towelPos.x - width/2, towelPos.y + 1, towelPos.z };
    Vector3 rightMount = { towelPos.x + width/2, towelPos.y + 1, towelPos.z };

    // --- Wall mounts ---
    DrawCube(leftMount,  mountSize, mountSize, mountSize, MOREGRAY);
    DrawCube(rightMount, mountSize, mountSize, mountSize, MOREGRAY);

    // --- Bar between mounts ---
    Vector3 barStart = { leftMount.x + mountSize/2, towelPos.y + 1, towelPos.z };
    Vector3 barEnd   = { rightMount.x - mountSize/2, towelPos.y + 1, towelPos.z };

    DrawCylinderEx(barStart, barEnd, barRadius, barRadius, 16, OTHERGRAY);

    rlPushMatrix();
        rlTranslatef(towelPos.x, towelPos.y, towelPos.z);
        rlRotatef(180, 1, 0, 0); 
        rlRotatef(90, 0, 1, 0); // rotate so it hangs down
        DrawModel(towelModel, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlPopMatrix();
    // Filler block behind the towel
DrawCube(
    (Vector3){towelPos.x, towelPos.y, towelPos.z - 0.1f},
    1.0f, 2.0f, 0.05f,
    (Color){50, 50, 50, 255}   // dark neutral filler
);

}


void DrawShower(Vector3 pos, Vector3 scale) {
    // -----------------------------
    // DRIPS (spawned here so they fall from the shower head)
    // -----------------------------
    SpawnDripW((Vector3){pos.x + 0.1f, pos.y + scale.y/1.25f, pos.z + 0.35f});
    // Frame corners
    Vector3 p1 = { pos.x - scale.x/2, pos.y, pos.z - scale.z/2 };
    Vector3 p2 = { pos.x + scale.x/2, pos.y, pos.z - scale.z/2 };
    Vector3 p3 = { pos.x + scale.x/2, pos.y, pos.z + scale.z/2 };
    Vector3 p4 = { pos.x - scale.x/2, pos.y, pos.z + scale.z/2 };

    Vector3 p1_top = { p1.x, pos.y + scale.y, p1.z };
    Vector3 p2_top = { p2.x, pos.y + scale.y, p2.z };
    Vector3 p3_top = { p3.x, pos.y + scale.y, p3.z };
    Vector3 p4_top = { p4.x, pos.y + scale.y, p4.z };

    // Draw vertical edges
    DrawLine3D(p1, p1_top, LIGHTGRAY);
    DrawLine3D(p2, p2_top, LIGHTGRAY);
    DrawLine3D(p3, p3_top, LIGHTGRAY);
    DrawLine3D(p4, p4_top, LIGHTGRAY);

    // Draw top edges
    DrawLine3D(p1_top, p2_top, LIGHTGRAY);
    DrawLine3D(p2_top, p3_top, LIGHTGRAY);
    DrawLine3D(p3_top, p4_top, LIGHTGRAY);
    DrawLine3D(p4_top, p1_top, LIGHTGRAY);

    // Draw bottom edges
    DrawLine3D(p1, p2, LIGHTGRAY);
    DrawLine3D(p2, p3, LIGHTGRAY);
    DrawLine3D(p3, p4, LIGHTGRAY);
    DrawLine3D(p4, p1, LIGHTGRAY);


    rlPushMatrix();
        rlTranslatef(pos.x + 0.1, pos.y + 0.5, pos.z - 1.125f + scale.z/2);
        rlScalef(0.75f, 0.75f, 0.75f);
        rlRotatef(30, 1, 0, 0); // tilt shower head forward
 
        // --- Pipe ---
        Vector3 pipeStart = {0, scale.y, scale.z/2 };
        Vector3 pipeEnd   = {0, scale.y - 0.25f,scale.z/2 };
        DrawCylinder(pipeStart, 0.05f, 0.05f, 0.25f, 12, GRAY);

        Vector3 jointPos = { pipeEnd.x, pipeEnd.y - 0.02f, pipeEnd.z };

        // --- Shower head disc ---
        float headRadius = 0.25f * scale.x;
        float headThickness = 0.06f;

        Vector3 headPos = { jointPos.x, jointPos.y - headThickness/2 + 0.2f, jointPos.z };

        DrawCylinder(headPos, headRadius, headRadius, headThickness, 24, GRAY);
        DrawCylinderWires(headPos, headRadius, headRadius, headThickness, 24, DARKGRAY);

        // --- Dome underside ---
        Vector3 domePos = { headPos.x, headPos.y - headThickness/2 + 0.02f, headPos.z };
        DrawHalfSphereUpper(domePos, headRadius * 0.95f, GRAY);
    rlPopMatrix();
}

void DrawSimpleLocker(Vector3 pos, Vector3 rot)
{
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z);
    rlRotatef(rot.x, 1, 0, 0);
    rlRotatef(rot.y, 0, 1, 0);
    rlRotatef(rot.z, 0, 0, 1);

    float w = 0.6f;
    float h = 1.8f;
    float d = 0.55f;

    Color body = (Color){80, 80, 85, 255};
    Color door = (Color){95, 95, 100, 255};

    // Body
    DrawCube((Vector3){pos.x - pos.x, pos.y + h/2 - pos.y, pos.z - pos.z}, w, h, d, body);

    // Door (slightly forward)
    //DrawCube((Vector3){pos.x + w*0.51f, pos.y + h/2, pos.z},
      //       0.04f, h, d*0.98f, door);

    DrawModel(lockerDoorModel,
          (Vector3){pos.x + w*0.51f - pos.x, pos.y + h/2 - pos.y, pos.z - pos.z},
          1.0f,
          WHITE);


    // Handle
    DrawCube((Vector3){pos.x + w*0.55f - pos.x, pos.y + h*0.55f - pos.y, pos.z + d*0.25f - pos.z},
             0.03f, 0.15f, 0.03f, DARKGRAY);

    // Vent slits
    for (int i = 0; i < 4; i++)
    {
        float y = pos.y + h*0.75f - i*0.09f;
        DrawCube((Vector3){pos.x + w*0.52f - pos.x, y - pos.y, pos.z - pos.z},
                 0.01f, 0.02f, d*0.7f, BLACK);
   }
    rlPopMatrix();
}

void DrawIVStand(Vector3 pos)
{
   // -----------------------------
   // Drips (spawned here so they fall from the stand)
   // -----------------------------
   SpawnDrip((Vector3){pos.x - 0.02f, pos.y + 0.5f, pos.z - 0.05f});
   DrawCylinder((Vector3){pos.x - 0.02f, pos.y + 0.09f, pos.z - 0.05f}, 0.125, 0.125, 0.01f, 16, (Color){200, 50, 50, 255});

   // -----------------------------
   // DIMENSIONS
   // -----------------------------
   float poleH = 1.6f;
   float poleR = 0.03f;

   float baseH = 0.05f;
   float baseR = 0.25f;

   float hookLen = 0.18f;
   float hookThick = 0.03f;

   // Bag
   float bagW = 0.20f;
   float bagH = 0.30f;
   float bagD = 0.08f;

   // -----------------------------
   // BASE
   // -----------------------------
   DrawCylinder(
       (Vector3){pos.x, pos.y + baseH/2.0f, pos.z},
       baseR, baseR, baseH,
       16,
       OTHERGRAY
   );

   // -----------------------------
   // POLE 2
   // -----------------------------
   rlPushMatrix();
   rlTranslatef(pos.x - 0.02f, pos.y + baseH + poleH/100.0f + 0.6f, pos.z - 0.05f);
   rlRotatef(3, 1, 0, 0); // rotate so it stands up
   DrawCylinder(
       (Vector3){0, 0, 0},
       poleR, poleR, poleH - 0.95f,
       16,
       OTHERGRAY
   );
   rlPopMatrix();
   // ------------------------------
   // pole
   // ------------------------------
   DrawCylinder(
       (Vector3){pos.x - 0.2f, pos.y + baseH + poleH/100.0f, pos.z - 0.05f},
       poleR, poleR, poleH,
       16,
       MOREGRAY
   );
   // -----------------------------
   // pole 3
   // -----------------------------
   rlPushMatrix();
   rlTranslatef(pos.x - 0.02f, pos.y + baseH + poleH/100.0f + 2.25f - poleH + 0.18, pos.z - 0.0502f);
   rlRotatef(3, 1, 0, 0); // rotate so it stands up
   DrawCylinder(
       (Vector3){0, -0.18f, 0.02f},
       poleR*0, poleR, poleH - 1.95f,
       16,
       OTHERGRAY
   );
   rlPopMatrix();
   // -----------------------------
   // HOOKS
   // -----------------------------
   float hookY = pos.y + baseH + poleH - 0.1f;

   // Left hook
   DrawCube(
       (Vector3){pos.x - hookLen/2.0f, hookY, pos.z - 0.08f},
       hookLen,
       hookThick,
       hookThick,
       MOREGRAY
   );

   // Right hook
   DrawCube(
       (Vector3){pos.x + hookLen/2.0f, hookY, pos.z - 0.08f},
       hookLen,
       hookThick,
       hookThick,
       MOREGRAY
   );

   // -----------------------------
   // SQUARE BAG (stylized)
   // -----------------------------
   rlPushMatrix();
   rlTranslatef(pos.x - bagW/2.0f, hookY - bagH/2.0f - 0.045f, pos.z + 0.08f - 0.13f);
   rlRotatef(180, 1, 0, 0); // rotate so front faces forward
   DrawModel(cubeModel, (Vector3){0, 0, 0}, 1.0f, WHITE);
   rlPopMatrix();

   DrawCube(
       (Vector3){
           pos.x - hookLen/2.0f,   // hangs from left hook
           hookY - bagH/2.0f + 0.013f,
           pos.z
       },
       bagW,
       bagH,
       bagD,
       (Color){200, 60, 60, 255}   // soft red, cartoony
   );
}

void DrawBioHazardBox(Vector3 pos, Vector3 scale)
{
    // Box dimensions
    float w = scale.x;
    float h = scale.y;
    float d = scale.z;
rlPushMatrix();
    rlTranslatef(pos.x, pos.y, pos.z + 0.26f);
    rlRotatef(180, 1, 0, 0); // rotate so front faces forward
    rlScalef(w*2, h*2, d*2);
    DrawModel(cubeModel, (Vector3){0, 0, 0}, 1.0f, WHITE);
rlPopMatrix();
DrawCube((Vector3){pos.x, pos.y, pos.z}, w/2, h/2, d/2, BROWN);
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
   DrawSphere(jointPos, radius, OTHERERGRAY);

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


void DrawRecoilStarter(Vector3 pos, float scale, Color housingColor, Color handleColor)
{

   // -----------------------------
   // MAIN ROUND HOUSING
   // -----------------------------
   float radius = 0.45f * scale;
   float thickness = 0.12f * scale;

   rlPushMatrix();
   rlTranslatef(pos.x, pos.y, pos.z);
   DrawModel(pullstartModel, (Vector3){0,0,0}, scale, WHITE);
   rlPopMatrix();


   // -----------------------------
   // CENTER HUB
   // -----------------------------
   Vector3 hubPos = pos;
   hubPos.z += thickness * 0.55f;

   DrawCylinder(hubPos, radius * 0.25f, radius * 0.25f, thickness * 0.3f, 16, DARKGRAY);

   // -----------------------------
   // VENT BLADES (fake slots)
   // -----------------------------
   for (int i = 0; i < 12; i++)
   {
       rlPushMatrix();
       rlTranslatef(pos.x, pos.y + thickness, pos.z + thickness * 0.6f);
       rlRotatef(90, 1, 0, 0); // tilt vents forward
       rlRotatef(i * (360.0f / 12.0f), 0, 0, 1);

       DrawCube((Vector3){radius * 0.55f, 0, 0}, 
                radius * 0.25f, 
                radius * 0.05f, 
                thickness * 0.1f, 
                BLACK);

       rlPopMatrix();
   }

   // -----------------------------
   // PULL CORD
   // -----------------------------
   Vector3 cordStart = pos;
   cordStart.x += radius * 0.9f;
   cordStart.z += thickness * 0.2f;

   Vector3 cordEnd = cordStart;
   cordEnd.x += 0.4f * scale;

   DrawCylinderEx(cordStart, cordEnd, 0.03f * scale, 0.03f * scale, 8, BLACK);
   
   // -----------------------------
   // HANDLE
   // -----------------------------
   Vector3 handlePos = cordEnd;
   handlePos.x += 0.14f * scale;
   DrawCube(handlePos, 
            0.25f * scale, 
            0.12f * scale, 
            0.12f * scale, 
            handleColor);
}

void DrawGenerator(void)
{
   SpawnSmoke((Vector3){9.67, 1, -0.6f});
   SpawnSmoke((Vector3){9.67f, 0.9f, -0.6f});
   SpawnSmoke((Vector3){9.67f, 0.8f, -0.6f});
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
   rlPushMatrix();
   rlTranslatef(10.5, 0.78, -0.6);
   rlRotatef(90, 0, 0, 1);
   rlRotatef(90, 1, 0, 0);
       DrawTubeJointRotatable(
   (Vector3){0, 0, 0},
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
rlPopMatrix();
   rlPushMatrix();
   rlTranslatef(10, 0.78, 0.0);
   rlRotatef(90, 0, 0, 1);
   rlRotatef(180, 1, 0, 0);
   rlRotatef(180, 1, 0, 0);
       DrawTubeJointRotatable(
   (Vector3){0, 0, 0},
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

rlPopMatrix();

       DrawTubeJointRotatable(
   (Vector3){9.67, 0.19, -0.6},
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
rlPushMatrix();
rlTranslatef(10, 0.5, -1.35);
rlRotatef(90, 1, 0, 0);
rlRotatef(180, 0, 1, 0);
rlRotatef(180, 1, 0, 0);
       DrawRecoilStarter((Vector3){0, 0, 0}, 0.5f, GRAY, RED);
rlPopMatrix();

DrawCylinder((Vector3){9.67, 0.33, -0.6}, 0.25, 0.19, 0.5, 16, DARKGRAY);
DrawSphere((Vector3){10.5, 0.19, -0.6}, 0.19, GRAY);
DrawSphere((Vector3){9.67, 0.19, -0.6}, 0.19, GRAY);
DrawCube((Vector3){10, 0, -0.65}, 0.9, 1.5, 1.4, OTHERGRAY);
}

void DrawMedWingBed(Vector3 pos)
{
   // -----------------------------
   // DIMENSIONS
   // -----------------------------
   float legH      = 0.30f;
   float legR      = 0.05f;

   float frameW    = 1.9f;
   float frameH    = 0.10f;
   float frameD    = 0.75f;

   float mattressH = 0.20f;

   float headH     = 0.45f;
   float headT     = 0.06f;

   // Y positions
   float yLegCenter       = pos.y + legH/2.0f;
   float yFrameCenter     = pos.y + legH + frameH/2.0f;
   float yMattressCenter  = yFrameCenter + frameH/2.0f + mattressH/2.0f;

   // -----------------------------
   // LEGS
   // -----------------------------
   float lx = frameW/2 - 0.15f;
   float lz = frameD/2 - 0.15f;

   Vector3 legs[4] = {
       { pos.x + lx, yLegCenter, pos.z + lz },
       { pos.x - lx, yLegCenter, pos.z + lz },
       { pos.x + lx, yLegCenter, pos.z - lz },
       { pos.x - lx, yLegCenter, pos.z - lz }
   };

   for (int i = 0; i < 4; i++)
       DrawCylinder(legs[i], legR, legR, legH, 16, OTHERERGRAY);

   // -----------------------------
   // FRAME
   // -----------------------------
   DrawCube(
       (Vector3){pos.x, yFrameCenter, pos.z},
       frameW, frameH, frameD,
       OTHERGRAY
   );

   // -----------------------------
   // MATTRESS
   // -----------------------------
   DrawCube(
       (Vector3){pos.x, yMattressCenter, pos.z},
       frameW * 0.95f,
       mattressH,
       frameD * 0.95f,
       (Color){200, 220, 230, 255}
   );

   // -----------------------------
   // HEADBOARD
   // -----------------------------
   DrawCube(
       (Vector3){
           pos.x - frameW/2 + headT/2,
           yFrameCenter + frameH/2.0f + headH/2.0f,
           pos.z
       },
       headT,
       headH,
       frameD,
       MOREGRAY
   );

   // -----------------------------
   // PILLOW
   // -----------------------------
   float pillowW = frameW * 0.40f;
   float pillowD = frameD * 0.55f;
   float pillowH = 0.1f;

   DrawCube(
       (Vector3){
           pos.x - frameW*0.35f,
           yMattressCenter + mattressH/2.0f + pillowH/2.0f,
           pos.z
       },
       pillowD,
       pillowH,
       pillowW,
       (Color){235, 240, 245, 255}
   );

   // -----------------------------
   // BLANKET
   // -----------------------------
   float blanketW = frameW * 0.92f;
   float blanketD = frameD * 0.95f;   // shorter so pillow shows
   float blanketH = 0.06f;

   DrawCube(
       (Vector3){
           pos.x + 0.05f,   // tiny shift so it looks tucked
           yMattressCenter + mattressH/2.0f + blanketH/2.0f,
           pos.z
       },
       blanketW,
       blanketH,
       blanketD,
       (Color){180, 200, 210, 255}   // soft medical blue‑gray
   );
}

void DrawScalpel(Vector3 pos, float rotY)
{
   rlPushMatrix();
       rlTranslatef(pos.x, pos.y, pos.z);
       rlRotatef(rotY, 0, 1, 0);

       // HANDLE
       rlPushMatrix();
           rlTranslatef(-0.10f, 0.0f, 0.0f);
           rlScalef(0.20f, 0.02f, 0.03f);
           DrawCube((Vector3){0,0,0}, 1, 1, 1, (Color){140,140,150,255});
       rlPopMatrix();

       // -------------------------
       // BLADE (ONE triangle)
       // -------------------------
       Color bladeColor = {190, 195, 200, 255};

       Vector3 baseBack  = {0.00f,  0.0f,  0.00f};
       Vector3 baseFront = {0.00f,  0.0f,  0.016f};
       Vector3 tip       = {0.1f, -0.006f, 0.00f};

       DrawTriangle3D(baseBack, baseFront, tip, bladeColor);

   rlPopMatrix();
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

   // --- TRAPEZOID BACK ---
   extern Model trapModel; // your trapezoid model



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

Mesh m = GenMeshCube(0.04f, 1.8f, 0.55f);  // thickness, height, depth

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

    initall();

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
        ClearBackground(RAYWHITE);
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



UpdateSmoke(GetFrameTime());
UpdateDrips(GetFrameTime());
UpdateDripsW(GetFrameTime()/2);
//if (GeneratorActive)   
//{  
DrawSmoke3D();
//continue;
//}
DrawGenerator();

DrawDripsW();

        rlPushMatrix();
            rlTranslatef(-1.0f, 0.0f, 0.0f);
            DrawMedWingBed((Vector3){6.0f, 0.0f, 6.0f});
            DrawMedWingBed((Vector3){6.0f, 0.0f, 4.0f});
            DrawIVStand((Vector3){5.0f, 0.0f, 5.5f});
            DrawDrips();
            DrawScalpel((Vector3){6.5, 0.575, 5}, -30.0f);
            DrawScalpel((Vector3){6.0, 0.575, 5}, 30.0f);
            DrawScalpel((Vector3){5.5, 0.575, 5}, -60.0f);
        rlPopMatrix();
        rlPushMatrix();
        rlTranslatef(2.5, 0, 5);
            rlScalef(0.5, 0.5, 0.5);
            DrawDesk();
        rlPopMatrix();

        rlPushMatrix();
        rlTranslatef(7.5, 0, 5);
        rlScalef(0.5, 0.5, 0.5);
            DrawDesk();
        rlPopMatrix();
        DrawComputer((Vector3){10, 0.5f, 5}, 1.0f, 180.0f);
        rlPushMatrix();
            rlTranslatef(10.0, 0, 4.25);
            rlScalef(1.25, 1.5, 1.25);
            DrawCafeChair();
        rlPopMatrix();

        DrawShower((Vector3){-3.5f, 0.0f, 6.3f}, (Vector3){1.0f, 2.0f, 1.0f});
        DrawShower((Vector3){-6.5f, 0.0f, 6.3f}, (Vector3){1.0f, 2.0f, 1.0f});
        rlPushMatrix();
        rlTranslatef(-3.5f, 1.0f, 3.8f);
        rlRotatef(0.0f, 0, 1, 0);
        DrawTowel((Vector3){0, 0, -0.4f});
        rlPopMatrix();
        DrawTowel((Vector3){-6.5f, 1.0f, 3.4f});

        DrawSimpleLocker((Vector3){-1.5f, 0, 8.5f}, (Vector3){0, 0, 0});
        DrawSimpleLocker((Vector3){1.5f, 0, 8.5f}, (Vector3){0, 180, 0});
        DrawSimpleLocker((Vector3){-1.5f, 0, 11.5f}, (Vector3){0, 0, 0});
        DrawSimpleLocker((Vector3){1.5f, 0, 11.5f}, (Vector3){0, 180, 0});

        DrawSimpleLocker((Vector3){3.5f, 0, 11.5f}, (Vector3){0, 0, 0});
        DrawSimpleLocker((Vector3){6.5f, 0, 11.5f}, (Vector3){0, 180, 0});
        DrawSimpleLocker((Vector3){3.5f, 0, 8.5f}, (Vector3){0, 0, 0});
        DrawBioHazardBox((Vector3){3.75f, 2, 8.75f}, (Vector3){1, 1, 1});
        SpawnDrip((Vector3){5, 1.75, 9});
        DrawSimpleLocker((Vector3){6.5f, 0, 8.5f}, (Vector3){0, 180, 0});
        rlPushMatrix();
        rlTranslatef(2.5, 0, 10);
        rlScalef(0.5, 0.75, 0.5);
        DrawDesk();
        rlPopMatrix();
        


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
        rlPushMatrix();
            rlTranslatef(5, 0, 0.0f);
            DrawRoom2();
        rlPopMatrix();
        rlPushMatrix();
            rlTranslatef(0, 0, 5);
            DrawRoom2();
        rlPopMatrix();
        DrawRoom3();
        DrawRoom4();
        rlPushMatrix();
            rlTranslatef(0, 0, 5);
            DrawRoom4();
        rlPopMatrix();
        rlPushMatrix();
            rlTranslatef(10.0f, 0.0f, 5.0f);
            DrawRoom();
        rlPopMatrix();
        rlPushMatrix();
            rlTranslatef(0, 0, 5);
            DrawRoom3();
        rlPopMatrix();
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

        BeginMode2D((Camera2D){ .zoom = 1.0f });

        DrawText("Use North, South, East, and West to move around", 50, 50, 20, WHITE);

        DrawText("Press ESC to exit", 50, 80, 20, WHITE);

        DrawRectangle(150, 650, 300, 75, (Color){128, 128, 128, 150});
        DrawText("South", 275, 675, 20, BLACK);
        // Define here when done with graphics

        DrawRectangle(150, 500, 300, 75, (Color){128, 128, 128, 150});
        DrawText("North", 275, 525, 20, BLACK);
        // Define here when done with graphics

        DrawRectangle(0, 575, 300, 75, (Color){140, 140, 140, 150});
        DrawText("West", 125, 600, 20, BLACK);
        // Define here when done with graphics

        DrawRectangle(300, 575, 300, 75, (Color){140, 140, 140, 150});
        DrawText("East", 375, 600, 20, BLACK);
        // Define here when done with graphics

        DrawRectangle(880, 550, 400, 200, (Color){0, 0, 0, 150});
        DrawText("HIDE", 1025, 625, 40, WHITE);

        DrawRectangle(880, 350, 400, 200, (Color){255, 255, 255, 150});
        DrawText("UNHIDE", 1005, 425, 40, BLACK);

        DrawRectangle(500, 480, 300, 75, (Color){128, 128, 128, 150});
        DrawText("Search", 625, 505, 20, BLACK);

        EndMode2D();
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
