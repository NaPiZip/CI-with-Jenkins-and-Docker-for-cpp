<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/e/e9/Jenkins_logo.svg/556px-Jenkins_logo.svg.png" alt="Jenkins logo" height="42px" width="42px" align="left">
<img src="https://proxy.duckduckgo.com/iu/?u=https%3A%2F%2Fd3nmt5vlzunoa1.cloudfront.net%2Fphpstorm%2Ffiles%2F2015%2F10%2Flarge_v-trans.png&f=1" alt="Docker logo" height="42px" width="42px" align="left"><br>

# CI with Jenkins and Docker on Windows for C/C++ code
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

## How to get the Jenkins host up and running
This section describes how to set up a "local" Jenkins host, the process is pretty straight forward, assuming Docker is already up and running on the local machine.

1. Run the Jenkins docker image:
```
docker run \
  -u root \
  --rm \
  -d \
  -p 8080:8080 \
  -p 50000:50000 \
  -v jenkins-data:/var/jenkins_home \
  -v /var/run/docker.sock:/var/run/docker.sock \
  jenkinsci/blueocean
```
2. Configure the Jenkins host according to post installation [manual](https://jenkins.io/doc/book/installing#setup-wizard).

## Contributing
To get started with contributing to my GitHub repository, please contact me [Slack](https://join.slack.com/t/napi-friends/shared_invite/enQtNDg3OTg5NDc1NzUxLWU1MWNhNmY3ZTVmY2FkMDM1ODg1MWNlMDIyYTk1OTg4OThhYzgyNDc3ZmE5NzM1ZTM2ZDQwZGI0ZjU2M2JlNDU).
