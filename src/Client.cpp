//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <User.h>
#include <condition_variable>
#include <Client.h>


using boost::asio::ip::tcp;
using namespace std;
//--------------------------------------Write Class--------------------------------

class TaskWrite {

private:
    ConnectionHandler &connectionHandler;
    User *activeUser;
//    std::mutex& mutex;

public:

    TaskWrite(ConnectionHandler &connectionHandler,User* activeUser) : connectionHandler(connectionHandler)
            ,activeUser(activeUser)  {}

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

//                activeUser = new User(lineVector[2], lineVector[3]);
                //Creating Frame
                string frame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:" + lineVector[2] +
                               "\npasscode:" + lineVector[3] + "\n\n";
                bool isConnected = connectionHandler.sendLine(frame);
                if(lineVector[2]==activeUser->getUserName()){
                    connectionHandler.setWriteIsLoggedOut();
                }
                if (isConnected == false) {
                    cout << "Could not connect to server" << endl;
                    connectionHandler.close();
                    connectionHandler.setWriteIsLoggedOut();
                }
                activeUser->setSubscribeId(0);
            }

            if (lineVector[0] == "join") {
                //Actions regarding the Command
                int subscribeId = activeUser->getSubscribeId();
                int receiptId = activeUser->getReceiptCounter();

                activeUser->setreceiptIdToPrint(receiptId, "Joined club " + lineVector[1]);
                string frame =
                        "SUBSCRIBE\ndestination:" + lineVector[1] + "\n" + "id:" + to_string(subscribeId) +
                        +"\nreceipt:" + to_string(receiptId) + "\n\n";
                activeUser->increaseReceiptCounter();
                //Sending The Lines
                connectionHandler.sendLine(frame);

            }
            if (lineVector[0] == "add") {
                //Actions regarding the Command
                int bookLocation = line.find(lineVector[2]);
                string bookName = line.substr(bookLocation, line.size() - 1);
                string genre=lineVector[1];
                Book* book = new Book(bookName, genre);
                activeUser->addToInventory(book);
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               activeUser->getUserName() + " has added the book " + bookName + "\n";
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "borrow") {
                //adding the book to my wishing books
                int bookLocation = line.find(lineVector[2]);
                string bookName = line.substr(bookLocation, line.size() - 1);

                activeUser->addToMyWishing(bookName);
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               activeUser->getUserName() + " wish to borrow " + bookName + "\n";
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
                        "UNSUBSCRIBE\ndestination:" + lineVector[1] + "\n" + "id:" + to_string(subscribeId) +
                        "\nreceipt:" + to_string(receiptId) +
                        "\n\n";
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "return") {

                int bookLocation = line.find(lineVector[2]);
                string bookName = line.substr(bookLocation, line.size() - 1);

                Book *book = new Book(bookName, lineVector[1]);
                string prevowner = activeUser->getPreOwnerFromBorrowedBooks(bookName);
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               "Returning " + bookName + " to " + prevowner + "\n";
                activeUser->removeFromInventory(book->getName());
                activeUser->removeFromBorrowedBooks(bookName);
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "status") {
                //Creating Frame
                string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                               "book status\n";
                connectionHandler.sendLine(frame);
            }

            if (lineVector[0] == "logout") {
                //Creating Frame
                int receiptId = activeUser->getReceiptCounter();
                string frame =
                        "DISCONNECT\nreceipt:" + to_string(activeUser->getReceiptCounter()) + "\n\n";
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
//    std::mutex& mutex;

public:

    TaskRead(ConnectionHandler &connectionHandler,User* activeUser):
            connectionHandler(connectionHandler),activeUser(activeUser)  {}

    void operator()() {
        while (!connectionHandler.getReadIsLoggedOut()) {
            string line = "";
            connectionHandler.getFrameAscii(line, '\0');
            line=line+"^@";
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
                    connectionHandler.setWriteIsLoggedOut();
                    connectionHandler.close();
                    for (Book *book : activeUser->getInventory()){
                        delete (book);
                    }
                    return;
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
                    if (bodyVector[bodyVector.size()-1] == activeUser->getUserName()){
                        string bookGenre = lineVector[3].substr(12);
                        int locationOfFirstSpace = lineVector[5].find(" ");
                        int locationOfTo=lineVector[5].find("to");
                        string bookName =lineVector[5].substr(locationOfFirstSpace+1,locationOfTo -locationOfFirstSpace-2 );
                        Book *bookToAdd = new Book(bookName, bookGenre);
                        activeUser->addToInventory(bookToAdd);
                    }
                }

                //Borrow Message case
                if (containBorrow) {

                    int LocationOfName = lineVector[5].find(bodyVector[4]);
                    string bookName = lineVector[5].substr(LocationOfName, lineVector[5].size() - 1);
                    if (activeUser->iHaveTheBook(bookName)) {
                        string frame =
                                "SEND\ndestination:" + lineVector[3].substr(12) + "\n\n" +
                                activeUser->getUserName() + " has " + bookName + "\n";
                        connectionHandler.sendLine(frame);
                        }
                    }
                if(containHasAdded){

                }


                //Taking Message case
                if (containTaking) {
                    int LocationOfFrom = lineVector[5].find("from");
                    string nameTotake = lineVector[5].substr(LocationOfFrom + 5, lineVector[5].size() - 1);
                    int LocationOfSpace = lineVector[5].find(" ");
                    string temp = lineVector[5].substr(0, LocationOfFrom - 1);
                    string bookToRemove = temp.substr(LocationOfSpace + 1);

                    //if the name is mine ,delete the book
                    if (nameTotake == activeUser->getUserName()) {
                        activeUser->removeFromInventory(bookToRemove);

                        }
                    }

                //Status Message case
                if (containStatus) {
                    string myBooks = activeUser->printInventoryByGenre(lineVector[3].substr(12));
                    string frame = "SEND\ndestination:" + lineVector[3].substr(12) + "\n\n"
                                   + activeUser->getUserName()+":" + myBooks+"\n";
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
                        activeUser->addToBorrowedBooks(bookname);
                        string frame ="SEND\ndestination:" + topic + "\n\n" + "Taking "
                                      + bookname +" from " + previewsowner + "\n";
                        connectionHandler.sendLine(frame);
                    }
                }
            }

            if (lineVector[0] == "ERROR") {
                connectionHandler.setReadIsLoggedOut();
                connectionHandler.setWriteIsLoggedOut();
                connectionHandler.close();
                return;
            }
        }

    }

};

//--------------------------------------Main Class--------------------------------

int main(int argc, char *argv[]) {

    bool isConnected=false;
    std:: mutex mutex;

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
                           "\npasscode:" + lineVector[3] + "\n\n";
            bool isConnected=connectionHandler->sendLine(frame);

            if(!isConnected){
                cout<<"Could not connect to server"<<endl;
                connectionHandler->close();
                return 0;
            }

            //receiving the first frame
            string line = "";
            connectionHandler->getFrameAscii(line, '\0');
            line =line+"^@";
            cout<<line<<endl;
            vector<string> lineVector;
            boost::split(lineVector, line, boost::is_any_of("\n"));

            if (lineVector[0] == "CONNECTED") {
                isConnected = true;
                activeUser->setSubscribeId(0);
                TaskWrite taskWrite(*connectionHandler, activeUser);
                TaskRead taskRead(*connectionHandler, activeUser);
                std::thread thWrite(std::ref(taskWrite));
                std::thread thRead(std::ref(taskRead));
                thRead.join();
                thWrite.join();
                delete (connectionHandler);
                delete activeUser;
                return 0;


            }

            if (lineVector[0] == "ERROR"){
                connectionHandler->close();
                return 0;
            }
        }
    }
}


Client::~Client() {
}
