#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
#Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.

RootFolder := RegExReplace(A_ScriptDir, "\\[^\\]+$")
BinaryFolder := RootFolder "\client"

SetWorkingDir, %BinaryFolder%  ; Ensures a consistent starting directory.

;
; First instance
;
Run launcher.exe

; console
WinWait m2o-reborn-cli
WinActivate
WinSetTitle, m2o-cli-1
WinMove, m2o-cli-1,, 0, (A_ScreenHeight / 1.5), (A_ScreenWidth / 2), (A_ScreenHeight / 3)

; game window
WinWait Mafia II
WinActivate
WinSetTitle, m2o-app-1
WinMove, m2o-app-1,, 0, 0, (A_ScreenWidth / 2), (A_ScreenHeight / 1.5)

;
; Second instance
;
Run launcher.exe

; console
WinWait m2o-reborn-cli
WinActivate
WinSetTitle, m2o-cli-2
WinMove, m2o-cli-2,, (A_ScreenWidth / 2), (A_ScreenHeight / 1.5), (A_ScreenWidth / 2), (A_ScreenHeight / 3)

; game window
WinWait Mafia II
WinActivate
WinSetTitle, m2o-app-2
WinMove, m2o-app-2,, (A_ScreenWidth / 2), 0, (A_ScreenWidth / 2), (A_ScreenHeight / 1.5)
