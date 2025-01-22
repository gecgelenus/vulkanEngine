#include "Service.hpp"
#include <string.h>

Service::Service(int port)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    
}

Service::~Service()
{
}

void Service::run()
{
    listen(serverSocket, 5);
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    
    while (true)
    {
        char buffer[1024] = {0};
        
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::string b(buffer);
        
        if(bytesRead > 0){
            std::cout << "Message from client: " << buffer << std::endl;
        }else if(bytesRead == -1 || buffer == "CLOSE"){
            std::cout << "An error occured on socket or client disconnected." << buffer << std::endl;
            close(clientSocket);
            clientSocket = accept(serverSocket, nullptr, nullptr);
        }

        
    }
    
}
