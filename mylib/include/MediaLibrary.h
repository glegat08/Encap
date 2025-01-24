#pragma once
#include <string>
#include <map>
#include <memory>
#include "Client.h"
#include "Media.h"

class MediaLibrary 
{
public:

    void addClient(const std::string& name, const std::string& firstName, const std::string& age,
        const std::string& phone, const std::string& mail);

    void removeClient(const std::string& identifier);
    void listClients();
    void addMedia(Media* media);
    void removeMedia(const std::string& type, const std::string& title);
    void listMedia(bool onlyAvailable = false) const;
    void rentMedia(const std::string& clientMail, const std::string& type, const std::string& title);
    void returnMedia(const std::string& type, const std::string& title);
    void showClientMedia(const std::string& identifier);

private:
    std::map<std::string, Client> m_clients;
    std::map<std::string, std::unique_ptr<Media>> m_medias;
    std::string generateUniqueKey(const std::string& type, const std::string& title) const;
};