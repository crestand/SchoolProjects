#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048

static _Atomic unsigned int cli_count = 0;
static int uid = 10;

int token = 1;

/* Client yapisi */
typedef struct
{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char name[32];
	char pass[32];
} client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_overwrite_stdout()
{
	printf("\r%s", "> ");
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

/* Client adresini bastirir */
void print_client_addr(struct sockaddr_in addr)
{
	printf("%d.%d.%d.%d",
		   addr.sin_addr.s_addr & 0xff,
		   (addr.sin_addr.s_addr & 0xff00) >> 8,
		   (addr.sin_addr.s_addr & 0xff0000) >> 16,
		   (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

/* Client'leri queue'ya ekler */
void queue_add(client_t *cl)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (!clients[i])
		{
			clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Client'leri queue'dan cikarir */
void queue_remove(int uid)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->uid == uid)
			{
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Verilen mesaji, verilen tokeni kullanarak cozer*/
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

/* Verilen mesaji, verilen tokeni kullanarak sifreler*/
char *crypt_msg(char *message, int token)
{
	char *mess2 = malloc(strlen(message));

	for (int i = 0; i < strlen(message); i++)
	{
		mess2[i] = message[i] + token;
	}
	mess2[strlen(message)] = '\0';
	return mess2;
}

/* Mesajin sahibi disindaki diger clientlara mesaji iletir */
void send_message(char *s, int uid)
{
	pthread_mutex_lock(&clients_mutex);

	char crypted_message[BUFFER_SZ];
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->uid != uid) // Mesaji, gonderen client disindaki diger clientlera gonderir
			{
				strcpy(crypted_message, crypt_msg(s, token));
				if (write(clients[i]->sockfd, crypted_message, strlen(crypted_message)) < 0)
				{
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Mesaji sadece verilen uid'ye sahip olan client'a gonderir */
int send_private_message(char *s, int uid)
{
	pthread_mutex_lock(&clients_mutex);
	int exit_status = EXIT_FAILURE;

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->uid == uid) // Dogru client'i bulana kadar arar
			{
				s = crypt_msg(s, token);
				if (write(clients[i]->sockfd, s, strlen(s)) < 0)
				{
					perror("ERROR: write to descriptor failed");
				}
				else
				{
					exit_status = EXIT_SUCCESS;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
	// Exit status'e bakilarak mesajin basarili bir sekilde gonderilip gonderilmedigine karar veriliyor
	return exit_status;
}

/* Online olan butun client'lari dondurur*/
void get_all_clients(char *buffer, int uid)
{
	pthread_mutex_lock(&clients_mutex);

	strcpy(buffer, "clients=/");
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->uid != uid)
			{
				strcat(buffer, clients[i]->name);
				strcat(buffer, "/");
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Verilen isimdeki client'in uid'sini dondurur*/
void get_client_uid(char *recv_name, int *uid)
{
	pthread_mutex_lock(&clients_mutex);

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (strcmp(clients[i]->name, recv_name) == 0)
			{
				*uid = clients[i]->uid;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Client'lara sifrelemede kullanilacak token'i yollar*/
void send_token(int uid)
{
	pthread_mutex_lock(&clients_mutex);

	char s[32];
	sprintf(s, "token:%d", token); // Uygun formata sokuyoruz
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (clients[i])
		{
			if (clients[i]->uid == uid)
			{
				// printf("Server send %s\n", s);
				if (write(clients[i]->sockfd, s, strlen(s)) < 0)
				{
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Verilen char arrayi icinde p1 ile p2 arasindaki karakterleri dondurur*/
char *extract_between(const char *str, const char *p1, const char *p2)
{
	const char *i1 = strstr(str, p1);
	if (i1 != NULL)
	{
		const size_t pl1 = strlen(p1);
		const char *i2 = strstr(i1 + pl1, p2);
		if (p2 != NULL)
		{
			/* Iki isaretci de bulunduktan sonra arasini donduruyoruz*/
			const size_t mlen = i2 - (i1 + pl1);
			char *ret = malloc(mlen + 1);
			if (ret != NULL)
			{
				memcpy(ret, i1 + pl1, mlen);
				ret[mlen] = '\0';
				return ret;
			}
		}
	}
}
/* Gelen kullanici adi ve sifreye gore server'a baglanmaya izin verir*/
int user_login(char *user_name, char *password)
{
	char filename[] = "users.txt";
	FILE *file = fopen(filename, "r");

	if (file != NULL)
	{
		char line[1000];
		while (fgets(line, sizeof line, file) != NULL)
		{
			char *user, *pass;
			user = strtok(line, "\t");
			pass = strtok(NULL, "\t");
			str_trim_lf(user, strlen(user));
			str_trim_lf(pass, strlen(pass));

			if (strcmp(user_name, user) == 0 && strcmp(password, pass) == 0) // Eslesen bir kullanici bulundu
			{
				fclose(file);
				return EXIT_SUCCESS;
			}
		}
		fclose(file);
	}
	else
	{
		perror(filename);
	}
	return EXIT_FAILURE;
}

/* Client'larla olan iletisimi kontrol eder */
void *handle_client(void *arg)
{
	char buff_out[BUFFER_SZ];
	char login_info[128];
	char name[32];
	char recv_name[32];
	char pass[32];
	int leave_flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	send_token(cli->uid);
	//
	if (recv(cli->sockfd, login_info, 128, 0) <= 0)
	{
		printf("Didn't enter the userid.\n");
		leave_flag = 1;
	}
	else
	{
		// Gelen giris yapma bilgisinden isim ve sifre ayristiriliyor
		strcpy(login_info, decrypt_msg(login_info, token));
		strcpy(name, extract_between(login_info, "login=", "&"));
		strcpy(pass, extract_between(login_info, "password=", "&"));

		if (user_login(name, pass) > 0) // Bilgilerin dogrulugu kontrol ediliyor
		{
			leave_flag = 1;
			send_private_message("Uncorrect username or password!\n", cli->uid);
		}
		else
		{
			strcpy(cli->name, name);
			strcpy(cli->pass, pass);
			sprintf(buff_out, "%s has joined\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			send_private_message("Login succesful!\n", cli->uid);
		}
	}

	bzero(buff_out, BUFFER_SZ);

	while (1)
	{
		if (leave_flag)
		{
			break;
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		// Mesaj geldi ve desifre ediyoruz
		strcpy(buff_out, decrypt_msg(buff_out, token));

		if (receive > 0)
		{
			if (strlen(buff_out) > 0)
			{
				str_trim_lf(buff_out, strlen(buff_out));
				if (strcmp(buff_out, "get=clients") == 0)
				{
					get_all_clients(buff_out, cli->uid);
					send_private_message(buff_out, cli->uid);
				}
				else
				{

					int recv_uid = -1;
					strcpy(recv_name, extract_between(buff_out, "client=", "x&")); // Mesajin iletilecegi client ayristiriliyor
					strcpy(buff_out, extract_between(buff_out, "x&msg=", "&"));	   // Gonderilecek mesaj ayristiriliyor
					strcat(buff_out, " <- ");
					strcat(buff_out, cli->name);
					get_client_uid(recv_name, &recv_uid);
					if (send_private_message(buff_out, recv_uid) == 0) // Mesaj basarili bir sekilde iletildi
					{
						send_private_message("Message sent!\n", cli->uid);
					}
					else
					{
						send_private_message("Message couldn't sent!\n", cli->uid); // Mesaj iletilemedi
					}
				}
			}
		}

		else if (receive == 0 || strcmp(buff_out, "exit") == 0)
		{
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		}
		else
		{
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

	/* Client'i queue'dan siliyoruz */
	close(cli->sockfd);
	queue_remove(cli->uid);
	free(cli);
	cli_count--;
	pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char **argv)
{
	// Yanlis sayida arguman girildiyse dogru kullanimi yazdirir
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Rastgele token olusturuluyor
	srand(time(NULL));
	token = rand();

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	int option = 1;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t tid;

	/* Socket ayarlari */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);

	/* Pipe sinyallerini görmezden geliyoruz*/
	signal(SIGPIPE, SIG_IGN);

	if (setsockopt(listenfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&option, sizeof(option)) < 0)
	{
		perror("ERROR: setsockopt failed");
		return EXIT_FAILURE;
	}

	/* Bind */
	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR: Socket binding failed");
		return EXIT_FAILURE;
	}

	/* Listen */
	if (listen(listenfd, 10) < 0)
	{
		perror("ERROR: Socket listening failed");
		return EXIT_FAILURE;
	}

	printf("--- WELCOME TO THE CHATROOM ---\n");

	while (1)
	{
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);

		// Maksimum client sayisina erisilip erisilmedigini kontrol ediyoruz
		if ((cli_count + 1) == MAX_CLIENTS)
		{
			printf("Max clients reached. Rejected: ");
			print_client_addr(cli_addr);
			printf(":%d\n", cli_addr.sin_port);
			close(connfd);
			continue;
		}

		/* Client ayarlari */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = cli_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;

		// Client'i queue'ya ekliyoruz ve thread'i forkluyoruz
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void *)cli);
		/* CPU optimizasyonu*/
		sleep(1);
	}

	return EXIT_SUCCESS;
}
