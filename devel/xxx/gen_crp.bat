@REM Template batch file for generating the crp for a library.
@REM $$Id$$

@if NOT %1==$NAME$ goto end_$NAME$
@set OUTPUT=\temp\$NAME$.crp
@toupper "xxx	+$NAME$	" >%OUTPUT%
@echo # >>%OUTPUT%
@tolower "xxx	+$NAME$	" >>%OUTPUT%
@echo # >>%OUTPUT%
@toupper "NAME	+" >>%OUTPUT%
@echo $NAME$ >>%OUTPUT%
@echo COPYRIGHT_DATES	+$COPYRIGHT_DATES$2004 >>%OUTPUT%
@echo DESCRIPTION	+$DESCRIPTION$ >>%OUTPUT%
type \cvs\epeios\devel\xxx\$PROJECT$.crp >>%OUTPUT%
REM needed by the rest of the calling script.
set PROJECT=$PROJECT$
:end_$NAME$

