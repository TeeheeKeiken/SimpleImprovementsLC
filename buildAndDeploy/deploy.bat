cd ..

REM Deploy output
set /p deployPath=<buildAndDeploy\deploy.path
xcopy bin\%projectFolder% "%deployPath%" /v /y

cd buildAndDeploy