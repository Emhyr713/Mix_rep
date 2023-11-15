#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

    string EncryptMessage;

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

    //cout << "LongMessage:" << LongMessage << endl;



    // int lenMsg = strlen(LongMessage);           // Количество символов
    // char Temp[1024 + 5];                        // Временная переменная для форматирования сообщения
    // char CharlenMsg[4];                         // Переменная для числа в char
    // sprintf(CharlenMsg, "%d", lenMsg);          // Перевод числа в char
    // strcpy(Temp, CharlenMsg);                   // Добавление размера сообщения
    // strcat(Temp, " ");                          // Добавление " "
    // strcat(Temp, LongMessage);                  // Добавление сообщения
    // cout << "Temp1:" << Temp << endl;           // Печать форматированного сообщения

    // lenMsg = strlen(Temp);                      // Обновление длины после манипуляций
    // int count = lenMsg/32;                      // Подсчёт итераций
    // if (lenMsg%32 > 0) count ++;                // Корректировка количества итераций

    // // Дозаполнение если сообщение не кратно 32
    // if(lenMsg%32 != 0)
    // {
    //     int rest = (lenMsg/32 + 1) * 32 - lenMsg;   // Подсчёт остатка для заполенния символами
    //     for(int j = 0; j < rest; j++)
    //         strcat(Temp, "`");                      // Дозаполнение символами
    // }

    //printf("EncryptMessage :\n");

    // Разбиение текстового файла на отрывки по 32 байта, его шифрование и склейка обратно
    for(int i = 0; i < count; i++)
    {
        // char Msg32[32];

        // memcpy(Msg32, &Temp[i*32], 32 );
        // Msg32[32] = '\0';

        char Msg32[32];
        string Temp = LongMessage.substr(i*32, 32);
        Temp.copy(Msg32, 32);
        Msg32[32] = '\0';   

        //cout << "Msg32:" << Msg32 << endl;
        //cout << "strlenMsg32:" << strlen(Msg32) << endl;

        const uint8_t *data = (uint8_t*)Msg32;      // данные
        uint8_t ct2[32] = {0};                      // Для хранения зашифрованных данных

        // Шифровать 32-байтовый открытый текст
        aesEncrypt(key2, 16, data, ct2, 32); 
        ct2[32] = '\0';

        //uint8_t plain2[32] = {0};                       // Для хранения расшифрованных данных

	    // // Расшифровка 32-байтового зашифрованного текста
        // aesDecrypt(key2, 16, ct2, plain2, 32);  
        // plain2[32] = '\0';


        // cout << endl << "Decr16____________________" << endl;
        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%.2X ", Msg32[j]);
        // }

        // cout << endl << "Decr_c____________________" << endl;
        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%c", Msg32[j]);
        // }

        // cout << endl << "Encrpt16____________________" << endl;
        // //printf("EncryptMessage :");
        // for (int j = 0; j < 32; j++) {
        //     //EncryptMessage += ct2[j];
        //     printf("%.2X ", ct2[j]);
        // }
        //cout << endl << "Encrpt_c____________________" << endl;

        // Было!!
        // printf("EncryptMessage :");
        // for (int j = 0; j < 32; j++) {
        //     EncryptMessage += ct2[j];
        //     printf("%c", ct2[j]);
        // }
        // cout  << endl;

        // Стало!!
        //printf("EncryptMessage1 :");
        string Temp1(ct2, ct2 + 32);
        // cout << Temp1 << endl;
        // cout << "______________________" << endl;
        EncryptMessage += Temp1;


        // cout << endl << "Msg16____________________" << endl;
        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%.2X ", Msg32[j]);
        // }
        // cout << endl << "Msg_c____________________" << endl;

        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%c", Msg32[j]);
        // }
        // cout << endl << "====================" << endl;
        // cout << endl << endl;

    }

    //cout << "Result: " << EncryptMessage << endl;
    //cout << endl << endl;
    return EncryptMessage;
}

