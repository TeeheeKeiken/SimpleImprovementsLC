REM Start scenario in debug
set /p clonk=<clonk.path
set /p scenario=<scenario.path
start "" /wait "%clonk%" /console "%scenario%"
exit