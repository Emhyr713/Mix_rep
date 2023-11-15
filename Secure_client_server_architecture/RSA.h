#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <string.h>
using namespace std;

#include <chrono>
 
//int CipherKey[4];
//int n, e = 0, d;
 
// Двоичное преобразование
int BianaryTransform(int num, int bin_num[])
{
	int i = 0,  mod = 0;
	// Преобразуется в двоичный, обратный временно сохраняется в массиве temp []
	while(num != 0)
	{
		mod = num%2;
		bin_num[i] = mod;
		num = num/2;
		i++;
	}
 
	 // Возвращает количество цифр в двоичных числах
	return i;
}
 
 // Повторное возведение в квадрат в степень
 // b = e
 // a = PublicKey
long long Modular_Exonentiation(long long a, int b, int n)
{
	int c = 0, bin_num[1000];
	long long d = 1;
	int k = BianaryTransform(b, bin_num)-1;
 
	for(int i = k; i >= 0; i--)
	{
		c = 2*c;
		d = (d*d)%n;
		if(bin_num[i] == 1)
		{
			c = c + 1;
			d = (d*a)%n;
		}
	}
	return d;
}
 
 // Генерация простых чисел в пределах 1000 есть
int ProducePrimeNumber(int prime[])
{
	int c = 0, vis[1001];
	memset(vis, 0, sizeof(vis));
	for(int i = 2; i <= 1000; i++)if(!vis[i])
	{
		prime[c++] = i;
		for(int j = i*i; j <= 1000; j+=i)
			vis[j] = 1;
	}
 
	return c;
}
 
 
 // Расширенный алгоритм Евклида 
int Exgcd(int m,int n,int &x)
{
	int x1,y1,x0,y0, y;
	x0=1; y0=0;
	x1=0; y1=1;
	x=0; y=1;
	int r=m%n;
	int q=(m-r)/n;
	while(r)
	{
		x=x0-q*x1; y=y0-q*y1;
		x0=x1; y0=y1;
		x1=x; y1=y;
		m=n; n=r; r=m%n;
		q=(m-r)/n;
	}
	return n;
}
 
 // Инициализация RSA 
 // Создание n, e
void RSA_Initialize(int *n, int *e, int *d)
{
	// Вынимаем простые числа в пределах 1000 и сохраняем их в массиве prime []
	int prime[5000];
	int count_Prime = ProducePrimeNumber(prime);
 
	// Случайно возьмем два простых числа p, q
	srand((unsigned)time(NULL));
	int ranNum1 = rand()%count_Prime;
	int ranNum2 = rand()%count_Prime;
	int p = prime[ranNum1], q = prime[ranNum2];
 
	*n = p*q;
 
	int On = (p-1)*(q-1);
 
	// Используем расширенный алгоритм Евклида, чтобы найти e, d
	for(int j = 3; j < On; j+=1331)
	{
		int gcd = Exgcd(j, On, *d);
		if( gcd == 1 && *d > 0)
		{
			*e = j;
			break;
		}
	}
}

// void RSA_Initialize_Prepare_msg(char* OpenValueChar, int *d)
void RSA_Initialize_Prepare_msg(char (&OpenValueChar)[128], int *d, int *n)
{
	int e = 0;

	while(!e)
		RSA_Initialize(n, &e, d);

	// cout << "Для передачи клиенту: " << endl;
	// cout << "e = " << e << endl;
	// cout << "n = " << *n << endl << endl;
	// cout << "Должно хранится у сервера: " << endl;
	// cout << "d = " << *d << endl << endl;
	// cout << "____________________________" << endl;

	string n_str = to_string(*n);
	string e_str = to_string(e);
	string n_e_lengh = to_string(n_str.length() + e_str.length());
	
    string OpenValue = n_e_lengh + " " + n_str + " " + e_str;
	strcpy(OpenValueChar, OpenValue.c_str());
}
 
//  // шифрование RSA
// int RSA_Encrypt(int PublicKey, int n, int e)
// {
// 	cout<<"Открытые данные (e, n) : e = "<< e <<" n = "<< n <<'\n';
// 	cout<<"Используя открытые данные, (e, n) шифруем число:"<<'\n';

// 	CipherKey = Modular_Exonentiation(PublicKey, e, n);
// 	cout<< CipherKey;
// 	cout<<'\n'<<'\n';
// 	return CipherKey;
// }

 // шифрование RSA
string RSA_Encrypt(int PublicKey[4], int n, int e)
{
	//cout << " ШИФРОВАНИЕ: " << endl;
	//cout<<"Открытые данные (e, n) : e = "<< e <<" n = "<< n <<'\n';
	//cout<<"Используя открытые данные, (e, n) шифруем число:"<<'\n';

	int CipherKey[4];
	string result;

	for(int i = 0; i < 4; i++)
		{
			//cout<< "PublicKey[i] " << PublicKey[i] << endl;
			CipherKey[i] = Modular_Exonentiation(PublicKey[i], e, n);
			//cout<< "CipherKey[i] " << CipherKey[i] << endl;
			result += to_string(CipherKey[i]) + " ";
		}

	//cout << "result: " << result;
	//cout<<'\n'<<'\n';

	return result;
}
 
 // Расшифровка RSA
