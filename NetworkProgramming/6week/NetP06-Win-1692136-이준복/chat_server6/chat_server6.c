/*----------------------
 ���ϸ� : chat_server4.c
 ��  �� : ä�ü���, chat_server3 + /to, /sleep, /wakeup, ��/���� �˸�
 �������� 1 : 1:1 ��ȭ ��� /with
 �������� 2 : 1:1 �������� /filesend
 ���� : chat_server4 [port]
------------------------*/
#ifdef WIN32
#define  _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:6054)
#pragma warning(disable:6031)
#pragma warning(disable:6385)

#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
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

#define MAXCLIENTS 64		// �ִ� ä�� ������ ��
#define EXIT	"exit"		// ä�� ���� ���ڿ�
int maxfdp;              	// select() ���� �����ؾ��� # of socket ���� getmax() return �� + 1
int getmax(int);			// �ִ� ���Ϲ�ȣ ���
int num_chat = 0;         	// ä�� ������ ��
int client_fds[MAXCLIENTS];	// ä�ÿ� ������ ���Ϲ�ȣ ���
void RemoveClient(int);		// ä�� Ż�� ó�� �Լ�
void Wisper(char* buf, int n); // �ӼӸ� �Լ�
void put(char* filename, int to, int from); // �������� �Լ�

#define BUF_LEN	128
#define CHAT_SERVER "0.0.0.0"
#define CHAT_PORT "40000"
char userlist[MAXCLIENTS][BUF_LEN]; // user name ������
int usersleep[MAXCLIENTS] = { 0, }; // sleep �������� ���� ����
int userwith[MAXCLIENTS] = { 0, }; // 1:1 ��ȭ ��� ����

// chat_client/server3
#define CHAT_CMD_LOGIN		"/login"// connect�ϸ� user name ���� "/login user1"
#define CHAT_CMD_LIST		"/list"// userlist ��û
#define CHAT_CMD_EXIT		"/exit"// ����
// chat_client/server4 
#define CHAT_CMD_TO			"/to"// �ӼӸ� "/to user2 Hi there.."
#define CHAT_CMD_SLEEP		"/sleep"// �����(������) ����
#define CHAT_CMD_WAKEUP		"/wakeup"// wakeup �Ǵ� message �����ϸ� �ڵ� wakeup
// ���� ������ ���� ��ɾ��
// ����1 : 1:1 ��ȭ
#define CHAT_CMD_WITH		"/with"// [user1] /with user2, user1�� user2�� 1:1 ��ȭ ��û
#define CHAT_CMD_WITH_YES	"y"// 1:1 ��ȭ ��� [user2] /withyes user1
#define CHAT_CMD_WITH_NO	"n"// 1:1 ��ȭ �ź� [user2] /withno user1
#define CHAT_CMD_WITH_END	"/end"// 1:1 ä�� ���� [user1] /end or [user2] /end
// ����2 : 1:1 ���� ����
#define CHAT_CMD_FILESEND	"/filesend"// [user1] /filesend user2 data.txt ���� ���� ��û
#define CHAT_CMD_FILE_YES	"/fileyes"// [user2] /fileyes user1 ���� ���� ���
#define CHAT_CMD_FILE_NO	"/fileno"// [user2] /fileno user1 ���� ���� �ź�
#define CHAT_CMD_FILE_NAME	"/filename"// [user1] /filename data.txt 765 ���� ���� ����
#define CHAT_CMD_FILE_DATA	"/filedata"// [user1] /filedata data...
#define CHAT_CMD_FILE_END	"/fileend"// [user1] /fileend �������� ��

