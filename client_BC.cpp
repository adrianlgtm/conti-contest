#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ctime>

void errno_abort(const char* header)
{
    perror(header);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    struct sockaddr_in send_addr, recv_addr;
    int trueflag = 1, count = 0;
    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        errno_abort("socket");
#ifndef RECV_ONLY

    if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST,&trueflag, sizeof trueflag) < 0)
        errno_abort("setsockopt");

    memset(&send_addr, 0, sizeof send_addr);
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = (in_port_t) htons(5000);
    // broadcasting address for unix (?)
    inet_aton("255.255.255.255", &send_addr.sin_addr);

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                   &trueflag, sizeof trueflag) < 0)
        errno_abort("setsockopt");

    memset(&recv_addr, 0, sizeof recv_addr);
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = (in_port_t) htons(5001);
    recv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    if (bind(fd, (struct sockaddr*) &recv_addr, sizeof recv_addr) < 0)
        errno_abort("bind");

    while ( 1 ) {


    while ( 1 ) {
        char sbuf[256] = {};
        snprintf(sbuf, sizeof(sbuf), "Hello(%d)!", count++);
        if (sendto(fd, sbuf, strlen(sbuf)+1, 0,
                   (struct sockaddr*) &send_addr, sizeof send_addr) < 0)
            errno_abort("send");
        printf("send: %s\n", sbuf);
        usleep(1000000/2);

        char rbuf[256] = {};
        if (recv(fd, rbuf, sizeof(rbuf)-1, 0) < 0)
            errno_abort("recv");
        printf("recv: %s\n", rbuf);
    }
    close(fd);
    return 0;
}