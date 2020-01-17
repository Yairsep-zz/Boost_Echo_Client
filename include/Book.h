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
    string borrowFrom;

public:
    Book(string &name,string &genre);
    virtual ~Book();
    //=======================Getters==================================
    string getGenre();
    string getName();
    string getPrivouesOwner();

    //=======================Setters==================================
    void setPrivouesOwner(string name);


};


#endif //BOOST_ECHO_CLIENT_BOOK_H