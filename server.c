#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

void prepare_server_address(struct sockaddr_in *server_addr) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);
}

int sum(int x, int y) {
    return x + y ;
}

int main() {
    int srvfd, clfd, adlen, valread;
    struct sockaddr_in server_addr, cladd;
    char buff[1024] = {0};
    int numbers[2] = {0};
    srvfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    int level = SOL_SOCKET;
    int option_name = SO_REUSEADDR | SO_REUSEPORT;
    void *option_value = &opt;
    socklen_t option_len = sizeof(opt);
    setsockopt(srvfd, level, option_name, option_value, option_len);

    prepare_server_address(&server_addr);

    struct sockaddr *bsckr = (struct sockaddr *)&server_addr;
    socklen_t bsckadd = sizeof(server_addr);
    bind(srvfd, bsckr, bsckadd);

    listen(srvfd, 2);

    adlen = sizeof(cladd);
    int count = 0;
    while (count < 2) {
        struct sockaddr *cltsor = (struct sockaddr *)&cladd;
        socklen_t *clptr = (socklen_t *)&adlen;
        clfd = accept(srvfd, cltsor, clptr);

        valread = read(clfd, buff, 1024);
        numbers[count] = atoi(buff);
        close(clfd);
        count = count + 1;
    }

    printf("Sum of two numbers: %d\n", sum(numbers[0], numbers[1]));
//
    return 0;
}
