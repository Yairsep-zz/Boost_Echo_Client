//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "User.h"


User::~User() {

    inventory.clear();
    borrowedFromMap.clear();
    borrowedToMap.clear();

}

User::User(const string name,const string password):userName(name),password(password) ,
        inventory(), borrowedFromMap(), borrowedToMap(),subscriptionToId(){
    receiptCounter = 0;
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

map<Book, string> User::getBorrowedFromMap() {
    return borrowedFromMap;
}

const string &User::getPreviousOwnerName(Book book) const {
    return borrowedFromMap.at(book);
}

User::User() {

}



