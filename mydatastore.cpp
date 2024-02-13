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

void MyDataStore::purchaseUserCart(string userName) {
    if(userCarts.find(userName) == userCarts.end()) {
        cerr << "User not found" << endl;
        return;
    }
    User* user = /* retrieve the User object for userName */;
    deque<Product*>& cart = userCarts[userName];
    for(auto it = cart.begin(); it != cart.end();) {
        Product* product = *it;
        if(product->getQty() > 0 && user->getBalance() >= product->getPrice()) {
            user->deductAmount(product->getPrice());
            product->subtractQty(1);
            it = cart.erase(it);
        } else {
            ++it;
        }
    }
}