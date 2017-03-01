#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "be_daemon.h"

static const char *LOG_FILE = "/tmp/daemon.log";

int main(int argc, char *argv[])
{
	int opt, i, j = 0;
	int logfd;
	char* logfile, *command;
	char *argvec[10];

	while((opt = getopt(argc, argv, "l:")) != -1)
	{
		switch(opt)
		{
		case 'l': logfile = optarg; break;
		case ':': fprintf(stderr, "Usage: %s [-l logfile] command.(Missing argument)\n", argv[0]); exit(1);
		case '?': fprintf(stderr, "Usage: %s [-l logfile] command.(Unreconized option)\n", argv[0]); exit(1);
		default : fprintf(stderr, "Usage: %s [-l logfile] command.\n", argv[0]); exit(1);
		}
	}

	if(optind >= argc) 
	{
		fprintf(stderr, "Usage: %s [-l logfile] command.(Missing command)\n", argv[0]); 
		exit(1);
	}

	command = argv[optind];
	argvec[0] = command;
	for(i = optind+1; i < argc; ++i) 
	{
		argvec[++j] = argv[i];
	}

	if(seteuid(0) == -1) 
	{
		fprintf(stderr, "Can't become root\n"); 
		exit(1);
	}

	if(!logfile) 
	{
		becomeDaemon(0);
	}
	else
	{ 
		becomeDaemon(DONT_REOPEN_STD_FDS | DONT_CHDIR);

		close(STDIN_FILENO);

		logfd = open(logfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		if(dup2(logfd, STDOUT_FILENO) != STDOUT_FILENO) 
			exit(1);
		if(dup2(logfd, STDERR_FILENO) != STDERR_FILENO)
			exit(1);
	}

	execvp(command, argvec);

	exit(0);
}
