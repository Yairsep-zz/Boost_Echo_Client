//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <boost/algorithm/string/join.hpp>
#include "User.h"



User::~User() {
    receiptIdToPrint.clear();
    subscriptionToId.clear();
    wishingBooks.clear();
    borrowedBooks.clear();
    inventory.clear();


}

User::User(const string name,const string password):userName(name),password(password),mutexInventory(),
    inventory(), borrowedBooks(),wishingBooks(), subscriptionToId(),subscribeId(0),
    receiptIdToPrint(),
    receiptId(0){
}

//User::User() :userName(""),password(""),mutexInventory(mutexInventory),
//              inventory(), borrowedBooks(),wishingBooks(), subscriptionToId(),subscribeId(0),
//              receiptIdToPrint(),
//              receiptId(0) {}


//=======================Getters==================================

const string &User::getUserName() const {
    return userName;
}

const string &User::getPassword() const {
    return password;
}

map<string, string> &User::getreceiptIdToPrint() {
    return receiptIdToPrint;
}


//=======================Setters==================================


void User::addToInventory(Book *book) {
    this->mutexInventory.lock();
    inventory.push_back(book);
    this->mutexInventory.unlock();
}


const map<string, string> &User::getSubscriptionToId() {
    return subscriptionToId;
}

int User::getSubscribeId() {
    return subscribeId++;
}

void User::setSubscribeId(int subscribeId) {

    User::subscribeId = subscribeId;
}



void User::setreceiptIdToPrint(int receiptId,string message) {
    receiptIdToPrint.insert(pair<string, string>(to_string(receiptId), message));
}



void User::increaseReceiptCounter() {
    this->receiptId=receiptId+1;
}

int User::getReceiptCounter() {
    return this->receiptId;
}

bool User::iHaveTheBook(string bookName) {

    for (Book *book : inventory) {
        if (book->getName() == bookName) {
            return true;
        }
    }
    return false;
}

void User::removeFromInventory(string bookName) {
    mutexInventory.lock();
    for (unsigned i = 0; i < inventory.size(); i++) {
        if (inventory.at(i)->getName() == bookName) {
            delete inventory.at(i);
            inventory.erase(inventory.begin() + i);
        }
    }

    mutexInventory.unlock();
}




string User::printInventoryByGenre(string genre) {

    string output="";
    for (Book* book : inventory) {
        if (book->getGenre() == genre) {
            string s = book->getName();
            output += s + ",";
        }
    }
    return output.substr(0,output.size()-1);
}



string User::getReceiptIdToPrint(string subID)  {
    return receiptIdToPrint.at(subID);
}

void User::addToMyWishing(string nameOfBook) {
    this->wishingBooks.push_back(nameOfBook);

}

void User::removeFromWishing(string nameOfBook) {
    for (unsigned i = 0; i < wishingBooks.size(); i++)
    {
        if (wishingBooks.at(i) == nameOfBook)
            wishingBooks.erase(wishingBooks.begin() + i);
    }

}

bool User::isInMyWishList(string name) {

    if (std::find(wishingBooks.begin(), wishingBooks.end(), name) != wishingBooks.end())
    {
        return true;
    }
    return false;
}


void User::addToBorrowedBooks(string bookToAdd) {
    this->borrowedBooks.push_back(bookToAdd);
}


string User::getPreOwnerFromBorrowedBooks(string bookName) {

    string output;
    for (unsigned i = 0; i <inventory.size(); i++) {
        if (inventory.at(i)->getName() == bookName)
            output= inventory.at(i)->getPrivouesOwner();
    }
    return output;
}

void User::removeFromBorrowedBooks(string bookToRemove) {
    for (unsigned i = 0; i < borrowedBooks.size(); i++)
    {
        if (borrowedBooks.at(i) == bookToRemove)
            borrowedBooks.erase(borrowedBooks.begin() + i);
    }
}

std::vector<Book *> User::getInventory() const {
    return inventory;
}
std::vector<string> User::getBorrowedBooks() const {
    return borrowedBooks;
}