int main(int argc, char* argv[]) {
	char buf[BUF_LEN], buf1[BUF_LEN], buf2[BUF_LEN], buf3[BUF_LEN], buf4[BUF_LEN], buf5[BUF_LEN];
	int i, j, k, n, ret;
	int server_fd, client_fd, client_len;
	unsigned int set = 1;
	char* ip_addr = CHAT_SERVER, * port_no = CHAT_PORT;
	fd_set  read_fds;     // �б⸦ ������ ���Ϲ�ȣ ����ü server_fd �� client_fds[] �� �����Ѵ�.
	struct sockaddr_in  client_addr, server_addr;
	int client_error[MAXCLIENTS];

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

	printf("chat_server5 waiting connection..\n");
	printf("server_fd = %d\n", server_fd);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set, sizeof(set));

	/* server_addr�� '\0'���� �ʱ�ȭ */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	/* server_addr ���� */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(port_no));

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Server: Can't bind local address.\n");
		exit(0);
	}
	/* Ŭ���̾�Ʈ�κ��� �����û�� ��ٸ� */
	listen(server_fd, 5);

	while (1) {
		FD_ZERO(&read_fds); // ���� �ʱ�ȭ
		FD_SET(server_fd, &read_fds); // accept() ��� ���� ����
		for (i = 0; i < num_chat; i++) // ä�ÿ� �������� ��� client ������ reac() ��� �߰�
			FD_SET(client_fds[i], &read_fds);
		maxfdp = getmax(server_fd) + 1;     // ���ô�� ������ ���� ���
		if (select(maxfdp, &read_fds, (fd_set*)0, (fd_set*)0, (struct timeval*)0) <= 0) {
			printf("select error <= 0 \n");
			exit(0);
		}
		// �ʱ� ���� ��, server_fd �� ��ȭ�� �ִ��� �˻�
		if (FD_ISSET(server_fd, &read_fds)) {
			// ��ȭ�� �ִ� --> client �� connect�� ���� ��û�� �� ��
			client_len = sizeof(client_addr);
			client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
			if (client_fd == -1) {
				printf("accept error\n");
			}
			else {
				printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr),
					ntohs(client_addr.sin_port));
				printf("client_fd = %d\n", client_fd);
				/* ä�� Ŭ���̾�Ʈ ��Ͽ� �߰� */
				printf("client[%d] ����. ���� ������ �� = %d\n", num_chat, num_chat + 1);
				client_fds[num_chat++] = client_fd;
			}
		}

		memset(client_error, 0, sizeof(client_error));
		/* Ŭ���̾�Ʈ�� ���� �޽����� ��� Ŭ���̾�Ʈ���� ��� */
		for (i = 0; i < num_chat; i++) {
			// ������ client���� I/O ��ȭ�� �ִ���.
			if (FD_ISSET(client_fds[i], &read_fds)) {
				// Read One �Ǵ� client ������ ���� Ȯ��
				memset(buf, 0, BUF_LEN);
				memset(buf1, 0, BUF_LEN);
				memset(buf2, 0, BUF_LEN);
				memset(buf3, 0, BUF_LEN);
				memset(buf4, 0, BUF_LEN);

				if ((n = recv(client_fds[i], buf, BUF_LEN, 0)) <= 0) {
					// client �� �� ���� ������ ���
					printf("recv error for client[%d]\n", i);
					client_error[i] = 1;
					continue;
				}
				printf("received %d from client[%d] : %s", n, i, buf);
				// "/login username" --> buf1 = /login, buf2 = username
				// "[username] /list" --> buf1 = [username], buf2 = /list
				// "[username] message .." buf1 = [username], buf2 = message ...
				// chat_client/server4 �߰� �κ�
				// "[username] /to user2 message .." buf1 = [username], buf2 = /to, buf3 = user2 , buf4 = message
				// "[username] /sleep  --> buf1 = [username], buf2 = /sleep
				// "[username] /wakeup  --> buf1 = [username], buf2 = /wakeup
				// "[username] /with user2" buf1 = [username], buf2 = /with, buf3 = user2
				// "[username] /filesend user2 filename" buf1 = [username], buf2 = /filesend, buf3 = user2, buf4 = filename

				sscanf(buf, "%s %s %s %s %s", buf1, buf2, buf3, buf4); // ó�� ���ڿ� �и� [username] 
				n = strlen(buf1); // "/login username" or "[username] Hello" ���� /login �� [username] �� �и�

				// /login ó��
				if (strcmp(buf1, CHAT_CMD_LOGIN) == 0) { // "/login"
					strcpy(userlist[i], buf + n + 1); // username ����
					printf(" userlist[%d] = %s\n", i, userlist[i]);
					sprintf(buf2, "[%s] ���� �����Ͽ����ϴ�.\n", userlist[i]);
					for (j = 0; j < num_chat; j++) {
						if (j != i) { // ���� ���� �ٸ� ����ڿ��� ������ �˸���.
							if (send(client_fds[j], buf2, BUF_LEN, 0) < 0) {
								printf("client[%d] send error.", j);
								client_error[j] = 1;
							}
						}
					}
					continue;
				}

				// /list ó��
				if (strcmp(buf2, CHAT_CMD_LIST) == 0) { // "/list"
					printf("Sending user list to client[%d] %s\n", i, userlist[i]);
					memset(buf, 0, BUF_LEN);
					sprintf(buf, "User List\nNo\tname\tstatus\n----------------------\n");
					if (send(client_fds[i], buf, BUF_LEN, 0) < 0) {
						printf("client[%d] send error.", i);
						client_error[i] = 1;
						continue;
					}
					for (j = 0; j < num_chat; j++) {
						memset(buf, 0, BUF_LEN);
						sprintf(buf, "%02d\t%s\t%s\n", j, userlist[j], (usersleep[j] == 0) ? "ONLINE" : (usersleep[j] == 1 ? "OFFLINE" : "WISPER"));
						if (send(client_fds[i], buf, BUF_LEN, 0) < 0) {
							printf("client[%d] send error.", i);
							client_error[i] = 1;
							break;
						}
					}
					memset(buf, 0, BUF_LEN);
					sprintf(buf, "----------------------\n");
					if (send(client_fds[i], buf, BUF_LEN, 0) < 0) {
						printf("client[%d] send error.", i);
						client_error[i] = 1;
						continue;
					}
					continue;
				}

				// /exit ó��
				// exit ��� /exit �� ����.
				if (strcmp(buf2, CHAT_CMD_EXIT) == 0) { // "/exit"
					RemoveClient(i);
					continue;
				}

				// /to ó�� �ӼӸ� ó��
				// [user1] /to user2 message ...
				if (strcmp(buf2, CHAT_CMD_TO) == 0) {
					Wisper(buf, n);
					continue;
				}

				// /sleep ó��
				// [user1] /sleep
				if (strcmp(buf2, CHAT_CMD_SLEEP) == 0) {
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							printf("sleep2\n");
							usersleep[j] = 1;
							break;
						}
					}
					continue;
				}

				// /wakeup ó��
				// [user1] /wakeup
				if (strcmp(buf2, CHAT_CMD_WAKEUP) == 0) {
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							if (j != userwith[userwith[j]])
								usersleep[j] = 0;
							else
								usersleep[j] = 2;
						}
					}
					continue;
				}

				//with ó��
				if (strcmp(buf2, CHAT_CMD_WITH) == 0) {
					// user1 ã��
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							k = j;
							break;
						}
					}
					// user2 ã��
					for (j = 0; j < num_chat; j++) {
						if (strcmp(userlist[j], buf3) == 0) {
							break;
						}
					}

					// user2 �ӼӸ� ��û ����
					sprintf(buf4, "%s���� 1:1 ��ȭ�� �䫊�߽��ϴ�. (y/n)\n", buf1);
					ret = send(client_fds[j], buf4, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[j] = 1;
					}
					// user2 �ӼӸ� ���� ����
					if ((ret = recv(client_fds[j], buf5, BUF_LEN, 0)) <= 0) {
						// client �� �� ���� ������ ���
						printf("recv error for client[%d]\n", i);
						client_error[j] = 1;
					}

					sscanf(buf5, "%s %s", buf1, buf2);
					if (strcmp(buf2, CHAT_CMD_WITH_YES) == 0) {
						usersleep[j] = 2;
						usersleep[k] = 2;
						userwith[j] = k;
						userwith[k] = j;

						printf("userwith[%d] %d\n", j, k);
						printf("userwith[%d] %d\n", k, j);

						printf("userwith[%d] = %d\n", j, k);
						printf("userwith[%d] = %d\n", k, j);

						sprintf(buf4, "[%s]�԰� 1:1��ȭ ����\n", userlist[j]);
						ret = send(client_fds[k], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", k);
							client_error[k] = 1;
						}
						sprintf(buf4, "[%s]�԰� 1:1��ȭ ����\n", userlist[k]);
						ret = send(client_fds[j], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", j);
							client_error[j] = 1;
						}
					}
					else {
						sprintf(buf4, "[%s]���� 1:1 ��ȭ�� �����߽��ϴ�.\n", userlist[j]);
						ret = send(client_fds[k], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", i);
							client_error[i] = 1;
						}
					}
					continue;
				}

				// end ó��
				if (strcmp(buf2, CHAT_CMD_WITH_END) == 0) {
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							break;
						}
					}

					sprintf(buf4, "[%s]�԰� 1:1 ��ȭ ����\n", userlist[j]);
					ret = send(client_fds[j], buf4, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[j] = 1;
					}
					sprintf(buf4, "[%s]�԰� 1:1 ��ȭ ����\n", userlist[userwith[j]]);
					ret = send(client_fds[userwith[j]], buf4, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[userwith[j]] = 1;
					}

					// 1 : 1 ��� ����
					usersleep[userwith[j]] = 0;
					usersleep[j] = 0;
					userwith[userwith[j]] = 0;
					userwith[j] = 0;
					continue;
				}

				//filesend ó��
				if (strcmp(buf2, CHAT_CMD_FILESEND) == 0) {
					// user1 ã��
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							k = j;
							break;
						}
					}
					// user2 ã��
					for (j = 0; j < num_chat; j++) {
						if (strcmp(userlist[j], buf3) == 0) {
							break;
						}
					}

					// user2 ���� ����
					sprintf(buf2, "%s���� ������ �������� �մϴ�. (y/n)\n", buf1);
					ret = send(client_fds[j], buf2, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[j] = 1;
					}
					// user2 �������� ���� ����
					if ((ret = recv(client_fds[j], buf5, BUF_LEN, 0)) <= 0) {
						// client �� �� ���� ������ ���
						printf("recv error for client[%d]\n", j);
						client_error[j] = 1;
					}

					// [username] y
					sscanf(buf5, "%s %s", buf1, buf2);
					
					// y �� ���� ����
					if (strcmp(buf2, CHAT_CMD_WITH_YES) == 0) {
						memset(buf2, 0, BUF_LEN);
						sprintf(buf2, "%s", CHAT_CMD_FILE_YES);
						send(client_fds[k], buf2, BUF_LEN, 0);
						memset(buf2, 0, BUF_LEN);
						sprintf(buf2, "%s", CHAT_CMD_FILE_DATA);
						send(client_fds[j], buf2, BUF_LEN, 0);

						while (1) {
							memset(buf5, 0, BUF_LEN);
							if (recv(client_fds[k], buf5, BUF_LEN, 0) <= 0) {
								printf("recv error client_fds[%d]\n",k);
								break;
							}
							if (strcmp(buf5, CHAT_CMD_FILE_END) == 0)
								break;
							if (send(client_fds[j], buf5, BUF_LEN, 0) <= 0) {
								printf("recv error client_fds[%d]\n", j);
								break;
							}
						}
						
						memset(buf5, 0, BUF_LEN);
						if (recv(client_fds[k], buf5, BUF_LEN, 0) <= 0) {
							printf("recv error client_fds[%d]\n", j);
							break;
						}
						memset(buf2, 0, BUF_LEN);
						memset(buf3, 0, BUF_LEN);
						sscanf(buf5, "%s %s", buf2, buf3);
						memset(buf5, 0, BUF_LEN);
						// �̺κ�
						sprintf(buf5, "0123456789abcdefghi%s %sbytes ���ſϷ�\n", buf2, buf3);
						if (send(client_fds[j], buf5, BUF_LEN, 0) <= 0) {
							printf("recv error client_fds[%d]\n", j);
							break;
						}
						memset(buf5, 0, BUF_LEN);
						sprintf(buf5, "%s %sbytes ���ۿϷ�\n", buf2, buf3);
						if (send(client_fds[k], buf5, BUF_LEN, 0) <= 0) {
							printf("recv error client_fds[%d]\n", k);
							break;
						}
					}
					else { // n�� ����
						memset(buf4, 0, BUF_LEN);
						sprintf(buf4, "%s", CHAT_CMD_FILE_NO);
						send(client_fds[k], buf4, BUF_LEN, 0);
						sprintf(buf4, "[%s]���� ���� ������ �ź��߽��ϴ�.\n", userlist[j]);
						ret = send(client_fds[k], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", k);
							client_error[k] = 1;
						}
					}
					continue;
				}


				// ���� ���� ã��
				for (j = 0; j < num_chat; j++) {
					if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
						break;
					}
				}

				// ���� ����� 1��1 ��尡 �ƴϸ�
				// ��� �����ڿ��� �޼��� ���
				if (usersleep[j] != 2) {
					for (j = 0; j < num_chat; j++) {
						if (!usersleep[j]) { // 1��1��峪 OFFLINE�� �ƴ϶��
							ret = send(client_fds[j], buf, BUF_LEN, 0);
							if (ret <= 0) {
								printf("send error for client[%d]\n", j);
								client_error[j] = 1;
							}
						}
					}
				}
				else {
					if (usersleep[userwith[j]] != 1) { // sleep ��尡 �ƴ϶��
						sprintf(buf4, "[1:1] %s %s\n", buf1, buf2);
						ret = send(client_fds[userwith[j]], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", userwith[j]);
							client_error[userwith[j]] = 1;
						}
					}
				}





			}
		}

		// ������ �� Client���� �ڿ��� ������ ���鼭 �����Ѵ�.
		for (i = num_chat - 1; i >= 0; i--) {
			if (client_error[i])
				RemoveClient(i);
		}
	}
}



