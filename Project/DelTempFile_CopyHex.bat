@echo off
set dt=%date:~0,4%%date:~5,2%%date:~8,2%-%time:~0,2%%time:~3,2%%time:~6,2%
echo %dt%
pause

@echo on
copy Objects\smartswitch.hex ..\output_smartswitch_ad_485_%dt%.hex
dir ..\output_smartswitch_ad_485_%dt%.hex