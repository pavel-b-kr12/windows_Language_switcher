# windows_Language_switcher
switch language according to ScrollLock key

put in
%HOMEPATH%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
## this is
the fork of <https://haali.su/winutils/>

## Difference:
* key event pass through, so you will see language stage as LEDs state
* bind to ScrollLock key only (you can recompile with other keycode)

## DOTO

* use any keys but show state with ScrollLock LED
* show state as backgrouds, windows borders or other easy and ALWAYS visible elements
* update language after switch windows, because OS auto-switching language per window not alwas possible to be disabled
* set lang with single key: RShift, RCtrl
* fix:  may not work if application has only one window and it's modal
## alternative
C# approach 

<https://github.com/kurumpa/dotSwitcher/commit/7e3936e217128e21d334250b053d896bfa3f6e1f>

<https://github.com/dims12/NormalKeyboardSwitcher>

Windows + Space keys to display the language menu

status: https://web.archive.org/web/20161029074558/http://www.keybroadapp.com/

## inf
See settings at:
 
win R

Rundll32 Shell32.dll,Control_RunDLL input.dll,,{C07337D3-DB2C-4D0B-9A93-B722A6C106E2}

or Settings -> Time & Language -> Language -> Spelling, typing, & keyboard settings -> Advanced keyboard settings -> Language bar options -> Advanced Key Settings
