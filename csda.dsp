# Microsoft Developer Studio Project File - Name="csda" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=csda - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "csda.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "csda.mak" CFG="csda - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "csda - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "csda - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Cast String Design/910Csd/csda_03-06-06", BXMDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "csda - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_SECDLL" /D "_GXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Ox /Ot /Og /Oi
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "csda - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_SECDLL" /D "_GXDLL" /D "_CMNDLL" /D "_MVCDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "csda - Win32 Release"
# Name "csda - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_remotedl1.cpp
# End Source File
# Begin Source File

SOURCE=.\ArchiveQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\AvailCasterScenId.cpp
# End Source File
# Begin Source File

SOURCE=.\bofspecmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterScen.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterScenArchiver.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterScenariosMaxIdSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterScenariosSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterScenInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringHeatChemsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringHeatsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringOrdersSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplateChemsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplateSortsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplateSpecsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplatesSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CastString.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorGanttBars.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorGanttView.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorMvc.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorScenMgrView.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorStrandSummaryView.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorTextView.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorValidnView.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringHeat.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringHeatValidnError.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringId.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringIdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringMiscProps.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringSummaryRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\CastStringValidator.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckStockApplicationRules.cpp
# End Source File
# Begin Source File

SOURCE=.\Chem.cpp
# End Source File
# Begin Source File

SOURCE=.\ChemRange.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CIBogieSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CICode.cpp
# End Source File
# Begin Source File

SOURCE=.\CICodeGroupDefSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CICodeGroupXrefSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CICodeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\clipFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\csda.cpp
# End Source File
# Begin Source File

SOURCE=.\csda.odl
# End Source File
# Begin Source File

SOURCE=.\csda.rc
# End Source File
# Begin Source File

SOURCE=.\CsdaEventLogMaxId.cpp
# End Source File
# Begin Source File

SOURCE=.\CsdaEventLogSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CsdaEventLogSet2.cpp
# End Source File
# Begin Source File

SOURCE=.\CSOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\CSOrderParmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DailyPlanLastDownloadTimeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DailyPlanLineupsUploadSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DailyPlanUploadSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DalyPlanOrdersOnlySet.cpp
# End Source File
# Begin Source File

SOURCE=.\DalyPlanSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DateTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DBConn.cpp
# End Source File
# Begin Source File

SOURCE=.\DocBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DocMvcMixin.cpp
# End Source File
# Begin Source File

SOURCE=.\DragSourceButton.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GanttColorChartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GanttDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatChemsGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatDetailsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatDetailsGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatLockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatSpecCrossApp.cpp
# End Source File
# Begin Source File

SOURCE=.\HeatSpecCrossAppSet.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLastLoadCountSet.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphBars.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphMvc.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphScenSeqView.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphSheetView.cpp
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphView.cpp
# End Source File
# Begin Source File

SOURCE=.\HMStartTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HMSupplyActual.cpp
# End Source File
# Begin Source File

SOURCE=.\HMSupplyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HMSupplyProjected.cpp
# End Source File
# Begin Source File

SOURCE=.\InitialScenLoaderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LongOpn.cpp
# End Source File
# Begin Source File

SOURCE=.\LongOpnFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MaintainListCtrlPosn.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBus.cpp
# End Source File
# Begin Source File

SOURCE=.\MillOrderChemsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\MiscConstants.cpp
# End Source File
# Begin Source File

SOURCE=.\MruOneScenDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MruOneScenMVC.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\msoutl85.cpp
# End Source File
# Begin Source File

SOURCE=.\MultipleOrderInsertDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultipleOrderInsertGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify.cpp
# End Source File
# Begin Source File

SOURCE=.\NOutputWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NReport.cpp
# End Source File
# Begin Source File

SOURCE=.\NStateBitmapButton.cpp
# End Source File
# Begin Source File

SOURCE=.\nTime.cpp
# End Source File
# Begin Source File

SOURCE=.\order.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderColor.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderInquiryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderNumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PerUserDefaultValuesSet.cpp
# End Source File
# Begin Source File

SOURCE=.\PickCasterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlanWorksheetReport.cpp
# End Source File
# Begin Source File

SOURCE=.\PpgCICodeSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\PpgHeatSizes.cpp
# End Source File
# Begin Source File

SOURCE=.\PpgOrderChemRange.cpp
# End Source File
# Begin Source File

SOURCE=.\PpgOrderRange.cpp
# End Source File
# Begin Source File

SOURCE=.\ProdnScen.cpp
# End Source File
# Begin Source File

