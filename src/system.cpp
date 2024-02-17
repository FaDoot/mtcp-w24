#include "socket/server_socket.hpp"
#include "socket/message_socket/connection_message_socket.hpp"
#include "messages/pose.hpp" // Assuming you have such a message class
#include "system_utils.hpp" // For pose update functionality
#include "mros_json.hpp"
#include <string>
#include <thread>
#include <memory>
#include <messages/pose.hpp>
#include <messages/twist.hpp>

#include <gtest/gtest.h>

#include "socket/message_socket/client_message_socket.hpp"
#include "socket/server_socket.hpp"
using namespace std::chrono_literals;  
int main() {
        // System properties
        int const domain = AF_INET;
        std::string const server_address = "127.0.0.1"; // Local IP
        int const controller_port = 8003; // Controller Port
        int const python_bridge_port = 8002; // Python Bridge Port

        // Initialize system propertie
       // (domain, controller_port);
        std::shared_ptr<ServerSocket> serverSocket;
        std::shared_ptr<ConnectionMessageSocket> connectionSocket;
        std::shared_ptr<ClientMessageSocket> bridge;
        serverSocket = std::make_shared<ServerSocket>(domain , server_address, controller_port, 1); //controller or bridge?
        bridge = std::make_shared<ClientMessageSocket>(domain, server_address, python_bridge_port); //connection?

     
        // Connect to Python bridge
        try{
            bridge->connect();
            std::cout << "Connected to Python bridge." << std::endl;
        } catch(std::exception const&e){
            std::cout << e.what() << std::endl;
        }
        
        

        connectionSocket = serverSocket->acceptConnection<ConnectionMessageSocket>();
        std::this_thread::sleep_for(1000ms);

        std::cout << "Connected to socket thing." << std::endl;
        // Initialize pose at (0, 0, 0)
        Messages::Pose2d currentPose{0.0, 0.0, 0.0};
       //std::cout << " pose" << currentPose.x << std::endl;
        
        
        // std::string message = connectionSocket->receiveMessage();
        // std::cout << " we got da message" << message << std::endl;
        // Main loop to receive messages from the controller

        while (true) {
            //receve

           //test starts here
            // Messages::Pose2d pose = {0,0,0};
            // Json jpose;
            // jpose = pose;
            // std::cout << " sending: " << jpose.toString() << std::endl;
            // std::this_thread::sleep_for(1000ms);
            // bridge->sendMessage(jpose.toString());  

            // pose = {1,1,1};
            // jpose = pose;
            // std::cout << " sending: " << jpose.toString() << std::endl;
            // std::this_thread::sleep_for(1000ms);
            // bridge->sendMessage(jpose.toString());

            // pose = {2,2,2};
            // jpose = pose;
            // std::cout << " sending: " << jpose.toString() << std::endl;
            // std::this_thread::sleep_for(1000ms);
            // bridge->sendMessage(jpose.toString());

            // pose = {3,3,3};
            // jpose = pose;
            // std::cout << " sending: " << jpose.toString() << std::endl;
            // std::this_thread::sleep_for(1000ms);
            // bridge->sendMessage(jpose.toString());
            //bullsht ends here

            Messages::Twist2d twist;

            std::string message = connectionSocket->receiveMessage();

            std::cout << "got this message: " << message << std::endl;

            Json twisty = Json::fromString(message);

            Messages::Twist2d::fromJson(twist, twisty);

            currentPose = mutatePose(twist, currentPose);
            std::this_thread::sleep_for(1000ms);

            
            Json jpose;
            jpose = currentPose;
            std::cout << " sending: " << jpose.toString() << std::endl;
            bridge->sendMessage(jpose.toString());
        }
}

