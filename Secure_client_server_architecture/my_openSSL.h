#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <resolv.h>
#include <netdb.h>

#include <iostream>
using namespace std;

#include "aes_1 copy.h"
#include "RSA.h"

#include <chrono>

#define FAIL    -1

struct Params{
    int s_server;               // сокет сервера
    int s_client;               // сокет клиента
    int choice;

    SSL *ssl     = NULL;        // Для OpenSSL
    SSL_CTX *ctx = NULL;        // Для OpenSSL

    string key;                 // Для самописного шифратора
};

// Печать сертефиката
void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);   // Получение сертификата сервера
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);                         // Высвобождение памяти, занятой строкой
        X509_free(cert);                    // Высвобождение памяти, занятой копией сертификата
    }
    else
        printf("Info: No client certificates configured.\n");
    cout << "-------------------------" << endl;
}



// Инициализация доступных методов шифрования
//______________________
// Для клиента
SSL_CTX* InitCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();           // Загрузка всех шифронаборов
    SSL_load_error_strings();               // Регистрация ошибок
    method = TLSv1_2_client_method();       // Создание нового экземпляра клиентского метода
    ctx = SSL_CTX_new(method);              // Создание нового контекста

    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

// Для сервера
SSL_CTX* InitServerCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();           // Загрузка всех шифронаборов
    SSL_load_error_strings();               // Загрузка всех сообщений ошибок
    method = TLSv1_2_server_method();       // Создание нового экземпляра серверного метода
    ctx = SSL_CTX_new(method);              // Создание нового контекста
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
//______________________


// Загрузка сертификата
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    // Установление сертификата из файла сертификата
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    // Установление закрытого ключа из файла ключа
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    // Проверка закрытого ключа 
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

// // Отправка сообщения
// int SendMessage(Params info, char Request[1024], int socket)
// {
//     if(info.choice == 1)
//         SSL_write(info.ssl, Request, strlen(Request));  
//     else
//         {
//             string Msg = Request;
//             SendEncryptMsg(socket, info.key, Msg);
//         }
    
//     cout << "Sended:" << Request << endl;
//     return 0;
// }

// Отправка сообщения
int SendMessage(Params info, char Request[1024], int sizeMsg, int socket)
{
    if(info.choice == 1)
        SSL_write(info.ssl, Request, sizeMsg);  
    else
    {
        string Msg(Request, sizeMsg);
        SendEncryptMsg(socket, info.key, Msg);
    }
    
    //cout << "Sended:" << Msg << endl;
    // for(int i = 0; i < 1000; i++)
    // {
    //     printf("%c", Request[i]);
    // }
    //cout << "________________________________" << endl;
    
    return 0;
}


// Получение сообщения
string RecvMessage(Params info, int socket, int sizeMsg)
{
    char buf[1024] = {0};
    int bytes;
    //int sizeMsg = 1000;
    string Result;

    if(sizeMsg == 0) sizeMsg = sizeof(buf);

    if(info.choice == 1)
    {
        bytes = SSL_read(info.ssl, buf, sizeMsg);
        buf[bytes] = 0;

        if(sizeMsg == 0) Result = buf;
        else Result = string(buf, sizeMsg);

        // for(int i = 0; i < 1000; i++)
        // {
        //     printf("%c", buf[i]);
        // }
        // cout << "________________________________" << endl;
    }
    else
    {
        Result = RecvEncryptMsg(socket, info.key);
        //cout << "Received: " << Result << endl;
    }


       
    //cout << "Received:" << endl; cout << Result << endl;
    return Result;
}

//_________________________________________________________
// Для сервера
// Функция обмена сообщениями
// Если choice == 1, то через OpenSSL
// Если choice == 0, то через самописный шифратор

void serverInit(SSL **ssl, SSL_CTX **ctx, int *server)
{
    // Опционально
    *ctx = InitServerCTX();             // Инициализация методов шифрования;
    SSL_library_init();                 // Инициализация библиотеки SSL
                                        // Загрузка файлов сертификата
    LoadCertificates(*ctx, "../certificates/cert.pem", "../certificates/key.pem");
}


