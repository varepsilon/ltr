# Script for check documentation
if ! which doxygen >/dev/null; then
  echo "[WARNING] Doxygen not installed. Documentation checking skipped"
  exit 0
fi

DOX_VERSION=`doxygen --version`
if [ $DOX_VERSION \< 1.8.0 ]; then
  echo -n "[WARNING] Doxygen version is too old. "
  echo "Need: 1.8.0. Have: " $DOX_VERSION
  echo "Documentation checking skipped"
  exit 0
fi


echo "[DOXYGEN] Start"
cd ..
DOX_OUTPUT=`doxygen doc/doxygen.cfg 2>&1 1>/dev/null  | sort -u`
cd jenkins

if [ `echo $DOX_OUTPUT | wc -c` -gt 1 ]; then
  echo "$DOX_OUTPUT"
  echo "[ERROR] Documentation generation failed!"
  exit 1
fi

echo "[DOXYGEN] Success"
exit 0
