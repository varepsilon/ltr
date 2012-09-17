1. How to produce .out files for examples in documentation
    cd <LTR_ROOT>/doc/build                #  or any other build directory you'd like
    cmake ../
    Then compile the resulted project and the .out files will be produced automatically
2. How to build Doxygen documentation:
    cd <LTR_ROOT>
    doxygen doc/doxyfile.cfg
The documentation will appear in doc/ folder.

Online documentation is available at http://yaschool.github.com/ltr/