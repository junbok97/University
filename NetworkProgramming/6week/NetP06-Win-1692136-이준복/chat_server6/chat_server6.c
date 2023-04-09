/*----------------------
 파일명 : chat_server4.c
 기  능 : 채팅서버, chat_server3 + /to, /sleep, /wakeup, 입/퇴장 알림
 도전과제 1 : 1:1 대화 모드 /with
 도전과제 2 : 1:1 파일전송 /filesend
 사용법 : chat_server4 [port]
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

	// winsock 사용을 위해 필수적임
	signal(SIGINT, exit_callback);
	sversion = MAKEWORD(1, 1);
	WSAStartup(sversion, &wsadata);
}
#endif

#define MAXCLIENTS 64		// 최대 채팅 참가자 수
#define EXIT	"exit"		// 채팅 종료 문자열
int maxfdp;              	// select() 에서 감시해야할 # of socket 변수 getmax() return 값 + 1
int getmax(int);			// 최대 소켓번호 계산
int num_chat = 0;         	// 채팅 참가자 수
int client_fds[MAXCLIENTS];	// 채팅에 참가자 소켓번호 목록
void RemoveClient(int);		// 채팅 탈퇴 처리 함수
void Wisper(char* buf, int n); // 귓속말 함수
void put(char* filename, int to, int from); // 파일전송 함수

#define BUF_LEN	128
#define CHAT_SERVER "0.0.0.0"
#define CHAT_PORT "40000"
char userlist[MAXCLIENTS][BUF_LEN]; // user name 보관용
int usersleep[MAXCLIENTS] = { 0, }; // sleep 상태인지 상태 보관
int userwith[MAXCLIENTS] = { 0, }; // 1:1 대화 상대 보관

// chat_client/server3
#define CHAT_CMD_LOGIN		"/login"// connect하면 user name 전송 "/login user1"
#define CHAT_CMD_LIST		"/list"// userlist 요청
#define CHAT_CMD_EXIT		"/exit"// 종료
// chat_client/server4 
#define CHAT_CMD_TO			"/to"// 귓속말 "/to user2 Hi there.."
#define CHAT_CMD_SLEEP		"/sleep"// 대기모드(부재중) 설정
#define CHAT_CMD_WAKEUP		"/wakeup"// wakeup 또는 message 전송하면 자동 wakeup
// 도전 문제를 위한 명령어들
// 도전1 : 1:1 대화
#define CHAT_CMD_WITH		"/with"// [user1] /with user2, user1이 user2와 1:1 대화 요청
#define CHAT_CMD_WITH_YES	"y"// 1:1 대화 허락 [user2] /withyes user1
#define CHAT_CMD_WITH_NO	"n"// 1:1 대화 거부 [user2] /withno user1
#define CHAT_CMD_WITH_END	"/end"// 1:1 채팅 종료 [user1] /end or [user2] /end
// 도전2 : 1:1 파일 전송
#define CHAT_CMD_FILESEND	"/filesend"// [user1] /filesend user2 data.txt 파일 전송 요청
#define CHAT_CMD_FILE_YES	"/fileyes"// [user2] /fileyes user1 파일 전송 허락
#define CHAT_CMD_FILE_NO	"/fileno"// [user2] /fileno user1 파일 수신 거부
#define CHAT_CMD_FILE_NAME	"/filename"// [user1] /filename data.txt 765 파일 정보 전달
#define CHAT_CMD_FILE_DATA	"/filedata"// [user1] /filedata data...
#define CHAT_CMD_FILE_END	"/fileend"// [user1] /fileend 파일전송 끝

int main(int argc, char* argv[]) {
	char buf[BUF_LEN], buf1[BUF_LEN], buf2[BUF_LEN], buf3[BUF_LEN], buf4[BUF_LEN], buf5[BUF_LEN];
	int i, j, k, n, ret;
	int server_fd, client_fd, client_len;
	unsigned int set = 1;
	char* ip_addr = CHAT_SERVER, * port_no = CHAT_PORT;
	fd_set  read_fds;     // 읽기를 감지할 소켓번호 구조체 server_fd 와 client_fds[] 를 설정한다.
	struct sockaddr_in  client_addr, server_addr;
	int client_error[MAXCLIENTS];

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

	printf("chat_server5 waiting connection..\n");
	printf("server_fd = %d\n", server_fd);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set, sizeof(set));

	/* server_addr을 '\0'으로 초기화 */
	memset((char*)&server_addr, 0, sizeof(server_addr));
	/* server_addr 세팅 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(port_no));

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("Server: Can't bind local address.\n");
		exit(0);
	}
	/* 클라이언트로부터 연결요청을 기다림 */
	listen(server_fd, 5);

	while (1) {
		FD_ZERO(&read_fds); // 변수 초기화
		FD_SET(server_fd, &read_fds); // accept() 대상 소켓 설정
		for (i = 0; i < num_chat; i++) // 채팅에 참가중이 모든 client 소켓을 reac() 대상 추가
			FD_SET(client_fds[i], &read_fds);
		maxfdp = getmax(server_fd) + 1;     // 감시대상 소켓의 수를 계산
		if (select(maxfdp, &read_fds, (fd_set*)0, (fd_set*)0, (struct timeval*)0) <= 0) {
			printf("select error <= 0 \n");
			exit(0);
		}
		// 초기 소켓 즉, server_fd 에 변화가 있는지 검사
		if (FD_ISSET(server_fd, &read_fds)) {
			// 변화가 있다 --> client 가 connect로 연결 요청을 한 것
			client_len = sizeof(client_addr);
			client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
			if (client_fd == -1) {
				printf("accept error\n");
			}
			else {
				printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr),
					ntohs(client_addr.sin_port));
				printf("client_fd = %d\n", client_fd);
				/* 채팅 클라이언트 목록에 추가 */
				printf("client[%d] 입장. 현재 참가자 수 = %d\n", num_chat, num_chat + 1);
				client_fds[num_chat++] = client_fd;
			}
		}

		memset(client_error, 0, sizeof(client_error));
		/* 클라이언트가 보낸 메시지를 모든 클라이언트에게 방송 */
		for (i = 0; i < num_chat; i++) {
			// 각각의 client들의 I/O 변화가 있는지.
			if (FD_ISSET(client_fds[i], &read_fds)) {
				// Read One 또는 client 비정상 종료 확인
				memset(buf, 0, BUF_LEN);
				memset(buf1, 0, BUF_LEN);
				memset(buf2, 0, BUF_LEN);
				memset(buf3, 0, BUF_LEN);
				memset(buf4, 0, BUF_LEN);

				if ((n = recv(client_fds[i], buf, BUF_LEN, 0)) <= 0) {
					// client 가 비 정상 종료한 경우
					printf("recv error for client[%d]\n", i);
					client_error[i] = 1;
					continue;
				}
				printf("received %d from client[%d] : %s", n, i, buf);
				// "/login username" --> buf1 = /login, buf2 = username
				// "[username] /list" --> buf1 = [username], buf2 = /list
				// "[username] message .." buf1 = [username], buf2 = message ...
				// chat_client/server4 추가 부분
				// "[username] /to user2 message .." buf1 = [username], buf2 = /to, buf3 = user2 , buf4 = message
				// "[username] /sleep  --> buf1 = [username], buf2 = /sleep
				// "[username] /wakeup  --> buf1 = [username], buf2 = /wakeup
				// "[username] /with user2" buf1 = [username], buf2 = /with, buf3 = user2
				// "[username] /filesend user2 filename" buf1 = [username], buf2 = /filesend, buf3 = user2, buf4 = filename

				sscanf(buf, "%s %s %s %s %s", buf1, buf2, buf3, buf4); // 처음 문자열 분리 [username] 
				n = strlen(buf1); // "/login username" or "[username] Hello" 에서 /login 나 [username] 만 분리

				// /login 처리
				if (strcmp(buf1, CHAT_CMD_LOGIN) == 0) { // "/login"
					strcpy(userlist[i], buf + n + 1); // username 보관
					printf(" userlist[%d] = %s\n", i, userlist[i]);
					sprintf(buf2, "[%s] 님이 입장하였습니다.\n", userlist[i]);
					for (j = 0; j < num_chat; j++) {
						if (j != i) { // 본인 제외 다른 사용자에게 입장을 알린다.
							if (send(client_fds[j], buf2, BUF_LEN, 0) < 0) {
								printf("client[%d] send error.", j);
								client_error[j] = 1;
							}
						}
					}
					continue;
				}

				// /list 처리
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

				// /exit 처리
				// exit 대신 /exit 로 변경.
				if (strcmp(buf2, CHAT_CMD_EXIT) == 0) { // "/exit"
					RemoveClient(i);
					continue;
				}

				// /to 처리 귓속말 처리
				// [user1] /to user2 message ...
				if (strcmp(buf2, CHAT_CMD_TO) == 0) {
					Wisper(buf, n);
					continue;
				}

				// /sleep 처리
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

				// /wakeup 처리
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

				//with 처리
				if (strcmp(buf2, CHAT_CMD_WITH) == 0) {
					// user1 찾기
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							k = j;
							break;
						}
					}
					// user2 찾기
					for (j = 0; j < num_chat; j++) {
						if (strcmp(userlist[j], buf3) == 0) {
							break;
						}
					}

					// user2 귓속말 신청 전송
					sprintf(buf4, "%s님이 1:1 대화를 요쳥했습니다. (y/n)\n", buf1);
					ret = send(client_fds[j], buf4, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[j] = 1;
					}
					// user2 귓속말 수락 여부
					if ((ret = recv(client_fds[j], buf5, BUF_LEN, 0)) <= 0) {
						// client 가 비 정상 종료한 경우
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

						sprintf(buf4, "[%s]님과 1:1대화 시작\n", userlist[j]);
						ret = send(client_fds[k], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", k);
							client_error[k] = 1;
						}
						sprintf(buf4, "[%s]님과 1:1대화 시작\n", userlist[k]);
						ret = send(client_fds[j], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", j);
							client_error[j] = 1;
						}
					}
					else {
						sprintf(buf4, "[%s]님이 1:1 대화를 거절했습니다.\n", userlist[j]);
						ret = send(client_fds[k], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", i);
							client_error[i] = 1;
						}
					}
					continue;
				}

				// end 처리
				if (strcmp(buf2, CHAT_CMD_WITH_END) == 0) {
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							break;
						}
					}

					sprintf(buf4, "[%s]님과 1:1 대화 종료\n", userlist[j]);
					ret = send(client_fds[j], buf4, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[j] = 1;
					}
					sprintf(buf4, "[%s]님과 1:1 대화 종료\n", userlist[userwith[j]]);
					ret = send(client_fds[userwith[j]], buf4, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[userwith[j]] = 1;
					}

					// 1 : 1 모드 종료
					usersleep[userwith[j]] = 0;
					usersleep[j] = 0;
					userwith[userwith[j]] = 0;
					userwith[j] = 0;
					continue;
				}

				//filesend 처리
				if (strcmp(buf2, CHAT_CMD_FILESEND) == 0) {
					// user1 찾기
					for (j = 0; j < num_chat; j++) {
						if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
							k = j;
							break;
						}
					}
					// user2 찾기
					for (j = 0; j < num_chat; j++) {
						if (strcmp(userlist[j], buf3) == 0) {
							break;
						}
					}

					// user2 파일 전송
					sprintf(buf2, "%s님이 파일을 보내려고 합니다. (y/n)\n", buf1);
					ret = send(client_fds[j], buf2, BUF_LEN, 0);
					if (ret <= 0) {
						printf("send error for client[%d]\n", j);
						client_error[j] = 1;
					}
					// user2 파일전송 수락 여부
					if ((ret = recv(client_fds[j], buf5, BUF_LEN, 0)) <= 0) {
						// client 가 비 정상 종료한 경우
						printf("recv error for client[%d]\n", j);
						client_error[j] = 1;
					}

					// [username] y
					sscanf(buf5, "%s %s", buf1, buf2);
					
					// y 면 파일 전송
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
						// 이부분
						sprintf(buf5, "0123456789abcdefghi%s %sbytes 수신완료\n", buf2, buf3);
						if (send(client_fds[j], buf5, BUF_LEN, 0) <= 0) {
							printf("recv error client_fds[%d]\n", j);
							break;
						}
						memset(buf5, 0, BUF_LEN);
						sprintf(buf5, "%s %sbytes 전송완료\n", buf2, buf3);
						if (send(client_fds[k], buf5, BUF_LEN, 0) <= 0) {
							printf("recv error client_fds[%d]\n", k);
							break;
						}
					}
					else { // n면 종료
						memset(buf4, 0, BUF_LEN);
						sprintf(buf4, "%s", CHAT_CMD_FILE_NO);
						send(client_fds[k], buf4, BUF_LEN, 0);
						sprintf(buf4, "[%s]님이 파일 수신을 거부했습니다.\n", userlist[j]);
						ret = send(client_fds[k], buf4, BUF_LEN, 0);
						if (ret <= 0) {
							printf("send error for client[%d]\n", k);
							client_error[k] = 1;
						}
					}
					continue;
				}


				// 보낸 유저 찾기
				for (j = 0; j < num_chat; j++) {
					if (strncmp(buf1 + 1, userlist[j], strlen(userlist[j])) == 0) {
						break;
					}
				}

				// 보낸 사람이 1대1 모드가 아니면
				// 모든 참가자에게 메세지 방송
				if (usersleep[j] != 2) {
					for (j = 0; j < num_chat; j++) {
						if (!usersleep[j]) { // 1대1모드나 OFFLINE이 아니라면
							ret = send(client_fds[j], buf, BUF_LEN, 0);
							if (ret <= 0) {
								printf("send error for client[%d]\n", j);
								client_error[j] = 1;
							}
						}
					}
				}
				else {
					if (usersleep[userwith[j]] != 1) { // sleep 모드가 아니라면
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

		// 오류가 난 Client들을 뒤에서 앞으로 가면서 제거한다.
		for (i = num_chat - 1; i >= 0; i--) {
			if (client_error[i])
				RemoveClient(i);
		}
	}
}



