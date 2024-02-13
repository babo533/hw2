#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <set>
#include "movie.h"
#include "util.h"
#include "product.h"


using namespace std;

// Constructor
Movie::Movie(const std::string category, const std::string name, double price, int qty, const std::string& filmGenre, const std::string& filmRating)
    : Product(category, name, price, qty) {
    this->filmGenre_ = filmGenre;
    this->filmRating_ = filmRating;
}

// Destructor
Movie::~Movie() {
    
}

std::set<std::string> Movie::keywords() const {
    set<string> ans = parseStringToWords(this->name_); // Include movie name in keywords
    set<string> genreKeywords = parseStringToWords(this->filmGenre_); // Include genre in keywords
    ans = setUnion(ans, genreKeywords); // Combine the two sets
    return ans;
}

bool Movie::isMatch(std::vector<std::string>& searchTerms) const {
    set<string> keys = this->keywords();
    for (const string& term : searchTerms) {
        if (keys.find(term) != keys.end()) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

std::string Movie::displayString() const {
    std::stringstream ss;
    ss << name_ << "\n" << filmGenre_ << " " << filmRating_;
    ss << "\n" << std::fixed << std::setprecision(2) << price_ << " - " << qty_ << " left." << std::endl;
    return ss.str();
}





void Movie::dump(std::ostream& os) const {
    Product::dump(os); // Call base class method to output common attributes
    os << this->filmGenre_ << "\n" << this->filmRating_ << endl;
}

//getters
std::string Movie::getFilmGenre() const {
    return this->filmGenre_;
}

std::string Movie::getFilmRating() const {
    return this->filmRating_;
}