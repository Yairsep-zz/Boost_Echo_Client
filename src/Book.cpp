//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include "Book.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;
Book:: Book(string name,string genre):name(name),genre(genre){}

string Book::getGenre() {
    return this->genre;
}

string Book::getName() {
    return this->name;
};
