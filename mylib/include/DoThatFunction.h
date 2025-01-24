#pragma once
#include <string>

class MediaLibrary;
class Console;

namespace DoThatFunction
{
	// DOTHATCOMMAND IS THE MAIN METHOD WHICH EXECUTE THE COMMANDS
	void DoThatCommand(const std::string& command, MediaLibrary& library);

	// ALL THE AVAILABLE COMMANDS
	void printHelp(Console& console);
	void addClient(const std::string& command, MediaLibrary& library);
	void removeClient(const std::string& command, MediaLibrary& library);
	void listClients(MediaLibrary& library);
	void addMedia(const std::string& command, MediaLibrary& library);
	void removeMedia(const std::string& command, MediaLibrary& library);
	void listMedia(const std::string& command, MediaLibrary& library);
	void rent(const std::string& command, MediaLibrary& library);
	void returnMedia(const std::string & command, MediaLibrary& library);
	void showClientMedia(const std::string& command, MediaLibrary& library);
	void addBook(const std::string& command, MediaLibrary& library);
	void addMovie(const std::string& command, MediaLibrary& library);
	void addGame(const std::string& command, MediaLibrary& library);
	void exit();
}