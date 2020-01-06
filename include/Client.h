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

    Frame* createJoinFrame();
    Frame* createAddFrame();
    Frame* createBorrowFrame();
    Frame* createReturnFrame();
    Frame* createStatusFrame();
    Frame* createLogoutFrame();
    Frame* createLoginFrame();

public:

};



#endif //BOOST_ECHO_CLIENT_CLIENT_H

