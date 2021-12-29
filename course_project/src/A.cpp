#include <unistd.h>
#include <sstream>
#include <set>
#include <string>
#include <iostream>

#include <zmq.hpp>

using namespace std;

int main(){
    const string endpoint = "tcp://localhost:5555";

    zmq::context_t context;

    zmq::socket_type type = zmq::socket_type::req;
    zmq::socket_t socket (context, type);
    cout << "Please, enter the string" << endl;
    socket.connect(endpoint);

    //TO B
    const string to_b = "tcp://localhost:5554";
    zmq::socket_type b_type = zmq::socket_type::req;
    zmq::socket_t b_socket (context, b_type);
    b_socket.connect(to_b);

    pid_t C = fork();
    if (C == -1) {
        perror("fork");
        return -1;
    }
    if (C == 0) {
        pid_t B = fork();
        if (B == -1) {
            perror("fork");
            return -1;
        }
        if (B == 0){
            execl("./B", "./B",NULL);
        } else {
            execl("./C", "./C",NULL);
        }
    }

    string s;
    while(cin >> s) {

        // Sending to C
        zmq::message_t reply;
        zmq::message_t message(s.size());
        memcpy(message.data(), s.c_str(), s.size());
        socket.send(message);

        if (s == "exit") {
            socket.disconnect(endpoint);
            b_socket.disconnect(to_b);
            return 0;
        }

        // Receive from C
        socket.recv(reply);
        std::string received_msg(static_cast<char*>(reply.data()), reply.size());

        //Sending to B
        string c = "TO B";
        int cnt = s.size();
        string cnt_s = to_string(cnt);
        zmq::message_t message_to_b(cnt_s.size());
        memcpy(message_to_b.data(), cnt_s.c_str(), cnt_s.size());
        b_socket.send(message_to_b);
        
        //Get from B
        zmq::message_t rep;
        b_socket.recv(rep);
    }
}