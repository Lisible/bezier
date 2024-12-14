#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>

#define LOG(...)                                                               \
  do {                                                                         \
    fprintf(stderr, __VA_ARGS__);                                              \
    fprintf(stderr, "\n");                                                     \
  } while (0)

#define QUADRATIC_BEZIER_CURVE_CONTROL_POINT_COUNT 4
#define CONTROL_POINT_RECTANGLE_SIZE 20.f

struct vec2 {
  float x;
  float y;
};
struct vec2 vec2_add(struct vec2 lhs, struct vec2 rhs) {
  return (struct vec2){.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
}

struct vec2 vec2_mul_scalar(struct vec2 lhs, float rhs) {
  return (struct vec2){.x = lhs.x * rhs, .y = lhs.y * rhs};
}

struct vec2 bezier_quadratic(
    float t, const struct vec2
                 control_points[QUADRATIC_BEZIER_CURVE_CONTROL_POINT_COUNT]) {

  struct vec2 a = vec2_mul_scalar(control_points[0], powf(1.f - t, 3.f));
  struct vec2 b =
      vec2_mul_scalar(control_points[1], 3 * powf(1.f - t, 2.f) * t);
  struct vec2 c = vec2_mul_scalar(control_points[2], 3 * (1.f - t) * t * t);
  struct vec2 d = vec2_mul_scalar(control_points[3], t * t * t);
  return vec2_add(a, vec2_add(b, vec2_add(c, d)));
}

struct state {
  struct vec2 control_points[QUADRATIC_BEZIER_CURVE_CONTROL_POINT_COUNT];
  int set_control_point_count;
};
void state_render(const struct state *state, SDL_Renderer *renderer) {
  for (int set_control_point_index = 0;
       set_control_point_index < state->set_control_point_count;
       set_control_point_index++) {
    const struct vec2 *set_control_point =
        &state->control_points[set_control_point_index];
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(
        renderer,
        &(const SDL_FRect){
            set_control_point->x - CONTROL_POINT_RECTANGLE_SIZE / 2.f,
            set_control_point->y - CONTROL_POINT_RECTANGLE_SIZE / 2.f,
            CONTROL_POINT_RECTANGLE_SIZE, CONTROL_POINT_RECTANGLE_SIZE});
  }

  if (state->set_control_point_count ==
      QUADRATIC_BEZIER_CURVE_CONTROL_POINT_COUNT) {
    float prev_t = 0.f;
    for (int ti = 0; ti < 100; ti += 1) {
      float t = ti / 100.f;
      struct vec2 first_point = bezier_quadratic(prev_t, state->control_points);
      struct vec2 second_point = bezier_quadratic(t, state->control_points);
      SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
      SDL_RenderLine(renderer, first_point.x, first_point.y, second_point.x,
                     second_point.y);
      prev_t = t;
    }
  }
}
void state_on_click(struct state *state, struct vec2 position) {
  if (state->set_control_point_count <
      QUADRATIC_BEZIER_CURVE_CONTROL_POINT_COUNT) {
    state->control_points[state->set_control_point_count++] = position;
  }
}

int main(void) {
  bool sdl_initialized = SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = NULL;
  if (sdl_initialized) {
    window = SDL_CreateWindow("bezier", 1280, 720, 0);
  }

  SDL_Renderer *renderer = NULL;
  if (window) {
    renderer = SDL_CreateRenderer(window, NULL);
  }

  struct state state = {0};

  if (renderer) {
    bool running = true;
    while (running) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        bool quit = event.type == SDL_EVENT_QUIT;
        bool escape_pressed =
            event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE;

        if (quit || escape_pressed) {
          running = false;
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                   event.button.button == SDL_BUTTON_LEFT) {
          state_on_click(
              &state, (struct vec2){.x = event.button.x, .y = event.button.y});
        }
      }

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderClear(renderer);
      state_render(&state, renderer);
      SDL_RenderPresent(renderer);
    }
  }

  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }
  if (sdl_initialized) {
    SDL_Quit();
  }

  return 0;
}