/* ä�� Ż�� ó�� */
void RemoveClient(int i) {
#ifdef WIN32
	closesocket(client_fds[i]);
#else
	close(client_fds[i]);
#endif
	// ������ client�� ������ �ڸ��� �̵� (��ĭ�� ���� �ʿ䰡 ����)
	char buf[BUF_LEN];
	printf("client[%d] %s ����. ���� ������ �� = %d\n", i, userlist[i], num_chat - 1);
	sprintf(buf, "[%s] ���� �����Ͽ����ϴ�\n", userlist[i]);
	for (int j = 0; j < num_chat; j++) {
		if (j != i) { // ���� ���� �ٸ� ����ڿ��� ������ �˸���.
			if (send(client_fds[j], buf, BUF_LEN, 0) < 0) {
				printf("client[%d] send error.", j);
			}
		}
	}
	if (i != num_chat - 1) {
		client_fds[i] = client_fds[num_chat - 1]; // socket ���� 
		strcpy(userlist[i], userlist[num_chat - 1]); // username 
		usersleep[i] = usersleep[num_chat - 1]; // sleep ����
	}
	num_chat--;
}

// client_fds[] ���� �ִ� ���Ϲ�ȣ Ȯ��
// select(maxfds, ..) ���� maxfds = getmax(server_fd) + 1
int getmax(int k) {
	int max = k;
	int r;
	for (r = 0; r < num_chat; r++) {
		if (client_fds[r] > max) max = client_fds[r];
	}
	return max;
}

void Wisper(char* buf, int n) {
	char buf1[BUF_LEN] = { 0 }, buf2[BUF_LEN] = { 0 }, buf3[BUF_LEN] = { 0 };
	sscanf(buf + n + 2 + strlen(CHAT_CMD_TO), "%s %s", buf1, buf2);
	sprintf(buf3, "[�ӼӸ�] [%s] %s\n", buf1, buf2);
	for (int j = 0; j < num_chat; j++) {
		if (strcmp(userlist[j], buf1) == 0) {
			if (usersleep[j] != 1) {
				if (send(client_fds[j], buf3, BUF_LEN, 0) < 0) {
					printf("client[%d] send error.", j);
				}
			}
			break;
		}
	}
}