string RSA_Decrypt(string EncryptMsg, int d, int n)
{	
	//cout << " ДЕШИФРОВАНИЕ: " << endl << endl;
	//cout<<"Закрытые данные (d, n) : d = "<< d <<" n = "<< n <<'\n';
	//cout<<"Используя закрытые данные (d, n) расшифровываем:"<<'\n';

	int  CipherKey[4];
	string DecryptCipherKey;
	string result;

	//cout << "EncryptMsg: " << endl;



	int pos = 0;
	int delta = 0;
	for(int i = 0; i < 4; i++)
	{
		string Temp;
		string Temp1;

		Temp1 = EncryptMsg.substr(pos, EncryptMsg.find(' '));
		CipherKey[i] = stoi(Temp1);

		delta = Temp1.length();
		EncryptMsg = EncryptMsg.substr (delta, EncryptMsg.length() - delta);

		int len = EncryptMsg.length();
		
		if (i == 3) Temp = EncryptMsg.substr (pos, len);
		else Temp = EncryptMsg.substr (pos + 1, len);

		EncryptMsg = Temp;

		//cout << "i " << i << " " << CipherKey[i] << endl;

		DecryptCipherKey = to_string(Modular_Exonentiation(CipherKey[i], d, n));
		result += DecryptCipherKey;
	}	



	// for(int i = 0; i < 4; i++)
	// {
	// 	DecryptCipherKey = to_string(Modular_Exonentiation(CipherKey[i], d, n));
	// 	result += DecryptCipherKey;
	// }

	return result;
}
 
 // Генерация открытого ключа
int Initialize()
{
	srand((unsigned)time(NULL));
	int PublicKey = rand()%1000;
	cout<<"Число для передачи клиентом: "<<'\n';
	cout<< PublicKey << endl;
	return PublicKey;
}

 // Извлечение из сообщения "n" и "e"
void ParseOpenValue(char (&OpenValueChar)[128], int *n, int *e)
{
	int i = 0;
	string Temp;
	//cout << "OpenValueChar: " << OpenValueChar << endl;

	while (OpenValueChar[i] != ' ')
	{
		Temp += OpenValueChar[i];
		i++;
	}
	int count = stoi(Temp);
	i++;

	Temp = "";
	while (OpenValueChar[i] != ' ')
	{
		Temp += OpenValueChar[i];
		i++;
		count--;
	}
	*n = stoi(Temp);
	i++;

	Temp = "";
	while (count > 0)
	{
		Temp += OpenValueChar[i];
		i++;
		count--;
	}
	*e = stoi(Temp);


	return;
}

 // Отправка через сокет открытых данных и получение зашифрованных
string SendOpenRecvSecur(intptr_t client_socket)
{	

        //Подготовка параметров для шифрования
        char n_e_Value[128] = "";
        int d = 0; 
        int n = 0;
        RSA_Initialize_Prepare_msg(n_e_Value, &d, &n);

        // Отправка открытых значений шифрования на клиенту
        if(send(client_socket, n_e_Value, strlen(n_e_Value), 0) == -1)
        {
            printf("Failed to send message\n");
            return "Failed to send message";
        }
        //printf ("a message has been sent: %s\n", n_e_Value);

        // Принятие зашифрованного ключа от клиента
        char client_response[1024] = "";
        if(recv(client_socket, &client_response, sizeof(client_response), 0) == -1 )
        {
            printf("Failed to accept message\n");
            return "Failed to accept message\n";
        }
       // printf ("a message has been received: %s\n", client_response);

		
		string result = RSA_Decrypt(string(client_response), d, n);



		// cout << "1	Отправлено клиенту: " << endl
		// << "	n_e_Value: " << n_e_Value << endl;

		// cout << "1	Получено от клиента: " << endl
		// << "	client_response: " << client_response << endl;

		// cout << "1	В результате функции: " << endl
		// << "	result: " << result << endl;
		// cout << "____________________________" << endl;

	return result;
}


 // Получение открытых данных и отправка зашифрованных данных через сокет
string RecvOpenSendSecur(intptr_t sockfd)
{
	// Приём открытых данных от сервера
	char server_response[128];
	if(recv(sockfd, &server_response, sizeof(server_response), 0) == -1 )
	{
		printf("Failed to accept message\n");
        return "Failed to accept message";
	}
	server_response[strlen(server_response)] = '\0';

	int n = 0;
	int e = 0;
	ParseOpenValue(server_response, &n, &e);

	// С помощью открытых данных шифруем число
	int PublicKey[4] = {1111, 1111, 1111, 1111};	// Сделать рандом

	string result;
	// Останется у клиента
	for(int i = 0; i < 4; i++)
			result += to_string(PublicKey[i]);

	// Отправлено серверу						
	string CipherKey = RSA_Encrypt(PublicKey, n, e);

	char message_to_server[128];
	strcpy(message_to_server, CipherKey.c_str());

    if(send(sockfd, message_to_server, sizeof(message_to_server), 0)== -1)
	{
        printf("Failed to send message\n");
        return "Failed to send message";
	}
	//cout << "message_to_client: " << message_to_server << endl;

	// cout << "2	Получено от сервера: " << endl
	// << "	server_response: " << server_response << endl;

	// cout << "2	Отправлено серверу: " << endl
	// << "	message_to_server: " << message_to_server << endl;

	// cout << "2	В результате функции: " << endl
	// << "	result: " << result << endl;
	// cout << "____________________________" << endl;

	return result;
}