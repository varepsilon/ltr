# Script for check documentation
cd ..;
if ( doxygen doc/doxygen.cfg 2>&1 1>$- | grep -w '..*'; ); then
  echo "[ERROR] Documentation generation failed!"
  false
fi
cd jenkins;
