#include "Server.h"
#include <ws2tcpip.h>
#include <thread>
#include <cstdarg>
#include <iostream>

namespace Goat {
Server::Server(int threads) {
    //Set field variables to defaults
    threads_per_function = threads;
    max_buffer_length = Server::DEFAULT_MAX_BUFFER_LENGTH;
    on = false;
	tracing = true;
	tracing_currently = false;
} Server::~Server() {
    // Stop only if on
    if (on)
        stop();
}

void Server::traceThreads(){
	// Let server object know that tracing is happening
	tracing_currently = true;
	
	// While server object wants tracing
	while(tracing)
		
		// Print and pop
		while(!traces.empty()){
			std::cout<<"Trace: "<<traces.front()<<"\n";
			traces.pop();
		}
	
	// Let server object know tracing is finished
	tracing_currently = false;
}

void Server::trace(std::string s){
	traces.push(s);
}

void Server::tracef(const char* format, ...){
	const int init_buf_size = 65;// How much does it matter what this number is?
	
	// Create argument list
	va_list arglist;
	va_start(arglist, format);
	
	// Create buffer
	char* buffer = new char[init_buf_size];
	
	// Copy to buffer
	int len = vsprintf(buffer, format, arglist);
	
	// If length is too large, copy again
	if(len>init_buf_size){
		buffer = new char[len+1];
		vsprintf(buffer,format, arglist);
	}
	
	// Push to traces
	traces.push(std::string(buffer));
	
	// Clean up
	va_end(arglist);
	delete[] buffer;
}

bool Server::notifyPort(int port) {
    // Client variables
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		tracef("notifyPort: failed with error code: %d",WSAGetLastError());
        return 0;
    }

    // Create a socket
    if ((s =
                socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        tracef("notifyPort: failed to create socket: %d",WSAGetLastError());
		return 0;
    }

    // Prepare server struct
    server.sin_addr.s_addr = inet_addr("127.0.0.1");	//Should be IP address
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to server
    if (connect(s, (struct sockaddr *) &server, sizeof(server))
            < 0)
        return 0;

    return 1;
}

void Server::stop() {
	// Turn off Server
	trace("stop: stopping server...");
    on = false;

    // Notify each port until all threads are turned off
    for (auto i:port_functions)
		while(notifyPort(i.port));
	
	// Turn off trace thread
	if(tracing){
		tracing = false;
		while(tracing_currently);
		tracing = true;
	}
	
	// Trace thread is off, so we use cout
    std::cout<<"Stopped server.\n";
}

void Server::addPortFunction(int port,
                             Server::ResponseFunction function,
                             int threads) {
    port_functions.push_back(Server::PortFunction
                             (port, function, threads));
}

SOCKET Server::makeServerSocket(int port) {
	// Declare needed socket variables
    WSADATA wsaData;
    SOCKET server_socket = INVALID_SOCKET;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
	int iresult;//integer result

    // Initialize Winsock
    iresult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iresult != 0) {
        trace("makeServerSocket: WSAStartup failed with error: "+iresult);
        return INVALID_SOCKET;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iresult =
        getaddrinfo(NULL, std::to_string(port).c_str(), &hints,
                    &result);
    if (iresult != 0) {
        trace("makeServerSocket: getaddrinfo failed with error: "+iresult);
        return INVALID_SOCKET;
    }
	
    // Create a SOCKET for connecting to server
    server_socket =
        socket(result->ai_family, result->ai_socktype,
               result->ai_protocol);
    if (server_socket == INVALID_SOCKET) {
        trace("makeServerSocket: socket failed with error: "+WSAGetLastError());
        freeaddrinfo(result);
        return INVALID_SOCKET;
    }
	
    // Bind the TCP listening socket
    if (bind(server_socket, result->ai_addr,
             (int) result->ai_addrlen) == SOCKET_ERROR) {
        trace("makeServerSocket: bind failed with error: "+WSAGetLastError());
        freeaddrinfo(result);
        closesocket(server_socket);
        return INVALID_SOCKET;
    }

	// Free result
    freeaddrinfo(result);

	// Listen
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        trace("makeServerSocket: listen failed with error: "+WSAGetLastError());
        closesocket(server_socket);
        return INVALID_SOCKET;
    }