string AES_decrypt (string EncryptedLongMessage, string PublicKey)
{
    // Из string в uint8_t
    const char *Convert = PublicKey.c_str();
    const uint8_t *key2 = (uint8_t*)Convert;

    string DecryptedLongMessage;
    // Вычисление счётчика
    int lenMsg = EncryptedLongMessage.length();
    int count = lenMsg/32;
    if (lenMsg%32 > 0) count ++;

    //cout << "EncryptedLongMessage:" << EncryptedLongMessage << endl;
    //cout << "lenMsg:" << lenMsg << endl;

    //


    //разбиение текстового файла на отрывки по 32 байта, его расшифрование и склейка обратно
    for(int i = 0; i < count ; i++)
    {
        // Конвертация из string в uint8_t
        char Msg32[32];
        string Temp = EncryptedLongMessage.substr(i*32, 32);
        Temp.copy(Msg32, 32);
        Msg32[32] = '\0';
        const uint8_t *ct2 = (uint8_t*)Msg32;           // Зашифрованные данные

        uint8_t plain2[32] = {0};                       // Для хранения расшифрованных данных

	    // Расшифровка 32-байтового зашифрованного текста
        aesDecrypt(key2, 16, ct2, plain2, 32);  
        plain2[32] = '\0';


        //         cout << endl << "Decr16____________________" << endl;
        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%.2X ", plain2[j]);
        // }

        // cout << endl << "Decr_c____________________" << endl;
        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%c", plain2[j]);
        // }

        // cout << endl << "Encrpt_c____________________" << endl;

        // for (int j = 0; j < 32; j++)
        // {
        //     printf("%c", ct2[j]);
        // }


        // // cout << endl << "Encr_c____________________" << endl;
        // // for (int j = 0; j < 32; j++)
        // // {
        // //     printf("%c", Msg32[j]);
        // // }
        // cout << endl << "====================" << endl;
        // cout << endl << endl;


        // Стало... Было
        // for (int j = 0; j < 32; j++)
        // {
        //     DecryptedLongMessage += plain2[j];
        //     //printf("%c", plain2[j]);
        // }

        // Стало!!
        //printf("DecryptedLongMessage :");
        string Temp1(plain2, plain2 + 32);
        DecryptedLongMessage += Temp1;
        // cout << Temp1 << endl;
        // cout << "______________________" << endl;

        
        // Было!
        //DecryptedLongMessage += (char *)plain2 ;
    }

    //cout << "DecryptedLongMessage:" << DecryptedLongMessage << endl;

    // Очищение
    // Нахождение начала исходного сообщения
    int pos = DecryptedLongMessage.find(" ");    
    // Извлечение размера исходного сообщения
    int Len = stoi(DecryptedLongMessage.substr(0, DecryptedLongMessage.find(" "))); 
    string ClearData = DecryptedLongMessage.substr (pos+1,Len);

    //cout << "ClearData: " << ClearData << endl;
    return ClearData;
}

// string AES_encrypt (char LongMessage[1024], string PublicKey)
// {
//     // Из string в uint8_t
//     const char *Convert = PublicKey.c_str();
//     const uint8_t *key2 = (uint8_t*)Convert;

//     string EncryptedLongMessage;
//     //cout << "LongMessage: " << LongMessage << endl;

//     // Вычисление счётчика
//     //int lenMsg = LongMessage.length();
//     int lenMsg = strlen(LongMessage);
//     char Temp[1024 + 8];

//     char CharlenMsg[4];
//     sprintf(CharlenMsg, "%d", lenMsg);
//     strcpy(Temp, CharlenMsg);
//     strcat(Temp, " ");
//     strcat(Temp, LongMessage);

//     return "";


//     // string Temp = to_string(lenMsg) + " " + LongMessage;
//     // LongMessage = Temp;
//     // int pos1 = LongMessage.find(" ");    // position of " " in str
//     // lenMsg += pos1+1;

//     // int count = lenMsg/32;
//     // if (lenMsg%32 > 0) count ++;



//     // // Дозаполнение
//     // if(lenMsg%32 != 0)
//     // {
//     //     int rest = (lenMsg/32 + 1) * 32 - lenMsg;
//     //     for(int j = 0; j < rest; j++)
//     //         LongMessage += "`";
//     // }
//     // // разбиение текстового файла на отрывки по 32 байта, его шифрование и склейка обратно
//     // for(int i = 0; i < count; i++)
//     // {
//     //     int delta = 32;
        
//     //     char Msg32[32];
//     //     string Temp = LongMessage.substr(i*32, delta);
//     //     Temp.copy(Msg32, delta);
//     //     Msg32[delta] = '\0';                                 // Обязательно!!

//     //     printf("output cipher text\n");
//     //     for (int i = 0; i < 32; ++i) {
//     //         printf("%c ", Msg32[i]);
//     //     }
//     //     cout << endl;

//     //     printf("output cipher text\n");
//     //     for (int i = 0; i < 32; ++i) {
//     //         printf("%d ", Msg32[i]);
//     //     }
//     //     cout << endl;

//     //     const uint8_t *data = (uint8_t*)Msg32;               // данные

//     //     uint8_t ct2[32] = {0};    // Внешнее приложение для памяти выходных данных, используемое для хранения зашифрованных данных

