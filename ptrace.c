#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	pid_t child;
	long orig_rax, rax;
	int status, result, option;
	struct user_regs_struct regs;

	child = fork();
	if (child == 0){
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		//execl("~/lab/prefetch/mmap", "mmap", NULL);
		//execl("/usr/games/gnome-sudoku", "gnome-sudoku", NULL);
		execl("/usr/bin/firefox", "firefox", NULL);
		//execl("./test", "test", NULL);
	} else {
		ptrace(PTRACE_SYSCALL, child, 0, 0);

		while (waitpid(child, &status, 0) != -1) {
			option = ptrace(PTRACE_SETOPTIONS, child, 0,
					PTRACE_O_TRACEMMAP);
			if (option != 0) 
				printf("setoptions error\n");
			result = ptrace(PTRACE_GETREGS, child, 0, &regs);

			if (regs.rax != -1) {
				printf("syscall: %3lld\n", regs.orig_rax);
			}
			ptrace(PTRACE_SYSCALL, child, 0, 0);
		}

		/*while(1){
			wait(&status);
			if(WIFEXITED(status))
				break;

			int r = ptrace(PTRACE_SETOPTIONS, child, 0,
					PTRACE_O_TRACEMMAP);
			if (r != 0)
				printf("setoptions error\n");
			ptrace(PTRACE_GETREGS, child, NULL, &regs);
			orig_rax = ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL);
			
			if (orig_rax != -1) {
				printf("syscall activate %ld\n", orig_rax);
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}*/
	}

	printf("End Ptrace\n");
	return 0;
	
}
