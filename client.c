#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

void prepare_server_address(struct sockaddr_in *server_addr) {
    sa_family_t family = AF_INET;
    in_addr_t addr = INADDR_ANY;
    in_port_t port = htons(PORT);

    server_addr->sin_family = family;
    server_addr->sin_addr.s_addr = addr;
    server_addr->sin_port = port;
}

int get_input_from_user() {
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);
    return number;
}

int main() {
    int clmn;
    char bff[1024] = {0};
    int number;
    struct sockaddr_in seradd;

    clmn = socket(AF_INET, SOCK_STREAM, 0);

    prepare_server_address(&seradd);

    struct sockaddr *server_sockaddr = (struct sockaddr *)&seradd;
    socklen_t selen = sizeof(seradd);
    connect(clmn, server_sockaddr, selen);

    number = get_input_from_user();
    snprintf(bff, sizeof(bff), "%d", number);

    int sockfd = clmn;
    void *buf = bff;
    size_t len = strlen(bff);
    int flags = 0;
    send(sockfd, buf, len, flags);

    close(clmn);

    return 0;
}
