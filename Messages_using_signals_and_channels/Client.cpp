#include <Windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#define LEN 256
#define _CRT_SECURE_NO_WARNINGS
#define WINVER 0x0601 /*API Windows 7*/

int show_err(const char*);

HANDLE myCreatePipeClient(	LPCWSTR path,
							DWORD &flags,			/*флаги типа канала*/
							DWORD &len_out,		/*размера выходного буфера*/
							DWORD &len_in,		/*размера входного буфера*/
							DWORD &cnt			/*количество реализаций*/,
							BOOL  &bRes)
{
	/*Ждем свободный канал бесконечно*/
	WaitNamedPipe(path, NMPWAIT_WAIT_FOREVER);
	/*открываем существующий канал*/
	HANDLE Pipe = CreateFile(path,          /*имя канала*/
		GENERIC_READ | GENERIC_WRITE,      /*режим доступа*/
		FILE_SHARE_READ | FILE_SHARE_WRITE,/*режим совместного использования*/
		NULL,                              /*особые атрибуты*/
		// защиты 
		OPEN_EXISTING,                     /*открывать существующий или ошибка*/
		0,                                 /*атрибуты файла*/
		NULL                               /*особые атрибуты*/);
	if (Pipe == INVALID_HANDLE_VALUE) {
		printf(" неудача!\n");
		show_err("Не удалось открыть существующий канал");
		return NULL;
	}
	printf(" ok!\n");
	/*получение свойств канала*/
	bRes = GetNamedPipeInfo(Pipe,       /*дескриптр канала*/
							&flags,   /*флаги типа канала*/
							&len_out, /*размера выходного буфера*/
							&len_in,  /*размера входного буфера*/
							&cnt      /*количество реализаций*/);
	if (!bRes)
	{
		show_err("Ошибка получения свойств канала");
		return NULL;
	}
	cout << "Свойства " << path << " канала:\n"
		<< "\tразмера выходного буфера : " << len_out << " байт\n"
		<< "\tразмера входного буфера\t : " << len_in << " байт\n"
		<< "\tколичество реализаций\t : " << cnt << " байт\n";
	//printf("Свойства именованного канала:\n\tразмера выходного буфера: %d байт\n\tразмера входного буфера: %d байт\n\tколичество реализаций: %d\n", len_out, len_in, cnt);
	return Pipe;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char name[LEN];
	char server[LEN];
	char path[LEN];

	LPCWSTR path1 = L"\\\\.\\pipe\\name1";
	LPCWSTR path2 = L"\\\\.\\pipe\\Pipe2";

	BOOL bRes = TRUE;
	DWORD len_in = 0, len_out = 0, cnt = 1, flags = 0;

	cout << "Клиент запущен" << endl;
	cout << "\\\\.\\pipe\\name1" << endl;
	cout << "Нажмите ENTER для соединения" << endl;

	while (getchar() != '\n');
	
	cout << "Соединение с " << path1 << " ..." << endl;

	HANDLE fd = myCreatePipeClient(path1, flags, len_out, len_in, cnt, bRes);

	// Копия запроса от клиента
	char AnswerFromServer[LEN];

	while (bRes) {
		DWORD len = 0;
		char* buf = (char*)malloc(len_out * sizeof(char));
		
		if (strcmp(AnswerFromServer, "add") == 0) printf("Введите число: ");
		else printf("Введите сообщение: ");

		fgets(buf, 256, stdin);							// Считать всю строку
		buf[strcspn(buf, "\n")] = 0;					// Убрать endline

		if (strcmp(AnswerFromServer, "add") == 0)		// Подготовка передачи данных через Pipe2
		{
			cout << "Пересылка через Pipe2: " << endl;
			DWORD len_in1 = 0, len_out1 = 0, cnt1 = 1, flags1 = 0;
			HANDLE Pipe2 = myCreatePipeClient(path2, flags1, len_out1, len_in1, cnt1, bRes);

			bRes = WriteFile(Pipe2, buf, strlen(buf) + 1, &len,	 NULL);

			if (!bRes || (strlen(buf) + 1 != len)) {
				show_err("Ошибка записи в канал");
				free(buf);
				break;
			}
			else
				printf("Отправлено по каналу 2.\n");

			CloseHandle(Pipe2);
		}
		else
		{
			bRes = WriteFile(fd, buf, strlen(buf) + 1, &len, NULL);
			printf("Отправлено.\n");
		}

		if (!bRes || (strlen(buf) + 1 != len)) {
			show_err("Ошибка записи в канал");
			free(buf);
			break;
		}			

		// Копирование запроса от клиента
		strcpy_s(AnswerFromServer, buf);

		// Очистка буфера
		free(buf);
		buf = (char*)malloc(len_in * sizeof(char));

		/*чтение из канала*/
		bRes = ReadFile(fd, buf, len_in, &len, NULL);
		if (!bRes || 0 == len) {
			show_err("Ошибка чтение из канала");
			free(buf);
			break;
		}
		else {
			buf[len] = '\0';
			printf("Получено: %s\n", buf);
		}
		free(buf);
	}
	/*закрываем дескриптор канала*/
	CloseHandle(fd);
	
	return 0;
}

int show_err(const char* msg) {
	int      no = GetLastError();
	WCHAR str_err[10000] = { 0 };
	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		no,
		MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT),
		(LPWSTR)&str_err,
		sizeof(str_err),
		NULL))
		printf("%s: %d\n%s\n", msg, no, str_err);
	else
		printf("%s:\nномер ошибки %d\n", msg, no);
	return no;
}