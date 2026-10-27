// .hpp
// Hand Gesture Mapping Data Frame
// by Kyle Furey

#pragma once
#include <glm/glm.hpp>

namespace handmap {
    struct DataFrame final {
        /** Constants */
        static constexpr char CSV_NAME[] = "hand-gesture-data.csv";
        static constexpr size_t COLUMN_COUNT = 37;
        static constexpr size_t ROW_COUNT = 10000;
        static constexpr char GESTURE_NAMES[10][6] = {
            "TODO",
            "TODO",
            "TODO",
            "TODO",
            "TODO",
            "TODO",
            "TODO",
            "TODO",
            "TODO",
            "TODO",
        };

        /** Columns */
        int symbol;
        glm::vec3 a[6];
        glm::vec3 g[6];
    };
}

/** Columns as strings */
#define HANDMAP_DATAFRAME_HEADER \
    "symbol",\
    "0_ax",\
    "0_ay",\
    "0_az",\
    "0_gx",\
    "0_gy",\
    "0_gz",\
    "1_ax",\
    "1_ay",\
    "1_az",\
    "1_gx",\
    "1_gy",\
    "1_gz",\
    "2_ax",\
    "2_ay",\
    "2_az",\
    "2_gx",\
    "2_gy",\
    "2_gz",\
    "3_ax",\
    "3_ay",\
    "3_az",\
    "3_gx",\
    "3_gy",\
    "3_gz",\
    "4_ax",\
    "4_ay",\
    "4_az",\
    "4_gx",\
    "4_gy",\
    "4_gz",\
    "5_ax",\
    "5_ay",\
    "5_az",\
    "5_gx",\
    "5_gy",\
    "5_gz"

/** Expands every column value for data_frame */
#define HANDMAP_DATAFRAME_EXPAND(data_frame)\
    data_frame.symbol,\
    data_frame.a[0].x,\
    data_frame.a[0].y,\
    data_frame.a[0].z,\
    data_frame.g[0].x,\
    data_frame.g[0].y,\
    data_frame.g[0].z,\
    data_frame.a[1].x,\
    data_frame.a[1].y,\
    data_frame.a[1].z,\
    data_frame.g[1].x,\
    data_frame.g[1].y,\
    data_frame.g[1].z,\
    data_frame.a[2].x,\
    data_frame.a[2].y,\
    data_frame.a[2].z,\
    data_frame.g[2].x,\
    data_frame.g[2].y,\
    data_frame.g[2].z,\
    data_frame.a[3].x,\
    data_frame.a[3].y,\
    data_frame.a[3].z,\
    data_frame.g[3].x,\
    data_frame.g[3].y,\
    data_frame.g[3].z,\
    data_frame.a[4].x,\
    data_frame.a[4].y,\
    data_frame.a[4].z,\
    data_frame.g[4].x,\
    data_frame.g[4].y,\
    data_frame.g[4].z,\
    data_frame.a[5].x,\
    data_frame.a[5].y,\
    data_frame.a[5].z,\
    data_frame.g[5].x,\
    data_frame.g[5].y,\
    data_frame.g[5].z
