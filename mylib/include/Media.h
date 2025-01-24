#pragma once
#include <string>

class Media 
{
public:
    Media(const std::string& title) : m_title(title) {}
    virtual ~Media() = default;

    const std::string& getTitle();
    bool isRented() const;
    const std::string& getRentedTo();
    void rent(const std::string& clientMail);
    void returnMedia();
    virtual std::string getType() const = 0;
    virtual const std::string& getAgeLimit() const = 0;

protected:
    std::string m_title;
    bool m_isRented = false;
    std::string m_rentedTo;
};

class Book : public Media 
{
public:
    Book(const std::string& title, const std::string& isbn);

    std::string getType() const override;
    const std::string& getAgeLimit() const override;
    const std::string& getIsbn();

private:
    std::string m_isbn;
};

class Movie : public Media 
{
public:
    Movie(const std::string& title, const std::string& support, const std::string& ageLimit);

    std::string getType() const override;
    const std::string& getAgeLimit() const override;
    const std::string& getSupport();

private:
    std::string m_support;
    std::string m_ageLimit;
};

class Game : public Media 
{
public:
    Game(const std::string& title, const std::string& studio, const std::string& genre, const std::string& pegi);

    std::string getType() const override;
    const std::string& getAgeLimit() const override;
    const std::string& getStudio();
    const std::string& getGenre();

private:
    std::string m_studio;
    std::string m_pegi;
    std::string m_genre;
};