int AcceptClientByServer(Params *info, int *time)
{   
    // Принятие соединения
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    auto begin = std::chrono::steady_clock::now(); 
    
    cout << "-------------------------" << endl;
    info->s_client = accept(info->s_server, (struct sockaddr*)&addr, &len);  
    printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    cout << "info->choice " << info->choice << endl;

    auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    *time = elapsed_ms.count();

    // Если через OpenSSL
    if(info->choice == 1)
    {
        info->ssl = SSL_new(info->ctx);               // Создание нового SSL подключения;
        SSL_set_fd(info->ssl, info->s_client);        // Подключение сокета к SSL

        if ( SSL_accept(info->ssl) == FAIL ) 
            return -1;
    }

    // Если через самописный шифронабор
    if(info->choice == 0)
    {
        info->key = SendOpenRecvSecur(info->s_client);
        cout << "key: " << info->key << endl;
        cout << "_________________________" << endl;
    }

    return info->s_client;
}




// Закрытие сокетов
void ClosingSocketsServer(Params *info, int end)
{

    //cout << "info->s_server " << info->s_server << endl;
    //cout << "info->s_client " << info->s_client << endl;

    // Если закрывается сокет клиента
    if(end == 0)
    {
        // Если используется библиотека OpenSSL
        if(info->choice == 1)
            SSL_free(info->ssl);                // Закрытие соединения SSL

        close(info->s_client);                  // Закрытие сокета клиента
    }
    
    // Если закрывается серверный клиент
    if(end == 1)
    {
        // Если используется библиотека OpenSSL
        if(info->choice == 1)
            SSL_CTX_free(info->ctx);            // Закрытие методов шифрования

        close(info->s_server);                  // Закрытие сокета сервера
    }
    
}
// Для сервера
//_________________________________________________________



//_________________________________________________________
// Для клиента
// Соединение по SSL
void clientInit(SSL **ssl, SSL_CTX **ctx, int *server)
{
    *ctx = InitCTX();                   // Инициализация методов шифрования;
    *ssl = SSL_new(*ctx);               // Создание SSL соединения
    // Инициализация библиотеки SSL
    SSL_library_init();
    SSL_set_fd(*ssl, *server);          // Прикрепление сокета к SSL
    if ( SSL_connect(*ssl) == -1 )      // Соединение по SSL
    {
        ERR_print_errors_fp(stderr);
        printf("SSL_connect(ssl) == FAIL\n");
        return;
    }
    // Печать шифронабора
    printf("Connected with %s encryption\n", SSL_get_cipher(*ssl));
    // Печать сертификата сервера
    ShowCerts(*ssl);   
}

// Функция обмена сообщениями для клиента
// Если choice == 1, то через OpenSSL
// Если choice == 0, то через самописный шифратор
void PrepareClient(Params *info)
{
    cout << "info->choice " << info->choice << endl;

    if(info->choice == 1)
        clientInit(&(info->ssl), &(info->ctx), &(info->s_server));
    else
    {
	    info->key = RecvOpenSendSecur(info->s_server);
	    cout << "PublicKey: " << info->key << endl;
    }
}

// Закрытие сокетов
void ClosingSocketsClient(Params *info)
{
    //cout << "info->s_server " << info->s_server << endl;
    //cout << "info->s_client " << info->s_client << endl;

    if(info->choice == 1)
    {
        SSL_free(info->ssl);                // Закрытие соединения SSL
        SSL_CTX_free(info->ctx);            // Закрытие методов шифрования
    }
    close(info->s_server);                  // Закрытие сокета
}

// Для клиента
//_________________________________________________________


// Подключение клиентом к серверу
int OpenConnection(Params *info)
{
    info->s_server = socket(PF_INET, SOCK_STREAM, 0);
    int server = info->s_server;
    
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5050);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(server, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        close(server);
        perror("error connect");
        //printf("error connect\n");
        abort();
    }

    // Установление шифрации
    PrepareClient(info);

    return server;
}

// Сервер включается в прослушивание для клиентов
int OpenListener(Params *info)
{
    //int sd;
    info->s_server = socket(PF_INET, SOCK_STREAM, 0);
    int sd = info->s_server;
    struct sockaddr_in addr;
    
    //int sd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5050);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("error bind");
        abort();
    }
    if ( listen(sd, 5) != 0 )
    {
        perror("error listen");
        abort();
    }

    cout << "Server has been started:" << " 127.0.0.1" << ":" << 5050 << endl;

    //Установка шифрации и загрузка сертификата OpenSSL
    if(info->choice == 1)
        serverInit(&(info->ssl), &(info->ctx), &(info->s_server));

    return sd;
}