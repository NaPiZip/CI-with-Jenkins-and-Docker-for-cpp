#!/bin/bash

CROSS_COMPILER_IMAGE_NAME=windows-static-x64

# Fix for build_starter script
cd $(dirname $0)

if [ ! -f ./dockcross ]; then
  echo "Creating dockcross script!"

  docker run --rm dockcross/$CROSS_COMPILER_IMAGE_NAME > ./dockcross_pre

  # Bash on Ubuntu
  LINUX=$(uname -o || awk '/Msys/ {print $0}')
  # MSYS, Git Bash, etc.
  MSYS=$(uname -o || awk '/Msys/ {print $0}')

  if [ -z "$DOCKER_TOOLBOX_INSTALL_PATH" -a -n "$LINUX" ]; then
    echo "Target is LINUX"
    # Fixing platform error for Docker inside the VM
    awk '!/USER_IDS=\(-e BUILDER_UID/ {print $0}' dockcross_pre > ./dockcross_sh
  elif [ -n "$MSYS" ]; then
    echo "Target is MSYS"
    # Fixing a platform error on MSYS see Readme for details
    awk '!/HOST_PWD=\$\{HOST_PWD\/\\/ {print $0}' dockcross_pre > ./dockcross_sh
  fi

  rm dockcross_pre
  chmod +x ./dockcross_sh
  echo "Done creating dockcross script!"
else
  echo "Skipped creating dockcross script, since it already exists!"
fi

#./dockcross_sh cmake -H. -Bbuild "-GUnix Makefiles"
#./dockcross_sh make -Cbuild
#./dockcross_sh make test -Cbuild

echo "Successfully terminated build.sh script!!"
