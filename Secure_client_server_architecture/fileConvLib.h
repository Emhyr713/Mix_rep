#include <iostream>
#include <cstring>

#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>

#include <unistd.h>

#include <openssl/md5.h>

using namespace std;

// Определение размера файла
off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

// Определение имени файла
string determinateName(char SourceFile[100])
{
    // Определение имени файла используя путь до файла
    int count = 0;
    int len = strlen(SourceFile);

    // Идти до '/'
    while(SourceFile[len - count-1] != '/')
        count++;

    // Скопировать от '/' до конца строки
    char NameFile[100];
    for(int j = 0; j < count; j++)
        NameFile[j] = SourceFile[len-count + j];
    NameFile[count] = 0;


    string result(NameFile);
    return result;
}

// Перезапись файла в текстовик
int fileToTXT(char SourceFile[100])
{
    FILE *main = fopen(SourceFile, "rb");

    // Файл результата
    char Result[100] = "./result.txt";
    FILE *ResultFile = fopen(Result, "wb+");

    // Проверка на открытие файла
    if(!main)
    {   
        cout << " Файл не открыт" << endl;
        return -1;
    }

    // Определение размера файла
    int sizeFile = (int)fsize(SourceFile);

    // Определение имени файла
    string NameFile = determinateName(SourceFile);

    // Записать имя файла и его размер в файл результата
    fprintf(ResultFile, "%d %s\n", sizeFile, NameFile.c_str());

    // переписываем информацию с файлами в файл main
    char byte[1];  // единичный буфер для считывания одного байта
    //int resSize = 0;
    while(!feof(main))
    {
        if(fread(byte,1,1,main)==1) fwrite(byte,1,1,ResultFile);
        //resSize++;
    }   

    // Закрытие файлов
    fclose(main);
    fclose(ResultFile);

    // Подсчёт количества размера файла
    ResultFile = fopen(Result, "rb");
    int resSize = (int)fsize(Result);
    fclose(ResultFile);

    return resSize;
}

// Перезапись текстовика в файл
int TXTtoFile(char SourceFile[100])
{
    FILE *fout = fopen(SourceFile, "rb");
    
    int sizeFile;
    char nameFile[100];

    fscanf(fout, "%d %s\n", &sizeFile, nameFile);

    //cout << "sizeFile: " << sizeFile << endl;
    //cout << "nameFile: " << nameFile << endl;
    
    FILE *fin = fopen(nameFile, "wb+");
    // Переписываем данные в файл fout
    char byte[1];  // единичный буфер для считывания одного байта
    while(!feof(fout))
    {
        if(fread(byte,1,1,fout)==1) fwrite(byte,1,1,fin);
    }

    cout << "Файл \"" << nameFile << "\" размером " <<  sizeFile << " байт получен и записан" << endl;

    fclose(fout);
    fclose(fin);
    return 0;
}


// Функция приёма файла
int recvFile(Params info, int socket)
{
    int count = 1;                      // Счётчик итераций получения
    int part = 1000;                    // Получение файла по 1000 байт

    int sizeFile;
    string strSizeFile;
    string nameFile;

    string HashResult;                  // Для сохранения всего файла в string

    ofstream fin;
    for(int i = 0; i < count; i++)
    {
        // Получение сообщения
        string RecvClient = RecvMessage(info, socket, part);

        // Проверка хэша
        HashResult += RecvClient;

        // Если получена первая часть файла
        if(i == 0)
        {

            // Извлечение имени и размера файла из первого сообщения
            strSizeFile = (RecvClient.substr(0, RecvClient.find(" ")));     // Размер в string 
            sizeFile = stoi(RecvClient.substr(0, RecvClient.find(" ")));    // Размер в int
            int pos = RecvClient.find(" ");                                 // индекс первого пробела в строке
            nameFile = RecvClient.substr(pos+1, RecvClient.find("\n") - (pos+1));

            // +2 компенсация за упущенный пробел и "\n"
            sizeFile += strSizeFile.length() + nameFile.length() + 2;

            // Подсчёт итераций приёмов файла
            count = sizeFile/part;
            if(sizeFile%part > 0) count++;
            
            fin.open("./Temp");
        }

        char Temp[1024];
        int sizeIteration = 1000;
        // Если итерация последняя
        if(i == count - 1)
            sizeIteration = sizeFile%part;

        // Для Хэша
        // string Temp1(RecvClient, sizeIteration);
        // HashResult += Temp1;

        // Перевод в char и запись в файл ./Temp
        RecvClient.copy(Temp, sizeIteration);
        fin.write(Temp, sizeIteration);

    }

    //Хэш
    string str = HashResult;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)str.c_str(), str.size(), result);

    //cout << "Хэш полученного файла:" << endl;
    for(long long c: result)
    {
        //printf("%.2X", (long long)c);
    }
    //cout << endl;

    // cout << "nameFile: " << nameFile << endl;
    // cout << "sizeFile: " << sizeFile << endl;
    // cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Преобразование файла в "человеческий вид"
    fin.close();  
    TXTtoFile("./Temp");
    //remove("Temp");

    return 0;
}

