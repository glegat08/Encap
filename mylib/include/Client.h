#pragma once
#include <string>
#include <vector>
#include <stdexcept>

class Client 
{
public:
    Client(const std::string& name, const std::string& firstName, const std::string& age,
        const std::string& phoneNumber, const std::string& mail);

    const std::string& getName();
    const std::string& getFirstName();
    const std::string& getAge() const;
    const std::string& getMail();
    const std::vector<std::string>& getRentedMedia();
    void addRental(const std::string& mediaTitle);
    void removeRental(const std::string& mediaTitle);
    bool canRent() const;
    const std::string& getPhone();

private:
    std::string m_name;
    std::string m_firstName;
    std::string m_age;
    std::string m_phoneNumber;
    std::string m_mail;
    std::vector<std::string> m_rentedMedia;
};