SOURCE=.\PSheetAvailOrders.cpp
# End Source File
# Begin Source File

SOURCE=.\PSheetMiscConstants.cpp
# End Source File
# Begin Source File

SOURCE=.\PstConformanceReport.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\sbdriver.cpp
# End Source File
# Begin Source File

SOURCE=.\Scenario.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMasterChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMasterCtrlView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMasterDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMasterGraphView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMasterMvc.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMgrChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMgrDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMgrMvc.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMgrView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenMru.cpp
# End Source File
# Begin Source File

SOURCE=.\Select909sDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Snapshot.cpp
# End Source File
# Begin Source File

SOURCE=.\Spec.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecChemRangePreset.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecChemRangePresetSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecCmp.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecDPAChemsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecDPASet.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecSelectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SteelSpecCrossAppSet.cpp
# End Source File
# Begin Source File

SOURCE=.\stlUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\StringBuilderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StringBuilderInputSet.cpp
# End Source File
# Begin Source File

SOURCE=.\StringBuilderOutputSet.cpp
# End Source File
# Begin Source File

SOURCE=.\StringStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StringStatusGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\SuperScen.cpp
# End Source File
# Begin Source File

SOURCE=.\TabNumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UnappOrderSet.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadValidnErrorsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\UsersSet.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\Week.cpp
# End Source File
# Begin Source File

SOURCE=.\WinUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_remotedl1.h
# End Source File
# Begin Source File

SOURCE=.\ArchiveQueue.h
# End Source File
# Begin Source File

SOURCE=.\AvailCasterScenId.h
# End Source File
# Begin Source File

SOURCE=.\bofspecmgr.h
# End Source File
# Begin Source File

SOURCE=.\CasterScen.h
# End Source File
# Begin Source File

SOURCE=.\CasterScenArchiver.h
# End Source File
# Begin Source File

SOURCE=.\CasterScenariosMaxIdSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterScenariosSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterScenInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringHeatChemsSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringHeatsSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringOrdersSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringsSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplateChemsSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplateSortsSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplateSpecsSet.h
# End Source File
# Begin Source File

SOURCE=.\CasterStringTemplatesSet.h
# End Source File
# Begin Source File

SOURCE=.\CastString.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorGanttBars.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorGanttView.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorMvc.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorScenMgrView.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorStrandSummaryView.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorTextView.h
# End Source File
# Begin Source File

SOURCE=.\CastStringEditorValidnView.h
# End Source File
# Begin Source File

SOURCE=.\CastStringHeat.h
# End Source File
# Begin Source File

SOURCE=.\CastStringHeatValidnError.h
# End Source File
# Begin Source File

SOURCE=.\CastStringId.h
# End Source File
# Begin Source File

SOURCE=.\CastStringIdDlg.h
# End Source File
# Begin Source File

SOURCE=.\CastStringMiscProps.h
# End Source File
# Begin Source File

SOURCE=.\CastStringSummaryRecord.h
# End Source File
# Begin Source File

SOURCE=.\CastStringValidator.h
# End Source File
# Begin Source File

SOURCE=.\CheckStockApplicationRules.h
# End Source File
# Begin Source File

SOURCE=.\Chem.h
# End Source File
# Begin Source File

SOURCE=.\ChemRange.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CIBogieSet.h
# End Source File
# Begin Source File

SOURCE=.\CICode.h
# End Source File
# Begin Source File

SOURCE=.\CICodeGroupDefSet.h
# End Source File
# Begin Source File

SOURCE=.\CICodeGroupXrefSet.h
# End Source File
# Begin Source File

SOURCE=.\CICodeSet.h
# End Source File
# Begin Source File

SOURCE=.\clipFormat.h
# End Source File
# Begin Source File

SOURCE=.\csda.h
# End Source File
# Begin Source File

SOURCE=.\CsdaEventLogMaxId.h
# End Source File
# Begin Source File

SOURCE=.\CsdaEventLogSet.h
# End Source File
# Begin Source File

SOURCE=.\CsdaEventLogSet2.h
# End Source File
# Begin Source File

SOURCE=.\CSOrder.h
# End Source File
# Begin Source File

SOURCE=.\CSOrderParmDlg.h
# End Source File
# Begin Source File

SOURCE=.\DailyPlanLastDownloadTimeSet.h
# End Source File
# Begin Source File

SOURCE=.\DailyPlanLineupsUploadSet.h
# End Source File
# Begin Source File

