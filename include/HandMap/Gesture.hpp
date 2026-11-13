// .hpp
// Hand Gesture Mapping Gesture Data
// by Kyle Furey

#pragma once
#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include <HandMap/DataFrame.hpp>
#include <HandMap/Hand.hpp>

namespace handmap {
    struct Gesture final {
        /** Constants */
        static constexpr size_t COUNT = DataFrame::ROW_COUNT / DataFrame::SYMBOL_COUNT;
        static constexpr char NAMES[COUNT + 1][8] = {
            "unknown",
            "0",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
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
                return glm::degrees(glm::acos(dot));
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
            glm::vec2 thumb_dir = glm::normalize(hand.thumb[3] - hand.thumb[2]);
            glm::vec2 index_dir = glm::normalize(hand.index[3] - hand.index[2]);
            glm::vec2 middle_dir = glm::normalize(hand.middle[3] - hand.middle[2]);
            glm::vec2 ring_dir = glm::normalize(hand.ring[3] - hand.ring[2]);
            glm::vec2 pinky_dir = glm::normalize(hand.pinky[3] - hand.pinky[2]);

            // Normalized angles (degrees)
            thumb = glm::degrees(glm::acos(glm::dot(thumb_base, thumb_dir)));
            index = glm::degrees(glm::acos(glm::dot(index_base, index_dir)));
            middle = glm::degrees(glm::acos(glm::dot(middle_base, middle_dir)));
            ring = glm::degrees(glm::acos(glm::dot(ring_base, ring_dir)));
            pinky = glm::degrees(glm::acos(glm::dot(pinky_base, pinky_dir)));
        }

        /** Comparison */
        static int fltcmp(float lhs, float rhs, float tolerance) {
            float diff = rhs - lhs;
            if (diff > tolerance) {
                return 1;
            }
            if (diff < -tolerance) {
                return -1;
            }
            return 0;
        }

        [[nodiscard]] uint64_t compare(const Gesture &gesture, float tolerance) const {
            uint64_t result = 0;
            auto array = reinterpret_cast<signed char *>(&result);
            array[0] = static_cast<signed char>(fltcmp(gesture.thumb, thumb, tolerance));
            array[1] = static_cast<signed char>(fltcmp(gesture.index, index, tolerance));
            array[2] = static_cast<signed char>(fltcmp(gesture.middle, middle, tolerance));
            array[3] = static_cast<signed char>(fltcmp(gesture.ring, ring, tolerance));
            array[4] = static_cast<signed char>(fltcmp(gesture.pinky, pinky, tolerance));
            return result;
        }
    };
}