//     //     // Шифровать 32-байтовый открытый текст
//     //     aesEncrypt(key2, 16, data, ct2, 32); 
//     //     ct2[delta] = '\0';
//     //     EncryptedLongMessage += (char *)ct2;
//     // }

//     // return EncryptedLongMessage;
// }

// string AES_decrypt (string EncryptedLongMessage, string PublicKey)
// {
//     // Из string в uint8_t
//     const char *Convert = PublicKey.c_str();
//     const uint8_t *key2 = (uint8_t*)Convert;

//     string DecryptedLongMessage;
//     // Вычисление счётчика
//     int lenMsg = EncryptedLongMessage.length();
//     int count = lenMsg/32;
//     if (lenMsg%32 > 0) count ++;


//     //разбиение текстового файла на отрывки по 32 байта, его расшифрование и склейка обратно
//     for(int i = 0; i < count ; i++)
//     {
//         int delta = 32;

//         char Msg32[32];
//         string Temp = EncryptedLongMessage.substr(i*32, delta);
//         Temp.copy(Msg32, delta);
//         Msg32[delta] = '\0';                                // Обязательно!!

//         const uint8_t *ct2 = (uint8_t*)Msg32;               // данные
//         uint8_t plain2[32] = {0};      // Внешнее приложение для памяти выходных данных, используемое для хранения расшифрованных данных

// 	    // Расшифровка 32-байтового зашифрованного текста
//         aesDecrypt(key2, 16, ct2, plain2, 32);  

//         plain2[delta] = '\0';
//         //cout << "plain2 " << i << " " << plain2 << endl;
//         DecryptedLongMessage += (char *)plain2 ;
//     }

//     // Очищение
//     string delimiter = " ";
//     string token = DecryptedLongMessage.substr(0, DecryptedLongMessage.find(delimiter));
//     int pos = DecryptedLongMessage.find(" ");    // position of "live" in str
//     int Len = stoi(token);
//     string ClearData = DecryptedLongMessage.substr (pos+1,Len);

//     //cout << "ClearData: " << ClearData << endl;

//     return ClearData;
// }


 // Шифрование через aes и отправка через сокет, возвращает зашифрованные данные
string SendEncryptMsg(intptr_t sockfd, string Key, string Message)
{


    // Шифрование:
    string Temp = AES_encrypt(Message, Key);
    string EncryptedLongMessage = to_string(Temp.length()) + " " + Temp;


    //string EncryptedLongMessage = AES_encrypt(Message, Key);
    //char message_to_server[1024] = AES_encrypt(Message, Key);

    //cout << "Оригинал: " << endl; cout << Message << endl << endl;
    //cout << "EncryptedLongMessage: " << endl; cout << EncryptedLongMessage << endl << endl;

    char message_to_server[1024];
    EncryptedLongMessage.copy(message_to_server, EncryptedLongMessage.length());
    

    //cout << "message_to_server:" << message_to_server << endl;

    if(send(sockfd, message_to_server, sizeof(message_to_server), 0)== -1)
	{
        printf("Failed to send message\n");
        return "Failed to send message";
	}

    // cout << "send" << endl;
    // for(int j = 0;j < 1000; j++)
    // {
    //     cout << message_to_server[j];
    // }
    // cout << endl;

	return EncryptedLongMessage;
}

 // Получение зашифрованных данных и их расшифровка, возвращает расшифрованные данные
string RecvEncryptMsg(intptr_t sockfd, string Key)
{
    // Приём зашифрованных данных через сокет
	char server_response[1024];
	if(recv(sockfd, server_response, sizeof(server_response), 0) == -1 )
	{
		printf("Failed to accept message\n");
        return "Failed to accept message";
	}

    //cout << "server_response:" << server_response << endl;

    // cout << "recv" << endl;
    // for(int j = 0;j < 1000; j++)
    // {
    //     cout << server_response[j];
    // }
    // cout << endl;

    //сout << 

    // Подготовка к дешифрации
    string Temp(server_response, sizeof(server_response));
    int Len = stoi(Temp.substr(0, Temp.find(" "))); 
    int pos = Temp.find(" ");
    string EndcryptedMsg (&server_response[pos+1], Len);

    //cout << "EndcryptedMsg:" << EndcryptedMsg << endl;

    // Расшифровка:
    string DecryptedLongMessage = AES_decrypt(EndcryptedMsg, Key);
    //cout << "Оригинал: " << endl; cout << EndcryptedMsg << endl << endl;
    //cout << "DecryptedLongMessage: " << endl; cout << DecryptedLongMessage << endl << endl;

	return DecryptedLongMessage;
}
