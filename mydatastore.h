#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map> 
#include <deque>
#include "product.h"
#include "util.h"
#include "user.h"

class MyDataStore : public DataStore {
public:
    MyDataStore();

    ~MyDataStore();

    void addProduct(Product* p);

    void addUser(User* u);

    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    void dump(std::ostream& ofile);

    void addProductToCart(std::string userName, Product* product);

    void displayUserCart(std::string userName);

    void purchaseUserCart(std::string userName);

private:
    std::set<User*> registeredUsers;
    std::set<Product*> inventory;
    std::map<std::string, std::set<Product*>> keywordProductMap; 
    std::unordered_map<std::string, std::deque<Product*>> userCarts; 
};

#endif

