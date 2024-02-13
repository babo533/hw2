#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "product.h" 

class Book : public Product {
public:
    // Constructor 
    Book(const std::string category, const std::string name, double price, int qty, const std::string& writer, const std::string& uniqueID);

    // Destructor
    virtual ~Book();

    // Implement Product's pure virtual functions
    virtual std::set<std::string> keywords() const override;
    virtual bool isMatch(std::vector<std::string>& searchTerms) const override;
    virtual std::string displayString() const override;
    virtual void dump(std::ostream& os) const override;

    // getters
    std::string getWriter() const;
    std::string getUniqueID() const;

private:
    std::string writer_; 
    std::string uniqueID_; 
};

#endif // BOOK_H
