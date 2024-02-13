#include "datastore.h"
#include "util.h"
#include "user.h"
#include "product.h"
#include "product_parser.h"
#include "mydatastore.h"

using namespace std;

MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
    for(set<User*>::iterator it = registeredUsers.begin(); it != registeredUsers.end(); ++it) {
        delete *it;
    }
    for(set<Product*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
        delete *it;
    }
}

void MyDataStore::addProduct(Product* p) {
    inventory.insert(p);
    set<string> keywords = p->keywords();
    for (set<string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
        keywordProductMap[*it].insert(p);
    }
}

void MyDataStore::addUser(User* u) {
    registeredUsers.insert(u);
    userCarts[u->getName()];
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    set<Product*> results;
    if (type == 0) {
        // AND search
        bool first = true;
        set<Product*> tempResults;
        for(size_t i = 0; i < terms.size(); ++i) {
            if(keywordProductMap.find(terms[i]) != keywordProductMap.end()) {
                if(first) {
                    tempResults = keywordProductMap[terms[i]];
                    first = false;
                } else {
                    tempResults = setIntersection(tempResults, keywordProductMap[terms[i]]);
                }
            } else {
                tempResults.clear();
                break;
            }
        }
        results = tempResults;
    } else {
        // OR search
        for(size_t i = 0; i < terms.size(); ++i) {
            if(keywordProductMap.find(terms[i]) != keywordProductMap.end()) {
                set<Product*> temp = keywordProductMap[terms[i]];
                for(set<Product*>::iterator it = temp.begin(); it != temp.end(); ++it) {
                    results.insert(*it);
                }
            }
        }
    }
    return vector<Product*>(results.begin(), results.end());
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

void MyDataStore::addProductToCart(string userName, Product* product) {
    if(userCarts.find(userName) != userCarts.end()) {
        userCarts[userName].push_back(product);
    } else {
        cerr << "User not found" << endl;
    }
}

void MyDataStore::displayUserCart(string userName) {
    if(userCarts.find(userName) != userCarts.end()) {
        cout << "User Cart [" << userName << "]:" << endl;
        for(deque<Product*>::iterator it = userCarts[userName].begin(); it != userCarts[userName].end(); ++it) {
            cout << (*it)->displayString() << endl;
        }
    } else {
        cerr << "User not found" << endl;
    }
}

void MyDataStore::purchaseUserCart(std::string userName) {
    // Check if the user exists and retrieve the User object
    User* user = nullptr;
    for(auto it = registeredUsers.begin(); it != registeredUsers.end(); ++it) {
        if((*it)->getName() == userName) {
            user = *it;
            break;
        }
    }

    // If user not found, print an error and return
    if(user == nullptr) {
        std::cerr << "User not found" << std::endl;
        return;
    }

    // Proceed with the purchase if the user is found
    if(userCarts.find(userName) != userCarts.end()) {
        std::deque<Product*>& cart = userCarts[userName];
        for(auto it = cart.begin(); it != cart.end();) {
            if((*it)->getQty() > 0 && user->getBalance() >= (*it)->getPrice()) {
                user->deductAmount((*it)->getPrice());
                (*it)->subtractQty(1);
                it = cart.erase(it); // Successful purchase, remove item from cart
            } else {
                ++it; // Move to next item if not enough stock or funds
            }
        }
    } else {
        std::cerr << "Cart is empty or does not exist for the user" << std::endl;
    }
}
