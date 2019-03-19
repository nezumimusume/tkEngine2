set dest=%1%
set cocos2dLibPath=..\..\..\etc-bin\cocos2dx\cocos2dx\out\%dest%\

copy %cocos2dLibPath%*.dll %dest% /Y

copy ..\resources\*.* %dest% /Y
goto end

:end