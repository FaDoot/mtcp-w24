#include <chrono>  
#include <iostream>
#include <memory>
#include <thread>  
#include <gtest/gtest.h>
#include "mros_json.hpp"
#include "socket/message_socket/client_message_socket.hpp"
#include "system_utils.hpp"
#include "fstream"
#include <messages/pose.hpp>
#include <messages/twist.hpp>
using namespace std;

using namespace std::chrono_literals; 
using namespace std ;
int main() {
    int const Domain = AF_INET;
    std::string const LocalAddress = "127.0.0.1";
    int const ServerPort = 8003;

    auto bridge_client_socket = std::make_shared<ClientMessageSocket>(Domain, LocalAddress, ServerPort);
    
    bridge_client_socket->connect();
    std::cout << "Connected to controller bridge." << std::endl;

    Messages::Twist2d twist = {0,0,0};
    // Json twist_json;
    //     twist_json = twist;
    //     cout << "twistdx = "<<twist.dx << endl;
    //     Messages::Twist2d::toJson(twist_json, twist);

    //     cout <<" json to string "<<twist_json.toString() << endl;
    std::ifstream fileStream("/root/mtcp-w24/data/control_inputs.txt");
    if(!fileStream.is_open()){
        cout << "file not open!" << endl;
    }
   
    //bridge_client_socket->sendMessage("{\"dtheta\": 0.000000,\"dx\": 0.000000,\"dy\": 0.000000}");
    while (fileStream >> twist.dx>> twist.dy>> twist.dtheta) {    
       
        Json twist_json;
        twist_json = twist;
        cout << "twistdx = "<<twist.dx << endl;
        Messages::Twist2d::toJson(twist_json, twist);

        cout << twist_json.toString() << endl;
        cout <<" sending this :  "<<twist_json.toString() << endl;

        bridge_client_socket->sendMessage(twist_json.toString());
        std::cout << twist_json.toString() << std::endl;
        std::this_thread::sleep_for(1000ms);

    }
    fileStream.close();

    return 0;
}