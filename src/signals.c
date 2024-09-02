#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "job_store.h"

void handle_sigchld(int sig)
{
	(void)sig; // suppress warning
	int status;
	pid_t pid;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		struct job *j = get_job(pid);
		if (j)
		{
			printf("\nCommand %s with PID %d exited with status %d\n", j->user_command, pid, status);
		}
		else
			printf("\nChild with PID %d exited with status %d\n", pid, status); // debugging only
		remove_job_from_store(pid);
	}
}

void handle_sigtstp(int sig)
{
	(void)sig; // suppress warning
	printf("\nCtrl-Z pressed\n");

	pid_t pgid = getpgrp();
	printf("Stopping process group %d\n", pgid);
	if (killpg(pgid, SIGSTOP) < 0)
		perror("killpg (SIGSTOP)");
}
