#include "datastore.h"
#include "util.h"
#include "user.h"
#include "product.h"
#include "product_parser.h"
#include "mydatastore.h"

using namespace std;

MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
    keywordProductMap.clear();
    userCarts.clear();
    for(set<User*>::iterator it = registeredUsers.begin(); it != registeredUsers.end(); ++it) {
        delete *it;
    }
    for(set<Product*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
        delete *it;
    }
    registeredUsers.clear();
    inventory.clear();
}

void MyDataStore::addProduct(Product* p) {
    inventory.insert(p); 
    set<string> keywords = p->keywords(); // Retrieve keywords for the product.
    for (set<string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
        // Check if the keyword already exists in the map.
        if (keywordProductMap.find(*it) == keywordProductMap.end()) {
            set<Product*> addSet; // If not, create a new set and add the product to it.
            addSet.insert(p);
            keywordProductMap.insert(std::make_pair(*it, addSet)); // Insert the new keyword and set into the map.
        } else {
            keywordProductMap[*it].insert(p); // If it exists, add the product to the existing set.
        }
    }
}


void MyDataStore::addUser(User* u) {
    registeredUsers.insert(u);
    userCarts[convToLower(u->getName())] = std::deque<Product*>();
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    std::vector<Product*> ans;
    set<Product*> temp;
    // AND search
    if (type == 0) {
        for (size_t i = 0; i < terms.size(); ++i) {
            if (keywordProductMap.find(terms[i]) != keywordProductMap.end()) {
                if (temp.empty()) { // Use empty() to check for the first term's products
                    temp = keywordProductMap[terms[i]];
                } else {
                    temp = setIntersection(temp, keywordProductMap[terms[i]]);
                }
            } else {
                temp.clear();
                break; // Exit early if any term is not found
            }
        }
    }
    // OR search
    else if (type == 1) {
        for (size_t i = 0; i < terms.size(); ++i) {
            if (keywordProductMap.find(terms[i]) != keywordProductMap.end()) {
                // Assuming setUnion is similar to setIntersection but performs union operation
                temp = setUnion(temp, keywordProductMap[terms[i]]);
            }
        }
    }

    // Convert set<Product*> to vector<Product*>
    for (set<Product*>::iterator it = temp.begin(); it != temp.end(); ++it) {
        ans.push_back(*it);
    }

    return ans;
}


void MyDataStore::dump(ostream& ofile) {
    ofile << "<products>" << endl;
    for(set<Product*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
        (*it)->dump(ofile);
    }
    ofile << "</products>" << endl;
    ofile << "<users>" << endl;
    for(set<User*>::iterator it = registeredUsers.begin(); it != registeredUsers.end(); ++it) {
        (*it)->dump(ofile);
    }
    ofile << "</users>" << endl;
}

void MyDataStore::addProductToCart(string username, Product* product) {
    string user = convToLower(username);
    if(userCarts.find(user) != userCarts.end()) {
        userCarts[user].push_back(product);
    } else {
        cerr << "User not found" << endl;
    }
}

void MyDataStore::displayUserCart(string userName) {
    string user = convToLower(userName);
    if(userCarts.find(user) != userCarts.end()) {
        cout << "User Cart [" << user << "]:" << endl;
        for(deque<Product*>::iterator it = userCarts[user].begin(); it != userCarts[user].end(); ++it) {
            cout << (*it)->displayString() << endl;
        }
    } else {
        cerr << "User not found" << endl;
    }
}

void MyDataStore::purchaseUserCart(std::string userName) {
    std::string lowercaseName = convToLower(userName);
    if (userCarts.find(lowercaseName) == userCarts.end()) {
        std::cerr << "User not found or cart does not exist" << std::endl;
        return;
    }
    
    User* user = nullptr;
    for (std::set<User*>::iterator itr = registeredUsers.begin(); itr != registeredUsers.end(); ++itr) {
        if (convToLower((*itr)->getName()) == lowercaseName) {
            user = *itr;
            break;
        }
    }
    
    if (user == nullptr) {
        std::cerr << "User not found" << std::endl;
        return;
    }
    
    std::deque<Product*>& cart = userCarts[lowercaseName];
    while (!cart.empty() && user != nullptr) {
        Product* currProd = cart.front();
        if (currProd->getQty() > 0 && user->getBalance() >= currProd->getPrice()) {
            currProd->subtractQty(1);
            user->deductAmount(currProd->getPrice());
            cart.pop_front(); // Proceed with purchase
        } else {
            std::cerr << "Insufficient stock or funds for " << currProd->getName() << std::endl;
            break; // Stop if unable to purchase current item
        }
    }
}