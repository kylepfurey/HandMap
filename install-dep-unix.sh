cd extern
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh"
"$HOME/vcpkg/vcpkg" install sdl3                \
                            sdl3-ttf            \
                            glm                 \
                            fast-cpp-csv-parser
git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git
cd openpose
git submodule update --init --recursive --remote
sudo apt-get install -y libopencv-dev
sudo bash ./scripts/ubuntu/install_deps.sh
mkdir build
cd build
cmake .. -DGPU_MODE=CPU_ONLY
cmake --build . --config Release
