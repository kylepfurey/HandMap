@echo off
cd extern
git clone https://github.com/microsoft/vcpkg.git "%USERPROFILE%\vcpkg"
call "%USERPROFILE%\vcpkg\bootstrap-vcpkg.bat"
"%USERPROFILE%\vcpkg\vcpkg.exe" install sdl3                ^
                                        sdl3-ttf            ^
                                        glm                 ^
                                        fast-cpp-csv-parser
git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git
cd openpose
git submodule update --init --recursive --remote
mkdir build
cd build
cmake .. -DGPU_MODE=CPU_ONLY
cmake --build . --config Release
