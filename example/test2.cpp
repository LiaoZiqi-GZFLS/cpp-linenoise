#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <io.h>

int main() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); // 获取标准输入句柄
    INPUT_RECORD irInBuf[128]; // 一次读取最多 128 个事件
    DWORD dwEventsRead;

    // 设置控制台编码为 UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    char buffer[50];
    int ret;
    int fd = 1; // 文件描述符 1 表示标准输出（控制台）

    while (1) {
        if (!ReadConsoleInput(hStdin, irInBuf, 1, &dwEventsRead)) { // 一次读取多个事件
            printf("ReadConsoleInput failed with error: %lu\n", GetLastError()); 
        }

        for (DWORD i = 0; i < dwEventsRead; ++i) {
            if (irInBuf[i].EventType == KEY_EVENT) { // 检查是否为键盘事件
                KEY_EVENT_RECORD ker = irInBuf[i].Event.KeyEvent;
                if (ker.bKeyDown) { // 检查按键是否按下
                    if (ker.uChar.UnicodeChar) { // 检查是否为有效字符
                        //std::wcout<<wchar_t(ker.uChar.UnicodeChar);
                        //std::cout<<char(ker.uChar.UnicodeChar);
                        ret = snprintf(buffer,sizeof(buffer),"%c",char(ker.uChar.UnicodeChar));
                        if (ret >= 0 && ret < sizeof(buffer)) {
                            //printf("%s", buffer);
                            //int bytesWritten = _write(fd, buffer, strlen(buffer));
                            printf("%d\n",ker.uChar.UnicodeChar);
                        } else {
                            printf("Buffer size is not enough or an error occurred.\n");
                        }
                    }
                }
            }
        }
    }

    return 0;
}