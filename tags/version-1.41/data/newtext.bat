for /f %%a IN ('dir /b dataquota.l*') do @(if "%%a" NEQ "dataquota.l01" (if "%%a" NEQ "dataquota.loc" type newtext.txt >> %%a))
