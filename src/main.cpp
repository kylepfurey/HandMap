// .hpp
// Hand Gesture Mapping Visualizer
// by Kyle Furey

#include <iostream>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <fast-cpp-csv-parser/csv.h>
#include <opencv2/core.hpp>
#include <openpose/headers.hpp>
#include <DataFrame.hpp>
#include <Hand.hpp>

using namespace std;
using namespace glm;
using namespace io;
using namespace cv;
using namespace op;
using namespace handmap;

/** Constants */
namespace handmap {
    static constexpr float WINDOW_WIDTH = 1920.0f;
    static constexpr float WINDOW_HEIGHT = 1080.0f;
    static constexpr char FONT_NAME[] = "calibri.ttf";
    static constexpr int FONT_SIZE = 64;
}

/** Demo program */
int main(int argc, char *argv[]) {
    // Load each column from the CSV
    CSVReader<DataFrame::COLUMN_COUNT> csv(DataFrame::CSV_NAME);
    DataFrame data{};
    csv.read_header(ignore_no_column, HANDMAP_DATAFRAME_HEADER);

    // Iterate each row
    while (csv.read_row(HANDMAP_DATAFRAME_EXPAND(data))) {
        // TODO - Build tree data structure
    }

    // Init SDL window, renderer, camera, and font
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CAMERA);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow(
        "Hand Map",
        static_cast<int>(WINDOW_WIDTH),
        static_cast<int>(WINDOW_HEIGHT),
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    SDL_CameraID *cameras = SDL_GetCameras(nullptr);
    SDL_Camera *camera = SDL_OpenCamera(cameras[0], nullptr);
    SDL_free(cameras);
    SDL_Texture *camera_texture = nullptr;
    TTF_Font *font = TTF_OpenFont(FONT_NAME, FONT_SIZE);

    // Initialize openpose
    Wrapper openpose{ThreadManagerMode::Asynchronous};
    WrapperStructPose op_pose;
    op_pose.poseMode = PoseMode::Disabled;
    WrapperStructHand op_hand;
    op_hand.enable = true;
    op_hand.detector = Detector::Provided;
    openpose.configure(op_pose);
    openpose.configure(op_hand);
    openpose.start();
    Hand left_hand{};
    Hand right_hand{};

    // Event loop
    bool run = true;
    float camera_width = 1.0f;
    float camera_height = 1.0f;
    bool capture = false;
    SDL_Event event;
    while (run) {
        // Handle input
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                default:
                    break;
                case SDL_EVENT_QUIT:
                    run = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.key) {
                        default:
                            break;
                        case SDLK_ESCAPE:
                            run = false;
                            break;
                        case SDLK_SPACE:
                            capture = true;
                            break;
                    }
                    break;
            }
        }

        // Get window scale info
        int temp_width;
        int temp_height;
        SDL_GetWindowSize(window, &temp_width, &temp_height);
        float window_width = static_cast<float>(temp_width);
        float window_height = static_cast<float>(temp_height);
        float width_ratio = static_cast<float>(temp_width) / WINDOW_WIDTH;
        float height_ratio = static_cast<float>(temp_height) / WINDOW_HEIGHT;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render camera frame to screen
        SDL_Surface *camera_surface = SDL_AcquireCameraFrame(camera, nullptr);
        if (camera_surface != nullptr) {
            camera_width = static_cast<float>(camera_surface->w);
            camera_height = static_cast<float>(camera_surface->h);
            if (camera_texture != nullptr) {
                SDL_DestroyTexture(camera_texture);
            }
            camera_texture = SDL_CreateTextureFromSurface(renderer, camera_surface);
            // Scan for hands on screen
            if (capture) {
                capture = false;
                Hand::scan(openpose, camera_surface, left_hand, right_hand);
            }
            SDL_ReleaseCameraFrame(camera, camera_surface);
        }
        if (camera_texture != nullptr) {
            SDL_RenderTexture(renderer, camera_texture, nullptr, nullptr);
        }

        // Render left and right hand joints to the screen
        auto draw_hand = [=](const Hand &hand) {
            SDL_RenderLine(renderer, hand.wrist.x, hand.wrist.y, hand.thumb[0].x, hand.thumb[0].y);
            SDL_RenderLine(renderer, hand.wrist.x, hand.wrist.y, hand.index[0].x, hand.index[0].y);
            SDL_RenderLine(renderer, hand.wrist.x, hand.wrist.y, hand.middle[0].x, hand.middle[0].y);
            SDL_RenderLine(renderer, hand.wrist.x, hand.wrist.y, hand.ring[0].x, hand.ring[0].y);
            SDL_RenderLine(renderer, hand.wrist.x, hand.wrist.y, hand.pinky[0].x, hand.pinky[0].y);
            for (int i = 0; i < 3; ++i) {
                SDL_RenderLine(renderer, hand.thumb[i].x, hand.thumb[i].y, hand.thumb[i + 1].x, hand.thumb[i + 1].y);
                SDL_RenderLine(renderer, hand.index[i].x, hand.index[i].y, hand.index[i + 1].x, hand.index[i + 1].y);
                SDL_RenderLine(renderer, hand.middle[i].x, hand.middle[i].y, hand.middle[i + 1].x, hand.middle[i + 1].y);
                SDL_RenderLine(renderer, hand.ring[i].x, hand.ring[i].y, hand.ring[i + 1].x, hand.ring[i + 1].y);
                SDL_RenderLine(renderer, hand.pinky[i].x, hand.pinky[i].y, hand.pinky[i + 1].x, hand.pinky[i + 1].y);
            }
        };
        if (left_hand.valid) {
            SDL_SetRenderScale(renderer, window_width / camera_width, window_height / camera_height);
            SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 1.0f, 1.0f);
            draw_hand(left_hand);
            SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        }
        if (right_hand.valid) {
            SDL_SetRenderScale(renderer, window_width / camera_width, window_height / camera_height);
            SDL_SetRenderDrawColorFloat(renderer, 1.0f, 0.0f, 0.0f, 1.0f);
            draw_hand(right_hand);
            SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        }

        // Render gesture name to screen
        stringstream text;
        if (left_hand.valid) {
            if (right_hand.valid) {
                text << "Both Hands Found"; // TODO - Replace with gesture names
            } else {
                text << "Left Hand Found"; // TODO - Replace with gesture names
            }
        } else if (right_hand.valid) {
            text << "Right Hand Found"; // TODO - Replace with gesture names
        } else {
            text << "No Hands On Screen";
        }
        SDL_Surface *text_surface = TTF_RenderText_Blended(
            font,
            text.str().c_str(),
            0,
            SDL_Color{255, 255, 255, 255}
        );
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        float text_width = static_cast<float>(text_surface->w);
        float text_height = static_cast<float>(text_surface->h);
        SDL_FRect text_rect{
            (window_width - text_width * width_ratio) / 2.0f,
            0.0f,
            text_width * width_ratio,
            text_height * height_ratio
        };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &text_rect);
        SDL_DestroySurface(text_surface);
        SDL_RenderTexture(renderer, text_texture, nullptr, &text_rect);
        SDL_DestroyTexture(text_texture);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    if (camera_texture != nullptr) {
        SDL_DestroyTexture(camera_texture);
    }
    TTF_CloseFont(font);
    SDL_CloseCamera(camera);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
