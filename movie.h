#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "product.h" 

class Movie : public Product {
public:
    // Constructor 
    Movie(const std::string category, const std::string name, double price, int qty, const std::string& filmGenre, const std::string& filmRating);

    // Destructor
    virtual ~Movie();

    // Override virtual functions from the Product class
    virtual std::set<std::string> keywords() const override;
    virtual bool isMatch(std::vector<std::string>& searchTerms) const override;
    virtual std::string displayString() const override;
    virtual void dump(std::ostream& os) const override;

    // getters
    std::string getFilmGenre() const;
    std::string getFilmRating() const;

private:
    std::string filmGenre_; 
    std::string filmRating_; 
};

#endif 
