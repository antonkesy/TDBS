#include "project.h"

#include <optional>
#include <thread>
#include <vector>

#include "fmt/format.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Color addColor = RED;

struct Particle {
  Color color;
};

struct Pixel {
  std::optional<Particle> particle{};
};

constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;
constexpr int PARTICLE_SIZE = 10;

const auto width = screenWidth / PARTICLE_SIZE;
const auto height = screenHeight / PARTICLE_SIZE;

std::vector<std::vector<Pixel>> pixels(screenWidth / PARTICLE_SIZE,
                                       std::vector<Pixel>(screenHeight /
                                                          PARTICLE_SIZE));

Color getRandomColor() {
  return Color{static_cast<unsigned char>(GetRandomValue(0, 255)),
               static_cast<unsigned char>(GetRandomValue(0, 255)),
               static_cast<unsigned char>(GetRandomValue(0, 255)), 255};
}

const Rectangle buttonRec = {50, 50, 150, 30};

void inputThread() {
  while (!WindowShouldClose()) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      int mouseX = GetMouseX();
      int mouseY = GetMouseY();

      if (CheckCollisionPointRec((Vector2){mouseX, mouseY}, buttonRec)) {
        continue;
      }

      if (mouseX >= 0 && mouseX < screenWidth && mouseY >= 0 &&
          mouseY < screenHeight) {
        const int particleX = mouseX / PARTICLE_SIZE;
        const int particleY = mouseY / PARTICLE_SIZE;

        if (!pixels[particleX][particleY].particle.has_value()) {
          pixels[particleX][particleY].particle = Particle{addColor};
        }
      }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      int mouseX = GetMouseX();
      int mouseY = GetMouseY();

      if (CheckCollisionPointRec((Vector2){mouseX, mouseY}, buttonRec)) {
        continue;
      }

      if (mouseX >= 0 && mouseX < screenWidth && mouseY >= 0 &&
          mouseY < screenHeight) {
        const int particleX = mouseX / PARTICLE_SIZE;
        const int particleY = mouseY / PARTICLE_SIZE;

        if (pixels[particleX][particleY].particle.has_value()) {
          pixels[particleX][particleY].particle = std::nullopt;
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void updateFallingSand() {
  for (int particleY = height - 1; particleY >= 0; --particleY) {
    for (int particleX = width - 1; particleX >= 0; --particleX) {
      if (pixels[particleX][particleY].particle.has_value()) {
        const auto isLastRow = particleY == height - 1;
        if (isLastRow) {
          continue;
        }

        const auto isBelowEmpty =
            !pixels[particleX][particleY + 1].particle.has_value();
        if (isBelowEmpty) {
          pixels[particleX][particleY + 1].particle =
              pixels[particleX][particleY].particle;
          pixels[particleX][particleY].particle = std::nullopt;
          continue;
        }

        const auto fallDownLeft = [&]() {
          const auto isLeftEmpty =
              particleX > 0 &&
              !pixels[particleX - 1][particleY + 1].particle.has_value();
          if (isLeftEmpty) {
            pixels[particleX - 1][particleY + 1].particle =
                pixels[particleX][particleY].particle;
            pixels[particleX][particleY].particle = std::nullopt;
          }
        };

        const auto fallDownRight = [&]() {
          const auto isRightEmpty =
              particleX < width - 1 &&
              !pixels[particleX + 1][particleY + 1].particle.has_value();

          if (isRightEmpty) {
            pixels[particleX + 1][particleY + 1].particle =
                pixels[particleX][particleY].particle;
            pixels[particleX][particleY].particle = std::nullopt;
          }
        };

        const auto randomDirection = GetRandomValue(0, 1);
        if (randomDirection == 0) {
          fallDownLeft();
          fallDownRight();
        } else {
          fallDownRight();
          fallDownLeft();
        }
      }
    }
  }
}
std::string ColorToString(Color color) {
  return fmt::format("Color({},{},{},{})", color.r, color.g, color.b, color.a);
}

void Project::testRun() const {
  InitWindow(screenWidth, screenHeight, "Falling Sand");

  std::thread inputWorker(inputThread);

  const auto updateInterval = std::chrono::milliseconds(50);

  auto lastUpdate = std::chrono::high_resolution_clock::now();
  while (!WindowShouldClose()) {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto deltaTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime -
                                                              lastUpdate);
    if (deltaTime > updateInterval) {
      lastUpdate = currentTime;
      updateFallingSand();
    }

    if (GuiButton(buttonRec,
                  fmt::format("#191#: {}", ColorToString(addColor)).c_str())) {
      addColor = getRandomColor();
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < height; ++j) {
        if (pixels[i][j].particle.has_value()) {
          Color color = pixels[i][j].particle->color;

          DrawRectangle(i * PARTICLE_SIZE, j * PARTICLE_SIZE, PARTICLE_SIZE,
                        PARTICLE_SIZE, color);
        }
      }
    }

    DrawFPS(10, 10);
    EndDrawing();

    SwapScreenBuffer();
  }

  inputWorker.join();
  CloseWindow();
}
