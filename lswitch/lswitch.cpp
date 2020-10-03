#define _WIN32_WINNT 0x500
#include <windows.h>
#include <tchar.h>
#include <winnls.h>
#include <iostream>
#include <locale.h>
#include <locale>
#include <shellapi.h>
using namespace std;

//defaults when command-line args <3
int k_lang_eng = VK_RCONTROL; //163
int k_lang_other = VK_PAUSE;    //19    //VK_RSHIFT
int k_lang_sw_LED = VK_SCROLL;   //145

void  failedx(const TCHAR* msg) {
    MessageBox(NULL, msg, _T("Error"), MB_OK | MB_ICONERROR);
    ExitProcess(1);
}

void  failed(const TCHAR* msg) {
    DWORD		fm;
    TCHAR* msg1, * msg2;
    const TCHAR* args[2];

    fm = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), 0, (LPTSTR)&msg1, 0, NULL);
    if (fm == 0)
        ExitProcess(1);
    args[0] = msg;
    args[1] = msg1;
    fm = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING |
        FORMAT_MESSAGE_ARGUMENT_ARRAY,
        _T("%1: %2"), 0, 0, (LPTSTR)&msg2, 0, (va_list*)&args[0]);
    if (fm == 0)
        ExitProcess(1);
    MessageBox(NULL, msg2, _T("Error"), MB_OK | MB_ICONERROR);
    ExitProcess(1);
}


TCHAR	g_prog_dir[MAX_PATH * 2];
DWORD	g_prog_dir_len;
HHOOK	g_khook;
HANDLE  g_hEvent;

bool bSkipInput=false;
void pressScrollLock()
{
    keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY, 0);
    Sleep(10);
    keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}
LRESULT CALLBACK KbdHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0)
        return CallNextHookEx(g_khook, nCode, wParam, lParam);
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;

        if (ks->vkCode == k_lang_other || ks->vkCode == k_lang_eng || ks->vkCode == k_lang_sw_LED)
        {
            if (wParam == WM_KEYDOWN) {
                if (bSkipInput)
                {
                    bSkipInput = false;
                    return 1; //do not do default key action
                }


                HWND hWnd = GetForegroundWindow();
                //if (hWnd!=NULL)
                //{
                hWnd = GetAncestor(hWnd, GA_ROOTOWNER); //this for working in modal windows (dialogs), 
                //but still can't work if application has only one window and it's modal
                //@@@ https://stackoverflow.com/questions/27720728/cant-send-wm-inputlangchangerequest-to-some-controls

                bool bScroll = (GetKeyState(k_lang_sw_LED) & 0x0001) != 0; //true == LED on for different lang (not eng)  //VK_NUMLOCK VK_CAPITAL VK_SCROLL
                
				//bool bEng = (int)GetKeyboardLayout(GetWindowThreadProcessId(hWnd, NULL)) == 67699721;
				//or
					auto hkl_num = LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(hWnd, NULL)));
					auto hkl_en_num = LOWORD(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)); // eng is  04090409 == 67699721
					bool bEng = hkl_num == hkl_en_num;
				

                if (ks->vkCode == k_lang_sw_LED) //scroll lock
                { //switch lang, fix LED to current lang

                    PostMessage(hWnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)HKL_NEXT);
                    bEng = !bEng;
                    if (bEng == bScroll)
                    { //LED shows wrong lang state, so fix it:
                        bSkipInput = true;
                        pressScrollLock();

                        return CallNextHookEx(g_khook, nCode, wParam, lParam); //do ~, e.g. switch LEDs on keyboard after press Lock keys
                    }

                    return 1;  //do not do default key action, so no change LED again
                }
                else
                    if (ks->vkCode == k_lang_eng)
                    { //set eng, LED off
                        if (!bEng)
                            pressScrollLock();

                        return 1; //do not do default key action
                    }
                    else
                        if (ks->vkCode == k_lang_other)
                        { //set 2nd lang, LED on
                            if (bEng)
                                pressScrollLock();

                            return 1; //do not do default key action
                        }
            }
        }
    }
skip:
    return CallNextHookEx(g_khook, nCode, wParam, lParam);
}



void CALLBACK TimerCb(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (WaitForSingleObject(g_hEvent, 0) == WAIT_OBJECT_0)
        PostQuitMessage(0);
}

void process_value(TCHAR cmd)
{
	UINT    nk = cmd;
}

int  WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	MSG	  msg;
	BOOL	  fQuit = FALSE;

	int argc;
	char** argv;
	{
		LPWSTR* lpArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
		argv = (char**)malloc(argc * sizeof(char*));
		//if argc<4
		int size, i = 0;
		for (; i < argc; ++i)
		{
			size = wcslen(lpArgv[i]) + 1;
			argv[i] = (char*)malloc(size);
			size_t si;
			wcstombs_s(&si, argv[i], size, lpArgv[i], size); //TODO fix size, I did not read about this function, but it seems working

			int num = atoi(argv[i]);
			switch (i)
			{
			case 1: k_lang_sw_LED = num; break;
			case 2: k_lang_eng = num; break;
			case 3: k_lang_other = num; break;
			default:					break;
			}
		}
		{ //free
			int i = 0;
			for (; i < argc; ++i)
				free(argv[i]);
			free(argv);
		}
	}


	g_hEvent = CreateEvent(NULL, TRUE, FALSE, _T("HaaliLSwitch"));
	if (g_hEvent == NULL)
		failed(_T("CreateEvent()"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		if (fQuit) {
			SetEvent(g_hEvent);
			goto quit;
		}
		failedx(_T("LSwitch is already running!"));
	}

	if (fQuit)
		failedx(_T("LSwitch is not running!"));


	if (SetTimer(NULL, 0, 500, TimerCb) == 0)
		failed(_T("SetTimer()"));

	g_khook = SetWindowsHookEx(WH_KEYBOARD_LL, KbdHook, GetModuleHandle(0), 0);
	if (g_khook == 0)
		failed(_T("SetWindowsHookEx()"));

	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (g_khook != 0)
		UnhookWindowsHookEx(g_khook);
quit:
	if (g_hEvent != 0)
		CloseHandle(g_hEvent);

	ExitProcess(0);
}
