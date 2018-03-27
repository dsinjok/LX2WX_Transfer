#ifndef _COMMON_H
#define _COMMON_H

#define MaxSize	1025
#define FILEDATA	'1'
#define	FILENAME	'2'

HANDLE hMutex; 

//------------------------------------------------------------------------------------  
//��������:      SendFile(SOCKET sockClient)  
/*��������:      �������� 
/*��ڲ���:      SOCKET sockClient                 
//���ڲ���:      
//ȫ�ֱ�������:   
//����ģ��:      ��  
//------------------------------------------------------------------------------------*/
void SendFile(SOCKET sockClient)  
{  
    char sendBuf[MaxSize];
    int byte = 0;
    char filename[256];
    FILE *fp = NULL;
  
    while(1)  
    {  
        WaitForSingleObject(hMutex, INFINITE);  
        //gets(sendBuf);
        fflush(stdin);
        gets(filename);
        
        fp = fopen(filename,"rb");
        if( NULL == fp)
        {
        	fprintf(stderr,"file open error. filename: %s\n",filename);
        	break;
        }
        else
        {
	       	for(;(!feof(fp));)
	       	{
	       		memset(sendBuf, '\0', sizeof(sendBuf));   
	       		byte	=	fread(sendBuf+1, 1, MaxSize,fp);
	       		sendBuf[0]	=	FILEDATA;
		       	byte	=	send(sockClient,sendBuf,strlen(sendBuf)+1,0);;//�������ӿͻ��˽�������  
		        if (byte<=0)  
		        {  
		            break;  
		        }
		        else memset(sendBuf, '\0', sizeof(sendBuf));
	       	}
	       	fclose(fp); 
	       	sendBuf[0]		=	FILENAME;
	       	strcpy(sendBuf+1, filename);
	       	byte	=	send(sockClient,sendBuf,strlen(sendBuf)+1,0);
	   		if (byte<=0)  
	        {  
	            break;  
	        }
	        else memset(sendBuf, '\0', sizeof(sendBuf)); 
			     	
        }
        
   
          
        //Sleep(1000);  
        ReleaseMutex(hMutex);  
  
    }  
    closesocket(sockClient);//�ر�socket,һ��ͨ�����  
}  
//------------------------------------------------------------------------------------  
//��������:     RecFile()  
/*��������:     ���պ��� 
/*��ڲ���:     SOCKET sockClient             
//���ڲ���:      
//ȫ�ֱ�������:   
//����ģ��:      ��  
//------------------------------------------------------------------------------------*/  
void RecFile(SOCKET sockClient)  
{  
    char revBuf[MaxSize];
    char filename[MaxSize-1];
    memset((void *)filename, '\0', MaxSize);
    memset((void *)revBuf, '\0', MaxSize);
    int byte = 0; 
    FILE *fp;
    fp = fopen("tmp.txt","wb");
  
    while(1)  
    {  
        WaitForSingleObject(hMutex, INFINITE);
        
  
        byte= recv(sockClient,revBuf,MaxSize,0);//�������ӿͻ��˽�������  
        if (byte<=0)  
        {
            break;
        }
		else
		{
			if('1' == revBuf[0])fwrite(revBuf+1, 1, byte, fp);
			else if('2' == revBuf[0])strcpy(filename ,revBuf+1);
		}    
  
        printf("%s\n",revBuf);  
        printf("name : %s\n",filename);
  
        ReleaseMutex(hMutex);
        memset((void *)revBuf, '\0', MaxSize);

    }
    printf("a client link out.\n");
    closesocket(sockClient);//�ر�socket,һ��ͨ����� 
   	fclose(fp); 
    if(strlen(filename) > 0)rename("tmp.txt",filename);
} 

//------------------------------------------------------------------------------------  
//��������:      sockinit(void)  
/*��������:      windows socket ��ʼ�� 
/*��ڲ���:      void                
//���ڲ���:      
//ȫ�ֱ�������:   
//����ģ��:      ��  
//------------------------------------------------------------------------------------*/
int sockinit(void)
{
	DWORD version 		= MAKEWORD(2,2);
	WSADATA	wsdata;
	return WSAStartup(version,&wsdata);
}


//------------------------------------------------------------------------------------  
//��������:      sockclear(void)  
/*��������:      windows socket ����
/*��ڲ���:      void                
//���ڲ���:      
//ȫ�ֱ�������:   
//����ģ��:      ��  
//------------------------------------------------------------------------------------*/
void sockclear(void)
{
	WSACleanup();
}

#endif


