#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 5050
#define BUF_LEN 1024


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    struct sockaddr from_addr;
    socklen_t from_addr_len;
    int result;
    int option;
    int fd;
    char data[BUF_LEN];
    ssize_t nread;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(fd == -1)
    {
        perror("scoket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(addr.sin_addr.s_addr ==  (in_addr_t)-1)
    {
        perror("inet_addr");
        exit(EXIT_FAILURE);
    }

    option = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    result = bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    if(result == -1)
    {
        close(fd);
        perror("bind");
        exit(EXIT_FAILURE);
    }

    nread = recvfrom(fd, data, BUF_LEN, 0, &from_addr, &from_addr_len);

    if(nread == -1)
    {
        close(fd);
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    printf("read %ld\n", nread);
    write(STDOUT_FILENO, data, nread);
    close(fd);

    return EXIT_SUCCESS;
}
#pragma GCC diagnostic pop
