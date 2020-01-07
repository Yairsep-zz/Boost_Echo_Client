//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//
#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H

#include <vector>
#include <iostream>
#include <string>
using namespace std;
class Book{



private:
    string name;
    string genre;
    bool borrowTo;
    bool borrowFrom;

public:
    Book(const string &name, const string &genre, bool borrowTo, bool borrowFrom);
    Book(string name,string genre);
    //=======================Getters==================================
    string getGenre();
    string getName();
    bool isBorrowTo();
    bool isBorrowFrom();

    //=======================Setters==================================
    void setBorrowTo();
    void setBorrowFrom();

};


#endif //BOOST_ECHO_CLIENT_BOOK_H