#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

int sock;

int main(int argc, char *argv[]) {
    struct hostent *he;
    struct sockaddr_in sin;
    char packet[1024];
    char *address;
    int port;
    int i;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <ip address> <port>\n", argv[0]);
        return (-1);
    }

    address = argv[1];
    port = atoi(argv[2]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    he = gethostbyname(address);
    fprintf(stderr, "Attempting a connection with %s on port %d\n", address, port);

    if (!he) {
        if ((sin.sin_addr.s_addr = inet_addr(address)) == -1)
            return (-1);
    } else {
        bcopy(he->h_addr, (struct in_addr *) &sin.sin_addr, he->h_length);
    }

    if (connect(sock, (struct sockaddr *) &sin, sizeof
            (sin)) < 0) {
        fprintf(stderr, "Connection refused by remote host.\n");
        return (-1);
    }

    sprintf(packet, "cafe");

    write(sock, packet, strlen(packet));
    close(sock);
    fprintf(stderr, "Operation Completed. Exiting...");
}