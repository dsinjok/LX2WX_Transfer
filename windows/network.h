#ifndef _NETWORK_H_
#define _NETWORK_H_

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

//LINUX
#ifdef __linux__
#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef _SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifndef _NETINET_IN_H
#include <netinet/in.h>
#endif
#endif					//END OF LINUX

// WINDOWS
#ifdef _WIN32
#ifndef _WINSOCKAPI_
#include <winsock.h>
#endif

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#endif					//END OF WINDOWS

#endif					//END OF _NETWORK_H_


