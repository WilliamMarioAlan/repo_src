/* select_epoll_demo.c
 对比 select() 与 epoll() 的 TCP 多连接服务器简例
gcc -o server select_epoll_demo.c
./server           # 启动 select 版本
./server epoll     # 启动 epoll 版本

可以通过多开几个 telnet localhost 8888 测试连接效果，
在连接数增多时，select() 会变慢，而 epoll() 基本无变化
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/select.h>

#define PORT 8888
#define MAX_CLIENTS 1024
#define MAX_EVENTS 100

// 设置非阻塞
int set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void select_server(int listenfd) {
    fd_set readfds;
    int clients[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i] = -1;

    printf("[SELECT] Listening on port %d\n", PORT);
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(listenfd, &readfds);
        int maxfd = listenfd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != -1) {
                FD_SET(clients[i], &readfds);
                if (clients[i] > maxfd) maxfd = clients[i];
            }
        }

        int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (ready < 0) { perror("select"); exit(1); }

        if (FD_ISSET(listenfd, &readfds)) {
            int connfd = accept(listenfd, NULL, NULL);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i] == -1) { clients[i] = connfd; break; }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != -1 && FD_ISSET(clients[i], &readfds)) {
                char buf[1024];
                int n = read(clients[i], buf, sizeof(buf));
                if (n <= 0) {
                    close(clients[i]);
                    clients[i] = -1;
                } else {
                    write(clients[i], buf, n);
                }
            }
        }
    }
}

void epoll_server(int listenfd) {
    int epfd = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    printf("[EPOLL] Listening on port %d\n", PORT);
    while (1) {
        int nfd = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfd; i++) {
            if (events[i].data.fd == listenfd) {
                int connfd = accept(listenfd, NULL, NULL);
                set_nonblock(connfd);
                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            } else {
                char buf[1024];
                int n = read(events[i].data.fd, buf, sizeof(buf));
                if (n <= 0) {
                    close(events[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                } else {
                    write(events[i].data.fd, buf, n);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(PORT)
    };

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, 128);

    if (argc > 1 && strcmp(argv[1], "epoll") == 0)
        epoll_server(listenfd);
    else
        select_server(listenfd);

    return 0;
}

