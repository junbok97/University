/*
���ϸ� : file_server2.c
��  �� : file �� �����ؼ� �����ϴ� ���� file_server2���� filename�� ���� ���� �޴´�.
���� : file_server2 [port]
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

	// winsock ����� ���� �ʼ�����
	signal(SIGINT, exit_callback);
	sversion = MAKEWORD(1, 1);
	WSAStartup(sversion, &wsadata);
}
#endif

#define BUF_LEN 128
#define file_SERVER "0.0.0.0"
#define file_PORT "30000"

int main(int argc, char* argv[]) {
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;			/* ���Ϲ�ȣ */
	int len, msg_size;
	char buf[BUF_LEN + 1];
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
	/* ���� ���� */
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

	/* server_addr�� '\0'���� �ʱ�ȭ */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	/* server_addr ���� */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(port_no));

	/* bind() ȣ�� */
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Server: Can't bind local address.\n");
		exit(0);
	}

	/* ������ ���� ������ ���� */
	listen(server_fd, 5);

	/* iterative  file ���� ���� */
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

		while (1) {
			msg_size = recv(client_fd, buf, BUF_LEN, 0);
			if (msg_size <= 0) {
				printf("recv error\n");
				break;
			}
			buf[msg_size] = '\0'; // ���ڿ� ���� NULL�� �߰��ϱ� ����
			printf("Received len=%d : %s", msg_size, buf);
			// ���� exit�̸� ����
			if (strcmp(buf, "exit\n") == 0)
				break;
			msg_size = send(client_fd, buf, msg_size, 0);
			if (msg_size <= 0) {
				printf("send error\n");
				break;
			}
			printf("Sending len=%d : %s", msg_size, buf);
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
