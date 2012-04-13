! [ -d ltr1/lib ] && mkdir -p ltr1/lib
mv lib/* ltr1/lib/
cd ltr1

! [ -d build/serialization_test ] && mkdir -p build/serialization_test
rm -rf build/serialization_test/*
cd build/serialization_test
cmake ../../serialization_test/
make -j2

./tester
