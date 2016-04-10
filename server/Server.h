#pragma once

//Make sure compiling with at least C++11
#if __cplusplus <= 199711L
#    error Need C++11
#endif

//Include headers
#include <functional>
#include <string>
#include <vector>
#include <queue>
#ifdef __WIN32__
	#include <winsock2.h>
#else
	#define SOCKET int;
#endif
#include "request.h"

//Start Goat namespace
namespace Goat {
/*
   Generic Server class
   Allows user to define custom functions on ports
 */
class Server {
    //Static constants
    const static int DEFAULT_THREADS_PER_FUNCTION = 4096;
    const static int DEFAULT_MAX_BUFFER_LENGTH = 2000;

    //Number of threads running on each PORT FUNCTION at once
    int threads_per_function;

    //Max length an incoming buffer can be
    int max_buffer_length;

    //Is the server on?
    bool on;

    //Is tracing on?
    bool tracing;
	bool tracing_currently;//Lets server know if actual tracing function is still running

    // Define the type of response function available
    typedef std::function < std::string(const Goat::Request&) >
    ResponseFunction;

    // Class declaration for the Port Function type, hidden from user
    struct PortFunction {
        int port;	//The port the function will respond on
        int threads;	//defaults to -1, implies copy threads_per_function
        ResponseFunction function;	//The function proper
        PortFunction(int p, ResponseFunction f, int t) {
            port = p;
            function = f;
            threads = t;
        }
    };

    // List of port functions
    std::vector < PortFunction > port_functions;

    // List of traces
    std::queue < std::string > traces;

    // Trace threads
    void traceThreads();

    // Networking private methods
    SOCKET makeServerSocket(int port);
    bool listenWithSocket(SOCKET, PortFunction);
    bool notifyPort(int port);

protected:
    // Add member to port_functions
    void addPortFunction(int port, ResponseFunction function,
                         int threads = -1);

    // Setters
    void setMaxBufferLength(int=DEFAULT_MAX_BUFFER_LENGTH);
    void setThreadsPerFunction(int=DEFAULT_THREADS_PER_FUNCTION);
    void setTracing(bool=true);

    // Tracing (tracing queue needed for multithreading)
    void trace(std::string s);
	void tracef(const char* format, ...); //Trace format

	// Protected constructor (simulates abstract class)
	Server(int threads = DEFAULT_THREADS_PER_FUNCTION);
public:
    // Destructor: turn off server if on by running stop()
    ~Server();/*TODO: The destructor should wait for run() to finish, so it can clean up*/
	
    // Run all threads
    void run();

    // Stop threads
    void stop();
};
}
