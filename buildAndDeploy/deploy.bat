cd ..

REM Deploy output
set /p deployPath=<buildAndDeploy\deploy.path
xcopy bin\SimpleImprovements.c4d "%deployPath%" /v /y

cd buildAndDeploy