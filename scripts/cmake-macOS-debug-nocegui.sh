cd ..
rm -rf build
mkdir build
cd build
cmake -D CMAKE_OSX_DEPLOYMENT_TARGET="10.9" -D CMAKE_OSX_SYSROOT="macosx10.13" -D CMAKE_BUILD_TYPE=Debug -DBREAKPAD=OFF -DCEGUI=OFF -DBUNDLED_CEGUI=OFF -G Xcode ../