    return server_socket;
}

bool Server::listenWithSocket(SOCKET server_socket,
                              Server::PortFunction pf) {
    //Variables
    // Set up receiving buffer
    char *recvbuf = new char[max_buffer_length];
						
	// Set up request object
	Request request;
	request.setServerSocket(server_socket);
	request.setServerPort(pf.port);
						
    // Set up client socket
    SOCKET client_socket = INVALID_SOCKET;
    int buffer_length;
    int response_length;
	
	// Loop accepting client socket
    while (this->on) {
        // Accept a client socket
        client_socket = accept(server_socket, NULL, NULL);
		
		// Turn off if needed
        if (!(this->on))
            break;

        //Check if client socket is proper
        if (client_socket == INVALID_SOCKET) {
            trace("listenWithSocket: accept failed with error: "+WSAGetLastError());
            continue;
        }else{
			trace("listenWithSocket: accept successful");
		}
        // Receive until the peer shuts down the connection
        do {
			//Get request as cstring
            buffer_length =
                recv(client_socket, recvbuf,
                     this->max_buffer_length, 0);
            if (buffer_length > 0) {
				// Add terminating null
				recvbuf[buffer_length] = '\0';
				
                //Note: TODO: use getpeername to get IP

                //Trace bytes received
                tracef("listenWithSocket: bytes received on %d: %d",server_socket, buffer_length);
                
				// Form request as Request object
				request.setData(std::string(recvbuf));
				request.setClientSocket(client_socket);
				
                //Get Response
                std::string sreply = pf.function(request);

                //Send response
                response_length =
                    send(client_socket,
                         sreply.c_str(),
                         sreply.length(), 0);

                //Response error
                if (response_length == SOCKET_ERROR) {
                    tracef
                    ("listenWithSocket: send failed with error: %d",WSAGetLastError());
                    closesocket(client_socket);
                    continue;
                }
                tracef("listenWithSocket: bytes sent: %u",
                       sreply.length());
            } else if (buffer_length == 0)
                trace("listenWithSocket: connection closing...");
            else {
                tracef
                ("listenWithSocket: recv failed with error: %d",
                 WSAGetLastError());
            }

        } while (buffer_length > 0 && this->on);

    }

    // shutdown the connection since we're done
    if (shutdown(client_socket, SD_SEND) == SOCKET_ERROR) {
        tracef("listenWithSocket: shutdown failed with error: %d",
               WSAGetLastError());
    } else {
        trace("listenWithSocket: shutdown successfully");
    }

    // cleanup
    closesocket(client_socket);
    delete[] recvbuf;

    return 1;
}

void Server::run() {
    //Turn on
    on = true;

    //Prepare list of threads and sockets
    std::vector < std::thread > threads;
    std::vector < SOCKET > sockets;

    //Fill threads
    for (unsigned int i = 0; i < port_functions.size(); i++) {
        //Make new listener socket
        sockets.push_back(makeServerSocket
                          (port_functions[i].port));

        //Add threads for each socket
        for (int j = 0;
                j < (port_functions[i].threads ==
                     -1 ? threads_per_function :
                     port_functions[i].threads); j++) {
            //Append new thread to list
            threads.push_back(std::thread(&
                                          (Server::listenWithSocket),
                                          this,
                                          sockets.
                                          back(),
                                          port_functions
                                          [i]));
        }
    }

	// Start tracing thread
	threads.push_back(std::thread(&(Server::traceThreads),this));	
	
	//  Wait to start tracing, ensures no conflict with threads.size()
	while(!tracing_currently);
	
    // Show total threads
	tracef("Run: total threads: %lu\n",threads.size());
	
	// Command center
    std::string s;
    while (s != "exit")
        std::cin >> s;

    // Stop threads
    stop();

    // Join threads
	for (unsigned int i = 0; i < threads.size(); i++)
        threads[i].join();

    // Clean up
    WSACleanup();
    for (unsigned int i = 0; i < sockets.size(); i++) {
        closesocket(sockets[i]);
    }
    return;
}
}
