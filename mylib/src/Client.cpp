#include "Client.h"
#include <algorithm>

Client::Client(const std::string& name, const std::string& firstName, const std::string& age,
    const std::string& phoneNumber, const std::string& mail)
    : m_name(name), m_firstName(firstName), m_age(age)
    , m_phoneNumber(phoneNumber), m_mail(mail)
{
}

const std::string& Client::getName()
{ 
    return m_name; 
}

const std::string& Client::getFirstName()
{
    return m_firstName; 
}

const std::string& Client::getAge() const 
{ 
    return m_age;
}

const std::string& Client::getMail()
{
    return m_mail; 
}

const std::vector<std::string>& Client::getRentedMedia()
{ 
    return m_rentedMedia; 
}

void Client::addRental(const std::string& mediaTitle)
{
    if (m_rentedMedia.size() >= 5)
    {
        throw std::runtime_error("Maximum number of rentals reached");
    }
    m_rentedMedia.push_back(mediaTitle);
}

void Client::removeRental(const std::string& mediaTitle)
{
    auto it = std::find(m_rentedMedia.begin(), m_rentedMedia.end(), mediaTitle);
    if (it != m_rentedMedia.end())
    {
        m_rentedMedia.erase(it);
    }
}

bool Client::canRent() const
{
    return m_rentedMedia.size() < 5;
}

const std::string& Client::getPhone()
{
    return m_phoneNumber;
}
