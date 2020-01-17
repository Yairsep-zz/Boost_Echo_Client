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
    mutex mutexInventory;
    vector<Book*> inventory;
    vector<string> borrowedBooks;
    vector <string> wishingBooks;
    map<string,string> subscriptionToId;
    int subscribeId;
    map<string,string> receiptIdToPrint;
    int receiptId;


public:
//    User (string userName,string password, std:: mutex& mutex);
    User (string userName,string password);
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

    std::vector<Book*> getInventory() const;
    std::vector<string> getBorrowedBooks() const;

    const map<string, string> &getSubscriptionToId();
     map<Book*,string>& getborowedFromMap();


    //=======================Setters==================================
    void removeFromInventory(string bookName);
    void addToInventory(Book *book);
    void setreceiptIdToPrint(int receiptId,string messageToPrint);
    void increaseReceiptCounter();

    void addToMyWishing(string nameOfBook);
    void removeFromWishing(string nameOfBook);

    //Borrowed map
    void addToBorrowedBooks(string bookToAdd);
    string getPreOwnerFromBorrowedBooks(string bookName);

    //

    //getters////
    string printInventoryByGenre (string genre);
    void removeFromBorrowedBooks(string bookToRemove);



};





#endif //BOOST_ECHO_CLIENT_USER_H