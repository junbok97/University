#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>



#define READ 0
#define WRITE 1
#define SIZE 1024


char* cmd[SIZE];
char* ptr;
int cmd_index;

void init() {

	// 전역변수 초기화
	for (int i = 0; i < SIZE; i++)
		cmd[i] = NULL;
	cmd_index = 0;
	ptr = NULL;
}

void output_redirect(char* buf) {

	init();
	int fd;

	// 명령인수 쪼개기
	cmd[cmd_index++] = strtok(buf, "> ");
	while (ptr = strtok(NULL, "> ")) {
		cmd[cmd_index++] = ptr;
	}

	// 맨 마지막 index가 > 아웃풋파일이름
	if ((fd = creat(cmd[--cmd_index], 0644)) == -1) {
		printf("file create error\n");
		exit(1);
	}
	dup2(fd, 1);
	close(fd);

	// 파일이름 지우기
	cmd[cmd_index] = NULL;
	execvp(cmd[0], cmd);

}

void input_redirect(char* buf) {

	init();
	int fd;

	// 명령인수 쪼개기 
	cmd[cmd_index++] = strtok(buf, "< ");
	while (ptr = strtok(NULL, "< ")) {
		cmd[cmd_index++] = ptr;
	}

	// 맨 마지막 index가 < 인풋파일이름
	if ((fd = open(cmd[--cmd_index], O_RDONLY)) == -1) {
		printf("file open error\n");
		exit(1);
	}
	dup2(fd, 0);
	close(fd);

	// 파일이름 지우기
	cmd[cmd_index] = NULL;
	execvp(cmd[0], cmd);
}

int main() {

	char buf[SIZE];
	char* command1, * command2;
	int filedes[2], fd;
	int amper, status;
	int parent_pid = getpid();
	int child_pid;



	while (1) {

		// 자식 프로세스가 살아남았을 경우
		if (parent_pid != getpid()) {
			printf("부모프로세스 오류\n");
			exit(1);
		}
			

		amper = 0;
		memset(buf, 0, SIZE);

		//입력받는부분
		printf("[1692136이준복] ");
		fgets(buf, SIZE, stdin);
		buf[strlen(buf) - 1] = '\0';

		if (!strcmp(buf, "exit"))
			exit(0);

		// 후면 처리가 있으면
		if (strchr(buf, '&') != NULL) {
			char* str = strtok(buf, "&");
			char buf2[SIZE];
			memset(buf2, 0, SIZE);
			sprintf(buf2, "%s", str);
			memset(buf, 0, SIZE);
			sprintf(buf, "%s", buf2);
			amper = 1;
		}


		child_pid = fork();
		// 자식 프로세스라면
		if (child_pid == 0) {

			// 출력 재지정을 사용하면
			if (strchr(buf, '>') != NULL) {
				output_redirect(buf);
			}
			// 입력 재지정을 사용하면
			if (strchr(buf, '<') != NULL) {
				input_redirect(buf);
			}

			// 파이프를 사용하면
			if (strchr(buf, '|') != NULL) {
				command1 = strtok(buf, "| ");
				command2 = strtok(NULL, "| ");
				if (pipe(filedes) == -1) { // 파이프 설정
					printf("fail to call pipe()\n");
					continue;
				}
				if (fork() == 0) { // 자식 프로세스 이면
					close(filedes[READ]);
					dup2(filedes[WRITE], 1); // 표준 출력 재지정
					close(filedes[WRITE]);
					execlp(command1, command1, NULL);
					perror("pipe");
				}
				else { // 부모 프로세스 이면
					close(filedes[WRITE]);
					dup2(filedes[READ], 0); // 표준 입력 재지정
					close(filedes[READ]);
					execlp(command2, command2, NULL);
					perror("pipe");
				}
			}
			else {
				// 명령인수 쪼개기
				init();
				cmd[cmd_index++] = strtok(buf, " ");
				while (ptr = strtok(NULL, " ")) {
					cmd[cmd_index++] = ptr;
				}

				// 명령인수 끝 설정
				cmd[cmd_index] = NULL;
				execvp(cmd[0], cmd);

			}

		}
		else { // 부모 프로세스라면
			if (amper == 0) 
				waitpid(child_pid,&status,0);			
		}

	}

}
