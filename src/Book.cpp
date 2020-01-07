//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "Book.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;



Book::Book(const string &name, const string &genre, bool borrowTo, bool borrowFrom) : name(name), genre(genre),
                                                                                      borrowTo(borrowTo),
                                                                                      borrowFrom(borrowFrom) {}
//=======================Getters==================================

string Book::getGenre() {
    return genre;
}

string Book::getName() {
    return name;
}

bool Book::isBorrowTo() {
    return borrowTo;
}

bool Book::isBorrowFrom() {
    return borrowFrom;
}

//=======================Setters==================================

void Book::setBorrowTo() {
    borrowTo = true;
}

void Book::setBorrowFrom() {
    borrowFrom = true;
}
