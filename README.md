<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/e/e9/Jenkins_logo.svg/556px-Jenkins_logo.svg.png" alt="Jenkins logo" height="42px" width="42px" align="left">
<img src="https://proxy.duckduckgo.com/iu/?u=https%3A%2F%2Fd3nmt5vlzunoa1.cloudfront.net%2Fphpstorm%2Ffiles%2F2015%2F10%2Flarge_v-trans.png&f=1" alt="Docker logo" height="42px" width="42px" align="left"><br>

<h1>CI with Jenkins and Docker on Windows for C/C++ code</h1>
<div>
    <a href="https://github.com/NaPiZip/CI-with-jenkins-and-docker-for-cpp">
        <img src="https://img.shields.io/badge/Document%20Version-0.0.1-brightgreen"/>
    </a>
    <a href="https://www.microsoft.com">
        <img src="https://img.shields.io/badge/Windows%2010%20x64-10.0.17134%20Build%2017134-blue.svg"/>
    </a>
    <a href="https://jenkins.io/">
        <img src="https://img.shields.io/badge/Jenkins%20Version-2.176.2-blue"/>
    </a>
    <a href="https://docs.docker.com/toolbox/toolbox_install_windows/">
        <img src="https://img.shields.io/badge/Docker%20Toolbox-17.05.0--ce%20Build%2089658be-blue.svg"/>
    </a>

</div>

