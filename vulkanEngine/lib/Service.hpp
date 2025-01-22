#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>


class Service{

public:
    Service(int port);
    ~Service();


    void run();


private:
    int serverSocket;




};