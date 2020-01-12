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
#
using namespace std;
class User {

private:
    string userName;
    string password;
    vector<Book> inventory;
    map<Book*,string> borrowedFromMap;
    vector <string> wishingBooks;
    map<string,string> subscriptionToId;
    int subscribeId;
    map<string,string> receiptIdToPrint;
    int receiptId;


public:

    User (string userName,string password);
    User ();
    ~User();
    void setSubscribeId(int subscribeId);
    int getSubscribeId();
    bool iHaveTheBook(string bookName);

    string getReceiptIdToPrint(string subID);


    //=======================Getters==================================
    map<string,string> &getreceiptIdToPrint();
    const string &getUserName() const;
    int getReceiptCounter();
    bool isInMyWishList(string name);

    const string &getPassword() const;

    const vector<Book> &getInventory() const;

    const map<string, string> &getSubscriptionToId();
     map<Book*,string>& getborowedFromMap();


    //=======================Setters==================================
    void removeFromInventory(string bookName);
    void addToInventory(Book *book);
    void setreceiptIdToPrint(int receiptId,string messageToPrint);
    void increaseReceiptCounter();
    void addToBorrowedMap(Book* book,string owner);
    void addToMyWishing(string nameOfBook);
    void removeFromWishing(string nameOfBook);
    ////
    //geters////
    string printInventoryByGenre (string genre);
    Book getBookFromInventory(string bookName);



};





#endif //BOOST_ECHO_CLIENT_USER_H