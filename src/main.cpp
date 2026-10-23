
#include <iostream>
#include <cassert>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <fast-cpp-csv-parser/csv.h>
#include <HandMap/DataFrame.hpp>

using namespace std;
using namespace io;
using namespace handmap;

int main(int argc, char *argv[]) {
    // Load each column from the CSV
    CSVReader<DataFrame::COLUMN_COUNT> csv(DataFrame::CSV_NAME);
    DataFrame data{};
    csv.read_header(ignore_no_column, HANDMAP_DATAFRAME_HEADER);
    size_t row_count = 0;

    // Iterate each row
    while (csv.read_row(HANDMAP_DATAFRAME_EXPAND(data))) {
        ++row_count;
    }
    assert(row_count == DataFrame::ROW_COUNT);

    return 0;
}
