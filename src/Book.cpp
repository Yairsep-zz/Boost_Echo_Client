//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "Book.h"
#include <string>
using namespace std;



Book::Book( string &name,  string &genre) : name(name), genre(genre),borrowFrom() {}
//=======================Getters==================================

string Book::getGenre() {
    return genre;
}

string Book::getName() {
    return name;
}


//=======================Setters==================================



void Book::setPrivouesOwner(string name) {
this->borrowFrom=name;
}

string Book::getPrivouesOwner() {
    return this->borrowFrom;
}

Book::~Book() {

}



