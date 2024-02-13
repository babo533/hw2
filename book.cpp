#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <set>
#include "book.h"
#include "util.h"
#include "product.h"

using namespace std;

  Book::Book(const std::string category, const std::string name, double price, int qty, const std::string& writer, const std::string& uniqueID) : Product(category, name, price, qty) {
    this->writer_ = writer;
    this->uniqueID_ = uniqueID;
  }

      // Destructor
  Book::~Book() {

  }

  // getters
  std::string Book::getWriter() const {
    return writer_;
  }

  std::string Book::getUniqueID() const {
    return uniqueID_;
  }

  std::set<std::string> Book::keywords() const {
    set<string> ans = parseStringToWords(name_); 
    ans.insert(uniqueID_); // Add the uniqueID (ISBN) directly since it's a valid keyword on its own
    set<string> authorKeywords = parseStringToWords(writer_); 
    ans = setUnion(ans, authorKeywords); // Combine the two sets of keywords
    return ans;
  }

  bool Book::isMatch(std::vector<std::string>& searchTerms) const {
    set<string> keys = keywords();
    for (const string& term : searchTerms) { 
      if (keys.find(term) != keys.end()) {
        return true; // If any term is found within keywords, return true
      }
    }
    return false; 
  }

  std::string Book::displayString() const {
    stringstream ans;
    ans << name_ << "\nAuthor: " << writer_ << " ISBN: " << uniqueID_;
    ans << "\n" << fixed << setprecision(2) << price_ << " - " << qty_ << " left." << endl; 
    return ans.str();
  }

  void Book::dump(std::ostream& os) const {
    Product::dump(os); 
    os << uniqueID_ << "\n" << writer_ << endl; // output book-specific details
  }
