set "CWD=%cd%"
set "CWD=%CWD:\=/%"

cd /d "C:\Program Files\Docker Toolbox"

"C:\Program Files\Git\bin\bash.exe" --login -i "C:\Program Files\Docker Toolbox\start.sh" %CWD%/build.sh
