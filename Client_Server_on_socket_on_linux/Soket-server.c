#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <locale.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <stdint.h>

void invert_str(char *client_response)
{
    int len = strlen(client_response);
    //int end_str = strlen(client_response)-15;
    //int i = 0;
    char message[len];

    //printf("%d\n", len);

    // Переворачивание строки
	for (int i = len - 1; i >= 0; i--)
		message[len - (i + 1)] = client_response[i];
        
    message[len] = '\0';

    // // Вывод ФИО
    // while (i < end_str)
    // {
    //     if(client_response[i] == 32)
    //     {
    //         message[end_str-1-i] =  client_response[i];
    //     }
    //     else
    //     {
    //         message[end_str-2-i] = client_response[i];
    //         message[end_str-1-i] = client_response[i+1];
    //         i++;
    //     }
    //     i++;
    // }
    // Вывод М3О-310Б-20
    // while (i < len)
    // {
    //     message[i] =  client_response[i];
    //     i++;
    // }
    //message[end_str] = '\0';

    strcpy(client_response, message);
    //return client_response;
    return;
}

void print_time_log(FILE *log_file)
{
    long int ttime;
	ttime = time (NULL);
    char *time_char;
    time_char = ctime (&ttime); 
    if (time_char[strlen(time_char)-1] == '\n') 
        time_char[strlen(time_char)-1] = '\0';
    fprintf(log_file, "%s", time_char);
    printf("%s", time_char);
}

int main() 
{   	
    setlocale(LC_ALL, "Rus");

    // Для логфайла
    FILE *log_file = fopen("log_file_serv_c.txt","a+");

    // Информация сервера
    char server_message[256] = "It's server by c";
    char info_of_server[256] = "the server was written by Nikita Kilmishkin";

    // Создание сокета сервера
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Параметры сервера
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5050);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind the socket to our specified IP and port
    bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(sockfd, 5);

    // Логгирование времени запуска сервера
    print_time_log(log_file);
    fprintf (log_file, ", the server started listening\n");
    printf(", the server started listening\n");
    fflush(log_file);

    // Для информации о клиенте
    struct sockaddr_in clientInfo; 
    int clientInfo_size = sizeof(clientInfo);

    while (1)
    {
        intptr_t client_socket;

        // Принять соединение
        if((client_socket =  ) == -1)
        {
            print_time_log(log_file);
            fprintf (log_file, ", Failed to connect \n");
            printf(", Failed to connect\n");
            fflush(log_file);
            return -1;
        }

        // Печать информации о клиенте
        char buffer_addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientInfo.sin_addr, buffer_addr, sizeof( buffer_addr ));
        int buffer_port = (int) ntohs(clientInfo.sin_port);
        //printf( "client_address: %s:%d \n", buffer_addr,  buffer_port);

        // Логгирование времени присоединения клиента
        print_time_log(log_file);
        fprintf (log_file, ", a client with the address %s:%d has joined\n",buffer_addr,  buffer_port);
        printf( ", a client with the address %s:%d has joined\n", buffer_addr,  buffer_port);
        fflush(log_file);

        // Имитация загрузки сервера и маршрута
        sleep (2);

        // Принять сообщение
        char client_response[1000] = "";
        if(recv(client_socket, &client_response, sizeof(client_response), 0) == -1 )
        {
            print_time_log(log_file);
            fprintf (log_file, ", Failed to accept message\n");
            printf(", Failed to accept message\n");
            fflush(log_file);
            return -1;
        }
        
        // Логгирование времени получения сообщения
        print_time_log(log_file);
        fprintf (log_file, ", message received: %s\n", client_response);
        printf(", message received: %s\n", client_response);
        fflush(log_file);
        
        invert_str(client_response);
        strcat(client_response, ". ");
        strcat(client_response, info_of_server);
        client_response[strlen(client_response)] = '\0';

        // Отослать сообщение обратно
        if(send(client_socket, client_response, strlen(client_response), 0) == -1)
        {
            print_time_log(log_file);
            fprintf (log_file, ", Failed to send message\n");
            printf(", Failed to send message\n");
            fflush(log_file);
            return -1;
        }

        // Логгирование времени отправки сообщения сообщения
        print_time_log(log_file);
        fprintf (log_file, ", a message has been sent: %s\n", client_response);
        printf (", a message has been sent: %s\n", client_response);
        fflush(log_file);

        //Отключение клиента от сервера
        recv(client_socket, &client_response, sizeof(client_response), 0);

        // Логгирование времени отключения клиента от сервера
        print_time_log(log_file);
        fprintf (log_file, ", the client with %s:%d the address is disabled \n", buffer_addr,  buffer_port);
        printf (", the client with %s:%d the address is disabled \n",buffer_addr,  buffer_port);
        fflush(log_file);
    }

    // Закрытие сокета и логфайла
    close(sockfd); 
    close(log_file);
    
	return 0;
}