This is a tutorial showing how to use Jenkins and Docker Toolbox on a Windows machine in order to set up a Continuous Integration system for C/C++. The central part is the Docker Toolbox which is used in order to run the Jenkins host, as well as to run a Docker container with a compiler image for compiling C/C++ source code, the container image is called Dockcross .Dockercross is a cross compiling toolchains in Docker, which utilizes Docker images in order to compile executables for all kinds of platforms a link can be found [here](https://github.com/dockcross/dockcross).


The main goal is to install as little software as possible, such that the coupling between tools is as minimal as needed. This means after installing the Docker Toolbox and cloning the Git repo, there should be no additional software needed in order to develop C/C++ code. To summarize the goals:
- Create a CI build environment with as little tool coupling.
- Should support automated testing after each code commit.
- The Git repo should be self containing.
- There should be the possibility of extension of the toolchain.

## Installation
The following programs are needed to be installed:<br>
1. Git<br>
2. Docker Toolbox for Windows<br>

We will use the following Docker images:
1. Dockcross image, [GitHub](https://github.com/dockcross/dockcross)<br>
2. Jenkis images, [GitHub](https://github.com/jenkinsci)<br>

*Note:<br>
We are using an adapted version of the Jenkins Docker image, since we need to issue Docker commands within Jenkins and due to the way Dockcross is working we cannot use the Jenkins docker plugins. This is related to the fact that we are not starting a container and running build commands within the running container. Instead we are just issuing a single run command which terminates the running container right after the build!*

## System architecture
The following image displays the system architecture, showing the host PC, which is using the Docker Toolbox. The Docker Toolbox then runs an image of a Jenkins container, the configuration on the Jenkins container clones the GitHub repository and constantly monitors it for changes. The Jenkin image then builds the source code and runs all unit tests with the help of Dockcross.

<p align="center">
<img src="https://raw.githubusercontent.com/NaPiZip/CI-with-jenkins-and-docker-for-cpp/master/images/Architecture.JPG" alt="Architecture"/></p>

## Tutorial
This section describes how to set up a "local" Jenkins host which is triggered by the changes of the git repository, builds the executables and runs all the defined tests. The test results are then visualized with the help of xUnit.

1. Make sure Dockcross is working on the system by running the following command (this is actually not necessary, since we are later running within a Docker container, but it helps  getting familiar with the usage of Dockcross as well as checking if the Docker Toolbox works correct):
  ```
  docker run --rm dockcross/windows-static-x64 > ./dockcross_sh
  ```
  <b>Dockcross issues</b><br>
  The following section shows a problem which occurred on my host Windows machine, trying to run `dockcross`. It looks like there was a path formatting error going on, when using Dockcross in MSYS on a Windows host machine.
  ```
  $ ./dockcross_sh cmake -h
  C:\Program Files\Docker Toolbox\docker.exe: Error response from daemon: invalid mode: /work.
  See 'C:\Program Files\Docker Toolbox\docker.exe run --help'.
  Error response from daemon: No such container: dockcross_32348
  ```
  Adding a little bit of `echo "..."` statements into the `dockcross_sh` file showed that the following `docker run` call was made by the script:
  ```
  $ docker run -ti --name dockcross_17497 -v e:/080_Github/CI-with-jenkins-and-docker-for-cpp:/work dockcross/windows-static-x64:latest cmake -h
  ```
  The argument `-v e:/080...` is leading to an path error, the correct formatting in MSYS should look like:
  ```
  /e/080_Github...
  ```
  I tracked down the error to lines 199 and 200 of the `dockcross_sh` file, shown here:
  ```
  199 HOST_PWD=${HOST_PWD/\//}
  200 HOST_PWD=${HOST_PWD/\//:\/}
  ```
  Those two lines are responsible for the formatting error, removing them solved the issue, since we don't want the Windows formatting. After the modifications everything worked as expected:
  ```
  $ ./dockcross_sh cmake -h
  == Using MXE wrapper: /usr/src/mxe/usr/bin/x86_64-w64-mingw32.static-cmake
       - cmake version 3.10.0
       - warnings for unused CMAKE_POLICY_DEFAULT variables can be ignored
  == Using MXE toolchain: /usr/src/mxe/usr/x86_64-w64-mingw32.static/share/cmake/mxe-conf.cmake
  ...
  ```
2. Build the Jenkins Docker image, located under `/jenkins` by issuing the following command within the directory:
  ```
  $ docker build . -t jenkins
  ```
3. Run the Jenkins Docker image, for a description of the arguments see [here](https://jenkins.io/doc/tutorials/build-a-java-app-with-maven/#on-macos-and-linux):
  ```
  $ docker run \
    -u jenkins \
    --rm \
    -d \
    -p 8080:8080 \
    -p 50000:50000 \
    -v jenkins-data:/var/jenkins_home \
    -v /var/run/docker.sock:/var/run/docker.sock \
    jenkins
  ```
4. Configure the Jenkins host according to the [Post-installation setup wizard](https://jenkins.io/doc/book/installing#setup-wizard). The following plugins are important, e.g. for test visualization purposes or a better look of the dashboard in Jenkins:
  - git:3.12.0
  - xunit:2.3.5
  - test-results-analyzer:0.3.5
  - modernstatus:1.2

5. Try running the `build_full.sh` script in within the Jenkins Docker image. Don't forget to mount your local directory containing the project files `-v /e/080_Github/CI-with-jenkins-and-docker-for-cpp/:/home`, since we want to test the correct functionality before setting up a Jenkins project:
  ```
  $ docker run  -u jenkins  --rm  -d  -p 8080:8080  -p 50000:50000  -v jenkins-data:/var/jenkins_home  -v /var/run/docker
  .sock:/var/run/docker.sock  -v /e/080_Github/CI-with-jenkins-and-docker-for-cpp/:/home jenkins
  a38f21bbf08e925764960dc4bd02b57ca25f67c8709cec52047c86684999833f

  $ docker exec -it a38f bash

  jenkins@a38f21bbf08e:/$ cd home/

  jenkins@a38f21bbf08e:/home$ ./build_full.sh
  Creating dockcross script!
  Target is LINUX
  Done creating dockcross script!"
  Running build script

  == Using MXE wrapper: /usr/src/mxe/usr/bin/x86_64-w64-mingw32.static-cmake
       - cmake version 3.10.0
       - warnings for unused CMAKE_POLICY_DEFAULT variables can be ignored
  == Using MXE toolchain: /usr/src/mxe/usr/x86_64-w64-mingw32.static/share/cmake/mxe-conf.cmake
  == Using MXE runresult: /usr/src/mxe/usr/share/cmake/modules/TryRunResults.cmake

  ...
  17/21 Test #17: PathExists.singleTest ..............   Passed    1.36 sec
        Start 18: TreeToLinkedList.singleTest
  18/21 Test #18: TreeToLinkedList.singleTest ........   Passed    1.37 sec
        Start 19: FindPaht.singleTest
  19/21 Test #19: FindPaht.singleTest ................   Passed    1.37 sec
        Start 20: cGraphTest.setup
  20/21 Test #20: cGraphTest.setup ...................   Passed    1.37 sec
        Start 21: cGraphTest.Dijkstras_algo
  21/21 Test #21: cGraphTest.Dijkstras_algo ..........   Passed    1.38 sec

  100% tests passed, 0 tests failed out of 21

  Total Test time (real) =  30.54 sec
  make: Leaving directory '/home/build'
  Successfully terminated!!
  ```
  <b>Dockcross issues within Jenkins</b><br>
  The following error occurred during the build, due to an issue while running Dockcross using the Docker Toolbox within a Unix Docker container:
  ```
  bash-4.4# ./dockcross cmake --help
  cp: target '/home/root/' is not a directory
  chown: cannot access '/home/root': No such file or directory
  ```
  Debugging showed the following `docker run` arguments get passed:<br>
  ```
  -ti --name dockcross_12203 -v /home:/work -e BUILDER_UID=0 -e BUILDER_GID=0 -e BUILDER_USER=root -e BUILDER_GROUP=root dockcross/windows-static-x64:latest cmake --help
  ```
  This issue took me quite a while to figure out. The problem here was that we are running Docker inside of a Docker container (the Jenkins image build earlier). Docker does not allow you to mount local resources of one container to other container, this is the `-v /home:/work` argument in the call. Docker refers to volumes for sharing data beween container, see [here](https://stackoverflow.com/questions/23137544/how-to-map-volume-paths-using-dockers-volumes-from). Adding an additional argument to the call of Dockcross in `build_full.sh` resolved that error, this change is only needed inside a Docker image.
  ```
  ...
  ADDITIONAL_ARGS="-l dc" # Default arg here is only a label
  ...
  # Fixing the mount issue only if we are inside a docker container
  if [ -z "$DOCKER_TOOLBOX_INSTALL_PATH" -a -n "$LINUX" ]; then
    # Changing the workspace -w and adding the volume
    CONTAINER_ID=$(cat /etc/hostname)
    ADDITIONAL_ARGS="-w $PWD --volumes-from $CONTAINER_ID"
  fi

  ./dockcross_sh -a "$ADDITIONAL_ARGS" cmake -H. -Bbuild "-GUnix Makefiles"

  ```
  The default argument for `ADDITIONAL_ARGS` is only a label, this will do nothing in case we are not running on within a Docker container. It seems that call is now working:
  ```
  ./dockcross -a "-w /home --volumes-from a38f21bbf08e" cmake --help

  cp: cannot create symbolic link '/home/jenkins/.wine/dosdevices/c:': Read-only file system
  cp: cannot create symbolic link '/home/jenkins/.wine/dosdevices/z:': Read-only file system
  cp: cannot create symbolic link '/home/jenkins/.wine/drive_c/users/root/My Pictures': Read-only file system
  cp: cannot create symbolic link '/home/jenkins/.wine/drive_c/users/root/My Documents': Read-only file system
  cp: cannot create symbolic link '/home/jenkins/.wine/drive_c/users/root/My Music': Read-only file system
  cp: cannot create symbolic link '/home/jenkins/.wine/drive_c/users/root/My Videos': Read-only file system
  == Using MXE wrapper: /usr/src/mxe/usr/bin/x86_64-w64-mingw32.static-cmake
       - cmake version 3.10.0
       - warnings for unused CMAKE_POLICY_DEFAULT variables can be ignored
  ...
  ```
  It still looks like that Dockcross is trying to copy a lot of overhead files, which are read only. In order to fix that issue I removed the following section of the `dockcross_sh` file, using a little awk script in `build_full.sh`.
  ```
  if [ -z "$UBUNTU_ON_WINDOWS" -a -z "$MSYS" ]; then
      USER_IDS=(-e BUILDER_UID="$( id -u )" -e BUILDER_GID="$( id -g )" -e BUILDER_USER="$( id -un )" -e BUILDER_GROUP="$( id -gn )")
  fi
  ```
  This section adds arguments which get processed wit the `entrypont.sh` file of Dockcross, here is the part of the `entrypoint.sh` file showing what's going on:
  ```
  ...
  # If we are running docker natively, we want to create a user in the container
  # with the same UID and GID as the user on the host machine, so that any files
  # created are owned by that user. Without this they are all owned by root.
  # The dockcross script sets the BUILDER_UID and BUILDER_GID vars.
  If we are running docker natively, we want to create a user in the container
  # with the same UID and GID as the user on the host machine, so that any files
  # created are owned by that user. Without this they are all owned by root.
  # The dockcross script sets the BUILDER_UID and BUILDER_GID vars.
  if [[ -n $BUILDER_UID ]] && [[ -n $BUILDER_GID ]]; then

     groupadd -o -g $BUILDER_GID $BUILDER_GROUP 2> /dev/null
     useradd -o -m -g $BUILDER_GID -u $BUILDER_UID $BUILDER_USER 2> /dev/null
     export HOME=/home/${BUILDER_USER}
     shopt -s dotglob
     cp -r /root/* $HOME/
     chown -R $BUILDER_UID:$BUILDER_GID $HOME
     ...

  ```
  The section does the copying as well as changes of ownership. All the patches are found in `build_full.sh`.

6. Configuration of the Jenkins project. This step is pretty straight forward, that's the reason why I am just adding a short list of steps:
  - Create a new `Pipeline` whit a representative name.
  - Add a description
  - Set the git repository url in the `General` section as followed:
  ```
  https://github.com/NaPiZip/CI-with-jenkins-and-docker-for-cpp.git
  ```
  - Set the `Build Triggers` to `Poll SCM` and enter the desired poll interval. For example:
  ```
  # Every five minutes (MINUTE HOUR DOM MONTH DOW)
  5 * * * *
  ```
  There is also the option using a GitHub hook, but since we are running a local Jenkis container without a public IP Address, I decided to poll the SCM.
  - Select the `pipeline script from SCM` in the `Pipeline` section.
    - SCM: `Git`
    - Repository URL: `https://github.com/NaPiZip/CI-with-jenkins-and-docker-for-cpp.git`
    - Additional Behaviors: `Advanced sub-module behaviors` with `Recursively update submodules`
    - Script Path: `jenkins/Jenkins`


## Additional errors and debugging
If docker is causing a permission error:
```
docker: Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Post http://%2Fvar%2Frun%2Fdocker.sock/v1.26/containers/create: dial unix /var/run/docker.sock: connect: permission denied.
```
Then run the following command in the container:
```
$ chmod 777 /var/run/docker.sock

```
## Example images
Here are some example images showing the Jenkins server up and running on the host.

<p align="center">
<img src="https://raw.githubusercontent.com/NaPiZip/CI-with-jenkins-and-docker-for-cpp/master/images/Project_screen.JPG" alt="Project view"/></p>

<p align="center">
<img src="https://raw.githubusercontent.com/NaPiZip/CI-with-jenkins-and-docker-for-cpp/master/images/Test_results_view.JPG" alt="Test results view"/></p>

## Helpful links
The following general links helped me to get everything set up or when I had road blocks.
- [Using CTest in CMake](https://cmake.org/cmake-tutorial/#s3)
- [CTest advanced configuration of the report](https://docs.nersc.gov/services/cdash/with_cmake/)
- [Jenkins pipeline order of execution help](https://issues.jenkins-ci.org/browse/JENKINS-41239)
- [How to set the build name in Jenkins](https://stackoverflow.com/questions/53451345/how-to-set-build-name-in-jenkins-job-dsl)

## Future topics
Depending on if I feel I have time I will try to cover the following topics:
- Creating a custom Dockcross container, containing a Clang compiler.
- Running additional profiling tools, e.g. Clang-Tidy, CppCheck etc. ...
- Moving the setup to an Raspberry.
- Moving the setup to AWS or Google Cloud.

## Contributing
To get started with contributing to my GitHub repository, please contact me [Slack](https://join.slack.com/t/napi-friends/shared_invite/enQtNDg3OTg5NDc1NzUxLWU1MWNhNmY3ZTVmY2FkMDM1ODg1MWNlMDIyYTk1OTg4OThhYzgyNDc3ZmE5NzM1ZTM2ZDQwZGI0ZjU2M2JlNDU).
