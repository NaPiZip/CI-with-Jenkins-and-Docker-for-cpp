#!/bin/bash

cd googletest
# In order to display all cmake vars use 'cmake -LAH'
.././dockcross cmake -Bbuild -H. '-GUnix Makefiles' \
      -Dgtest_build_samples=OFF \
      -Dgtest_build_tests=ON \
      -Dgmock_build_tests=ON \
      -Dcxx_no_exception=OFF \
      -Dcxx_no_rtti=OFF \
      -DCMAKE_COMPILER_IS_GNUCXX=OFF

.././dockcross make -Cbuild -i
.././dockcross make test -Cbuild
cd ..
