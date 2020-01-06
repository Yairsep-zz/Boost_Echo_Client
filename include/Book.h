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

private :
    string name;
    string genre;
public:
    Book(string name,string genre);
    string getGenre();
    string getName();

};


#endif //BOOST_ECHO_CLIENT_BOOK_H