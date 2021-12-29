#include <unistd.h>
#include <sstream>
#include <string>
#include <zmq.hpp>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    const string endpoint = "tcp://*:5555";

    // FROM A
    zmq::context_t context;
    zmq::socket_type type = zmq::socket_type::rep;
    zmq::socket_t socket (context, type);
    socket.bind(endpoint);

    // TO B
    const string to_b = "tcp://localhost:5554";
    zmq::socket_type b_type = zmq::socket_type::req;
    zmq::socket_t b_socket (context, b_type);
    b_socket.connect(to_b);

    std::string message;
    while ( 1) {
        // Get message from A

        zmq::message_t message;
        socket.recv(message);
        std::string received_msg(static_cast<char*>(message.data()), message.size());
        if (received_msg == "exit") {
            b_socket.send(message);
            socket.unbind(endpoint);
            socket.disconnect(to_b);
            exit(0);
        }
        cout << "C: " << received_msg << endl;

        //Sending message to A
        string answer = "Get new string";
        zmq::message_t ans(answer.size());
        memcpy(ans.data(), answer.c_str(), answer.size());
        //cout << "C: Sending to A..." << endl;
        socket.send(ans);

        //Sending message to B
        b_socket.send(message);

        zmq::message_t ans_from_b;
        
        //Receiving message from B
        b_socket.recv(ans_from_b);

    }
}