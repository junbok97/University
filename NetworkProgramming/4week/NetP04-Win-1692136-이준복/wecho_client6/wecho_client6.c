/*
 파일명 : echo_client.c
 기  능 : echo 서비스를 요구하는 TCP(연결형) 클라이언트
 컴파일 : cc -o echo_client echo_client.c
 사용법 : echo_client [host] [port]
*/
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

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

int menu_scr() {
	int menu;
	do {
		printf("*** 대/소문자 변환 메뉴입니다. ***\n");
		printf("(1) 모두 대문자 변환\n");
		printf("(2) 모두 소문자 변환\n");
		printf("(3) 대문자->소문자 소문자->대문자 변환\n");
		printf("(4) 종료\n");
		printf("선택하세요 : ");
		scanf("%d", &menu);
		rewind(stdin);
	} while (menu < 1 || menu > 4);
	return menu;
}

void input_id_pass(char* id, char* pass) {
	printf("ID : ");
	scanf("%s", id);
	printf("Password : ");
	scanf("%s", pass);
	rewind(stdin);
}

#define ECHO_SERVER "127.0.0.1"
#define ECHO_PORT "40000"
#define BUF_LEN 128
#define LOGIN_OK "200"
#define LOGIN_INVALID_ID "401"
#define LOGIN_INVALID_PASS "402"

int main(int argc, char* argv[]) {
	int s, n, len_in, len_out, select;
	struct sockaddr_in server_addr;
	char* ip_addr = ECHO_SERVER, * port_no = ECHO_PORT;
	char buf[BUF_LEN + 1] = { 0 };
	char buf2[BUF_LEN + 1] = { 0 };
	char id[BUF_LEN + 1] = { 0 };
	char pass[BUF_LEN + 1] = { 0 };

	if (argc == 3) {
		ip_addr = argv[1];
		port_no = argv[2];
	}

	init_winsock();

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
	main_socket = s;


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

	// Welcome message 수신
	if ((n = recv(s, buf, BUF_LEN, 0)) <= 0) {
		printf("read error\n");
		exit(0);
	}
	printf("Received %d bytes : %s\n", n, buf);

	while (1) {
		char id[BUF_LEN], pass[BUF_LEN];
		input_id_pass(id, pass);
		sprintf(buf2, "%s %s", id, pass);
		
		if (send(s, buf2, BUF_LEN, 0) <= 0) {
			printf("send error\n");
			exit(0);
		}
		if ((n = recv(s, buf, 256, 0)) <= 0) {
			printf("recv error\n");
			exit(0);
		}
		printf("%d\n", n);
		if (strncmp(buf, LOGIN_OK, 3) == 0) {
			printf("%s\n", &buf[4]);
			break;
		}
		else {
			printf("%s\n", buf);
		}
	}

	while (1) {
		int menu;
		menu = menu_scr();
		/* 키보드 입력을 받음 */
		if (menu != 4) {
			memset(buf, '\0', BUF_LEN);
			printf("Input string : ");
			if (fgets(buf, BUF_LEN, stdin)) {
				len_out = BUF_LEN;
			}
			else {
				printf("gets error\n");
				exit(0);
			}
			sprintf(buf2, "%d %s", menu, buf);
		}
		else
			sprintf(buf2, "%d\n", menu);
		
		/* echo 서버로 메시지 송신 */
		if (send(s, buf2, BUF_LEN, 0) < 0) {
			printf("send error\n");
			exit(0);
		}

		// 4번 선택하면 종료
		if (menu == 4) {
			break;
		}

		if ((n = recv(s, buf, BUF_LEN, 0)) < 0) {
			printf("recv error\n");
			exit(0);
		}
		printf("Received %d bytes : %s\n", n, buf);

	}

	closesocket(s);
	return(0);
}