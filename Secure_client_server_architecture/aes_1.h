#include <stdint.h>
#include <stdio.h>
#include <string.h>

//#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

#include "aes.h"

#include <iostream>
using namespace std;

// Вывод данных
void printDec(uint8_t *ptr, int len, char *tag) {
    int count = 0;
    printf("%s\ndata[%d]: ", tag, len);
    for (int i = 0; i < len; ++i) {
        printf("%d ", *ptr++);
        count++;
    }
    printf("\n\n");
    cout << "count " << count << endl << endl;
}

void printHex(uint8_t *ptr, int len, char *tag) {
    printf("%s\ndata[%d]: ", tag, len);
    for (int i = 0; i < len; ++i) {
        printf("%.2X ", *ptr++);
    }
    printf("\n");
}

string AES_encrypt (string LongMessage, string PublicKey)
{
    // Из string в uint8_t
    const char *Convert = PublicKey.c_str();
    const uint8_t *key2 = (uint8_t*)Convert;

    // vector<uint8_t> myVector(PublicKey.begin(), PublicKey.end());
    // const uint8_t key2[] = &myVector[0];

    string EncryptedLongMessage;
    

    // Вычисление счётчика
    int lenMsg = LongMessage.length();

    string Temp = to_string(lenMsg) + " " + LongMessage;
    LongMessage = Temp;
    int pos1 = LongMessage.find(" ");    // position of " " in str
    lenMsg += pos1+1;

    int count = lenMsg/32;
    if (lenMsg%32 > 0) count ++;

    // Дозаполнение
    if(lenMsg%32 != 0)
    {
        int rest = (lenMsg/32 + 1) * 32 - lenMsg;
        for(int j = 0; j < rest; j++)
            LongMessage += "`";
    }

    //cout << "LongMessage: " << LongMessage << endl;
    // разбиение текстового файла на отрывки по 32 байта, его шифрование и склейка обратно
    for(int i = 0; i < count; i++)
    {
        int delta = 32;
        
        char Msg32[32];
        string Temp = LongMessage.substr(i*32, delta);
        Temp.copy(Msg32, delta);
        Msg32[delta] = '\0';                                 // Обязательно!!

        const uint8_t *data = (uint8_t*)Msg32;               // данные

        uint8_t ct2[32] = {0};    // Внешнее приложение для памяти выходных данных, используемое для хранения зашифрованных данных

        // Шифровать 32-байтовый открытый текст
        aesEncrypt(key2, 16, data, ct2, 32); 
        ct2[delta] = '\0';
        EncryptedLongMessage += (char *)ct2;
    }

    return EncryptedLongMessage;
}

string AES_decrypt (string EncryptedLongMessage, string PublicKey)
{
    // Из string в uint8_t
    const char *Convert = PublicKey.c_str();
    const uint8_t *key2 = (uint8_t*)Convert;

    string DecryptedLongMessage;
    // Вычисление счётчика
    int lenMsg = EncryptedLongMessage.length();
    //std::cout << "[DEBUG] EncryptedLongMessage =  " << EncryptedLongMessage << endl;
    int count = lenMsg/32;
    if (lenMsg%32 > 0) count ++;


    //разбиение текстового файла на отрывки по 32 байта, его расшифрование и склейка обратно
    for(int i = 0; i < count ; i++)
    {
        int delta = 32;

        char Msg32[32];
        string Temp = EncryptedLongMessage.substr(i*32, delta);
        Temp.copy(Msg32, delta);
        Msg32[delta] = '\0';                                // Обязательно!!

        const uint8_t *ct2 = (uint8_t*)Msg32;               // данные
        uint8_t plain2[32] = {0};      // Внешнее приложение для памяти выходных данных, используемое для хранения расшифрованных данных

	    // Расшифровка 32-байтового зашифрованного текста
        aesDecrypt(key2, 16, ct2, plain2, 32);  

        plain2[delta] = '\0';
        //cout << "plain2 " << i << " " << plain2 << endl;
        DecryptedLongMessage += (char *)plain2 ;
    }

    // Очищение
    string delimiter = " ";
    //cout <<"[DEBUG] DecryptedLongMessage = " << DecryptedLongMessage << endl;
    //cout << "[DEBUG] DecryptedLongMessage = ";  
    //cout << DecryptedLongMessage << endl;

    string token = DecryptedLongMessage.substr(0, DecryptedLongMessage.find(delimiter));
    int pos = DecryptedLongMessage.find(" ");    // position of "live" in str
    int Len = stoi(token);
    string ClearData = DecryptedLongMessage.substr (pos+1,Len);

    //cout << "ClearData: " << ClearData << endl;

    return ClearData;
}


 // Шифрование через aes и отправка через сокет, возвращает зашифрованные данные
string SendEncryptMsg(intptr_t sockfd, string Key, string Message)
{
    //cout << "Key " << Key << endl;

    // Шифрование:
    string EncryptedLongMessage = AES_encrypt(Message, Key);
    //cout << "Оригинал: " << endl; cout << Message << endl << endl;
    //cout << "EncryptedLongMessage: " << endl; cout << EncryptedLongMessage << endl << endl;

    char message_to_server[1024];
    EncryptedLongMessage.copy(message_to_server, EncryptedLongMessage.length());

    if(send(sockfd, message_to_server, sizeof(message_to_server), 0)== -1)
	{
        printf("Failed to send message\n");
        return "Failed to send message";
	}

	return EncryptedLongMessage;
}

 // Получение зашифрованных данных и их расшифровка, возвращает расшифрованные данные
string RecvEncryptMsg(intptr_t sockfd, string Key)
{
    // Приём зашифрованных данных через сокет
	char server_response[1024];
	if(recv(sockfd, &server_response, sizeof(server_response), 0) == -1 )
	{
		printf("Failed to accept message\n");
        return "Failed to accept message";
	}
	server_response[strlen(server_response)] = '\0';
    //cout << "server_response " << server_response << endl;
    
    
    // Расшифровка:
    string Message(server_response);
    string DecryptedLongMessage = AES_decrypt(Message, Key);
    //cout << "Оригинал: " << endl; cout << Message << endl << endl;
    //cout << "DecryptedLongMessage: " << endl; cout << DecryptedLongMessage << endl << endl;

	return DecryptedLongMessage;
}
