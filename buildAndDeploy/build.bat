cd ..

REM Clear bin and copy files
rmdir bin\%projectFolder% /s /q
del bin /s /q
xcopy %projectFolder% bin\%projectFolder% /i /s /e /y /v

REM Version format without local changes: 	"dev_<branch>_<lastcommitdate>_<commithash>"
REM Version format with local changes:		"dev_<branch>_local_<lastcommitdate>_<commithash>"
for /F "tokens=*" %%a in ('git log -1 --format^="%%ad_%%H" --date^=iso-strict') do set commit=%%a
for /F "tokens=*" %%a in ('git rev-parse --abbrev-ref HEAD') do set branch=%%a
REM Disable echo for a bit so the output stays readable
echo off
for /F "tokens=*" %%a in ('git diff') do set diff=%%a
set changesType=_local
if "%diff%"=="" set changesType=
echo on
echo dev_%branch%%changesType%_%commit% >> bin\%projectFolder%\Version.txt

REM Pack everything
set /p c4group=<buildAndDeploy\c4group.path
"%c4group%" bin\%projectFolder% -p

cd buildAndDeploy