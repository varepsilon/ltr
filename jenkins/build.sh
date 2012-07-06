[ -z $JOBS ] && JOBS=2
! [ -d build ] && mkdir build
rm -rf build/*
cd build
cmake ../
make -j$JOBS
