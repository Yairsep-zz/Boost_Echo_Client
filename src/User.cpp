//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "User.h"


User::~User() {

    inventory.clear();
    borrowedFromMap.clear();
    borrowedToMap.clear();
    genreMap.clear();
}

User::User(const std::string &userName) : userName(userName) , password(password), inventory(std::vector<Book>()),
    borrowedFromMap(map<Book,string>()) , borrowedToMap(map<Book,string>()), genreMap(map<string,int>()){
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


void User::setUserName(const string &userName) {

}

void User::setPassword(const string &password) {

}

void User::addToInventory(Book *book) {
    inventory.push_back(*book);
}

void User::addToBorrowedFrom(Book *book) {
    borrowedFromMap.insert(book,)
}

void User::addToBorrowedTo(Book *book) {
    borrowedToMap.insert(book,)
}

void User::addToSetSubscribeToGenre(string genre) {
    genreMap.insert(genre, )
}

