#include <unistd.h>
#include <sstream>
#include <string>
#include <zmq.hpp>
#include <iostream>

using namespace std;
 
int main(){
    const string to_c = "tcp://*:5554";

    zmq::context_t context;
    zmq::socket_type type = zmq::socket_type::rep;
    zmq::socket_t socket (context, type);
    socket.bind(to_c);


    while (1) {

        // Receive the message from C
        zmq::message_t message;
        socket.recv(message);

        std::string received_msg(static_cast<char*>(message.data()), message.size());
        if (received_msg == "exit") {
            socket.unbind(to_c);
            exit(0);
        }
        cout << "B: size of message from C:" << received_msg.size() << endl;

        // Send to C
        sleep(1);
        string answer = "b: Get new string";
        zmq::message_t ans(answer.size());
        memcpy(ans.data(), answer.c_str(), answer.size());
        socket.send(ans);

        // Get message from A
        zmq::message_t message_from_a;
        socket.recv(message_from_a);
        std::string cnt(static_cast<char*>(message_from_a.data()), message_from_a.size());
        cout << "B: size of message from A:" << cnt << endl;

        // Send message to A
        string z = "get cnt from a";
        zmq::message_t buf(z.size());
        memcpy(buf.data(), z.c_str(), z.size());
        socket.send(buf);

    }
    return 0;
}