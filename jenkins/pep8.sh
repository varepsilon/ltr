msg=$(for file in $(find ../ltr_site -name "*.py") ; do python pep8.py $file; done)
if [ -n "$msg" ]
then
    echo "[ERROR] PEP8 style check failed:"
    echo "$msg"
    false
fi
