cd ..
rm -rf build
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBREAKPAD=OFF -DCEGUI=OFF -DBUNDLED_CEGUI=OFF -DlibJPEG=OFF -Wno-dev ../
