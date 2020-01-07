//
// Created by yairsep@wincs.cs.bgu.ac.il on 06/01/2020.
//

#include <thread>
#include "Client.h"
#include "../include/connectionHandler.h"
#include <boost/algorithm/string.hpp>
#include <include/CONNECT.h>
#include <include/User.h>

using boost::asio::ip::tcp;

//--------------------------------------Write Class--------------------------------

class TaskWrite{
private:
    ConnectionHandler &connectionHandler;
    User* user= new User();
public:

    TaskWrite(ConnectionHandler &connectionHandler):connectionHandler(connectionHandler){}

    void operator()(){
        while (!connectionHandler.isLoggedOut()){
            //Reading line from the command:
            string line;
            getline(cin,line);
            vector<string> lineVector;
            boost::split(lineVector, line, boost::is_any_of(" "));

            if(lineVector[0]=="login"){

                //Actions regarding the Command
                user = new User(lineVector[2],lineVector[3]);

                //Creating Frame
                string frame="CONNECT accept-version:1.2 host:stomp.cs.bgu.ac.il login:"+lineVector[2]+" "+"passcode:"+lineVector[3]+"\n\n"+"\0";
                vector<string> output;
                boost::split(output, frame, boost::is_any_of(" "));

                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }
                user->setSubscribeId(0);
            }
            if(lineVector[0]=="join"){
                //TODO ADDING THE TOPIC TO MAP
                //Actions regarding the Command
                int subscribeId=user->getSubscribeId();


                //Creating Frame
                string frame="SUBSCRIBE\ndestination:"+lineVector[1]+"\n"+"id:"+to_string(subscribeId)+
                             +"\nreceipt:"+to_string(user->receiptCounter) +"\n\n"+"\0";
                user->receiptCounter++;
                vector<string> output;
                boost::split(output, frame, boost::is_any_of("\n"));

                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }
            }

            if(lineVector[0]=="exit"){
                //TODO check Unsubscribe frame sturcture

                //Actions regarding the Command
                int subscribeId=user->getSubscribeId();
                vector<Book> inventory = user->getInventory();
                for (auto it = inventory.begin() ;it != inventory.end(); ++it ){
                    if (it->getGenre()==lineVector[1]){
                        inventory.erase(it);
                    }
                }
                //Creating Frame
                string frame="UNSUBSCRIBE\ndestination:"+lineVector[1]+"\n"+"id:"+to_string(subscribeId)+
                             "\n\n"+"\0";
                vector<string> output;
                boost::split(output, frame, boost::is_any_of("\n"));

                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }
            }


            if(lineVector[0]=="add"){
                //TODO check if further action needed
                //Actions regarding the Command

                Book *book = new Book(lineVector[2],lineVector[1] );
                user->addToInventory(book);

                //Creating Frame
                string frame="SEND\ndestination:"+lineVector[1]+"\n"+"\n"+
                             user->getUserName() + "has  added the book" + lineVector[2]+"\n"
                             +"\0";
                vector<string> output;
                boost::split(output, frame, boost::is_any_of("\n"));

                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }

            }

            if(lineVector[0]=="borrow"){
                //TODO check if further action needed

                //Actions regarding the Command


                //Creating Frame
                string frame="SEND\ndestination:"+lineVector[1]+ "\n\n"+
                             user->getUserName()+"\nwish to borrow" + lineVector[2]+
                             "\0";
                vector<string> output;
                boost::split(output, frame, boost::is_any_of(" "));
                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }

                //In case other user has the book
                //Todo Complete this task
                bool otherUserHasTheBook;
                string bookOwner;
                Book *borrowedBook = new Book((lineVector[2]);
                if (otherUserHasTheBook) {
                    user->addToInventory(borrowedBook);
                    string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                                   "Taking" +lineVector[2]+ "from "+bookOwner+"\n"+
                                   "\0";
                    vector<string> output;
                    boost::split(output, frame, boost::is_any_of(" "));
                    for(int i=0; i < output.size(); i++) {
                        connectionHandler.sendLine(output[i]);
                    }
                }
            }

            if(lineVector[0]=="return") {

                //Actions regarding the Command
                vector<Book> inventory = user->getInventory();
                Book *book = new Book(lineVector[2], lineVector[1]);
                bool found = false;
                for (auto it = inventory.begin(); it != inventory.end(); ++it) {
                    if (it->getName() == book->getName())
                        found = true;
                    inventory.erase(it);
                }
                if(!found){
                    //TODO CHECK IF I DONT HAVE THE BOOK
                }
                else {

                    //Creating Frame
                    string frame = "SEND\ndestination:" + lineVector[1] + "\n\n" +
                                   "Returning" + lineVector[2] + "to" + user->getPreviousOwnerName(*book) +
                                   "\n\n" + "\0";

                    vector<string> output;
                    boost::split(output, frame, boost::is_any_of(" "));
                    //Sending The Lines
                    for (int i = 0; i < output.size(); i++) {
                        connectionHandler.sendLine(output[i]);
                    }
                }
            }
            if(lineVector[0]=="status"){

                //Actions regarding the Command


                //Creating Frame
                string frame="SEND\ndestination:"+lineVector[1]+"\n\n"+
                             "book status"
                             +"\0";
                vector<string> output;
                boost::split(output, frame, boost::is_any_of(" "));
                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }
            }

            if(lineVector[0]=="logout"){

                //Creating Frame
                string frame="DISCONNECT\nreceipt:"+to_string(user->receiptCounter) +"\n\n"+"\0";
                vector<string> output;
                boost::split(output, frame, boost::is_any_of(" "));

                //Sending The Lines
                for(int i=0; i < output.size(); i++) {
                    connectionHandler.sendLine(output[i]);
                }

                //Actions regarding the Command

                //Deleting the Inventory
                for (auto it = user->getInventory().begin(); it != user->getInventory().end(); ++it) {
                    user->getInventory().erase(it);
                }

                //TODO Closing the Socket

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

        //TODO confirm and check how to read from the connection handler

        string line;
        getline(cin,line);
        connectionHandler.getLine( line);
        vector<string> lineVector;
        boost::split(lineVector, line, boost::is_any_of("\n"));

        if (lineVector[0] == "CONNECTED") {

            cout << "Login successful";

        }


        if (lineVector[0] == "RECEIPT") {

            //Joined Club Case
            bool joinedClub;
            if(joinedClub){
                cout << "Joined club *(Genre)*";
            }

            //Exited Club Case
            bool exitedClub;
            if(exitedClub){
                cout << "Exited club *(Genre)*";
            }

            //Discconect Case
            bool Discconect;
            if(Discconect){

            }

        }

        if (lineVector[0] == "MESSAGE") {


            //Return Message case



            //Borrow Message case



            //Status Message case

        }


        if (lineVector[0] == "ERROR") {

            //User already logged in
            bool alreadyLoggedIn;
            if(alreadyLoggedIn){
                cout << "User already logged in";
            }

            //Wrong password case
            bool wrongPassword;
            if(wrongPassword){
                cout << "Wrong Password";
            }

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