// Функция отправки файла
int sendFile(char PathFile[128], Params info, int socket)
{
    int size = fileToTXT(PathFile);         // Конвертация файла с получением его размера
    int part = 1000;                        // Число одного перевода
    int count = size/part;                  // Количество переводов
    if(size%part > 0) count++;              // Поправка количества переводов для некратных файлов

    string HashResult;                      // Для сохранения всего файла в string

    ifstream f("./result.txt");             // Промежуточный результат
    
    for(int i = 0;i < count; i++)
    {
        char SendFromClient[1024];
        int sizeMsg = part;
        if(i == (count-1)) sizeMsg = size%part; // Если перевод последний

        f.read(SendFromClient, sizeMsg);    // Чтение файла заданного количества байт
        SendFromClient[sizeMsg] = 0;        // Установка терминального нуля

        // Для Хэша
        string Temp(SendFromClient, sizeMsg);
        HashResult += Temp;

        // Отправка
        SendMessage(info, SendFromClient, sizeMsg, socket);

        //cout << "sizeMsg: " << sizeMsg << endl;
    }

    //cout << "size: " << size << endl;

    f.close();

    //Хэш
    string str = HashResult;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)str.c_str(), str.size(), result);

    //cout << "Хэш отправленного файла:" << endl;
    for(long long c: result)
    {
        //printf("%.2X", (long long)c);
    }
    //cout << endl;

    cout << "Файл \"" << PathFile << "\"полностью отправлен" << endl;
    return 0;
}


// Передача длинного сообщения, хранящегося в string
int sendLooongMsg(string Msg, Params info, int socket)
{
    int size = Msg.length() + to_string(Msg.length()).length() + 1;
    int part = 1000;                        // Число одного перевода
    int count = size/part;                  // Количество переводов
    if(size%part > 0) count++;              // Поправка количества переводов для некратных файлов

    string MsgToSend = to_string(Msg.length()) + " " + Msg;
    // cout << "size Msg: " << Msg.length() << endl;
    // cout << "count: " << Msg.length() << endl;
    //cout << "Temp: " << Temp << endl;
    
    for(int i = 0;i < count; i++)
    {
      char charSendMsg[1024];
      int sizeMsg = part;
      if(i == (count-1)) sizeMsg = size%part; // Если перевод последний

      (MsgToSend.substr(i*1000, i*1000+sizeMsg)).copy(charSendMsg, sizeMsg);
      charSendMsg[sizeMsg] = 0;            // Установка терминального нуля

      // Отправка
      SendMessage(info, charSendMsg, sizeMsg, socket);
    }

    return size;
}

// Приём длинного сообщения, хранящегося в string
string recvLooongMsg(Params info, int socket)
{
            
    int count = 1;                      // Счётчик итераций получения
    int part = 1000;                    // Получение файла по 1000 байт
    int sizeFile;
    string result;
    int startMsg = 0;

    for(int i = 0; i < count; i++)
    {
        int sizeMsg = part;
        string RecvMsg = RecvMessage(info, socket, part);

        //cout << "RecvMsg: " << RecvMsg << endl;
        if(i == 0)
        {
            sizeFile = stoi(RecvMsg.substr(0, RecvMsg.find(" ")));    // Размер в int
            startMsg = RecvMsg.substr(0, RecvMsg.find(" ")).length() + 1;
            sizeFile += startMsg;
            count = sizeFile/part;                      // Количество переводов
            if(sizeFile%part > 0) count++;              // Поправка количества переводов для некратных файлов
        }

        // Если итерация последняя
        if(i == count - 1)
            sizeMsg = sizeFile%part;

        result += RecvMsg.substr(startMsg, sizeMsg);
        startMsg = 0;
    }

    return result;
}