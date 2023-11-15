#include <unistd.h>
#include <stdio.h>

#include "../src/my_openSSL.h"
#include "../src/fileConvLib.h"
#include <openssl/md5.h>

#include <chrono>

#include <iostream>
using namespace std;

#define FAIL -1

// int main()
// {
//     int server = OpenListener();        // Создание сокета сервера
//     int choise = 1;

//     // Опционально
//     SSL_CTX *ctx = InitServerCTX();     // Инициализация методов шифрования;
//     SSL_library_init();                 // Инициализация библиотеки SSL
//     // Загрузка файлов сертификата
//     LoadCertificates(ctx, "../certificates/cert.pem", "../certificates/key.pem");
//     //____________

//     while (1)
//     {
//         // Принятие соединения
//         struct sockaddr_in addr;
//         socklen_t len = sizeof(addr);
//         int client = accept(server, (struct sockaddr*)&addr, &len);  
//         printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        
//         // Опционально
//         SSL *ssl = SSL_new(ctx);         // Создание нового SSL подключения;
//         SSL_set_fd(ssl, client);         // Подключение сокета к SSL
//         Servlet(ssl);                    // Получение от клиента, а затем отправка сервером
//         //____________
//     }

//     //Опиционально
//     SSL_CTX_free(ctx);                   // Закрытие методов шифрования

//     close(server);                       // Закрытие сокета сервера
//     return 0;
// }



int main()
{
    Params info;
    int AcceptTime;

    // Выбор метода шифрования
    info.choice = 1;

    int begin1 = 0;
    //auto begin1 = std::chrono::steady_clock::now() - std::chrono::steady_clock::now();
    // auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(begin1);
    // cout << elapsed_ms1.count() << "mcs" << endl;

    // Начало отсчёта
    auto begin = std::chrono::steady_clock::now();
    
    // Создание сокета сервера и установление защищенного соединения
    int socket_serv = OpenListener(&info);

    // Переменные для теста
    char ServerResponse1[1024] = "Hi, it`s server";
    char ServerResponse2[1024] = "Connection OK";

    char sendFile1[128] = "../Test111.txt";
    char sendFile2[128] = "../Test.txt";  
    char sendFile3[128] = "../Test_jpg.jpg";  
    char sendFile4[128] = "../Test_picture.jpg";


    while (1)
    {
        // Принятие подключения,
        // возможная подготовка к шифрованию через SSL  
        int socket_client = AcceptClientByServer(&info, &AcceptTime);
        //cout << "AcceptTime: " << AcceptTime << endl;
 
        // // Отправка сообщения                                  
        // SendMessage(info, ServerResponse2, strlen(ServerResponse2), socket_client);

        // Передача указанного файла
        // recvFile(info, socket_client);
        // sendFile(sendFile2, info, socket_client);

        // string Answer1 = RecvMessage(info, socket_client, 0);
        // SendMessage(info, ServerResponse1, strlen(ServerResponse1), socket_client);   // Отправка сообщения
        // string Answer2 = RecvMessage(info, socket_client, 0);

        string result = recvLooongMsg(info, socket_client);
        sendFile(sendFile4, info, socket_client);

        // Закрытие сокета клиента
        ClosingSocketsServer(&info, 0);

        cout << "__________________________________"<< endl;

        // Конец отсчёта
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
  	    std::cout << "Полное время без учёта accept: " << elapsed_ms.count() - begin1 - AcceptTime << "mcs" << std::endl;
        begin1 = elapsed_ms.count();

        // // Результаты
        // cout << "Answer1: " << Answer1 << endl;
        // cout << "Answer2: " << Answer2 << endl;

        //cout << "result: " << result << endl;

        string str = result;
        unsigned char result1[MD5_DIGEST_LENGTH];
        MD5((unsigned char*)str.c_str(), str.size(), result1);

        cout << "Хэш сообщения, принятого сервером:" << endl;
        for(long long c: result1)
        {
            printf("%.2X", (long long)c);
        }
        cout << endl;
    }

    
    ClosingSocketsServer(&info, 1);
    return 0;
}


        //int socket = socket_client;

        // // функция
        // // int recvLooongMsg(string Msg, int socket){
            
        // int count = 1;                      // Счётчик итераций получения
        // int part = 1000;                    // Получение файла по 1000 байт
        // int sizeFile;
        // string result;
        // int startMsg = 0;

        // for(int i = 0; istring Msg, 
        //     //cout << "RecvMsg: " << RecvMsg << endl;
        //     if(i == 0)
        //     {
        //         sizeFile = stoi(RecvMsg.substr(0, RecvMsg.find(" ")));    // Размер в int
        //         startMsg = RecvMsg.substr(0, RecvMsg.find(" ")).length() + 1;
        //         sizeFile += startMsg;
        //         count = sizeFile/part;                      // Количество переводов
        //         if(sizeFile%part > 0) count++;              // Поправка количества переводов для некратных файлов
        //     }

        //     // Если итерация последняя
        //     if(i == count - 1)
        //         sizeMsg = sizeFile%part;

        //     result += RecvMsg.substr(startMsg, sizeMsg);
        //     startMsg = 0;
        // }
        // //}

	// auto begin = steady_clock::now();

	// auto end = steady_clock::now();
	// auto elapsed_ms = duration_cast<std::chrono::microseconds>(end - begin);
  	// cout << "Время: " << elapsed_ms.count() << "mcs" << std::endl;


        // Получение сообщения
        // SendMessage(info, ServerResponse1, socket_client);
        // string result = RecvMessage(info, socket_client);
        // for(int j = 0;j < 64; j++)
        // {
        //     cout << result[j];
        // }
        // cout << endl;



        // //char RecvClient[1024];            // Буфер сообщения
        // int count = 1;                      // Счётчик итераций получения
        // int part = 1000;                    // Получение файла по 1000 байт

        // int sizeFile;
        // string strSizeFile;
        // string nameFile;

        // cout << "_________________________" << endl;
        // //ofstream fin("./result123.txt");
        // ofstream fin;
        // for(int i = 0; i < count; i++)
        // {
        //     // Получение сообщения
        //     string RecvClient = RecvMessage(info, socket_client);

        //     // Если получена первая часть файла
        //     if(i == 0)
        //     {

        //         // Извлечение имени и размера файла из первого сообщения
        //         strSizeFile = (RecvClient.substr(0, RecvClient.find(" ")));     // Размер в string 
        //         sizeFile = stoi(RecvClient.substr(0, RecvClient.find(" ")));    // Размер в int
        //         int pos = RecvClient.find(" ");                                 // индекс первого пробела в строке
        //         nameFile = RecvClient.substr(pos+1, RecvClient.find("\n") - (pos+1));

        //         // +2 компенсация за упущенный пробел и "\n"
        //         sizeFile += strSizeFile.length() + nameFile.length() + 2;

        //         // Подсчёт итераций приёмов файла
        //         count = sizeFile/part;
        //         if(sizeFile%part > 0) count++;
                
        //         fin.open("./Temp");
        //     }

        //     char Temp[1024];
        //     int sizeIteration = 1000;
        //     // Если итерация последняя
        //     if(i == count - 1)
        //         sizeIteration = sizeFile%part;

        //     // Перевод в char и запись в файл ./Temp
        //     RecvClient.copy(Temp, sizeIteration);
        //     fin.write(Temp, sizeIteration);

        // }

        // // cout << "nameFile: " << nameFile << endl;
        // // cout << "sizeFile: " << sizeFile << endl;
        // // cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

        // // Преобразование файла в "человеческий вид"
        // fin.close();  
        // TXTtoFile("./Temp");