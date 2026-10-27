cd extern
git clone https://github.com/microsoft/vcpkg.git "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh"
"$HOME/vcpkg/vcpkg" install sdl3                \
                            sdl3-ttf            \
                            glm                 \
                            fast-cpp-csv-parser
