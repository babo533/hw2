#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <set>
#include "clothing.h"
#include "util.h"
#include "product.h"

using namespace std;

// Constructor implementation
Clothing::Clothing(const std::string& category, const std::string& name, double price, int qty, const std::string& size_, const std::string& brand_)
: Product(category, name, price, qty) {
    this->size_ = size_;
    this->brand_ = brand_;
}

// Destructor
Clothing::~Clothing() {

}

// getters
std::string Clothing::getBrand() const {
    return brand_;
}


std::string Clothing::getSize() const {
    return size_;
}


std::set<std::string> Clothing::keywords() const {
  set<string> clotheName = parseStringToWords(name_);
  set<string> brandName = parseStringToWords(brand_);
  clotheName = setUnion(clotheName, brandName);
  // Not adding size as a keyword unless you decide otherwise
  return clotheName;
}

// Implementation of displayString() from Product interface
std::string Clothing::displayString() const {
    std::stringstream ss;
    ss << name_ << "\n" << size_ << " " << brand_;
    ss << "\n" << std::fixed << std::setprecision(2) << price_ << " - " << qty_ << " left." << std::endl;
    return ss.str();
}



// Implementation of dump() from Product interface
void Clothing::dump(std::ostream& os) const {
    Product::dump(os); // Dump common product info
    os << size_ << "\n" << brand_ << endl;
}
