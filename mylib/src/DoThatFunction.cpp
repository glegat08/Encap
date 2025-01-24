#include "DoThatFunction.h"
#include "UseThatHelpingFunction.h"
#include "MediaLibrary.h"
#include "Console.h"
#include "Media.h"

void DoThatFunction::DoThatCommand(const std::string& command, MediaLibrary& library)
{
    Console console;

    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    try
    {
        if (cmd == "help")
            DoThatFunction::printHelp(console);
        else if (cmd == "clear")
			console.clear(command);
        else if (cmd == "addClient")
            DoThatFunction::addClient(command, library);
        else if (cmd == "removeClient")
            DoThatFunction::removeClient(command, library);
        else if (cmd == "listClient")
            DoThatFunction::listClients(library);
        else if (cmd == "addMedia")
            DoThatFunction::addMedia(command, library);
        else if (cmd == "removeMedia")
            DoThatFunction::removeMedia(command, library);
        else if (cmd == "listMedia")
            DoThatFunction::listMedia(command, library);
        else if (cmd == "rent")
            DoThatFunction::rent(command, library);
        else if (cmd == "return")
            DoThatFunction::returnMedia(command, library);
        else if (cmd == "showMedia")
            DoThatFunction::showClientMedia(command, library);
        else
            console.println("Unknown command. Type 'help' for available commands.", Color::Orange);
    }
    catch (const std::exception& e)
    {
        console.println("Error: " + std::string(e.what()), Color::Red);
    }
}



// THOSE METHOD BELOW ARE USED FOR THE DOTHATCOMMAND METHOD
// THEY ARE USED TO EXECUTE THE COMMANDS



void DoThatFunction::printHelp(Console& console)
{
    console.println("Available commands:", Color::Blue);

    // Client section
    console.println("\nClient Management:", Color::Green);
    console.println("  addClient <name> <firstname> <age> <phone> <mail>", Color::White);
    console.println("  removeClient <name/mail>", Color::White);
    console.println("  listClient", Color::White);
    console.println("  showMedia <clientIdentifier>", Color::White);

    // Media section
    console.println("\nMedia Management:", Color::Green);
    console.println("  addMedia <type> <arguments>", Color::White);
    console.println("    Book: <title> <isbn>", Color::White);
    console.println("    Movie: <title> <support> <ageLimit>", Color::White);
    console.println("    Game: <title> <studio> <genre> <pegi>", Color::White);
    console.println("  removeMedia <type> <title>", Color::White);
    console.println("  listMedia [available]", Color::White);

    // Rental section
    console.println("\nRental Management:", Color::Green);
    console.println("  rent <clientMail> <mediaType> <mediaTitle>", Color::White);
    console.println("  return <mediaType> <mediaTitle>", Color::White);

    // General section
    console.println("\nGeneral Commands:", Color::Green);
    console.println("  help - Display this help message", Color::White);
    console.println("  exit - Quit the application", Color::White);

    // More information
    console.println("\nNotes:", Color::Orange);
    console.println("- Please use quotes for names with spaces", Color::White);
    console.println("- Age must be a positive number", Color::White);
    console.println("- PEGI ratings: 3, 7, 12, 16, 18", Color::White);
    console.println("- When removing or returning media, specify both type and title", Color::White);
}

void DoThatFunction::addBook(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, type, title, isbn;

    iss >> cmd >> type;

    title = UseThat::getNameWithSpaces(iss);
    isbn = UseThat::getNameWithSpaces(iss);

    if (title.empty())
        throw std::runtime_error("Media title is required");

    if (isbn.empty())
        throw std::runtime_error("ISBN is required");

    if (!std::all_of(isbn.begin(), isbn.end(), ::isdigit))
		throw std::runtime_error("ISBN must be a number");

	if (isbn.length() != 13)
		throw std::runtime_error("ISBN must be 13 digits");

    library.addMedia(new Book(title, isbn));
}

void DoThatFunction::addMovie(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, type, title, support, ageLimit;

    iss >> cmd >> type;

    title = UseThat::getNameWithSpaces(iss);

    if (!(iss >> support))
        throw std::runtime_error("Support type is required");

    iss >> ageLimit;

	if (ageLimit.size() < 0)
		throw std::runtime_error("Age limit must be positive");
	if (ageLimit.length() != 2)
		throw std::runtime_error("Age limit must be 2 digits");
    if (!std::all_of(ageLimit.begin(), ageLimit.end(), ::isdigit))
		throw std::runtime_error("Age limit must be a number");

    if (support != "DVD" && support != "Blu-Ray")
        throw std::runtime_error("Support is invalid. Valid values are: DVD, Blu-Ray");

    if (title.empty())
        throw std::runtime_error("Media title is required");

    library.addMedia(new Movie(title, support, ageLimit));
}

