//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "Book.h"
#include "connectionHandler.h"
using namespace std;
class User {

    User (const std:: string& userName);
    ~User();
private:
    string userName;
    string password;
    vector<Book> inventory;
    map<Book,string> borrowedFromMap;
    map<Book,string> borrowedToMap;
    map<string,int> genreMap;


public:

    //=======================Getters==================================

    const string &getUserName() const;

    const string &getPassword() const;

    const vector<Book> &getInventory() const;



    //=======================Setters==================================
    void setUserName(const string &userName);

    void setPassword(const string &password);

    void addToInventory(Book *book);

    void addToBorrowedFrom(Book* book);

    void addToBorrowedTo(Book* book);

    void addToSetSubscribeToGenre(string genre);

};





#endif //BOOST_ECHO_CLIENT_USER_H