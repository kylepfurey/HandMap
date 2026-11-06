// .hpp
// Hand Gesture Mapping Hand Data
// by Kyle Furey

#pragma once
#include <memory>
#include <array>
#include <vector>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <opencv2/core.hpp>
#include <openpose/headers.hpp>

namespace handmap {
    struct Hand final {
        /** Joints */
        bool valid;
        glm::vec2 wrist;
        glm::vec2 thumb[4];
        glm::vec2 index[4];
        glm::vec2 middle[4];
        glm::vec2 ring[4];
        glm::vec2 pinky[4];

        /** Scanning */
        static void scan(
            op::Wrapper &openpose,
            SDL_Surface *camera_surface,
            Hand &left_hand,
            Hand &right_hand
        ) {
            SDL_Surface *bgr = SDL_ConvertSurface(camera_surface, SDL_PIXELFORMAT_BGR24);
            cv::Mat matrix(bgr->h, bgr->w, CV_8UC3, bgr->pixels, bgr->pitch);
            auto datums = std::make_shared<std::vector<std::shared_ptr<op::Datum> > >();
            datums->emplace_back(std::make_shared<op::Datum>());
            auto &datum = datums->at(0);
            float camera_width = static_cast<float>(bgr->w);
            float camera_height = static_cast<float>(bgr->h);
            float half_width = camera_width / 2.0f;
            float size = half_width < camera_height ? half_width : camera_height;
            float y = (camera_height - size) / 2.0f;
            op::Rectangle<float> left_rect{0.0f, y, size, size};
            op::Rectangle<float> right_rect{camera_width - size, y, size, size};
            datum->handRectangles.emplace_back(std::array<op::Rectangle<float>, 2>{right_rect, left_rect});
            datum->cvInputData = OP_CV2OPCONSTMAT(matrix);
            if (openpose.emplaceAndPop(datums) && datums != nullptr && !datums->empty()) {
                auto &keypoints = datums->at(0)->handKeypoints;
                const float tolerance = 0.25f;
                auto make_hand = [&](int keypoint, Hand &hand) {
                    hand.valid = false;
                    if (!keypoints[keypoint].empty()) {
                        const float *points = keypoints[keypoint].getConstPtr();
                        float average = 0.0f;
                        for (int i = 0; i < 21; ++i) {
                            float confidence = points[i * 3 + 2];
                            average += confidence;
                            float x = points[i * 3 + 0];
                            float y = points[i * 3 + 1];
                            hand.vec2array(i) = glm::vec2{x, y};
                        }
                        average /= 21;
                        if (average > tolerance) {
                            hand.valid = true;
                        }
                    }
                };
                make_hand(0, left_hand);
                make_hand(1, right_hand);
            } else {
                left_hand.valid = false;
                right_hand.valid = false;
            }
            SDL_DestroySurface(bgr);
        }

    private:
        /** Array access */
        glm::vec2 &vec2array(int i) {
            switch (i) {
                case 0: return wrist;
                case 1: return thumb[0];
                case 2: return thumb[1];
                case 3: return thumb[2];
                case 4: return thumb[3];
                case 5: return index[0];
                case 6: return index[1];
                case 7: return index[2];
                case 8: return index[3];
                case 9: return middle[0];
                case 10: return middle[1];
                case 11: return middle[2];
                case 12: return middle[3];
                case 13: return ring[0];
                case 14: return ring[1];
                case 15: return ring[2];
                case 16: return ring[3];
                case 17: return pinky[0];
                case 18: return pinky[1];
                case 19: return pinky[2];
                case 20: return pinky[3];
            }
        }
    };
}