void DoThatFunction::addGame(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
	std::string cmd, type, title, studio, genre, pegi;

    iss >> cmd >> type;

    title = UseThat::getNameWithSpaces(iss);
    studio = UseThat::getNameWithSpaces(iss);
    genre = UseThat::getNameWithSpaces(iss);

    iss >> pegi;

    if (title.empty())
        throw std::runtime_error("Media title is required");

    if (studio.empty() || genre.empty())
        throw std::runtime_error("Studio and genre are required");

    if (!std::all_of(pegi.begin(), pegi.end(), ::isdigit))
		throw std::runtime_error("PEGI rating must be a number");

    int pegiRating = std::stoi(pegi);
    const std::vector<int> validPegiRatings = { 3, 7, 12, 16, 18 };

    if (std::find(validPegiRatings.begin(), validPegiRatings.end(), pegiRating) == validPegiRatings.end())
        throw std::runtime_error("Invalid PEGI rating. Accepted ratings are 3, 7, 12, 16, or 18.");

    library.addMedia(new Game(title, studio, pegi, genre));
}

void DoThatFunction::exit()
{
    Console console;
    console.println("\nType 'exit' again to confirm. Any other command will cancel.", Color::Orange);
    console.print("> ", Color::White);
    std::string confirmInput = console.readLine();

    if (confirmInput == "exit")
        console.println("\nThank you for coming to the Tahitian Library. Goodbye!", Color::Green);
}

void DoThatFunction::addClient(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, name, firstname, mail, phone, age;

    iss >> cmd;

	name = UseThat::getNameWithSpaces(iss);
    firstname = UseThat::getNameWithSpaces(iss);

    iss >> age;
    iss >> phone;
    iss >> mail;

    if (name.empty() || firstname.empty() || mail.empty())
        throw std::runtime_error("All client fields are required");

    if (mail.length() <= 10 || mail.substr(mail.length() - 10) != "@gmail.com")
        throw std::runtime_error("Invalid email address. It must be <yourtext>@gmail.com");

    if (phone.size() < 0 || !std::all_of(phone.begin(), phone.end(), ::isdigit))
		throw std::runtime_error("Phone number must be positive");

	if (phone.length() != 10)
		throw std::runtime_error("Phone number must be 10 digits");

    if (age.size() <= 0 || !std::all_of(age.begin(), age.end(), ::isdigit))
        throw std::runtime_error("Age must be positive number");

    library.addClient(name, firstname, age, phone, mail);

    Console console;
    console.println("Client added successfully.", Color::Green);
}

void DoThatFunction::removeClient(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd;

    iss >> cmd;

    std::string identifier = UseThat::getNameWithSpaces(iss);

    if (identifier.empty())
        throw std::runtime_error("Client identifier is required");

    library.removeClient(identifier);

    Console console;
    console.println("Client removed successfully.", Color::Green);
}

void DoThatFunction::listClients(MediaLibrary& library)
{
    library.listClients();
}

void DoThatFunction::addMedia(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, type;

    iss >> cmd;

    if (!(iss >> type))
        throw std::runtime_error("Media type is required");

    if (type != "Book" && type != "Movie" && type != "Game")
        throw std::runtime_error("Unknown media type. Valid types are: Book, Movie, Game");

    if (type == "Book")
        addBook(command, library);
    else if (type == "Movie")
        addMovie(command, library);
    else if (type == "Game")
        addGame(command, library);

    Console console;
    console.println("Media added successfully.", Color::Green);
}

void DoThatFunction::removeMedia(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, type, title;

    iss >> cmd >> type;
    title = UseThat::getNameWithSpaces(iss);

    if (type.empty())
        throw std::runtime_error("Media type is required");

    if (title.empty())
        throw std::runtime_error("Media title is required");

    library.removeMedia(type, title);

    Console console;
    console.println("Media removed successfully.", Color::Green);
}

void DoThatFunction::listMedia(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);

    std::string state;
    iss >> state;
    library.listMedia(state == "available");
}

void DoThatFunction::rent(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, clientMail, type, title;

    iss >> cmd >> clientMail >> type;

    title = UseThat::getNameWithSpaces(iss);

    if (clientMail.empty() || type.empty() || title.empty())
        throw std::runtime_error("Client mail, media type, and title are required");

    library.rentMedia(clientMail, type, title);

    Console console;
    console.println("Media rented successfully.", Color::Green);
}

void DoThatFunction::returnMedia(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, type, title;

    iss >> cmd >> type;
    title = UseThat::getNameWithSpaces(iss);

    if (type.empty())
        throw std::runtime_error("Media type is required");

    if (title.empty())
        throw std::runtime_error("Media title is required");

    library.returnMedia(type, title);

    Console console;
    console.println("Media returned successfully.", Color::Green);
}

void DoThatFunction::showClientMedia(const std::string& command, MediaLibrary& library)
{
    std::istringstream iss(command);
    std::string cmd, identifier;

    iss >> cmd;

    identifier = UseThat::getNameWithSpaces(iss);
    if (identifier.empty())
        throw std::runtime_error("Client identifier is required");

    library.showClientMedia(identifier);
}