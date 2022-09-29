#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define LENGTH 2048

// Global degiskenler
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[32];
char pass[32];
int token = -1;

void str_overwrite_stdout()
{
	printf("%s", "> ");
	fflush(stdout);
}

void str_trim_lf(char *arr, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{ // trim \n
		if (arr[i] == '\n')
		{
			arr[i] = '\0';
			break;
		}
	}
}

void catch_ctrl_c_and_exit(int sig)
{
	flag = 1;
}

/* Verilen mesajın her bir karekterinin ASCII karsılıgından token'i cıkararak
mesajı desifre eder*/
char *decrypt_msg(char *message, int token)
{

	char *mess2 = malloc(strlen(message));
	for (int i = 0; i < strlen(message); i++)
	{
		mess2[i] = message[i] - token;
	}
	mess2[strlen(message)] = '\0';

	return mess2;
}

/* Verilen mesajın her bir karekterinin ASCII karsılıgına token'i ekleyerek
mesajı sifreler*/
char *crypt_msg(char *message, int token)
{
	char *mess2 = malloc(strlen(message));

	for (int i = 0; i < strlen(message); i++)
	{
		mess2[i] = message[i] + token;
	}

	return mess2;
}

/* Mesaj gonderme islerine bakan kod*/
void send_msg_handler()
{
	char message[LENGTH] = {};
	char buffer[LENGTH + 32] = {};

	while (1)
	{
		str_overwrite_stdout();
		fgets(message, LENGTH, stdin);
		str_trim_lf(message, LENGTH);

		// Eger kullanici exit yazarsa dogrudan cikis yap
		if (strcmp(message, "exit") == 0)
		{
			break;
		}
		else if (strcmp(message, "get_clients") == 0) // Online olan clientleri dondurur
		{
			sprintf(buffer, "get=clients\n"); // Server'in istedigi formata sokar
			str_trim_lf(buffer, strlen(buffer));
			strcpy(buffer, crypt_msg(buffer, token));
			send(sockfd, buffer, strlen(buffer), 0);
		}
		else
		{
			/*Eger mesajın icerisinde '/' karakteri yoksa kullaniciyi duzgun formatta
			bir mesaj yazmasi icin uyarir*/
			if (strchr(message, '/') != NULL)
			{
				strtok(message, "/");
				char recv_name[32];
				strcpy(recv_name, message);
				strcpy(message, strtok(NULL, ""));
				sprintf(buffer, "client=%sx&msg=%s&\n", recv_name, message); // Serverin istedigi formata sokar

				str_trim_lf(buffer, strlen(buffer));
				strcpy(buffer, crypt_msg(buffer, token));
				send(sockfd, buffer, strlen(buffer), 0);
			}
			else
			{
				printf("Please enter reciever Name (userid/Message) \n");
			}
		}

		// Buffer'i temizler
		bzero(message, LENGTH);
		bzero(buffer, LENGTH + 32);
	}
	catch_ctrl_c_and_exit(2);
}

/* Mesaj alma islerine bakan kod*/
void recv_msg_handler()
{
	char message[LENGTH] = {};
	while (1)
	{
		int receive = recv(sockfd, message, LENGTH, 0);
		if (receive > 0)
		{
			// Mesaj geldi
			if (token < 0)
			{ // Eger token daha atanmadiysa token atanir
				printf("--- WELCOME TO THE CHATROOM ---\n");
				strtok(message, ":");
				token = atoi(strtok(NULL, ":"));
			}
			else
			{
				// Token atandiysa mesaj desifre edilir ve ekrana basilir
				strcpy(message, decrypt_msg(message, token));
				printf("%s\n", message);
			}
			str_overwrite_stdout();
		}
		else if (receive == 0)
		{
			break;
		}
		memset(message, 0, sizeof(message));
	}
}

int main(int argc, char **argv)
{
	// Yanlis sayida arguman girildiyse dogru kullanimi yazdirir
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	char login_info[128];
	int port = atoi(argv[1]);

	signal(SIGINT, catch_ctrl_c_and_exit);

	printf("Please enter your id: ");
	fgets(name, 32, stdin);
	str_trim_lf(name, strlen(name));

	if (strlen(name) > 32 || strlen(name) < 2)
	{
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

	printf("Please enter your password: ");
	fgets(pass, 32, stdin);
	str_trim_lf(pass, strlen(pass));

	if (strlen(pass) > 32 || strlen(pass) < 2)
	{
		printf("Password must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

	sprintf(login_info, "login=%s&password=%s&", name, pass); // Server'in istedigi formata sokar

	struct sockaddr_in server_addr;

	/* Socket ayarlari */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	// Server'a baglanma
	int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1)
	{
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	// Mesaj gonderme isini ayri bir threade alir
	pthread_t send_msg_thread;
	if (pthread_create(&send_msg_thread, NULL, (void *)send_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread(send)\n");
		return EXIT_FAILURE;
	}

	// Mesaj alma isini ayri bir threade alir
	pthread_t recv_msg_thread;
	if (pthread_create(&recv_msg_thread, NULL, (void *)recv_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread(recv)\n");
		return EXIT_FAILURE;
	}

	sleep(1);
	send(sockfd, crypt_msg(login_info, token), 32, 0);
	while (1)
	{
		if (flag)
		{
			printf("\nBye\n");
			break;
		}
	}

	close(sockfd);

	return EXIT_SUCCESS;
}
