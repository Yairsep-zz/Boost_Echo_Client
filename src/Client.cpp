//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <thread>
#include "Client.h"
#include "../include/connectionHandler.h"
#include <boost/algorithm/string.hpp>
#include <include/Frame.h>

using boost::asio::ip::tcp;

//--------------------------------------Write Class--------------------------------

class TaskWrite{
private:
    ConnectionHandler &connectionHandler;
public:

    TaskWrite(ConnectionHandler &connectionHandler):connectionHandler(connectionHandler);

    void operator()(){
        while (!connectionHandler.isLoggedOut()){
            //Reading line from the command:
            string line;
            getline(cin,line);
            vector<string> lineVector;
            boost::split(lineVector, line, boost::is_any_of("\n"));
            Frame frame =new Frame();

            if(lineVector[0]=="login"){

                string version = lineVector[0];
                string userName = lineVector[1];
                string password = lineVector[2];
                frame.createLoginFrame("CONNECT", version , userName , password);
                connectionHandler.sendLine(frame.toString());

            }

            if(lineVector[0]=="join"){

                string destination = lineVector[1];
                frame.createJoinFrame("SUBSCRIBE", destination);
                connectionHandler.sendLine(frame.toString());
            }
            if(lineVector[0]=="add"){

                string destination = lineVector[1];
                string book = lineVector[2];
                frame.createAddFrame("SEND", destination , book);
                connectionHandler.sendLine(frame.toString());
            }

            if(lineVector[0]=="borrow"){

                string destination = lineVector[1];
                string book = lineVector[2];
                frame.createBorrowFrame("SEND", destination , book);
                connectionHandler.sendLine(frame.toString());
            }

            if(lineVector[0]=="return"){

                string destination = lineVector[1];
                string book = lineVector[2];
                frame.createReturnFrame("SEND", destination , book);
                connectionHandler.sendLine(frame.toString());
            }
            if(lineVector[0]=="status"){

                string destination = lineVector[1];
                frame.createStatusFrame("SEND", destination);
                connectionHandler.sendLine(frame.toString());
            }

            if(lineVector[0]=="logout"){

                string receipt;
                frame.createLogoutFrame("DISCONNECT" , receipt);
                connectionHandler.sendLine(frame.toString());
            }

        }
    }


};



//--------------------------------------Read Class--------------------------------
class TaskRead {
private:
    ConnectionHandler &connectionHandler;
public:

    TaskRead(ConnectionHandler &connectionHandler) : connectionHandler(connectionHandler) {}

    void operator()() {
        string line;
        getline(cin,line);
        vector<string> lineVector;
        boost::split(lineVector, line, boost::is_any_of("\n"));

        if (lineVector[0] == "CONNECTED") {

        }
        if (lineVector[0] == "RECEIPT") {

        }

        if (lineVector[0] == "MESSAGE") {

        }

        if (lineVector[0] == "ERROR") {

        }
    }

};


//--------------------------------------Main Class--------------------------------


int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    TaskWrite taskWrite(ref(connectionHandler));
    TaskRead taskRead(ref(connectionHandler));

    std::thread thWrite(std::ref(taskWrite)); // we use std::ref to avoid creating a copy of the Task object
    std::thread thRead(std::ref(taskRead));

    thWrite.join();
    thRead.join();

    return 0;
};

//--------------------------------------Frames Functions--------------------------------

Frame *Client::createLoginFrame(string header,string version, string userName, string password, string destination) {
    return nullptr;
}
Frame *Client::createJoinFrame(string header,string destination, string id, string receipt) {

    return nullptr;
}

Frame *Client::createAddFrame(string header,string destination, string body) {

    return nullptr;
}

Frame *Client::createBorrowFrame(string header,string destination, string body) {

    return nullptr;
}

Frame *Client::createReturnFrame(string header,string destination, string body) {
    return nullptr;
}

Frame *Client::createStatusFrame(string header,string destination, string body) {
    return nullptr;
}

Frame *Client::createLogoutFrame(string header,string receipt){

    return nullptr;
}

