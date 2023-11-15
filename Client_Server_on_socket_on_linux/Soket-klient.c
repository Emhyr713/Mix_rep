#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() 
{
	FILE *file = fopen("/home/nikita/Документы/laba2/log_file_client_c.txt","w+");
	// Переменная для сохранения текущего времени
	long int ttime;
	ttime = time (NULL);

	// Создание сокета
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Параметры для сокета
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5050);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Присоединение к серверу
	int connection_status =  connect(sockfd,  (struct sockaddr *) &server_address, sizeof(server_address));
	
	// Проверка на соединение
	if (connection_status == -1)
	{
		printf("There was an error making a connection to the remote socket \n\n");
		return -1;
	}
	else
		printf("Соединение установлено \n\n");

	// Считывание текущее время и запись лога в файл
	ttime = time (NULL);
	fprintf (file, "Время подключения:\t\t%s",ctime (&ttime));

	// Печать информации о сервере
	char buffer_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_address.sin_addr, buffer_addr, sizeof(buffer_addr));
    int buffer_port = (int) ntohs(server_address.sin_port);
    printf( "server_address: %s : %d \n", buffer_addr,  buffer_port);
	fprintf (file, "server_address: %s : %d \n", buffer_addr,  buffer_port);

	sleep (3);

	// Сообщение
	char client_message[] = "Рудь Алекс Вольфович М3О-310Б-20";

	// Отправка информации с сервера и запись лога в файл
    send(sockfd, client_message, sizeof(client_message), 0);
	ttime = time (NULL);
	fprintf (file, "Время отправки сообщения:\t%s",ctime (&ttime));
	fprintf (file, "Отправленное сообщение:\t\t%s\n",client_message);

	sleep (3);

	// Приём информации с сервера
	char server_response[256];
	recv(sockfd, &server_response, sizeof(server_response), 0);
	printf("The server sent the data: %s\n\n", server_response);
	ttime = time (NULL);
	fprintf (file, "Время получения сообщения:\t%s",ctime (&ttime));
	fprintf (file, "Полученное сообщение:\t\t%s\n",server_response);

	// Закрытие сокета
	close(sockfd);
	return 0;
}