#!/bin/bash
DEBUG=#true

CROSS_COMPILER_IMAGE_NAME=windows-static-x64
ADDITIONAL_ARGS="-l dc" # Default arg here is only a label


# Bash on Ubuntu
LINUX=$(uname -o | awk '/Linux/ {print $0}')
# MSYS, Git Bash, etc.
MSYS=$(uname -o || awk '/Msys/ {print $0}')

if [ ! -f ./dockcross_sh ]; then
  echo "Creating dockcross script!"
  docker run --rm dockcross/$CROSS_COMPILER_IMAGE_NAME > ./dockcross_pre

  if [ -z "$DOCKER_TOOLBOX_INSTALL_PATH" -a -n "$LINUX" ]; then
    echo "Target is LINUX"
    # Fixing platform error for Docker inside the VM
     awk '/USER_IDS=\(-e BUILDER_UID/ {print "echo ";next}1' dockcross_pre > ./dockcross_sh
  elif [ -n "$MSYS" ]; then
    echo "Target is MSYS"
    # Fixing a platform error on MSYS see Readme for details
    awk '!/HOST_PWD=\$\{HOST_PWD\/\\/ {print $0}' dockcross_pre > ./dockcross_sh
  else
    echo "ERROR: Unsupported host!"
    exit
  fi

  if [ $DEBUG == true ]; then
    echo "Creating debug file"
    # adding echo statment befor running the docker command and showing its arguments
    awk '/docker run \$/ {print "echo $TTY_ARGS --name $CONTAINER_NAME \\\n -v \"$HOST_PWD\":/work \\\n $HOST_VOLUMES \\\n \"${USER_IDS[@]}\" \\\n $FINAL_ARGS \\\n $FINAL_IMAGE \"$@\"";print;next}1' dockcross_sh > ./dockcross_debug
    chmod +x ./dockcross_debug
  fi

  rm dockcross_pre
  chmod +x ./dockcross_sh
  echo "Done creating dockcross script!"
else
  echo "Skipped creating dockcross script, since it already exists!"
fi

# Fixing the mount issue only if we are inside a docker container
if [ -z "$DOCKER_TOOLBOX_INSTALL_PATH" -a -n "$LINUX" ]; then
  # Changing the workspace -w and adding the volume
  CONTAINER_ID=$(cat /etc/hostname)
  ADDITIONAL_ARGS="-w $PWD --volumes-from $CONTAINER_ID"
fi


if [ $DEBUG == true ]; then
  echo "Running debug build script"
  ./dockcross_debug -a "$ADDITIONAL_ARGS" cmake -H. -Bbuild "-GUnix Makefiles"
else
  echo "Running build script"
  ./dockcross_sh -a "$ADDITIONAL_ARGS" cmake -H. -Bbuild "-GUnix Makefiles"
  ./dockcross_sh -a "$ADDITIONAL_ARGS" make -Cbuild
  ./dockcross_sh -a "$ADDITIONAL_ARGS" make test -Cbuild
fi

echo "Successfully terminated!!"
