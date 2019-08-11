#!/bin/bash

CROSS_COMPILER_IMAGE_NAME=windows-static-x64

# Fix for build_starter script
cd $(dirname $0)

if [ ! -f ./dockcross ]; then
  echo "Creating dockcross script!"

  docker run --rm dockcross/$CROSS_COMPILER_IMAGE_NAME > ./dockcross_pre

  # Fixing a platform error see Readme for details
  awk '!/HOST_PWD=\$\{HOST_PWD\/\\/ {print $0}' dockcross_pre > ./dockcross_sh
  #rm dockcross_pre
  chmod +x ./dockcross_sh
  echo "Done creating dockcross script!"
else
  echo "Skipped creating dockcross script, since it already exists!"
fi

./dockcross_sh cmake -H. -Bbuild "-GUnix Makefiles"
./dockcross_sh make -Cbuild
./dockcross_sh make test -Cbuild

echo "Successfully terminated build.sh script!!"
