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
#include "Frame.h"


using namespace std;
class Client {

private:


public:


    Frame *createLoginFrame(string header, string version, string userName, string password, string destination);

    Frame *createJoinFrame(string header, string destination, string id, string receipt);

    Frame *createAddFrame(string header, string destination, string body);

    Frame *createBorrowFrame(string header, string destination, string body);

    Frame *createReturnFrame(string header, string destination, string body);

    Frame *createStatusFrame(string header, string destination, string body);

    Frame *createLogoutFrame(string header, string receipt);
};



#endif //BOOST_ECHO_CLIENT_CLIENT_H

