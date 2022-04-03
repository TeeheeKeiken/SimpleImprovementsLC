cd ..

REM Clear bin and copy files
rmdir bin\SimpleImprovements.c4d /s /q
del bin /s /q
xcopy SimpleImprovements.c4d bin\SimpleImprovements.c4d /i /s /e /y /v

REM Pack everything
set /p c4group=<buildAndDeploy\c4group.path
"%c4group%" bin\SimpleImprovements.c4d -p

cd buildAndDeploy