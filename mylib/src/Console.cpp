#include "Console.h"
#include "MediaLibrary.h"
#include "UseThatHelpingFunction.h"
#include <conio.h>
#include "DoThatFunction.h"
#include <windows.h>

void Console::run(MediaLibrary& library)
{
    initialize();
    println("Welcome to the Tahitian Library. Type 'help' for commands.", Color::Green);

    bool exitRequested = false;
    while (!exitRequested)
    {
        print("\n> ", Color::White);
        std::string input = readLine();

        if (input == "exit")
        {
            DoThatFunction::exit();
            exitRequested = true;
            continue;
        }

        try
        {
            processCommand(input, library);
        }
        catch (const std::exception& e)
        {
            println("Error: " + std::string(e.what()), Color::Red);
        }
    }
}

// MAIN METHOD THAT EXECUTE THE COMMANDS
void Console::processCommand(const std::string& command, MediaLibrary& library)
{
	DoThatFunction::DoThatCommand(command, library);
}

// OTHER METHODS THAT ARE USED

void Console::initialize() 
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

WORD Console::convertColorToWindows(Color foreground, Color background)
{
    WORD foregroundAttr = 0;
    WORD backgroundAttr = 0;

    switch (foreground)
    {
    case Color::Black:
        foregroundAttr = 0;
        break;
    case Color::Red:
        foregroundAttr = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case Color::Green:
        foregroundAttr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case Color::Blue:
        foregroundAttr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case Color::White:
        foregroundAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case Color::Orange:
        foregroundAttr = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    default:
        foregroundAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    switch (background)
    {
    case Color::Black:
        backgroundAttr = 0;
        break;
    case Color::Red:
        backgroundAttr = BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;
    case Color::Green:
        backgroundAttr = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;
    case Color::Blue:
        backgroundAttr = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;
    case Color::White:
        backgroundAttr = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;
    case Color::Orange:
        backgroundAttr = BACKGROUND_RED | BACKGROUND_GREEN;
        break;
    default:
        backgroundAttr = 0;
    }

    return foregroundAttr | backgroundAttr;
}

void Console::clear(const std::string& lastCommand)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

void Console::print(const std::string& text, Color color) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Cannot get console handle");

    WORD attributes = convertColorToWindows(color, Color::Black);

    SetConsoleTextAttribute(hConsole, attributes);

    DWORD written = 0;
    WriteConsole(hConsole, text.c_str(), text.size(), &written, nullptr);

    SetConsoleTextAttribute(hConsole, convertColorToWindows(Color::White, Color::Black));
}

void Console::println(const std::string& text, Color color) 
{
    print(text + "\n", color);
}

std::string Console::readLine() 
{
    std::string input;
    int cursorPos = 0;

    while (true) 
    {
        if (_kbhit()) 
        {
            int ch = _getch();

            if (ch == '\r' || ch == '\n') 
            {  // Enter
                println("");
                if (!input.empty()) 
                {
                    m_commandHistory.push_back(input);
                    m_historyIndex = m_commandHistory.size();
                }
                return input;
            }
            else if (ch == '\b' && cursorPos > 0) 
            {  // Backspace
                input.erase(--cursorPos, 1);
                print("\b \b");
            }
            else if (ch == 224) 
            {
                int specialKey = _getch();
                if (specialKey == 72 && !m_commandHistory.empty()) 
                {  // Up
                    if (m_historyIndex > 0) 
                    {
                        --m_historyIndex;
                        while (cursorPos > 0) 
                        {
                            print("\b \b");
                            --cursorPos;
                        }
                        input = m_commandHistory[m_historyIndex];
                        print(input);
                        cursorPos = input.length();
                    }
                }
                else if (specialKey == 80 && !m_commandHistory.empty()) 
                {  // Down
                    if (m_historyIndex < m_commandHistory.size()) 
                    {
                        m_historyIndex++;
                        while (cursorPos > 0) 
                        {
                            print("\b \b");
                            --cursorPos;
                        }
                        if (m_historyIndex < m_commandHistory.size()) 
                            input = m_commandHistory[m_historyIndex];
                        else 
                            input.clear();

                        print(input);
                        cursorPos = input.length();
                    }
                }
            }
            else if (ch >= 32 && ch <= 126) 
            {  // Printable characters
                input.insert(cursorPos++, 1, static_cast<char>(ch));
                print(std::string(1, static_cast<char>(ch)));
            }
        }
    }
}