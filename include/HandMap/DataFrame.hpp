
#pragma once

namespace handmap {
    struct DataFrame {
        /** Constants */
        static constexpr size_t COLUMN_COUNT = 37;
        static constexpr size_t ROW_COUNT = 10000;
        static constexpr char CSV_NAME[] = "hand-gesture-data.csv";

        /** Columns */
        int   symbol;
        float _0_ax,
              _0_ay,
              _0_az,
              _0_gx,
              _0_gy,
              _0_gz,
              _1_ax,
              _1_ay,
              _1_az,
              _1_gx,
              _1_gy,
              _1_gz,
              _2_ax,
              _2_ay,
              _2_az,
              _2_gx,
              _2_gy,
              _2_gz,
              _3_ax,
              _3_ay,
              _3_az,
              _3_gx,
              _3_gy,
              _3_gz,
              _4_ax,
              _4_ay,
              _4_az,
              _4_gx,
              _4_gy,
              _4_gz,
              _5_ax,
              _5_ay,
              _5_az,
              _5_gx,
              _5_gy,
              _5_gz;
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

/** Expands every column value for data_frame. */
#define HANDMAP_DATAFRAME_EXPAND(data_frame)\
data_frame.symbol,\
data_frame._0_ax,\
data_frame._0_ay,\
data_frame._0_az,\
data_frame._0_gx,\
data_frame._0_gy,\
data_frame._0_gz,\
data_frame._1_ax,\
data_frame._1_ay,\
data_frame._1_az,\
data_frame._1_gx,\
data_frame._1_gy,\
data_frame._1_gz,\
data_frame._2_ax,\
data_frame._2_ay,\
data_frame._2_az,\
data_frame._2_gx,\
data_frame._2_gy,\
data_frame._2_gz,\
data_frame._3_ax,\
data_frame._3_ay,\
data_frame._3_az,\
data_frame._3_gx,\
data_frame._3_gy,\
data_frame._3_gz,\
data_frame._4_ax,\
data_frame._4_ay,\
data_frame._4_az,\
data_frame._4_gx,\
data_frame._4_gy,\
data_frame._4_gz,\
data_frame._5_ax,\
data_frame._5_ay,\
data_frame._5_az,\
data_frame._5_gx,\
data_frame._5_gy,\
data_frame._5_gz
