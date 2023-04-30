#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080

void prepare_server_address(struct sockaddr_in *server_addr) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);
}

typedef struct {
    int clfd;
    int index;
} cldata;

int numbers[2] = {0};

void *clthr(void *arg) {
    cldata *data = (cldata *)arg;
    int valread;
    char buff[1024] = {0};
    valread = read(data->clfd, buff, 1024);
    numbers[data->index] = atoi(buff);
    close(data->clfd);
    free(arg);
    return NULL;
}

int sum(int x, int y) {
    return x + y;
}

int main() {
    int srvfd, clfd, adlen, valread;
    struct sockaddr_in server_addr, cladd;
    char buff[1024] = {0};
    int count = 0;
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

    pthread_t threads[2];

    while (count < 2) {
        struct sockaddr *cltsor = (struct sockaddr *)&cladd;
        socklen_t *clptr = (socklen_t *)&adlen;
        clfd = accept(srvfd, cltsor, clptr);

        cldata *data = (cldata *)malloc(sizeof(cldata));
        data->clfd = clfd;
        data->index = count;

        pthread_create(&threads[count], NULL, clthr, data);
        count = count + 1;
    }

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("Sum of two numbers: %d\n", sum(numbers[0], numbers[1]));

    return 0;
}
