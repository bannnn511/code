
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main() {
    struct sockaddr_in sa; // IPv4
    struct sockaddr_in6 sa6; // IPv6

    inet_pton(AF_INET, "10.12.110.57", &sa.sin_addr);
    inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &sa6.sin6_addr);

    // IPv4
    char ip4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
    printf("ipv4: %s\n", ip4);

    // IPv6
    char ip6[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
    printf("ipv6: %s\n", ip6);


    return 0;
}
