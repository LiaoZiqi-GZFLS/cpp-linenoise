#include <iostream>
#include <windows.h>
#include <cstdio>
#include <io.h>

int main() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); // 获取标准输入句柄
    INPUT_RECORD irInBuf[128]; // 一次读取最多 128 个事件
    DWORD dwEventsRead;

    // 启用控制台的 UTF-8 支持
    DWORD dwMode = 0;
    GetConsoleMode(hStdin, &dwMode);
    dwMode |= ENABLE_PROCESSED_INPUT; // 启用处理模式
    SetConsoleMode(hStdin, dwMode);

    SetConsoleCP(CP_UTF8); // 设置输入编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8); // 设置输出编码为 UTF-8

    char buffer[50]; // 临时缓冲区
    char history[1024] = {}; // 历史输入缓冲区
    int historyIndex = 0; // 历史输入索引
    int ret;
    int fd = 1; // 文件描述符 1 表示标准输出（控制台）

    while (1) {
        if (!ReadConsoleInput(hStdin, irInBuf, 128, &dwEventsRead)) { // 一次读取多个事件
            printf("ReadConsoleInput failed with error: %lu\n", GetLastError());
            break;
        }

        for (DWORD i = 0; i < dwEventsRead; ++i) {
            if (irInBuf[i].EventType == KEY_EVENT) { // 检查是否为键盘事件
                KEY_EVENT_RECORD ker = irInBuf[i].Event.KeyEvent;
                if (ker.bKeyDown) { // 检查按键是否按下
                    if (ker.uChar.UnicodeChar) { // 检查是否为有效字符
                        // 将宽字符转换为 UTF-8 字符
                        ret = WideCharToMultiByte(CP_UTF8, 0, &ker.uChar.UnicodeChar, 1, buffer, sizeof(buffer) - 1, nullptr, nullptr);
                        if (ret > 0) {
                            buffer[ret] = '\0'; // 确保字符串以 null 结尾
                            
                            // 输出字符
                            //int bytesWritten = _write(fd, buffer, ret);
                            std::cout<<char(ker.uChar.UnicodeChar);
                            //if (bytesWritten == -1) {
                                //printf("Write failed with error: %d\n", errno);
                            //}

                            // 将字符存储到历史输入缓冲区
                            if (historyIndex < sizeof(history) - 1) {
                                for (int j = 0; j < ret; ++j) {
                                    history[historyIndex++] = buffer[j];
                                }
                                history[historyIndex] = '\0'; // 确保历史输入以 null 结尾
                            } else {
                                printf("History buffer is full, input ignored.\n");
                            }
                        } else {
                            printf("Conversion to UTF-8 failed with error: %d\n", GetLastError());
                        }
                    }
                }
            }
        }
    }

    // 关闭句柄
    if (hStdin != INVALID_HANDLE_VALUE) {
        CloseHandle(hStdin);
    }

    return 0;
}