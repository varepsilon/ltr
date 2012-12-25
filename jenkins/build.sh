set -x

[ -z $JOBS ] && JOBS=2
! [ -d build ] && mkdir build
rm -rf build/*
cd build
if [ "$OS" = "Windows_NT" ]; then
  $COMSPEC /c '..\jenkins\build.cmd'
else
  cmake ../
  make -j$JOBS RUN_SERIALIZATION
  make -j$JOBS
fi
./ltr_test

if [ "$OS" != "Windows_NT" ]; then
  # build examples
  make install
  cd ../
  [ -d doc/build ] || mkdir doc/build
  cd doc/build
  cmake ../
  make -j$JOBS
fi