SOURCE=.\DailyPlanUploadSet.h
# End Source File
# Begin Source File

SOURCE=.\DalyPlanOrdersOnlySet.h
# End Source File
# Begin Source File

SOURCE=.\DalyPlanSet.h
# End Source File
# Begin Source File

SOURCE=.\DateTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\DBConn.h
# End Source File
# Begin Source File

SOURCE=.\DocBase.h
# End Source File
# Begin Source File

SOURCE=.\DocMvcMixin.h
# End Source File
# Begin Source File

SOURCE=.\DragSourceButton.h
# End Source File
# Begin Source File

SOURCE=.\EMailDlg.h
# End Source File
# Begin Source File

SOURCE=.\GanttColorChartDlg.h
# End Source File
# Begin Source File

SOURCE=.\GanttDisplay.h
# End Source File
# Begin Source File

SOURCE=.\HeaderInfo.h
# End Source File
# Begin Source File

SOURCE=.\HeatChemsGrid.h
# End Source File
# Begin Source File

SOURCE=.\HeatDetailsDlg.h
# End Source File
# Begin Source File

SOURCE=.\HeatDetailsGrid.h
# End Source File
# Begin Source File

SOURCE=.\HeatLockDlg.h
# End Source File
# Begin Source File

SOURCE=.\HeatSpecCrossApp.h
# End Source File
# Begin Source File

SOURCE=.\HeatSpecCrossAppSet.h
# End Source File
# Begin Source File

SOURCE=.\HMLastLoadCountSet.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphBars.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphDoc.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphMvc.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphScenSeqView.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphSheetView.h
# End Source File
# Begin Source File

SOURCE=.\HMLoadGraphView.h
# End Source File
# Begin Source File

SOURCE=.\HMStartTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\HMSupplyActual.h
# End Source File
# Begin Source File

SOURCE=.\HMSupplyMgr.h
# End Source File
# Begin Source File

SOURCE=.\HMSupplyProjected.h
# End Source File
# Begin Source File

SOURCE=.\InitialScenLoaderDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\LongOpn.h
# End Source File
# Begin Source File

SOURCE=.\LongOpnFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MaintainListCtrlPosn.h
# End Source File
# Begin Source File

SOURCE=.\MessageBus.h
# End Source File
# Begin Source File

SOURCE=.\MillOrderChemsSet.h
# End Source File
# Begin Source File

SOURCE=.\MiscConstants.h
# End Source File
# Begin Source File

SOURCE=.\modelTypes.h
# End Source File
# Begin Source File

SOURCE=.\MruOneScenDlgBar.h
# End Source File
# Begin Source File

SOURCE=.\MruOneScenMVC.h
# End Source File
# Begin Source File

SOURCE=.\MsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\msoutl85.h
# End Source File
# Begin Source File

SOURCE=.\MultipleOrderInsertDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultipleOrderInsertGrid.h
# End Source File
# Begin Source File

SOURCE=.\MultiSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Notify.h
# End Source File
# Begin Source File

SOURCE=.\NOutputWnd.h
# End Source File
# Begin Source File

SOURCE=.\NReport.h
# End Source File
# Begin Source File

SOURCE=.\NStateBitmapButton.h
# End Source File
# Begin Source File

SOURCE=.\nTime.h
# End Source File
# Begin Source File

SOURCE=.\Order.h
# End Source File
# Begin Source File

SOURCE=.\OrderColor.h
# End Source File
# Begin Source File

SOURCE=.\OrderInquiryDlg.h
# End Source File
# Begin Source File

SOURCE=.\OrderNumDlg.h
# End Source File
# Begin Source File

SOURCE=.\OrderSelection.h
# End Source File
# Begin Source File

SOURCE=.\OutputControlBar.h
# End Source File
# Begin Source File

SOURCE=.\OutputWnd.h
# End Source File
# Begin Source File

SOURCE=.\PerUserDefaultValuesSet.h
# End Source File
# Begin Source File

SOURCE=.\PickCasterDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlanWorksheetReport.h
# End Source File
# Begin Source File

SOURCE=.\PpgCICodeSelect.h
# End Source File
# Begin Source File

SOURCE=.\PpgHeatSizes.h
# End Source File
# Begin Source File

SOURCE=.\PpgOrderChemRange.h
# End Source File
# Begin Source File

SOURCE=.\PpgOrderRange.h
# End Source File
# Begin Source File

SOURCE=.\ProdnScen.h
# End Source File
# Begin Source File

SOURCE=.\PSheetAvailOrders.h
# End Source File
# Begin Source File

