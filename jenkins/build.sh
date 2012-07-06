[ -z $JOBS ] && JOBS=2
! [ -d build ] && mkdir build
rm -rf build/*
cd build
cmake ../
if [ "$os_type" == "windows" ]
  cmd.exe /E:ON /V:ON /C 'jenkins\build.cmd'
else
  make -j$JOBS
fi
