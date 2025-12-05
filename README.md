Simple and Fast Media Libraties

Rendering apps using C++

Note on dynamic linking:

#Dynamic linking
- When linking with g++ add the library location and the linkfiles: L<lib-location> -l<lib1> -l<lib2>, example
g++ -g --std=c++17 ./src/move_weight.cpp -o ./bin/move_weight -I/home/bvermeulen/Development/cpp/SFML-3.0.2/include -I./include -L/home/bvermeulen/Development/cpp/SFML-3.0.2/lib -lsfml0graphics -lsfml-system -lsfml-window
- During runtime the app needs to know the location of the linkfiles, do this in /etc/ld.so.conf by:

-- adding a file in ld.so.conf.d (for example mylibs.conf) and in there the <lib-location>, example
/home/bvermeulen/.pyenv/versions/3.14.0/lib
/home/bvermeulen/Development/cpp/SFML-3.0.2/lib

-- run sudo ldconfig
