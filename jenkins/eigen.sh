# Check correct Eigen inclusion
if ( grep -R -E -w "#include *(\"|<)Eigen" ../ltr/*; ); then
  echo "[ERROR] Incorrect included Eigen!"
  false
fi
