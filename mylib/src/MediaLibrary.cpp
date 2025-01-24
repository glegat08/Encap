#include "MediaLibrary.h"
#include "Console.h"
#include <string>

void MediaLibrary::addClient(const std::string& name, const std::string& firstName, const std::string& age, 
    const std::string& phone, const std::string& mail) 
{
    if (m_clients.find(mail) != m_clients.end())
        throw std::runtime_error("Client with this mail already exists");

    m_clients.emplace(mail, Client(name, firstName, age, phone, mail));
}

void MediaLibrary::removeClient(const std::string& identifier)
{
    Console console;
    std::vector<Client*> matchingClients;

    // Find clients matching the identifier
    for (auto& [email, client] : m_clients) 
    {
        if (client.getName() == identifier ||
            (client.getName() + " " + client.getFirstName()) == identifier) 
        {
            matchingClients.push_back(&client);
        }
    }

    // Handle multiple matching clients
    if (matchingClients.size() > 1) 
    {
        for (size_t i = 0; i < matchingClients.size(); ++i) 
        {
            console.println(std::to_string(i + 1) + ". " + matchingClients[i]->getName() 
                + " " + matchingClients[i]->getFirstName() 
                + " (Email: " + matchingClients[i]->getMail() + ")", Color::White);
        }
        throw std::runtime_error("Multiple clients found with this name. Use email for precise identification.");
    }

    // If exactly one client or no clients found
    if (matchingClients.empty()) 
    {
        auto iterator = m_clients.find(identifier);
        if (iterator == m_clients.end())
            throw std::runtime_error("Client not found");

        matchingClients.push_back(&(iterator->second));
    }

    // Check for rented media
    Client* clientToRemove = matchingClients[0];
    const auto& rentedMedia = clientToRemove->getRentedMedia();
    if (!rentedMedia.empty()) 
    {
        std::string errorMessage = "Cannot remove client. Please return the following media first:\n";
        for (const auto& media : rentedMedia) 
        {
            errorMessage += "- " + media + "\n";
        }
        throw std::runtime_error(errorMessage);
    }

    m_clients.erase(clientToRemove->getMail());
}

void MediaLibrary::listClients()
{
    Console console;

    for (auto& [mail, client] : m_clients) 
    {
        console.println("\nName: " + client.getName()
            + "\nFirstname: " + client.getFirstName()
            + "\nAge: " + client.getAge()
            + "\nPhone: " + client.getPhone()
            + "\nEmail: " + client.getMail()
            + "\nRentals: " + std::to_string(client.getRentedMedia().size()) + "/5\n");
    }
}

void MediaLibrary::addMedia(Media* media) 
{
    std::string uniqueKey = generateUniqueKey(media->getType(), media->getTitle());

    if (m_medias.find(uniqueKey) != m_medias.end())
        throw std::runtime_error("Media already exists");

    m_medias[uniqueKey] = std::unique_ptr<Media>(media);
}

void MediaLibrary::removeMedia(const std::string& type, const std::string& title)
{
    std::string uniqueKey = generateUniqueKey(type, title);
    auto iterator = m_medias.find(uniqueKey);

    if (iterator == m_medias.end())
        throw std::runtime_error("Media not found");

    if (iterator->second->isRented())
        throw std::runtime_error("Cannot remove rented media");

    m_medias.erase(iterator);
}

void MediaLibrary::listMedia(bool onlyAvailable) const
{
    Console console;

    for (const auto& [uniqueKey, media] : m_medias)
    {
        size_t Position = uniqueKey.find(':');
        std::string type = uniqueKey.substr(0, Position);
        std::string title = uniqueKey.substr(Position + 1);

        if (!onlyAvailable || !media->isRented())
        {
            console.println("\n" + type + ": " + title, Color::White);

            if (Book* book = dynamic_cast<Book*>(media.get()))
            {
                console.println("ISBN: " + book->getIsbn(), Color::White);
            }
            else if (Movie* movie = dynamic_cast<Movie*>(media.get()))
            {
                console.println("Support: " + movie->getSupport(), Color::White);
                console.println("Age limit: " + movie->getAgeLimit(), Color::White);
            }
            else if (Game* game = dynamic_cast<Game*>(media.get()))
            {
                console.println("Studio: " + game->getStudio(), Color::White);
                console.println("Genre: " + game->getGenre(), Color::White);
                console.println("PEGI: " + game->getAgeLimit(), Color::White);
            }

            console.println("Status: " + std::string(media->isRented() ? "Rented" : "Available"), Color::White);
        }
    }
}