SOURCE=.\PSheetMiscConstants.h
# End Source File
# Begin Source File

SOURCE=.\PstConformanceReport.h
# End Source File
# Begin Source File

SOURCE=.\ReportDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\resource2.h
# End Source File
# Begin Source File

SOURCE=.\sbdriver.h
# End Source File
# Begin Source File

SOURCE=.\Scenario.h
# End Source File
# Begin Source File

SOURCE=.\ScenMasterChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\ScenMasterCtrlView.h
# End Source File
# Begin Source File

SOURCE=.\ScenMasterDoc.h
# End Source File
# Begin Source File

SOURCE=.\ScenMasterGraphView.h
# End Source File
# Begin Source File

SOURCE=.\ScenMasterMvc.h
# End Source File
# Begin Source File

SOURCE=.\ScenMgr.h
# End Source File
# Begin Source File

SOURCE=.\ScenMgrChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\ScenMgrDoc.h
# End Source File
# Begin Source File

SOURCE=.\ScenMgrMvc.h
# End Source File
# Begin Source File

SOURCE=.\ScenMgrView.h
# End Source File
# Begin Source File

SOURCE=.\ScenMru.h
# End Source File
# Begin Source File

SOURCE=.\Select909sDlg.h
# End Source File
# Begin Source File

SOURCE=.\Snapshot.h
# End Source File
# Begin Source File

SOURCE=.\Spec.h
# End Source File
# Begin Source File

SOURCE=.\SpecChemRangePreset.h
# End Source File
# Begin Source File

SOURCE=.\SpecChemRangePresetSet.h
# End Source File
# Begin Source File

SOURCE=.\SpecCmp.h
# End Source File
# Begin Source File

SOURCE=.\SpecDPAChemsSet.h
# End Source File
# Begin Source File

SOURCE=.\SpecDPASet.h
# End Source File
# Begin Source File

SOURCE=.\SpecMgr.h
# End Source File
# Begin Source File

SOURCE=.\SpecSelectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SteelSpecCrossAppSet.h
# End Source File
# Begin Source File

SOURCE=.\stlext.h
# End Source File
# Begin Source File

SOURCE=.\stlUtils.h
# End Source File
# Begin Source File

SOURCE=.\StringBuilderDlg.h
# End Source File
# Begin Source File

SOURCE=.\StringBuilderInputSet.h
# End Source File
# Begin Source File

SOURCE=.\StringBuilderOutputSet.h
# End Source File
# Begin Source File

SOURCE=.\StringStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\StringStatusGrid.h
# End Source File
# Begin Source File

SOURCE=.\SuperScen.h
# End Source File
# Begin Source File

SOURCE=.\TabNumDlg.h
# End Source File
# Begin Source File

SOURCE=.\toolbars.h
# End Source File
# Begin Source File

SOURCE=.\UnappOrderSet.h
# End Source File
# Begin Source File

SOURCE=.\UploadDlg.h
# End Source File
# Begin Source File

SOURCE=.\UploadMgr.h
# End Source File
# Begin Source File

SOURCE=.\UploadValidnErrorsDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserInfo.h
# End Source File
# Begin Source File

SOURCE=.\UsersSet.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\Week.h
# End Source File
# Begin Source File

SOURCE=.\winUtils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\aip_tagl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_m.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_o.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\calcbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cand_hap.ico
# End Source File
# Begin Source File

SOURCE=.\res\cfolder.ico
# End Source File
# Begin Source File

SOURCE=.\res\csda.bmp
# End Source File
# Begin Source File

SOURCE=.\res\csda.ico
# End Source File
# Begin Source File

SOURCE=.\res\csda.rc2
# End Source File
# Begin Source File

SOURCE=.\res\csdaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor4.cur
# End Source File
# Begin Source File

SOURCE=.\res\end_of_o.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_cal.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_que.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_was.ico
# End Source File
# Begin Source File

SOURCE=.\res\inland_i.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inland_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\insert_o.bmp
# End Source File
# Begin Source File

SOURCE=.\res\insert_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ispat_logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\locked1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\locked_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\long_opn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ofolder.ico
# End Source File
# Begin Source File

SOURCE=.\res\order_im.bmp
# End Source File
# Begin Source File

SOURCE=.\res\order_li.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PAN_HAND.CUR
# End Source File
# Begin Source File

SOURCE=.\res\scen_mas.bmp
# End Source File
# Begin Source File

