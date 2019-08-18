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
1. Dockcross image, [GitHub link](https://github.com/dockcross/dockcross)<br>
2. Jenkis images, [GitHub link](https://github.com/jenkinsci)<br>

*Note:<br>
We are using an adapted version of the Jenkins docker image, since we need to issue docker commands within Jenkins and due to the way Dockcross is working we cannot use the Jenkins docker plugins!*

## Communication diagram
Image showing how docker, Dockcross and Jenkis communicate.

## Tutorial
This section describes how to set up a "local" Jenkins host which is triggered by changes of the git repository, then builds the executables and runs all the defined tests.

1. Make sure Dockcross is working on the system by running the following command(this is not necessarily needed since we are later running within a Docker container, but it helps in order to get familiar with the usage of Dockcross):
```
docker run --rm dockcross/windows-static-x64 > ./dockcross_sh
```
  *Dockcross issues*
  The following section show an error which occurred on my host windows machine, trying to run `dockcross`. It looks like there is a path formatting error going on, when using Dockcross in MSYS on a Windows machine.

  ```
  $ ./dockcross cmake -h
  C:\Program Files\Docker Toolbox\docker.exe: Error response from daemon: invalid mode: /work.
  See 'C:\Program Files\Docker Toolbox\docker.exe run --help'.
  Error response from daemon: No such container: dockcross_32348
  ```
  Adding a little bit of `echo "..."` statements into the `dockcross` file showed that the following call `docker run` call was made by the script:

  ```
  $ docker run -ti --name dockcross_17497 -v e:/080_Github/CI-with-jenkins-and-docker-for-cpp:/work dockcross/windows-static-x64:latest cmake -h
  ```
  The argument `-v e:/080...` is leading to an path error the correct formatting should look like:
  ```
  /e/080_Github...
  ```
  I tracked down the error to lines 199 and 200 of the `dockcross` file.

  ```
  199 HOST_PWD=${HOST_PWD/\//}
  200 HOST_PWD=${HOST_PWD/\//:\/}
  ```
  Those two lines are responsible for the reformatting, removing them solved the issue.
  ```
  $ ./dockcross_sh cmake -h
  == Using MXE wrapper: /usr/src/mxe/usr/bin/x86_64-w64-mingw32.static-cmake
       - cmake version 3.10.0
       - warnings for unused CMAKE_POLICY_DEFAULT variables can be ignored
  == Using MXE toolchain: /usr/src/mxe/usr/x86_64-w64-mingw32.static/share/cmake/mxe-conf.cmake
  ...
  ```
3. Build the Jenkins Docker image, located under `/jenkins` by issuing the following command within the directory:
```
$ docker build . -t jenkins
```
4. Run the Jenkins docker image:
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
5. Configure the Jenkins host according to the [Post-installation setup wizard](https://jenkins.io/doc/book/installing#setup-wizard). The following plugins are important:
  - git:3.12.0



## Pitfalls, errors and debugging

<b>Resolving the access error in Jenkins</b>
[This element needs more time to explore.](https://github.com/jenkinsci/docker#installing-more-tools)

<b>Dockercross error note</b><br>
The following error occurred during the build in Jenkis, due to the issue of running Dockcross using the Docker Toolbox within a Unix container.
```
bash-4.4# ./dockcross_pre cmake --help
cp: target '/home/root/' is not a directory
chown: cannot access '/home/root': No such file or directory
```

Debugging showed the following `docker run` arguments get passed:<br>
```
-ti --name dockcross_12203 -v /home:/work -e BUILDER_UID=0 -e BUILDER_GID=0 -e BUILDER_USER=root -e BUILDER_GROUP=root dockcross/windows-static-x64:latest cmake --help
```
This issue took ma a while to resolve. The problem is that we are running Docker inside of a Docker container (the jenkins image build earlier). Docker does not allow you to mount local resources to other container unless you are using volumes, [here](https://stackoverflow.com/questions/23137544/how-to-map-volume-paths-using-dockers-volumes-from) is an example of using volumes between container.

continue here -->

## Creating a custom dockcross container
TBD.

## Contributing
To get started with contributing to my GitHub repository, please contact me [Slack](https://join.slack.com/t/napi-friends/shared_invite/enQtNDg3OTg5NDc1NzUxLWU1MWNhNmY3ZTVmY2FkMDM1ODg1MWNlMDIyYTk1OTg4OThhYzgyNDc3ZmE5NzM1ZTM2ZDQwZGI0ZjU2M2JlNDU).
