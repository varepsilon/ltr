set -e

[ -z $JOBS ] && JOBS=2
! [ -d build ] && mkdir build
rm -rf build/*
if [ "$OS" = "Windows_NT" ]; then
  cd build
  $COMSPEC /c '..\jenkins\build.cmd'
else
  python ./pre-push_check.py
  cd build
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
  cd ../../jenkins
  ./pep8.sh
fi