SOURCE=.\res\semilock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\small_do.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splashwn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\unlocked.bmp
# End Source File
# Begin Source File

SOURCE=.\res\upload.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ZOOMDRAG.CUR
# End Source File
# Begin Source File

SOURCE=.\res\ZOOMIN.CUR
# End Source File
# Begin Source File

SOURCE=.\res\ZOOMOUT.CUR
# End Source File
# Begin Source File

SOURCE=.\res\ZOOMRECT.CUR
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\csda.cnt
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\csda.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMore.txt
# End Source File
# End Target
# End Project
# Section csda : {88BF0B6F-693D-11D3-85F5-00104B2D39AC}
# 	2:21:DefaultSinkHeaderFile:sbdriver.h
# 	2:16:DefaultSinkClass:CSBDriver
# End Section
# Section csda : {987AEC3C-E6FC-11D1-9DE3-00609732B8E1}
# 	2:21:DefaultSinkHeaderFile:_remotedl.h
# 	2:16:DefaultSinkClass:C_remoteDL
# End Section
# Section csda : {987AECB5-E6FC-11D1-9DE3-00609732B8E1}
# 	2:21:DefaultSinkHeaderFile:_remotedl2.h
# 	2:16:DefaultSinkClass:C_remoteDL2
# End Section
# Section csda : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section csda : {F3FB5E20-A837-11D2-9137-0060081AA7BC}
# 	2:21:DefaultSinkHeaderFile:_remotedl1.h
# 	2:16:DefaultSinkClass:C_remoteDL1
# End Section
# Section csda : {F4ABF870-6947-11D3-85F5-00104B2D39AC}
# 	2:21:DefaultSinkHeaderFile:sbdriver.h
# 	2:16:DefaultSinkClass:CSBDriver
# End Section
# Section csda : {987AEC3B-E6FC-11D1-9DE3-00609732B8E1}
# 	2:5:Class:C_remoteDL
# 	2:10:HeaderFile:_remotedl.h
# 	2:8:ImplFile:_remotedl.cpp
# End Section
# Section csda : {88BF0B6E-693D-11D3-85F5-00104B2D39AC}
# 	2:5:Class:CSBDriver
# 	2:10:HeaderFile:sbdriver.h
# 	2:8:ImplFile:sbdriver.cpp
# End Section
# Section csda : {95756C13-68C9-11D3-AA71-00105AE25435}
# 	2:5:Class:CSBDriver
# 	2:10:HeaderFile:sbdriver.h
# 	2:8:ImplFile:sbdriver.cpp
# End Section
# Section csda : {987AECB4-E6FC-11D1-9DE3-00609732B8E1}
# 	2:5:Class:C_remoteDL2
# 	2:10:HeaderFile:_remotedl2.h
# 	2:8:ImplFile:_remotedl2.cpp
# End Section
# Section csda : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section csda : {CA7EC1DF-6954-11D3-85F5-00104B2D39AC}
# 	2:21:DefaultSinkHeaderFile:sbdriver.h
# 	2:16:DefaultSinkClass:CSBDriver
# End Section
# Section csda : {F3FB5E1F-A837-11D2-9137-0060081AA7BC}
# 	2:5:Class:C_remoteDL1
# 	2:10:HeaderFile:_remotedl1.h
# 	2:8:ImplFile:_remotedl1.cpp
# End Section
# Section csda : {F4ABF86F-6947-11D3-85F5-00104B2D39AC}
# 	2:5:Class:CSBDriver
# 	2:10:HeaderFile:sbdriver.h
# 	2:8:ImplFile:sbdriver.cpp
# End Section
# Section csda : {1B401444-6A64-11D3-AA72-00105AE25435}
# 	2:21:DefaultSinkHeaderFile:sbdriver.h
# 	2:16:DefaultSinkClass:CSBDriver
# End Section
# Section csda : {CA7EC1DE-6954-11D3-85F5-00104B2D39AC}
# 	2:5:Class:CSBDriver
# 	2:10:HeaderFile:sbdriver.h
# 	2:8:ImplFile:sbdriver.cpp
# End Section
# Section csda : {1B401443-6A64-11D3-AA72-00105AE25435}
# 	2:5:Class:CSBDriver
# 	2:10:HeaderFile:sbdriver.h
# 	2:8:ImplFile:sbdriver.cpp
# End Section
# Section csda : {95756C14-68C9-11D3-AA71-00105AE25435}
# 	2:21:DefaultSinkHeaderFile:sbdriver.h
# 	2:16:DefaultSinkClass:CSBDriver
# End Section
