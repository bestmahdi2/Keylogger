## Windows
To change visibility of the window set the `#define` in line 29 to `visible` or `invisible`.

Simply compile into an .exe, and then run. Clion is good for this.

- `invisible` makes the window of the logger disappear, and it also starts up hidden from view. Note that it is still visible in the task manager.
- `visible` is visible, and the window does not close when typing. Great for testing it out.

Both of these save the keystrokes to a .out file when closed.

To compile:

`g++ klog_main.cpp -o klog_main.exe -static-libstdc++ -static-libgcc -luser32 -lgdi32 -lkernel32  -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic,-subsystem,windows`

---

Shortcut to capture/ignore mouse events:

`CTRL` `F!`

---

Please feel free to contribute to fix any problems!

Please note, this repo is for educational purposes only. No contributors are to fault for any actions done by this program.
