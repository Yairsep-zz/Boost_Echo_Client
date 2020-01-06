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
    string username;
    string password;
    vector<Book> inventory;
    map<Book,string> borrowedFrom;
    map<Book,string> borrowedTo;
    map<string,int> subscribeToGenre;


public:
    //TODO CHECK IF THE CONSTRUCTOR SHOULD BE EMPTY OR NOT!
    User(string username, vector<Book> inventory ,map<Book,string> borrowedFrom,map<Book,string> borrowedTo , map<string,int> subscribeToGenre  );
    ~User();
    const string &getUserName();
    map<Book,string> &getBorrowedFrom();
    map<Book,string> &getBorrowedTo();
    map<string,int> &getSubscribeToGenre();
    vector<Book> &getInventory();
};





#endif //BOOST_ECHO_CLIENT_USER_H