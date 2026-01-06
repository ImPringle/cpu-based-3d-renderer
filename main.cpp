#include <stdio.h>
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "./constants.h"

struct Point2D {
  float x;
  float y;
};

struct Point3D {
  float x;
  float y;
  float z;
};

struct Line2D {
  float x1;
  float y1;
  float x2;
  float y2;
};

struct Line3D {
  float x1;
  float y1;
  float z1;
  float x2;
  float y2;
  float z2;
};

Point2D point;
Line2D line;
int lastFrameTime = 0;
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int initialize_window() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "[ ERROR ] Error initializing SDL." << std::endl;
    return FALSE;
  }

  window = SDL_CreateWindow(
    "3D Projection",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (!window) {
    std::cout << "[ ERROR ] Error creating SDL window." << std::endl;
    return FALSE;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cout << "[ ERROR ] Error creating SDL renderer." << std::endl;
    return FALSE;
  }

  return TRUE;
}

void process_input() {
  SDL_Event event;
  //SDL_PollEvent(&event);
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        game_is_running = FALSE;
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          game_is_running = FALSE;
        }
        break;
    }
  }
}

Point2D project(const Point3D& p) {
  return Point2D{
    x: p.x / p.z,
    y: p.y / p.z
  };
}

Point3D translateZ(Point3D p, float dz) {
  return Point3D {
    x: p.x,
    y: p.y,
    z: p.z + dz
  };
}

Point3D rotateXZ(Point3D p, float angle) {
  const float c = std::cos(angle);
  const float s = std::sin(angle);
  return Point3D{
    x: p.x * c - p.z * s,
    y: p.y,
    z: p.x * s + p.z * c
  };
}

std::vector<Point2D> projectVertices(const std::vector<Point3D> vertices, float angle, float dz) {
  std::vector<Point2D> res;
  for (Point3D v : vertices) {
    res.push_back(project(translateZ(rotateXZ(v, angle),dz)));
  }
  return res;
}

void setup(std::vector<Point3D>& vertices) {
  vertices[0].x = -0.5;
  vertices[0].y = -0.5;
  vertices[0].z = -0.5;

  vertices[1].x = -0.5;
  vertices[1].y = 0.5;
  vertices[1].z = -0.5;

  vertices[2].x = 0.5;
  vertices[2].y = 0.5;
  vertices[2].z = -0.5;

  vertices[3].x = 0.5;
  vertices[3].y = -0.5;
  vertices[3].z = -0.5;

  vertices[4].x = -0.5;
  vertices[4].y = -0.5;
  vertices[4].z = 0.5;

  vertices[5].x = -0.5;
  vertices[5].y = 0.5;
  vertices[5].z = 0.5;

  vertices[6].x = 0.5;
  vertices[6].y = 0.5;
  vertices[6].z = 0.5;

  vertices[7].x = 0.5;
  vertices[7].y = -0.5;
  vertices[7].z = 0.5;
}

float toScreenX(float x) {
  return (x + 1.0f) / 2.0f * WINDOW_WIDTH;
}

float toScreenY(float y) {
  return (1.0f - (y + 1.0f) / 2.0f) * WINDOW_HEIGHT;
}

void update(float& angle, float& dz) {
  int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
    SDL_Delay(timeToWait);
  }
  lastFrameTime = SDL_GetTicks();

  //dz += 0.01f;
  angle += 0.01f;
}

void render(std::vector<Point3D> vertices, float angle, float dz) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  std::vector<Point2D> pv = projectVertices(vertices, angle, dz);

  //for (Point2D point : pv) {
    //Point2D point = project(translateZ(rotateXZ(p, angle),dz));
    // Render all the vertices
    //SDL_Rect pRect = {
    //  toScreenX(point.x) - (8 / 2),
    //  toScreenY(point.y) - (8 / 2),
    //  8,
    //  8
    //};

    //SDL_RenderFillRect(renderer, &pRect);
  //}

  //SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
  SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
  // lines
  SDL_RenderDrawLine(renderer, toScreenX(pv[0].x), toScreenY(pv[0].y), toScreenX(pv[1].x), toScreenY(pv[1].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[0].x), toScreenY(pv[0].y), toScreenX(pv[4].x), toScreenY(pv[4].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[0].x), toScreenY(pv[0].y), toScreenX(pv[3].x), toScreenY(pv[3].y));

  SDL_RenderDrawLine(renderer, toScreenX(pv[6].x), toScreenY(pv[6].y), toScreenX(pv[2].x), toScreenY(pv[2].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[6].x), toScreenY(pv[6].y), toScreenX(pv[5].x), toScreenY(pv[5].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[6].x), toScreenY(pv[6].y), toScreenX(pv[7].x), toScreenY(pv[7].y));

  SDL_RenderDrawLine(renderer, toScreenX(pv[2].x), toScreenY(pv[2].y), toScreenX(pv[1].x), toScreenY(pv[1].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[2].x), toScreenY(pv[2].y), toScreenX(pv[3].x), toScreenY(pv[3].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[1].x), toScreenY(pv[1].y), toScreenX(pv[5].x), toScreenY(pv[5].y));

  SDL_RenderDrawLine(renderer, toScreenX(pv[7].x), toScreenY(pv[7].y), toScreenX(pv[4].x), toScreenY(pv[4].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[7].x), toScreenY(pv[7].y), toScreenX(pv[3].x), toScreenY(pv[3].y));
  SDL_RenderDrawLine(renderer, toScreenX(pv[5].x), toScreenY(pv[5].y), toScreenX(pv[4].x), toScreenY(pv[4].y));

  SDL_RenderPresent(renderer);
}

void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
  game_is_running = initialize_window();

  float dz = 2;
  float angle = 0;

  std::vector<Point3D> vertices(8);
  
  setup(vertices);

  while (game_is_running) {
    process_input();
    update(angle, dz);
    render(vertices, angle, dz);
  }

  destroy_window();

  return 0;
}
