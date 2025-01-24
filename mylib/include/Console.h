#pragma once
#include <string>
#include <vector>
#include <Windows.h>

enum class Color 
{
    Black,
    Red,
    Green,
    Blue,
    White,
    Orange,
};

class MediaLibrary;

class Console 
{
public:
    Console() = default;
    void run(MediaLibrary& library);
    void print(const std::string& text, Color color = Color::White);
    void println(const std::string& text, Color color = Color::White);
    std::string readLine();
    void clear(const std::string& lastCommand);
    WORD convertColorToWindows(Color foreground, Color background);

private:
    void initialize();
    void processCommand(const std::string& command, MediaLibrary& library);
	std::vector<std::string> m_commandHistory;
	size_t m_historyIndex = 0;
};