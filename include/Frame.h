//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//
#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "Book.h"
#include "connectionHandler.h"

using namespace std;

class Frame {


private:

    string FrameName;
    string header;
    string body;

public:

    std:: string toString() const;
    Frame();
};


#endif //BOOST_ECHO_CLIENT_FRAME_H