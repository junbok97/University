/*
파일명 : file_server1.c
기  능 : echo_server1 와 동일
컴파일 : cc -o file_server file_server.c
사용법 : file_server1 [port]
*/
#ifdef WIN32
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#else
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef WIN32
WSADATA wsadata;
int	main_socket;

void exit_callback(int sig)
{
	closesocket(main_socket);
	WSACleanup();
	exit(0);
}

void init_winsock()
{
	WORD sversion;
	u_long iMode = 1;

	// winsock 사용을 위해 필수적임
	signal(SIGINT, exit_callback);
	sversion = MAKEWORD(1, 1);
	WSAStartup(sversion, &wsadata);
}
#endif

#define BUF_LEN 128
#define file_SERVER "0.0.0.0"
#define file_PORT "40000"

void dir(int client_fd, char *mode) {
	FILE* fp;
	char buf[BUF_LEN + 1] = { 0 };
	int n;

#ifdef WIN32
	fp = _popen(mode, "r");
#else
	if (strcmp(mode, "dir") == 0)
		fp = popen("ls -l", "r");
	else
		fp = popen(mode, "r");
#endif

	while (1) {
		memset(buf, 0, BUF_LEN + 1);
		n = fread(buf, 1, BUF_LEN, fp);
		if (n <= 0)
			break;

		if (send(client_fd, buf, BUF_LEN, 0) <= 0) {
			printf("filesize send error\n");
			return;
		}
	}

	strcpy(buf, "-EOF-");
	if (send(client_fd, buf, BUF_LEN, 0) <= 0) {
		printf("filesize send error\n");
		return;
	}
#ifdef WIN32
	_pclose(fp);
#else
	pclose(fp);
#endif
}

void put(int client_fd) {
	FILE* fp;
	char buf[BUF_LEN + 1];
	char filename[BUF_LEN + 1];
	int filesize, readsum , nread , n;
	
	if (recv(client_fd, buf, BUF_LEN, 0) <= 0) {
		printf("filename and filesize recv error\n");
		return;
	}

	sscanf(buf, "%s %d", filename, &filesize);
	printf("Receving %s %dbytes.\n", filename, filesize);

	if ((fp = fopen(filename, "wb")) == NULL) {
		printf("file open error\n");
		return;
	}

	readsum = 0;
	if (filesize < BUF_LEN)
		nread = filesize;
	else
		nread = BUF_LEN;
	while (readsum < filesize) {
		memset(buf, 0, BUF_LEN + 1);
		n = recv(client_fd, buf, nread, 0);
		if (n <= 0)
			break;
		if (fwrite(buf, n, 1, fp) <= 0) {
			printf("fwrite error\n");
			break;
		}
		readsum += n;
		if ((nread = (filesize - readsum)) > BUF_LEN)
			nread = BUF_LEN;
	}
	fclose(fp);
	printf("Recevied %s %dbytes.\n", filename, filesize);
}

void get(int client_fd) {

	FILE* fp;
	char buf[BUF_LEN + 1] = { 0 };
	char filename[BUF_LEN + 1] = { 0 };
	int filesize, readsum, nread, n;

	// get filename from client
	if (recv(client_fd, buf, BUF_LEN, 0) <= 0) {
		printf("filename recv error\n");
		return;
	}
	
	sscanf(buf, "%s", filename);

	if ((fp = fopen(filename, "rb")) == NULL) {
		printf("Can't open file %s\n", filename);
		return;
	}

	fseek(fp, 0, 2);
	filesize = ftell(fp);
	rewind(fp);

	// send filesize to client
	sprintf(buf, "%d", filesize);
	if (send(client_fd, buf, BUF_LEN, 0) <= 0) {
		printf("filesize send error\n");
		return;
	}

	printf("Sending %s %dbytes.\n", filename, filesize);

	readsum = 0;
	if (filesize < BUF_LEN)
		nread = filesize;
	else
		nread = BUF_LEN;
	while (readsum < filesize) {
		
		memset(buf, 0, BUF_LEN + 1);
		n = fread(buf, 1, nread, fp);
		
		if (n <= 0)
			break;
		
		if (send(client_fd, buf, n, 0) <= 0) {
			printf("send error\n");
			break;
		}
		readsum += n;

		if ((nread = (filesize - readsum)) > BUF_LEN)
			nread = BUF_LEN;
	}	
	fclose(fp);
	printf("FILE %s %dbytes sent\n", filename, filesize);

}

int main(int argc, char* argv[]) {
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;			/* 소켓번호 */
	int len, msg_size;
	char buf[BUF_LEN + 1];
	char mode[BUF_LEN + 1];
	unsigned int set = 1;
	char* ip_addr = file_SERVER, * port_no = file_PORT;


	if (argc == 2) {
		port_no = argv[1];
	}
#ifdef WIN32
	printf("Windows : ");
	init_winsock();
#else
	printf("Linux : ");
#endif
	/* 소켓 생성 */
	if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Server: Can't open stream socket.");
		exit(0);
	}
#ifdef WIN32
	main_socket = server_fd;
#endif

	printf("file_server1 waiting connection..\n");
	printf("server_fd = %d\n", server_fd);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set, sizeof(set));

	/* server_addr을 '\0'으로 초기화 */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	/* server_addr 세팅 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(port_no));

	/* bind() 호출 */
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Server: Can't bind local address.\n");
		exit(0);
	}

	/* 소켓을 수동 대기모드로 세팅 */
	listen(server_fd, 5);

	/* iterative  file 서비스 수행 */
	printf("Server : waiting connection request.\n");
	len = sizeof(client_addr);

	while (1) {
		/* 연결요청을 기다림 */
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
		if (client_fd < 0) {
			printf("Server: accept failed.\n");
			exit(0);
		}

		printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		printf("client_fd = %d\n", client_fd);

		while (1) {
			
			printf("\nWaiting client command\n");
			if (recv(client_fd, buf, BUF_LEN, 0) <= 0) {
				printf("recv error\n");
				break;
			}

			printf("Received %d %s", BUF_LEN, buf);
			
			// 모드 체크
			memset(mode, 0, BUF_LEN + 1);
			sscanf(buf, "%s", mode);

			if (strcmp(mode, "put") == 0) {
				put(client_fd);
			}
			else if (strcmp(mode, "get") == 0) {
				get(client_fd);
			}
			else if (strcmp(mode, "quit") == 0) {
				printf("Client end\n");
				break;
			}
			else
				dir(client_fd, mode);
		
		}
#ifdef WIN32
		closesocket(client_fd);
#else
		close(client_fd);
#endif

	}
#ifdef WIN32
	closesocket(server_fd);
#else
	close(server_fd);
#endif	
	return(0);
}

