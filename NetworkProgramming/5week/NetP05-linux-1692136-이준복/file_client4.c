/*
 파일명 : file_client3.c
 기  능 : file_client2에서 filename + filesize 전송
 컴파일 : cc -o file_client3 file_client3.c
 사용법 : file_client3 [host IP] [port]
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

#define ECHO_SERVER "127.0.0.1"
#define ECHO_PORT "40000"
#define BUF_LEN 128

// dir
void dir(int s) {
	
	char buf[BUF_LEN + 1] = { 0 };

	while (1) {
		memset(buf, 0, BUF_LEN + 1);
		if (recv(s, buf, BUF_LEN, 0) <= 0) {
			printf("filesize recv error\n");
			return;
		}
		if (strncmp(buf, "-EOF-", 5) == 0)
			break;
		printf("%s", buf);
	}
	printf("\n");
}

void ldir() {
#ifdef WIN32
	system("dir");
#else
	system("ls -l");
#endif
}

// put
void put(char* filename, int s) {
	FILE* fp;
	char buf[BUF_LEN + 1] = { 0 };
	int filesize, readsum , nread, n;
	
	if ((fp = fopen(filename, "rb")) == NULL) {
		printf("Can't open file %s\n", filename);
		exit(0);
	}

	fseek(fp, 0, 2);
	filesize = ftell(fp);
	rewind(fp);

	sprintf(buf, "%s %d", filename, filesize);
	if (send(s, buf, BUF_LEN, 0) <= 0) {
		printf("filename and filesize send error\n");
		exit(0);
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
		if (send(s, buf, n, 0) <= 0) {
			printf("send error\n");
			break;
		}
		readsum += n;
		if ((nread = (filesize - readsum)) > BUF_LEN)
			nread = BUF_LEN;
	}
	fclose(fp);
	printf("FILE %s %dbytes transferred\n", filename, filesize);

}

// get
void get(char* filename, int s) {

	FILE* fp;
	char buf[BUF_LEN + 1] = { 0 };
	int filesize, readsum = 0, nread = 0, n;

	// send filename to server
	sprintf(buf, "%s", filename);
	if (send(s, buf, BUF_LEN, 0) <= 0) {
		printf("filename send error\n");
		exit(0);
	}

	// get filesize from server
	if (recv(s, buf, BUF_LEN, 0) <= 0) {
		printf("filesize recv error\n");
		exit(0);
	}

	sscanf(buf, "%d", &filesize);	
	printf("Receiving %s %dbytes.\n", filename, filesize);



	if ((fp = fopen(filename, "wb")) == NULL) {
		printf("file open error\n");
		exit(0);
	}

	readsum = 0;
	if (filesize < BUF_LEN)
		nread = filesize;
	else
		nread = BUF_LEN;
	while (readsum < filesize) {

		memset(buf, 0, BUF_LEN + 1);
		n = recv(s, buf, nread, 0);
		
		if (n <= 0)
			break;

		if (fwrite(buf, n, 1, fp) <= 0) {
			printf("fwrite error\n");
			break;
		}
		readsum += n;
		// 남은 filesize만큼
		if ((nread = (filesize - readsum)) > BUF_LEN)
			nread = BUF_LEN;
	}
	fclose(fp);
	printf("FILE %s %dbytes received.\n", filename, filesize);

}

int main(int argc, char* argv[]) {
	int s, n, len_in, len_out;
	struct sockaddr_in server_addr;
	char* ip_addr = ECHO_SERVER, * port_no = ECHO_PORT;
	char buf[BUF_LEN + 1] = { 0 };
	char mode[BUF_LEN + 1] = { 0 };
	char filename[BUF_LEN + 1] = { 0 };

	if (argc == 3) {
		ip_addr = argv[1];
		port_no = argv[2];
	}
#ifdef WIN32
	printf("Windows : ");
	init_winsock();
#else // Linux
	printf("Linux : ");
#endif 

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
#ifdef WIN32
	main_socket = s;
#endif 

	/* echo 서버의 소켓주소 구조체 작성 */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	server_addr.sin_port = htons(atoi(port_no));

	/* 연결요청 */
	printf("Connecting %s %s\n", ip_addr, port_no);

	if (connect(s, (struct sockaddr*)&server_addr,
		sizeof(server_addr)) < 0) {
		printf("can't connect.\n");
		exit(0);
	}
	

	while (1) {

		printf("\nfile_client4> ");
		// 명령어입력
		fgets(buf, BUF_LEN, stdin);

		// 모드 체크
		memset(mode, 0, BUF_LEN + 1);
		sscanf(buf, "%s", mode);

		if (mode[0] == '!') {
			system(mode + 1);
		}
		else if (strcmp(mode, "ldir") == 0) {
			ldir();
		}
		else {
			
			// 서버로 buf 전송
			if (send(s, buf, BUF_LEN, 0) <= 0) {
				printf("send error\n");
				exit(0);
			}


			if (strcmp(mode, "put") == 0) {
				memset(filename, 0, BUF_LEN + 1);
				sscanf(buf, "%s %s", mode, filename);
				put(filename, s);
			}
			else if (strcmp(mode, "get") == 0) {
				memset(filename, 0, BUF_LEN + 1);
				sscanf(buf, "%s %s", mode, filename);
				get(filename, s);
			}
			else if (strcmp(mode, "quit") == 0) {
				printf("Client end.\n");
				break;
			}
			else
				dir(s);
		}

		
		
	}
#ifdef WIN32
	closesocket(s);
#else
	close(s);
#endif
	return(0);
}