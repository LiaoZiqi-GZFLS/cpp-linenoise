#include "..\linenoise.hpp"
#include <iostream>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void setCmdEnv(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    system("@chcp 65001>nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  
    return;
}

int main() {
    setCmdEnv();

    const auto path = "history.txt";

    // 设置自动补全回调
    linenoise::SetCompletionCallback([](const char* editBuffer, std::vector<std::string>& completions) {
        if (editBuffer[0] == 'h') {
            completions.push_back("hello");
            completions.push_back("hello there");
        }
        });

    // 启用多行模式
    linenoise::SetMultiLine(true);

    // 设置历史记录最大长度
    linenoise::SetHistoryMaxLen(4);

    // 加载历史记录
    linenoise::LoadHistory(path);

    while (true) {
        std::string line;
        auto quit = linenoise::Readline("hello> ", line);
        if (quit) {
            break;
        }
        std::cout << "echo: '" << line << "'" << std::endl;
        // 添加文本到历史记录
        linenoise::AddHistory(line.c_str());
    }

    // 保存历史记录
    linenoise::SaveHistory(path);

    return 0;
}