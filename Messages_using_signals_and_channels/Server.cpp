#include <Windows.h>
#include <locale.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;
#define LEN 256
#define _CRT_SECURE_NO_WARNINGS
#define WINVER 0x0601 /*API Windows 7*/

int show_err(const char*);
//string revcByPipe(HANDLE fd);

HANDLE myCreatePipe(LPCWSTR path)
{
	HANDLE MyPipe = INVALID_HANDLE_VALUE;
	MyPipe = CreateNamedPipe(path,		/*имя канала*/
		PIPE_ACCESS_DUPLEX,				/* режим открытия канала (в данном случае - канал двунаправленный )*/
		PIPE_TYPE_BYTE | PIPE_WAIT,		/* режим работы канала (в данном случае - передача байт и блокирующий)*/
		PIPE_UNLIMITED_INSTANCES,		/* максимальное количество реализаций канала (т.е. сколько может быть клиентов)*/
		LEN,							/* размер выходного буфера в байтах (придется ждать освобождения)*/
		LEN - 10,						/* размер входного буфера в байтах (придется ждать освобождения)*/
		0,								/* время ожидания в миллисекундах (для блокирующего режима работы NMPWAIT_USE_DEFAULT_WAIT)*/
		NULL							/* адрес структуры с особыми атрибутами*/);

	if (MyPipe == INVALID_HANDLE_VALUE)
	{
		show_err("Ошибка создания именованного канала");
		return NULL;
	}
	if (!ConnectNamedPipe(MyPipe,		/*дескриптор именованного канала*/
		NULL))							/*для асинхронного режима		*/
	{
		show_err("Статус");
		CloseHandle(MyPipe);
		return NULL;
	}

	return MyPipe;
}

//void ShowFunc(char sendMessage[], int massive[], int iMassive)
//{
//	cout << "sendMessage in Func " << sendMessage << endl;
//	// Если не пуст
//	if (iMassive != 0)
//	{
//		cout << "massive[0].length() + 1: " << strlen(sendMessage) + to_string(massive[0]).length() + 1 << endl;
//		strcpy_s(sendMessage, strlen(sendMessage) + to_string(massive[0]).length() + 1, to_string(massive[0]).c_str());
//		for (int i = 1; i < iMassive; i++)
//		{
//			cout << "massive[0].length() + 1: " << strlen(sendMessage) + to_string(massive[0]).length() + 1 << endl;
//			strcat_s(sendMessage, strlen(sendMessage) + 2,  " ");
//			strcat_s(sendMessage, strlen(sendMessage) + to_string(massive[i]).length() + 1, to_string(massive[i]).c_str());
//		}
//	}
//	// Если пуст
//	else
//		strcpy_s(sendMessage, 13, "Массив пуст");
//}

//void Sort(int massive[], int iMassive)
//{
//	while (iMassive--)
//	{
//		bool swapped = false;
//
//		for (int i = 0; i < iMassive; i++)
//		{
//			if (massive[i] > massive[i + 1])
//			{
//				swap(massive[i], massive[i + 1]);
//				swapped = true;
//			}
//		}
//
//		if (swapped == false)
//			break;
//	}
//}

// Дерево
struct tnode
{
	int key;
	tnode* left;
	tnode* right;
};

// Добавление дерева
struct tnode* addnode(int x, tnode* tree) {
	if (tree == NULL) { // Если дерева нет, то формируем корень
		tree = new tnode; // память под узел
		tree->key = x;   // поле данных
		tree->left = NULL;
		tree->right = NULL; // ветви инициализируем пустотой
		//cout << "tree " << tree << endl;
	}
	else  if (x < tree->key)   // условие добавление левого потомка
		tree->left = addnode(x, tree->left);
	else    // условие добавление правого потомка
		tree->right = addnode(x, tree->right);
	return(tree);
}

// Функция удаления поддерева
void freemem(tnode* tree) {
	if (tree != NULL) {
		freemem(tree->left);
		freemem(tree->right);
		//free(tree->key);
		free(tree);
	}
}

// Печать дерева
void print_Tree(tnode* tree, int level)
{
	if (tree)
	{
		print_Tree(tree->right, level + 1);
		for (int i = 0; i < level; i++) cout << "   ";
		cout << tree->key << endl;
		print_Tree(tree->left, level + 1);
	}
}

int main() {
	setlocale(LC_ALL, "RU");

	printf("Сервер запущен\n");
	printf("имя канала \\\\.\\pipe\\name1\n");

	LPCWSTR path1 = L"\\\\.\\pipe\\name1";		// Имя первого канала
	LPCWSTR path2 = L"\\\\.\\pipe\\Pipe2";		// Имя второго канала
	char buf[LEN];

	int massive[LEN];
	int iMassive = 0;

	HANDLE fd = myCreatePipe(path1);			// Создание основного канала

	BOOL bRes = TRUE;

	// Объявление дерева
	struct tnode* root;
	root = NULL;

	while (bRes)
	{
		DWORD len = 0;
		bRes = ReadFile(fd, buf, sizeof(buf), &len, NULL);				//блокируется на время чтения

		char sendMessage[256] = {0};

		if (strcmp(buf, "show") == 0)									// Отображение массива
		{
			cout << "Получено: show" << endl;
			cout << "======================" << endl;
			cout << "Печать дерева" << endl;
			cout << "----------------------" << endl;
			print_Tree(root, 0);
			cout << "======================" << endl;

			//Sort(massive, iMassive);
			//ShowFunc(sendMessage, massive, iMassive);

		}
		else if (strcmp(buf, "add") == 0)								// Добавление в массив
		{
			cout << "Получено: add" << endl;

			// Оповещение, что готовы добавить число
			bRes = WriteFile(fd, "Введите число ", 14 + 1, &len, NULL);	//блокируется на время записи
			FlushFileBuffers(fd);

			// Создание канала, чтение из Pipe2 и его закрытие
			HANDLE Pipe2 = myCreatePipe(path2);
			bRes = ReadFile(Pipe2, buf, sizeof(buf), &len, NULL);		//блокируется на время чтения
			DisconnectNamedPipe(Pipe2);									//отключаемся от именованного канала
			CloseHandle(Pipe2);											//закрываем дескриптор канала

			//// Добавление числа
			//massive[iMassive] = atoi(buf);
			//cout << "Добавлено число: " << massive[iMassive] << endl;
			//iMassive++;

			// Добавление узла в дерево
			root = addnode(atoi(buf), root);
			cout << "Добавлено число: " << atoi(buf) << endl;

			// Оповещение, что число добавлено
			strcpy_s(sendMessage, "Добавлено число: ");
			strcat_s(sendMessage, buf);
		}
		else									// Ответ в крайнем случае
		{
			printf("Получено сообщение: %s\n", buf);
			strcpy_s(sendMessage, buf);
		}

		// Отправка клиенту
		//cout << "sendMessage: " << sendMessage << endl;
		bRes = WriteFile(fd, sendMessage, strlen(sendMessage) + 1, &len, NULL); //блокируется на время записи		
		FlushFileBuffers(fd);
	}

	DisconnectNamedPipe(fd);	//отключаемся от именованного канала
	CloseHandle(fd);			//закрываем дескриптор канала
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
