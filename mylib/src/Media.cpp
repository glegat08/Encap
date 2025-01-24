#include "Media.h"
#include <stdexcept>

// MEDIA
const std::string& Media::getTitle()
{ 
    return m_title; 
}

bool Media::isRented() const 
{ 
    return m_isRented;
}

const std::string& Media::getRentedTo()
{
    return m_rentedTo;
}

void Media::rent(const std::string& clientMail)
{
    if (m_isRented)
        throw std::runtime_error("Media already rented");

    m_isRented = true;
    m_rentedTo = clientMail;
}

void Media::returnMedia()
{
    m_isRented = false;
    m_rentedTo.clear();
}

// BOOK
Book::Book(const std::string& title, const std::string& isbn)
    : Media(title), m_isbn(isbn)
{
}

std::string Book::getType() const
{
    return "Book";
}

const std::string& Book::getAgeLimit() const 
{ 
    return "0";
}

const std::string& Book::getIsbn()
{
    return m_isbn; 
}

// MOVIE
Movie::Movie(const std::string& title, const std::string& support, const std::string& ageLimit)
    : Media(title), m_support(support), m_ageLimit(ageLimit)
{
}

std::string Movie::getType() const 
{ 
    return "Movie";
}

const std::string& Movie::getAgeLimit() const 
{ 
    return m_ageLimit; 
}

const std::string& Movie::getSupport()
{ 
    return m_support; 
}

// GAME
Game::Game(const std::string& title, const std::string& studio, const std::string& pegi, const std::string& genre)
    : Media(title), m_studio(studio), m_genre(genre), m_pegi(pegi)
{
}

std::string Game::getType() const 
{ 
    return "Game";
}

const std::string& Game::getAgeLimit() const 
{ 
    return m_pegi;
}

const std::string& Game::getStudio()
{ 
    return m_studio; 
}

const std::string& Game::getGenre()
{ 
    return m_genre; 
}