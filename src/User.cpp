//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <boost/algorithm/string/join.hpp>
#include "User.h"


//Commands:
//login 127.0.0.1:7777 bob alice
//login 127.0.0.1:7777 alon brand

User::~User() {

    inventory.clear();
    borrowedFromMap.clear();

}

User::User(const string name,const string password):userName(name),password(password) ,
                                                    inventory(), borrowedFromMap(),subscriptionToId(),receiptIdToPrint(){
    receiptId = 0;
    subscribeId=0;

}



//=======================Getters==================================

const string &User::getUserName() const {
    return userName;
}

const string &User::getPassword() const {
    return password;
}

const vector<Book> &User::getInventory() const {
    return inventory;
}
map<string, string> &User::getreceiptIdToPrint() {
    return receiptIdToPrint;
}


//=======================Setters==================================


void User::addToInventory(Book *book) {
    inventory.push_back(*book);
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

User::User() {

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


    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if(it->getName()==bookName)
            return true;
    }
    return false;
}

void User::removeFromInventory(string bookName) {

    for (int i = 0; i < inventory.size(); i++)
    {
        if (inventory.at(i).getName() == bookName)
            inventory.erase(inventory.begin() + i);
    }
}




string User::printInventoryByGenre(string genre) {

    string output="";
    for (Book book : inventory) {
        if (book.getGenre() == genre) {
            string s = book.getName();
            output += s + ",";
        }
    }
    return output.substr(0,output.size()-1);
}



void User::addToBorrowedMap(Book* book, string owner) {
    borrowedFromMap.insert(pair<Book*,string>(book,owner));
}

map<Book *, string> &User::getborowedFromMap() {
    return this->borrowedFromMap;
}
string User::getReceiptIdToPrint(string subID)  {
    return receiptIdToPrint.at(subID);
}

void User::addToMyWishing(string nameOfBook) {
    this->wishingBooks.push_back(nameOfBook);

}

void User::removeFromWishing(string nameOfBook) {
    for (int i = 0; i < wishingBooks.size(); i++)
    {
        if (wishingBooks.at(i) == nameOfBook)
            wishingBooks.erase(wishingBooks.begin() + i);
    }

}

bool User::isInMyWishList(string name) {
    bool output=false;
    for (int i = 0; i < wishingBooks.size(); i++)
    {
        if (wishingBooks.at(i) == name)
            output=true;
    }
    return output;
}

Book User::getBookFromInventory(string bookName) {
    for (int i = 0; i <inventory.size(); i++) {
        if (inventory.at(i).getName() == bookName)
            return inventory.at(i);
    }
}








