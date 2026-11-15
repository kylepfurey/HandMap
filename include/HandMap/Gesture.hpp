// .hpp
// Hand Gesture Mapping Gesture Data
// by Kyle Furey

#pragma once
#include <cstddef>
#include <cmath>
#include <glm/glm.hpp>
#include <HandMap/DataFrame.hpp>
#include <HandMap/Hand.hpp>

namespace handmap {
    struct Gesture final {
        /** Constants */
        static constexpr size_t COUNT = DataFrame::ROW_COUNT / DataFrame::SYMBOL_COUNT;
        static constexpr char NAMES[COUNT + 1][10] = {
            "unknown", // not recognized
            "symbol 0", // unsure
            "symbol 1", // unsure
            "symbol 2", // unsure
            "symbol 3", // unsure
            "thumbs up",
            "palm",
            "pinky",
            "four",
            "shaka",
            "symbol 9", // unsure
        };

        /** Angles (degrees) */
        float thumb;
        float index;
        float middle;
        float ring;
        float pinky;

        /** Constructors */
        Gesture()
            : thumb(0.0f),
              index(0.0f),
              middle(0.0f),
              ring(0.0f),
              pinky(0.0f) {
        }

        explicit Gesture(const glm::vec3 a[6]) {
            // Compute angle from accelerometer
            auto angle = [&](int sensor) {
                float wrist = glm::length(a[0]);
                float finger = glm::length(a[sensor]);
                float dot = glm::dot(a[0], a[sensor]) / (wrist * finger);
                return glm::degrees(glm::acos(glm::clamp(dot, -1.0f, 1.0f)));
            };
            thumb = angle(1);
            index = angle(2);
            middle = angle(3);
            ring = angle(4);
            pinky = angle(5);
        }

        explicit Gesture(const Hand &hand) {
            // Base direction (wrist-out)
            glm::vec2 thumb_base = glm::normalize(hand.thumb[0] - hand.wrist);
            glm::vec2 index_base = glm::normalize(hand.index[0] - hand.wrist);
            glm::vec2 middle_base = glm::normalize(hand.middle[0] - hand.wrist);
            glm::vec2 ring_base = glm::normalize(hand.ring[0] - hand.wrist);
            glm::vec2 pinky_base = glm::normalize(hand.pinky[0] - hand.wrist);

            // Aim direction (intermediate-out)
            glm::vec2 thumb_dir = glm::normalize(hand.thumb[2] - hand.thumb[1]);
            glm::vec2 index_dir = glm::normalize(hand.index[2] - hand.index[1]);
            glm::vec2 middle_dir = glm::normalize(hand.middle[2] - hand.middle[1]);
            glm::vec2 ring_dir = glm::normalize(hand.ring[2] - hand.ring[1]);
            glm::vec2 pinky_dir = glm::normalize(hand.pinky[2] - hand.pinky[1]);

            // Normalized angles (degrees)
            thumb = glm::degrees(glm::acos(glm::clamp(glm::dot(thumb_base, thumb_dir), -1.0f, 1.0f)));
            index = glm::degrees(glm::acos(glm::clamp(glm::dot(index_base, index_dir), -1.0f, 1.0f)));
            middle = glm::degrees(glm::acos(glm::clamp(glm::dot(middle_base, middle_dir), -1.0f, 1.0f)));
            ring = glm::degrees(glm::acos(glm::clamp(glm::dot(ring_base, ring_dir), -1.0f, 1.0f)));
            pinky = glm::degrees(glm::acos(glm::clamp(glm::dot(pinky_base, pinky_dir), -1.0f, 1.0f)));
        }

        /** Comparison */
        [[nodiscard]] float compare(const Gesture &gesture) const {
            float result = 0.0f;
            result += std::fabs(gesture.thumb - thumb);
            result += std::fabs(gesture.index - index);
            result += std::fabs(gesture.middle - middle);
            result += std::fabs(gesture.ring - ring);
            result += std::fabs(gesture.pinky - pinky);
            return 1.0f - (result / (5.0f * 180.0f));
        }
    };
}