void MediaLibrary::rentMedia(const std::string& identifier, const std::string& type, const std::string& title)
{
    auto clientIterator = m_clients.find(identifier);
    if (clientIterator == m_clients.end())
        throw std::runtime_error("Client not found");

    std::string uniqueKey = generateUniqueKey(type, title);

    auto mediaIterator = m_medias.find(uniqueKey);
    if (mediaIterator == m_medias.end())
        throw std::runtime_error("Specified media not found");

    Media* selectedMedia = mediaIterator->second.get();

    // Check if media is already rented
    if (selectedMedia->isRented())
        throw std::runtime_error("Specified media is already rented");

    // Check client rental limits
    if (!clientIterator->second.canRent())
        throw std::runtime_error("Client has reached maximum number of rentals");

    // Check age restrictions
    if (type != "Book")
    {
        try
        {
            int clientAge = std::stoi(clientIterator->second.getAge());
            int mediaAgeLimit = std::stoi(selectedMedia->getAgeLimit());
            if (clientAge < mediaAgeLimit)
                throw std::runtime_error("Client is too young for this media");
        }
        catch (const std::invalid_argument& e)
        {
            throw std::runtime_error("Invalid age data");
        }
        catch (const std::out_of_range& e)
        {
            throw std::runtime_error("Age value out of range");
        }
    }

    selectedMedia->rent(identifier);

    clientIterator->second.addRental(title + " (" + type + ")");
}

void MediaLibrary::returnMedia(const std::string& type, const std::string& title)
{
    std::string uniqueKey = generateUniqueKey(type, title);

    auto mediaIterator = m_medias.find(uniqueKey);
    if (mediaIterator == m_medias.end())
        throw std::runtime_error("Specified media not found");

    Media* rentedMedia = mediaIterator->second.get();

    // Check if media is actually rented
    if (!rentedMedia->isRented())
        throw std::runtime_error("Specified media is not currently rented");

    // Find the client who rented the media
    auto clientIterator = m_clients.find(rentedMedia->getRentedTo());
    if (clientIterator != m_clients.end())
    {
        // Remove the rental from the client's list
        std::string fullRentalTitle = title + " (" + type + ")";
        clientIterator->second.removeRental(fullRentalTitle);
    }

    rentedMedia->returnMedia();
}

void MediaLibrary::showClientMedia(const std::string& identifier) 
{
    Console console;

    auto iterator = m_clients.find(identifier);
    if (iterator == m_clients.end())
    {
        for (auto& [email, client] : m_clients)
        {
            if (client.getName() == identifier ||
                (client.getName() + " " + client.getFirstName()) == identifier ||
                client.getFirstName() == identifier || client.getMail() == identifier)
            {
                iterator = m_clients.find(email);
                break;
            }
        }
        if (iterator == m_clients.end())
            throw std::runtime_error("Client not found");
    }

    console.println("\nMedia rented by " + iterator->second.getName() 
        + " " + iterator->second.getFirstName() + ":");

    const auto& rentedMedia = iterator->second.getRentedMedia();
    if (rentedMedia.empty())
    {
        console.println("No media rented", Color::White);
        return;
    }

    for (const auto& mediaTitle : iterator->second.getRentedMedia())
    {
        console.println("- " + mediaTitle, Color::White);
    }
}

std::string MediaLibrary::generateUniqueKey(const std::string& type, const std::string& title) const
{
    return type + ":" + title;
}