//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <User.h>

using boost::asio::ip::tcp;
using namespace std;
//--------------------------------------Write Class--------------------------------

class TaskWrite {

private:
    ConnectionHandler &connectionHandler;
    User *activeUser;
public:

    TaskWrite(ConnectionHandler &connectionHandler,User* activeUser) : connectionHandler(connectionHandler),activeUser(activeUser) {}

    void operator()() {
        while (!connectionHandler.getWriteIsLoggedOut()) {
            //Reading line from the command:
            string line;
            getline(cin, line);
            vector<string> lineVector;
            boost::split(lineVector, line, boost::is_any_of(" "));

            if (lineVector[0] == "login") {
                int locationOfIp = lineVector[1].find_first_of(':');
                int endOfPort = lineVector[1].find_first_of(" ");
                string host = lineVector[1].substr(0, locationOfIp);
                string port = lineVector[1].substr(locationOfIp, endOfPort);

                //Actions regarding the Command

                activeUser = new User(lineVector[2], lineVector[3]);
                //Creating Frame
                string frame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:" + lineVector[2] +
                               "\npasscode:" + lineVector[3] + "\n\n" + '\u0000';
                connectionHandler.sendLine(frame);
                activeUser->setSubscribeId(0);
            }

            if (lineVector[0] == "join") {
                //Actions regarding the Command
                int subscribeId = activeUser->getSubscribeId();
                int receiptId = activeUser->getReceiptCounter();

                activeUser->setreceiptIdToPrint(receiptId, "Joined club " + lineVector[1]);
                string frame =
                        "SUBSCRIBE\ndestination:" + lineVector[1] + "\n" + "id:" + to_string(subscribeId) +
                        +"\nreceipt:" + to_string(receiptId) + "\n\n" + '\u0000';
                activeUser->increaseReceiptCounter();
                //Sending The Lines
                connectionHandler.sendLine(frame);

            }
            if (lineVector[0] == "add") {
                //Actions regarding the Command
                Book *book = new Book(lineVector[2], lineVector[1]);
                activeUser->addToInventory(book);
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               activeUser->getUserName() + " has added the book " + lineVector[2] + "\n"
                               + '\u0000';
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "borrow") {
                //adding the book to my wishing books
                activeUser->addToMyWishing(lineVector[2]);
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               activeUser->getUserName() + " wish to borrow " + lineVector[2] +"\n"+
                               '\u0000';
                connectionHandler.sendLine(frame);

            }

            if (lineVector[0] == "exit") {
                //Actions regarding the Command
                int subscribeId = activeUser->getSubscribeId();
                int receiptId = activeUser->getReceiptCounter();
                activeUser->setreceiptIdToPrint(receiptId, "Exit club " + lineVector[1]);
                activeUser->increaseReceiptCounter();
                //Creating Frame
                string frame =
                        "UNSUBSCRIBE\ndestination:" + lineVector[1] + "\n" + "id:" + to_string(subscribeId) +"\nreceipt:"+to_string(receiptId)+
                        "\n\n" + '\u0000';
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "return") {
                Book *book = new Book(lineVector[2], lineVector[1]);
                string prevowner=activeUser->getPreOwnerFromBorrowedBooks(lineVector[2]);
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               "Returning " + lineVector[2] + " to "
                               +prevowner+
                               "\n" + '\u0000';
                activeUser->removeFromInventory(book->getName());
                activeUser->removeFromBorrowedBooks(lineVector[2]);
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "status") {
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               "book status\n"
                               + '\u0000';
                connectionHandler.sendLine(frame);

            }

            if (lineVector[0] == "logout") {
                //Creating Frame
                int receiptId = activeUser->getReceiptCounter();
                string frame =
                        "DISCONNECT\nreceipt:" + to_string(activeUser->getReceiptCounter()) + "\n\n" + '\u0000';
                activeUser->setreceiptIdToPrint(receiptId, "DISCONNECT");
                activeUser->increaseReceiptCounter();
                connectionHandler.sendLine(frame);
                connectionHandler.setWriteIsLoggedOut();
            }
        }
    }

};


//--------------------------------------TaskRead Class--------------------------------

class TaskRead {
private:
    ConnectionHandler &connectionHandler;
    User *activeUser;
public:

    TaskRead(ConnectionHandler &connectionHandler,User* activeUser) : connectionHandler(connectionHandler),activeUser(activeUser) {}

