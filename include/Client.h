//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_CLIENT_H
#define BOOST_ECHO_CLIENT_CLIENT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "Book.h"
#include "connectionHandler.h"
#include "User.h"


using namespace std;

class Client {

private:
    ConnectionHandler &connectionHandler;
    User* activeUser;
public:

};



#endif //BOOST_ECHO_CLIENT_CLIENT_H

