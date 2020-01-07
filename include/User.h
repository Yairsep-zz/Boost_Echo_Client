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

private:
    string userName;
    string password;
    vector<Book> inventory;
    map<Book,string> borrowedFromMap;
    map<Book,string> borrowedToMap;
    map<string,string> subscriptionToId;
    int subscribeId;
    int connectionId;

public:

    User (string userName,string password);
    User ();
    ~User();
    void setSubscribeId(int subscribeId);
    int getSubscribeId();
    int receiptCounter;


    //=======================Getters==================================

    const string &getUserName() const;

    const string &getPassword() const;

    const vector<Book> &getInventory() const;

    const map<string, string> &getSubscriptionToId();

    map<Book, string> getBorrowedFromMap();

    const string &getPreviousOwnerName(Book) const;

    //=======================Setters==================================

    void addToInventory(Book *book);


};





#endif //BOOST_ECHO_CLIENT_USER_H