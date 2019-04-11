#ifdef __SNET__
#include <conio.h>
#include <spectrum.h>
#include <sys/socket.h>
#include <sockpoll.h>
#include <netdb.h>
#include <string.h>
#include "snapCterm_Common.h"

uint_fast8_t io_initialized=0;

int sockfd, pfd, host_port;
struct sockaddr_in remoteaddr;
struct hostent *he;
char host_name[64];

void CommsInit(void)
{
    strcpy(host_name,"amstrad.simulant.uk");    //  will need to be removed menu option
    host_port = 464;                            //  will need to be removed menu option
    he=gethostbyname(host_name);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    remoteaddr.sin_port=htons(host_port);
    remoteaddr.sin_addr.s_addr=he->h_addr;
    connect(sockfd,&remoteaddr,sizeof(struct sockaddr_in));    
    io_initialized=1;
}

void RX(void)
{
    *RXAttr = PAPER_RED;      //Indicate RX started    
    pfd=poll_fd(sockfd);
    if (pfd & POLLIN)
    {
        *RXAttr = PAPER_RED + BRIGHT;
        rxbytes=recv(sockfd,rxdata,rxdata_Size,0);
    }
    *KBAttr = PAPER_BLACK;
}

void TX(void)
{
    if (io_initialized==1)
    {    
        *TXAttr = PAPER_GREEN;
        txbyte_count = 0;
        do
        {
            send(sockfd,&txdata[txbyte_count],sizeof(unsigned char), 0);
        }while(++txbyte_count<txbytes);
        txbytes = 0;
        txdata[0] = NULL;
        *TXAttr = PAPER_BLACK;    
    }
}

#endif