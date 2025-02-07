#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); // 获取标准输入句柄
    INPUT_RECORD irInBuf;
    DWORD dwEventsRead;

    // 设置控制台编码为 UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    while (1) {
        if (!ReadConsoleInput(hStdin, &irInBuf, 1, &dwEventsRead)) { // 一次读取一个事件
            printf("ReadConsoleInput failed with error: %lu\n", GetLastError());
            break;
        }

        if (dwEventsRead == 0) {
            printf("No events read\n");
            continue;
        }

        if (irInBuf.EventType == KEY_EVENT) { // 检查是否为键盘事件
            KEY_EVENT_RECORD ker = irInBuf.Event.KeyEvent;
            if (ker.bKeyDown) { // 检查按键是否按下
                if (ker.uChar.UnicodeChar) { // 检查是否为有效字符
                    // 输出宽字符
                    wprintf(L"Key pressed: %lc\n", ker.uChar.UnicodeChar);
                } else {
                    // 输出虚拟键码
                    printf("Key code: %d\n", ker.wVirtualKeyCode);
                }
            }
        } else if (irInBuf.EventType == FOCUS_EVENT) {
            // 处理输入法事件
            FOCUS_EVENT_RECORD fer = irInBuf.Event.FocusEvent;
            if (fer.bSetFocus) {
                printf("Focus gained\n");
            } else {
                printf("Focus lost\n");
            }
        } else if (irInBuf.EventType == MENU_EVENT) {
            // 处理菜单事件
            MENU_EVENT_RECORD mer = irInBuf.Event.MenuEvent;
            printf("Menu event: %d\n", mer.dwCommandId);
        } else if (irInBuf.EventType == MOUSE_EVENT) {
            // 处理鼠标事件
            MOUSE_EVENT_RECORD mer = irInBuf.Event.MouseEvent;
            printf("Mouse event: %d\n", mer.dwEventFlags);
        } else if (irInBuf.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            // 处理窗口大小调整事件
            WINDOW_BUFFER_SIZE_RECORD wbsr = irInBuf.Event.WindowBufferSizeEvent;
            printf("Window size changed: %d x %d\n", wbsr.dwSize.X, wbsr.dwSize.Y);
        }
    }

    return 0;
}