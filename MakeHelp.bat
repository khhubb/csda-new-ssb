@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CSDA.HPJ. >"hlp\csda.hm"
echo. >>"hlp\csda.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\csda.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\csda.hm"
echo. >>"hlp\csda.hm"
echo // Prompts (IDP_*) >>"hlp\csda.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\csda.hm"
echo. >>"hlp\csda.hm"
echo // Resources (IDR_*) >>"hlp\csda.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\csda.hm"
echo. >>"hlp\csda.hm"
echo // Dialogs (IDD_*) >>"hlp\csda.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\csda.hm"
echo. >>"hlp\csda.hm"
echo // Frame Controls (IDW_*) >>"hlp\csda.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\csda.hm"
REM -- Make help for Project CSDA


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\csda.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\csda.hlp" goto :Error
if not exist "hlp\csda.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\csda.hlp" Debug
if exist Debug\nul copy "hlp\csda.cnt" Debug
if exist Release\nul copy "hlp\csda.hlp" Release
if exist Release\nul copy "hlp\csda.cnt" Release
echo.
goto :done

:Error
echo hlp\csda.hpj(1) : error: Problem encountered creating help file

:done
echo.
