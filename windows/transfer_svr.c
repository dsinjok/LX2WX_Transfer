#ifndef _WIN32
#define _WIN32
#endif

#include <Windows.h>
#include "network.h"
#include "common.h"

#define MAX_QUEUE 100
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
        if(argc != 2)
        {
                printf("Usage: %s port\n",argv[0]);
                exit(-1);
        }
//-----------def-------------//
        struct sockaddr_in saddr;
        struct sockaddr_in clientaddr;
        const char *ip = "127.0.0.1";
        int port = 1000;
        int socketfd = 0;       //服务端
        int clientfd = 0;       //接入
        int opt = 1;            //端口复用
        int slen;               //accept sockaddr *
//-------------dll init-----------//
	if(sockinit() != 0 )
	{
		fprintf(stderr, "wsastartup error.\n");
		exit(0);
	}


//-------------socket-----------//

        port = atoi(argv[1]);
        memset(&saddr, sizeof(&saddr), 0);
        saddr.sin_family        = AF_INET;
        saddr.sin_port          = htons(port);
        saddr.sin_addr.s_addr   = htonl(INADDR_ANY);
        slen                    = sizeof(clientaddr);

        socketfd = socket(AF_INET, SOCK_STREAM, 0);

        if(socketfd < 0)
        {
                printf("socket error.\n");
                exit;
        }

        //端口复用
#if 1        
        if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        {
                printf("socket setsockopt error.\n");
                close(socketfd);
                exit(-1);
        }
#endif 
        if(bind(socketfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
        {
                printf("socket bind error.\n");
                closesocket(socketfd);
                exit(-1);
        }
        else
        {
                printf("socket bind success.\n");
        }


        if( listen(socketfd,MAX_QUEUE) == -1 )
        {
                printf("socket listen error.\n");
                closesocket(socketfd);
                exit(-1);
        }
        else
        {
                printf("socket listen success.\n");
        }

//---------------service success------------//
        printf("service success...\n");
//----------------accept--------------------//
        for(;;)
        {				
                clientfd = accept(socketfd, (struct sockaddr*)&clientaddr, &slen);
                if(clientfd == -1)
                {
                        printf("service accept error.\n");
                        continue;
                }
                else
                {
                        printf("client:%s link in.\n",inet_ntoa(clientaddr.sin_addr));
                }

                
                int	sockfd					= clientfd;
                struct sockaddr_in	ksaddr	= clientaddr;
                

                HANDLE hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendFile,(LPVOID)clientfd,0,0);
                if(hThread1 != NULL)
                {
                	CloseHandle(hThread1);
                }

				HANDLE hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RecFile,(LPVOID)clientfd,0,0);
  
		        if(hThread2!=NULL)
		        {  
		            CloseHandle(hThread2);
		        }  
		        Sleep(1000);
		
		}

//----------------main clear and exit------------//
        closesocket(socketfd);
        sockclear();
        return 0;
}

