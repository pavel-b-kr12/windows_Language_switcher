# windows_Language_switcher
switch language according to ScrollLock key

put in
%HOMEPATH%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
## this is
the fork of <https://haali.su/winutils/>

## Difference from original:
* key event pass through, so you will see language stage as LEDs state
* set and switch language with single keys:
* 'right ctrl' set English and off LED
* 'pause' set 2nd language and light LED. (it have to be right shift but seems usefull to type ;:)
* 'ScrollLock' switch language and set LED state as rules above said.
* binded to ScrollLock RCTRL RSHIFT keys only (but you can recompile with other keycodes)
* optional command-line arguments default: "..../lswitch.exe 145 163 19" where 1st arg for switch key (can be ScrollLock, CapsLock, NumLock), 2nd for eng, 3rd for other lang
* key codes for command line <https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes>
## DOTO
* show state as backgrounds, windows borders, cursor color or other easy and ALWAYS visible elements. Use <https://github.com/pavel-b-kr12/langcursor>
windows mode light dark switch SetWindowTheme(hwnd, L"DarkMode_Explorer", NULL); <https://stackoverflow.com/questions/53501268/win10-dark-theme-how-to-use-in-winapi>
* update language after switch windows or sent message to all windows, because OS auto-switching language per window not alwas possible to be disabled (but in current build it seems working and possible to disable autoswitch languages per window)
* fix:  may not work if application has only one window and it's modal
* make ultimate physical switch using Arduino, some low-force switch (or optical) and magnets to stick it on keyboard, publish to Instructable
* more keys for more than 2 languages also use windows shortcut to set LED
### done
[+] use any keys but show state with LED
[+] to detect eng use only The low word contains a Language Identifier for HKL
## alternative
* C# approach <https://github.com/kurumpa/dotSwitcher/commit/7e3936e217128e21d334250b053d896bfa3f6e1f>
* <https://github.com/dims12/NormalKeyboardSwitcher>
* Windows + Space keys to display the language menu

 status: 
* langcursor <https://github.com/pavel-b-kr12/langcursor> is the fixed version of <https://github.com/saaremaa/langcursor> what is improved version of <https://habr.com/ru/post/138940/>
There are versions with Lshift and always visible 
<https://web.archive.org/web/20140910214148/https://bitbucket.org/veg/langcursor/src/50c4913dd0d9/main.cpp>
* https://web.archive.org/web/20161029074558/http://www.keybroadapp.com/

## inf
See settings at:
 
win R

Rundll32 Shell32.dll,Control_RunDLL input.dll,,{C07337D3-DB2C-4D0B-9A93-B722A6C106E2}

or Settings -> Time & Language -> Language -> Spelling, typing, & keyboard settings -> Advanced keyboard settings -> Language bar options -> Advanced Key Settings