    void operator()() {
        while (!connectionHandler.getReadIsLoggedOut()) {
            string line = "";
            connectionHandler.getFrameAscii(line, '\0');
            vector<string> lineVector;
            cout<<line<<endl;
            boost::split(lineVector, line, boost::is_any_of("\n"));

            if (lineVector[0] == "CONNECTED") {
                cout << "CONNECTED Frame Arrived" <<endl;
            }

            if (lineVector[0] == "RECEIPT") {
                string receiptId=lineVector[1].substr(11);
                string messageToPrint=activeUser->getReceiptIdToPrint(receiptId);
                if (messageToPrint == "DISCONNECT") {
                    connectionHandler.setReadIsLoggedOut();
                    connectionHandler.close();

                }
                else
                    cout << messageToPrint <<endl;
            }

            if (lineVector[0] == "MESSAGE") {

                bool containBorrow = boost::algorithm::contains(lineVector[5], "borrow");
                bool containReturning = boost::algorithm::contains(lineVector[5], "Returning");
                bool containStatus = boost::algorithm::contains(lineVector[5], "status");
                bool containTaking = boost::algorithm::contains(lineVector[5], "Taking");
                bool containHasAdded= false;
                bool containHas= false;
                if(boost::algorithm::contains(lineVector[5], "has")){
                    if(boost::algorithm::contains(lineVector[5], "has added")) {
                        containHasAdded = true;
                    }
                    else
                        containHas=true;
                }

                vector<string> bodyVector;
                boost::split(bodyVector, lineVector[5], boost::is_any_of(" "));


                //Return Message case
                if (containReturning) {
                    if (activeUser->getUserName() == bodyVector[3]) {
                        string bookGenre = lineVector[3].substr(12);
                        Book *bookToAdd = new Book(bodyVector[1], bookGenre);
                        activeUser->addToInventory(bookToAdd);
                    }
                }

                //Borrow Message case
                if (containBorrow) {
                    if (activeUser->iHaveTheBook(bodyVector[4])) {
                        string frame =
                                "SEND\ndestination:"+lineVector[3].substr(12)+"\n\n" +activeUser->getUserName()+" has " + bodyVector[4]+"\n" + '\u0000';
                        connectionHandler.sendLine(frame);
                    }
                }

                //Taking Message case
                if (containTaking) {
                    //if the name is mine ,delete the book
                    string bookToRemove = bodyVector[1];
                    if (bodyVector[3] == activeUser->getUserName()) {
                        activeUser->removeFromInventory(bookToRemove);
                    }
                }

                //other client has the book
                if (containHasAdded) {
                    //TODO Delete this line
                    cout<< "someone has added a book"<<endl;
                }

                //Status Message case
                if (containStatus) {
                    string myBooks = activeUser->printInventoryByGenre(lineVector[3].substr(12));
                    string frame = "SEND\ndestination:" + lineVector[3].substr(12) + "\n\n"
                                   + activeUser->getUserName()+":" + myBooks+"\n" + '\u0000';
                    connectionHandler.sendLine(frame);
                }


                if(containHas){
                    string body=lineVector[5];
                    int locationOfName=lineVector[5].find(" ");
                    string tempbody=body.substr(locationOfName+1,body.size()-1);
                    int locationofbookname=tempbody.find(" ");
                    string bookname=tempbody.substr(locationofbookname+1,body.size()-1);
                    if(activeUser->isInMyWishList(bookname)) {
                        string previewsowner = lineVector[5].substr(0,locationOfName);
                        string topic = lineVector[3].substr(12);
                        Book *newBook = new Book(bookname, topic);
                        newBook->setPrivouesOwner(previewsowner);
                        activeUser->addToInventory(newBook);
                        activeUser->removeFromWishing(bookname);
                        activeUser->addToBorrowedBooks(newBook);
                        string frame ="SEND\ndestination:" + topic + "\n\n" + "Taking "
                                      + bookname +" from " + previewsowner + "\n" + '\u0000';
                        connectionHandler.sendLine(frame);
                    }
                }
            }

            if (lineVector[0] == "ERROR") {
                string error=lineVector[3];
                connectionHandler.setReadIsLoggedOut();
                connectionHandler.close();
            }
        }
    }

};

//--------------------------------------Main Class--------------------------------

int main(int argc, char *argv[]) {


    bool isConnected=false;

    while(!isConnected) {
        string line;
        getline(cin, line);
        vector<string> lineVector;
        boost::split(lineVector, line, boost::is_any_of(" "));
        if (lineVector[0] == "login") {
            int locationOfIp = lineVector[1].find_first_of(':');
            int endOfPort = lineVector[1].find_first_of(" ");
            string host = lineVector[1].substr(0, locationOfIp);
            User *activeUser = new User(lineVector[2], lineVector[3]);
            string port = lineVector[1].substr(locationOfIp + 1, endOfPort);
            short shortPort = boost::lexical_cast<short>(port);
            ConnectionHandler *connectionHandler = new ConnectionHandler(host, shortPort);
            connectionHandler->connect();
            string frame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:" + lineVector[2] +
                           "\npasscode:" + lineVector[3] + "\n\n" + '\u0000';
            connectionHandler->sendLine(frame);

            //receiving the first frame
            string line = "";
            connectionHandler->getFrameAscii(line, '\0');
            vector<string> lineVector;
            boost::split(lineVector, line, boost::is_any_of("\n"));

            if (lineVector[0] == "CONNECTED") {
                isConnected = true;
                activeUser->setSubscribeId(0);
                cout << "Connected frame arrived" << endl;
                TaskWrite taskWrite(*connectionHandler, activeUser);
                TaskRead taskRead(*connectionHandler, activeUser);
                std::thread thWrite(std::ref(taskWrite)); // we use std::ref to avoid creating a copy of the Task object
                std::thread thRead(std::ref(taskRead));
                thWrite.join();
                thRead.join();
            } else {
                string error = lineVector[3];
                cout << error << endl;
            }
        }
    }
    return 0;
}








