#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "be_daemon.h"


int becomeDaemon(int flags)
{
    int maxfd, fd, pid;

    switch(fork())
    {
        case -1: 
            return -1;
        case 0:  
            break;
        default: 
            _exit(0);
    }

    if(setsid() == -1) return -1;

    switch(pid=fork())
    {
        case -1: 
            return -1;
        case 0: 
            break;
        default:  
            printf("Daemon PID: %d\n", pid);
            _exit(0);
    }

    if(!(flags & DONT_UMASK0))
    {
        umask(0);
    }

    if(!(flags & DONT_CHDIR))
    {
        chdir("/");
    }

    if(!(flags & DONT_CLOSE_FILES))
    {
        maxfd = sysconf(_SC_OPEN_MAX);
        if(maxfd == -1) maxfd = MAX_CLOSE;

        for(fd = 0; fd < maxfd; ++fd) close(fd);
    }

    if(!(flags & DONT_REOPEN_STD_FDS))
    {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);

        if(fd != STDIN_FILENO) return -1;
        if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) 
            return -1;
        if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}