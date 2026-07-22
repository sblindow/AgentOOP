#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "components/InputState.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "components/MoveDir.hpp"
#include "components/Grounded.hpp"
#include "components/InputState.hpp"
#include "components/LookRotation.hpp"


#include "core/ECSCoordinator.hpp"
#include "Player.hpp"
#include "systems/MovementSystem.hpp"

//============================================================
// Defines and Macros
//============================================================

// Responsiveness for turning movement direction to looked direction
#define CROUCH_HEIGHT 0.0f
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f

#define NORMALIZE_INPUT 0

#define GLSL_VERSION 330 // Platform Desktop

//============================================================
// Types and Structs
//============================================================


//============================================================
// Global Variables Definition
//============================================================

static Vector2 sensitivity = {0.001f, 0.001f};

static float headTimer = 0.0f;
static float walkLerp = 0.0f;
static float headLerp = STAND_HEIGHT;
static Vector2 lean = {0};

//============================================================
// Module Functions Declaration
//============================================================

static void DrawLevel(Model levelModel);
static void UpdateCameraFPS(Camera *camera, Vector2& lookRotation);

// Draw game level

static void DrawLevel(Model levelModel) {

  DrawModel(levelModel, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, GRAY);
  
}

//============================================================
// Program main entry point
//============================================================

int main(void) {

  // Initialization
  // ----------------------------------------------------
  const int screenWidth = 1280;
  const int screenHeight = 720;

  SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable MultiSampling Anti Aliasing 4x (if available)

  InitWindow(screenWidth, screenHeight, "game window");

  

  DisableCursor(); // Limit cursor to relative movement inside the window

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  // Initialize ECS
  //
  core::ECSCoordinator coordinator;

  // register component types

  coordinator.registerComponent<Position>();
  coordinator.registerComponent<Velocity>();
  coordinator.registerComponent<MoveDir>();
  coordinator.registerComponent<Grounded>();
  coordinator.registerComponent<InputState>();
  coordinator.registerComponent<LookRotation>();

  // register systems

  coordinator.registerSystem<MovementSystem>();

  // register and create player

  game::Player player;
  player.createPlayer(coordinator);
  
  // Initialize camera variables
  // Note: UpdateCameraFPS() takes care of the rest
  Camera camera = {0};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  auto& playerPos = coordinator.getComponent<Position>(player.getPlayerID()).value;
  
  camera.position = (Vector3){
      playerPos.x,
      playerPos.y + (BOTTOM_HEIGHT + headLerp),
      playerPos.z,
  };

  
  UpdateCameraFPS(&camera, coordinator.getComponent<LookRotation>(player.getPlayerID()).rotation); // Update camera parameters
   
  // ----------------------------------------------------
  // Test mit manuell gesetztem Input


  // Load basic lighting shader
  Shader shader = LoadShader("../resources/shaders/glsl330/lighting.vs",
                             "../resources/shaders/glsl330/lighting.fs");
  // Get some required shader locations
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

  // Ambient light level (some basic lighting)
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f}, SHADER_UNIFORM_VEC4);

  auto& pos = coordinator.getComponent<Position>(player.getPlayerID());

  

  Model levelModel = LoadModel("../assets/levelmesh.glb");
  if (levelModel.meshCount == 0) {
    TraceLog(LOG_WARNING, "Level model failed to load!");
  }
  
  // Main Game Loop

  while (WindowShouldClose() == false) {

    float delta = GetFrameTime();

    // Update
    // ---------------------------------------------------------

    // write input into player-entity input-component
    //
    //
    // --- Eingabe einsammeln und in Components schreiben ---
    
    Vector2 mouseDelta = GetMouseDelta();

    auto& rot = coordinator.getComponent<LookRotation>(player.getPlayerID()).rotation;
    rot.x -= mouseDelta.x * sensitivity.x;
    rot.y += mouseDelta.y * sensitivity.y;

    auto& input = coordinator.getComponent<InputState>(player.getPlayerID());
    input.sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    input.forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    input.jumpPressed = IsKeyPressed(KEY_SPACE);
    input.crouchHeld = IsKeyDown(KEY_C);

    // --- ECS System ausführen ---
    
    coordinator.update(delta);

    // --- Kamera aus aktualisierten Components nachziehen ---

    auto& pos = coordinator.getComponent<Position>(player.getPlayerID()).value;
    bool grounded = coordinator.getComponent<Grounded>(player.getPlayerID()).isGrounded;
        
    headLerp = Lerp(headLerp, (input.crouchHeld ? CROUCH_HEIGHT : STAND_HEIGHT),
                    20.0f * delta);
    camera.position = (Vector3){
        pos.x,
        pos.y + (BOTTOM_HEIGHT + headLerp),
        pos.z,
    };

    if (grounded && ((input.forward != 0) || (input.sideway != 0))) {
      headTimer += delta * 3.0f;
      walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
      camera.fovy = Lerp(camera.fovy, 55.0f, 5.0f * delta);
    } else {
      walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
      camera.fovy = Lerp(camera.fovy, 60.0f, 5.0f * delta);
    }

    lean.x = Lerp(lean.x, input.sideway * 0.02f, 10.0f * delta);
    lean.y = Lerp(lean.y, input.forward * 0.015f, 10.0f * delta);

  

    UpdateCameraFPS(&camera,rot);
    // ---------------------------------------------------------

    // Draw
    // --------------------------------------------------------
    BeginDrawing();

      ClearBackground(RAYWHITE);
       
      BeginMode3D(camera);

        BeginShaderMode(shader);
        
          DrawLevel(levelModel);
          
        EndShaderMode();
      
      EndMode3D();

      DrawFPS(10,10);

    EndDrawing();
    // ----------------------------------------------------------
  }

  // De-Initialization
  // -----------------------------------------------------------
  UnloadModel(levelModel);
  UnloadShader(shader);
  
  CloseWindow();
  // -----------------------------------------------------------

  return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Update camera for FPS behavior
static void UpdateCameraFPS(Camera *camera, Vector2& lookRotation) {
  const Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};
  const Vector3 targetOffset = (Vector3){0.0f, 0.0f, -1.0f};

  // Left and right
  Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

  // Clamp view up
  float maxAngleUp = Vector3Angle(up, yaw);
  maxAngleUp -= 0.001f; // Avoid numerical errors
  if (-(lookRotation.y) > maxAngleUp) {
    lookRotation.y = -maxAngleUp;
  }

  // Clamp view down
  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f;  // Downwards angle is negative
  maxAngleDown += 0.001f; // Avoid numerical errors
  if (-(lookRotation.y) < maxAngleDown) {
    lookRotation.y = -maxAngleDown;
  }

  // Up and down
  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  // Rotate view vector around right axis
  float pitchAngle = -lookRotation.y - lean.y;
  pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f,
                     PI / 2 - 0.0001f); // Clamp angle so it doesn't go past
                                        // straight up or straight down
  Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

  // Head animation
  // Rotate up direction around forward axis
  float headSin = sinf(headTimer * PI);
  float headCos = cosf(headTimer * PI);
  const float stepRotation = 0.01f;
  camera->up =
      Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);

  // Camera BOB
  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
  bobbing.y = fabsf(headCos * bobUp);

  camera->position =
      Vector3Add(camera->position, Vector3Scale(bobbing, walkLerp));
  camera->target = Vector3Add(camera->position, pitch);
}
