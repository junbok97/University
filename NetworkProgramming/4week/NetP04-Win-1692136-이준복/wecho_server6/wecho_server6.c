/*
���ϸ� : wecho_server.c
��  �� : echo ���񽺸� �����ϴ� ����
������ : cc -o wecho_server wecho_server.c
���� : wecho_server [port]
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

	// winsock ����� ���� �ʼ�����
	signal(SIGINT, exit_callback);
	sversion = MAKEWORD(1, 1);
	WSAStartup(sversion, &wsadata);
}



#define BUF_LEN 128
#define ECHO_SERVER "0.0.0.0"
#define ECHO_PORT "40000"
#define LOGIN_ID "hansung"
#define LOGIN_PASS "computer"
#define LOGIN_OK "200"
#define LOGIN_INVALID_ID "401 Invalid ID"
#define LOGIN_INVALID_PASS "402 Invalid Password"

int main(int argc, char* argv[]) {
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;			/* ���Ϲ�ȣ */
	int len, msg_size;
	char id[BUF_LEN + 1];
	char pass[BUF_LEN + 1];
	char buf[BUF_LEN + 1];
	char buf2[BUF_LEN + 1];
	unsigned int set = 1;
	char* ip_addr = ECHO_SERVER, * port_no = ECHO_PORT;

	if (argc == 2) {
		port_no = argv[1];
	}

	init_winsock();

	/* ���� ���� */
	if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Server: Can't open stream socket.");
		exit(0);
	}
	main_socket = server_fd;

	printf("echo_server1 waiting connection..\n");
	printf("server_fd = %d\n", server_fd);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set, sizeof(set));

	/* server_addr�� '\0'���� �ʱ�ȭ */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	/* server_addr ���� */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//inet_addr(ip_addr);
	server_addr.sin_port = htons(atoi(port_no));

	/* bind() ȣ�� */
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Server: Can't bind local address.\n");
		exit(0);
	}

	/* ������ ���� ������ ���� */
	listen(server_fd, 5);

	/* iterative  echo ���� ���� */
	printf("Server : waiting connection request.\n");
	len = sizeof(client_addr);

	while (1) {
		/* �����û�� ��ٸ� */
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
		if (client_fd < 0) {
			printf("Server: accept failed.\n");
			exit(0);
		}

		printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		printf("client_fd = %d\n", client_fd);
		send(client_fd, "Weclome to Server!!\n", BUF_LEN, 0);

		// Login ó��
		while (1) {
			msg_size = recv(client_fd, buf, BUF_LEN, 0);
			if (msg_size <= 0) {
				printf("recv error\n");
				break;
			}
			sscanf(buf, "%s %s", id, pass);
			printf("Received id = %s pass = %s\n", id, pass);
			memset(buf2, 0, BUF_LEN);


			if (strcmp(id, LOGIN_ID) != 0) {
				sprintf(buf2, "%s\n", LOGIN_INVALID_ID);
				printf("Sending len = %d : %s\n", BUF_LEN, buf2);
				send(client_fd, buf2, BUF_LEN, 0);
			}
			else if (strcmp(pass, LOGIN_PASS) != 0) {
				sprintf(buf2, "%d %s\n", 402, "Invalid Password");
				printf("Sending len = %d : %s\n", BUF_LEN, buf2);
				send(client_fd, buf2, BUF_LEN, 0);
			}
			else {
				sprintf(buf2, "%s Welocme %s!!\n", LOGIN_OK, id);
				printf("Sending len = %d : %s\n", BUF_LEN, buf2);
				send(client_fd, buf2, BUF_LEN, 0);
				break;
			}
		}

		while (1) {
			msg_size = recv(client_fd, buf, BUF_LEN, 0);
			if (msg_size <= 0) {
				printf("recv error\n");
				break;
			}
			printf("Received len=%d : %s", msg_size, buf);
			memset(buf2, 0, BUF_LEN);
			strncpy(buf2, buf + 2, msg_size - 2);
			char* s = buf2;
			if (buf[0] == '1') {
				while (*s) {
					*s = toupper(*s);
					s++;
				}
			}
			else if (buf[0] == '2') {
				while (*s) {
					*s = tolower(*s);
					s++;
				}
			}
			else if (buf[0] == '3') {
				while (*s) {
					if (islower(*s))
						*s = toupper(*s);
					else
						*s = tolower(*s);
					s++;
				}
			}
			else if (buf[0] == '4') {
				printf("---Session finished for %s---\n", id);
				break;
			}


			msg_size = send(client_fd, buf2, BUF_LEN, 0);
			if (msg_size <= 0) {
				printf("send error\n");
				break;
			}
			printf("Sending len = %d : %s\n", BUF_LEN, buf2);
		}

		closesocket(client_fd); // close(client_fd);
	}
	closesocket(server_fd); // close(client_fd);
	return (0);
}