/* 채팅 탈퇴 처리 */
void RemoveClient(int i) {
#ifdef WIN32
	closesocket(client_fds[i]);
#else
	close(client_fds[i]);
#endif
	// 마지막 client를 삭제된 자리로 이동 (한칸씩 내릴 필요가 없다)
	char buf[BUF_LEN];
	printf("client[%d] %s 퇴장. 현재 참가자 수 = %d\n", i, userlist[i], num_chat - 1);
	sprintf(buf, "[%s] 님이 퇴장하였습니다\n", userlist[i]);
	for (int j = 0; j < num_chat; j++) {
		if (j != i) { // 본인 제외 다른 사용자에게 퇴장을 알린다.
			if (send(client_fds[j], buf, BUF_LEN, 0) < 0) {
				printf("client[%d] send error.", j);
			}
		}
	}
	if (i != num_chat - 1) {
		client_fds[i] = client_fds[num_chat - 1]; // socket 정보 
		strcpy(userlist[i], userlist[num_chat - 1]); // username 
		usersleep[i] = usersleep[num_chat - 1]; // sleep 상태
	}
	num_chat--;
}

// client_fds[] 내의 최대 소켓번호 확인
// select(maxfds, ..) 에서 maxfds = getmax(server_fd) + 1
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
	sprintf(buf3, "[귓속말] [%s] %s\n", buf1, buf2);
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