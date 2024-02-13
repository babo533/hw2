#ifndef CLOTHING_H
#define CLOTHING_H

#include "product.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>

class Clothing : public Product {
public:
    // Constructor
    Clothing(const std::string& category, const std::string& name, double price, int qty, const std::string& size, const std::string& brand);

    // Destructor
    virtual ~Clothing();

    // Implement Product's pure virtual functions
    virtual std::set<std::string> keywords() const override;
    virtual std::string displayString() const override;
    virtual void dump(std::ostream& os) const override;

    // getters specific to Clothing
    std::string getBrand() const;
    std::string getSize() const;
    

private:
    std::string size_;
    std::string brand_;
};

#endif // CLOTHING_H
