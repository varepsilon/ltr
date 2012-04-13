! [ -d ltr1/lib ] && mkdir -p ltr1/lib
mv lib/* ltr1/lib/
cd ltr1

! [ -d build/doc ] && mkdir -p build/doc
rm -rf build/doc/*
cd build/doc
cmake ../../doc/
make -j2

cd ../../
doxygen doc/doxygen.cfg
