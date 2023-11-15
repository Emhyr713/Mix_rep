#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include <ctime>
using namespace std;

const int count_petitions = 10000;		// Количество обращений
const int print_flag = 0;				// Печать (ДА/НЕТ)
const int count_page = 10;				// Количество используемых страниц
const int count_page_RAM = 6;			// Доступные страницы памяти
const int need_create = 1;				// Необходимость создать файл с случайными обращениями (ДА/НЕТ)

// Имя файлика
//const char file_name[] = "thread_petitions.txt";
//const char file_name[] = "thread_petitions_40.txt";
//const char file_name[] = "thread_petitions_50.txt";
const char file_name[] = "thread_petitions_100.txt";
//const char file_name[] = "thread_petitions_1000.txt";

struct petitions {
	// По заданию
	int num_petition;					// Порядковый номер обращения
	int num_page;						// Номер страницы обращения
	int time_petition;					// Время обращения
	int will_write;						// Запись (ДА/НЕТ)
};

// Чтение из файла в массив
int read_to_massive(petitions (*massive_petitions)[10000], int& count, const char file_name[]) {

	count = 0;
	ifstream fin(file_name);
	if (!fin) {
		cout << "Ошибка открытия файла" << endl;
		return -1;
	}

	// Для проверки символа конца строки / конца информации о процессе
	char check = ',';

	while (check != ';') {
		fin >> (*massive_petitions)[count].num_petition;
		fin >> (*massive_petitions)[count].num_page;
		fin >> (*massive_petitions)[count].time_petition;
		fin >> (*massive_petitions)[count].will_write;
		fin >> check;
		if (check != ',' && check != ';') {
			cout << "Ошибка целостности файла" << endl;
			return -2;
		}
		count++;
	}
	count;

	fin.close();
	return 0;
}

// Печать прочтённого массива из файла
void print_massive(petitions (*massive_petitions)[10000], int count) {
	setlocale(LC_ALL, "Russian");

	if (count == 0) {
		cout << "Массив пуст" << endl;
		return;
	}

	cout << "count: " << count;
	cout << " \t\t\t п е ч а т ь    м а с с и в а    о б р а щ е н и й:";
	
	cout << endl << "Порядковый номер:\t\t";
	for (int i = 0; i < count; i++) cout << setfill(' ') << setw(2) << (*massive_petitions)[i].num_petition << " " << setw(2);
	cout << endl << "Номер страницы обращения:\t";
	for (int i = 0; i < count; i++) cout << setfill(' ') << setw(2) << (*massive_petitions)[i].num_page << " " << setw(2);
	cout << endl << "Время обращения:\t\t";
	for (int i = 0; i < count; i++) cout << setfill(' ') << setw(2) << (*massive_petitions)[i].time_petition << " " << setw(2);
	cout << endl << "Запись (да/нет):\t\t";
	for (int i = 0; i < count; i++) cout << setfill(' ') << setw(2) << (*massive_petitions)[i].will_write << " " << setw(2);

	cout << endl << endl;
	return;
}

// Шапка для печати таблички
void shapka(int count_page_RAM) {
	setlocale(LC_ALL, "C");
	int setWide = 5;
	int setPrecision = 2;

	cout << "              NUM Page RAM" << endl;

	//		┌────┬────┬─────┐
	cout << "            " << char(218);			// ┌	
	for (int i = 0; i < count_page_RAM - 1; i++) {		// ────┰
		cout << setfill(char(196)) << setw(setWide) << char(194);
	}
	cout << setw(setWide) << char(191) << endl;			// ────┐

	cout << "   TAKT Page";
	for (int i = 0; i < count_page_RAM; i++) {
		cout << setprecision(setPrecision) << setfill(' ') << char(179) << setw(3) << i << setw(2);
	}
	cout << char(179) << " P_F" << endl;


	//┌────┼────┼────┼────┐
	cout << "  " << char(218);
	for (int i = 0; i < count_page_RAM + 2; i++) {	// ────┰
		cout << setfill(char(196)) << setw(setWide) << char(197);
	}
	cout << setw(setWide) << char(191) << endl;			// ────┐
}

// Пузырьковая сортировка
void sort_massive(petitions(*massive_process)[10000], int count) {
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			if ((*massive_process)[j + 1].time_petition < (*massive_process)[j].time_petition)
				swap((*massive_process)[j], (*massive_process)[j + 1]);
		}
	}
}

// Алгоритм менеджера памяти FIFO
void algoritm_FIFO(petitions (*massive_petitions)[10000], int count, int count_page_RAM, int print_flag) {
	if (print_flag == 1) shapka(count_page_RAM);
	int setWide = 5;
	int setPrecision = 2;

	int takts = 0;
	int completed_petition = 0;
	int current_petition = 0;
	int count_page_F = 0;


	int massive_RAM[10] = {-1};		// Массив страниц RAM
	for (int i = 0; i < 10; i++) massive_RAM[i] = -1;

	while (completed_petition < count) {
		while ((*massive_petitions)[current_petition].time_petition != takts) {
			takts++;
		}

		// Печать тактов и требуемой страницы
		if (print_flag == 1) cout << setfill(' ') << setw(3) << char(179) << "\x1B[34m" << setw(4) << takts << "\033[0m";
		if (print_flag == 1) cout << setfill(' ') << char(179) << "\x1B[35m" << setw(4) << (*massive_petitions)[current_petition].num_page << "\033[0m" << char(186);
		
		int find_page = -1;
		for (int i = 0; i < count_page_RAM; i++) {
			if (massive_RAM[i] == (*massive_petitions)[current_petition].num_page) {
				find_page = i;
			}
		}

		// Добавление в RAM, если страница не найдена
		if (find_page == -1) {
			for (int i = 0; i < count_page_RAM - 1; i++) {
				massive_RAM[i] = massive_RAM[i + 1];
			}
			massive_RAM[count_page_RAM - 1] = (*massive_petitions)[current_petition].num_page;
			count_page_F++;
		}

		if (print_flag == 1) {
			// Печать данных в RAM
			for (int i = 0; i < count_page_RAM; i++) {
				if (massive_RAM[i] == -1) cout << setfill(' ') << "\x1B[33m" << setw(4) << "Empt" << "\033[0m" << char(179);
				else cout << setfill(' ') << setw(4) << massive_RAM[i] << char(179);
			}

			// Печать, если page_fault
			if (find_page == -1) {
				cout << setfill(' ') << setw(4) << "\x1B[31m P_F\033[0m" << char(179);
			}
			else {
				cout << setfill(' ') << setw(4) << " \x1B[32m " << char(251) << "\033[0m " << char(179);
			}

			cout << endl;
			//├────┼────┼────┼─────┤
			cout << "  " << char(195);
			for (int i = 0; i < count_page_RAM + 2; i++) {
				cout << setfill(char(196)) << setw(setWide) << char(197);
			}
			cout << setw(setWide) << char(180);
			cout << endl;
		}
		completed_petition++;
		takts++;
		current_petition++;
	}
	cout << "FIFO:" << endl;
	cout << "____________________" << endl;
	cout << "count P_F: " << count_page_F << endl;
	cout << "count P_F / count: " << (double)count_page_F  / (double)count << endl;
	return;
}

// Поиск минимальной частоты обращений страниц
int find_min_frequency_NFU(int massive_RAM[10], int massive_frequency[10], int count_page_RAM) {
	// Если RAM не занята полностью, то найти и вернуть начало свободного места
	int i = 0;
	while (massive_RAM[i] == -1 && i < count_page_RAM) i++;
	if (i > 0) {
		i--;
		return i;
	}

	// Если в RAM нет свободного места
	int min_count_petition = 0;
	int min_count = massive_frequency[massive_RAM[0]];
	//cout << "min_count: " << min_count << "||";
	for (int i = 0; i < count_page_RAM; i++)
	{
		int freq = massive_frequency[massive_RAM[i]];
		//cout << " i:" << i << ": " << massive_RAM[i];
		//cout << "freq: " << freq << " ";
		if (freq < min_count) {
			min_count = massive_frequency[massive_RAM[i]];
			min_count_petition = i;
		}
	}
	//cout << "min_count_petition: " << massive_RAM[min_count_petition] << "(" << min_count << ")";
	return min_count_petition;
}

void algoritm_NFU(petitions (*massive_petitions)[10000], const int count, const int count_page_RAM, const int count_page, int print_flag) {
	if (print_flag == 1) shapka(count_page_RAM);
	int setWide = 5;
	int setPrecision = 2;

	int takts = 0;
	int completed_petition = 0;
	int current_petition = 0;
	int count_page_F = 0;

	// Массив счётчиков обращений к страницам
	int massive_frequency[10] = { 0 };

	// Массив страниц RAM
	int massive_RAM[10] = { -1 };		
	for (int i = 0; i < count_page_RAM; i++) massive_RAM[i] = -1;

	while (completed_petition < count) {
		// Пока нет тактов скип
		while ((*massive_petitions)[current_petition].time_petition != takts) {
			takts++;
		}

		// Печать тактов и требуемой страницы
		if (print_flag == 1) cout << setfill(' ') << setw(3) << char(179) << "\x1B[34m" << setw(4) << takts << "\033[0m";
		if (print_flag == 1) cout << setfill(' ') << char(179) << "\x1B[35m" << setw(4) << (*massive_petitions)[current_petition].num_page << "\033[0m" << char(186);

		// Увеличение счётчика
		massive_frequency[(*massive_petitions)[current_petition].num_page]++;

		// Поиск
		int find_page = -1;		// Флаг нахождения страницы
		for (int i = 0; i < count_page_RAM; i++) {
			if (massive_RAM[i] == (*massive_petitions)[current_petition].num_page) {
				find_page = i;
			}
		}

		int index_RAM = -1;		// Первоначальный индекс массива RAM, который будет заменяться
		int page_prev = 0;		// Сохранение страницы, которую будут заменять
		// Добавление в RAM, если страница не найдена
		if (find_page == -1) {
			index_RAM = find_min_frequency_NFU(massive_RAM, massive_frequency, count_page_RAM);
			page_prev = massive_RAM[index_RAM];
			massive_RAM[index_RAM] = (*massive_petitions)[current_petition].num_page;
			count_page_F++;
		}

		if (print_flag == 1) {
			// Печать данных в RAM
			for (int i = 0; i < count_page_RAM; i++) {
				if (massive_RAM[i] == -1) cout << setfill(' ') << "\x1B[33m" << setw(4) << "Empt" << "\033[0m" << char(179);
				else cout << setfill(' ') << setw(4) << massive_RAM[i] << char(179);
			}

			// Печать, если page_fault
			if (find_page == -1)
				cout << setfill(' ') << setw(4) << "\x1B[31m P_F\033[0m" << char(179);
			else
				cout << setfill(' ') << setw(4) <<  " \x1B[32m "<< char(251) << "\033[0m " << char(179);

			if (find_page == -1) cout << "change: RAM[" << index_RAM << "] = " << page_prev << "->" << massive_RAM[index_RAM] << " || ";
			if (find_page != -1) cout << setfill(' ') << setw(26);
			for (int i = 0; i < count_page; i++) {
				if (find_page == -1 && massive_RAM[index_RAM] == i) cout << i << ":" << "\x1B[31m"<< massive_frequency[i] << "\033[0m" << " | ";
				else cout << i << ":" << massive_frequency[i] << " | ";
			}

			cout << endl;
			//├────┼────┼────┼─────┤
			cout << "  " << char(195);
			for (int i = 0; i < count_page_RAM + 2; i++) {
				cout << setfill(char(196)) << setw(setWide) << char(197);
			}
			cout << setw(setWide) << char(180);
			cout << endl;
		}
		completed_petition++;
		takts++;
		current_petition++;
	}

	cout << "NFU:" << endl;
	cout << "____________________" << endl;
	cout << "count P_F: " << count_page_F << endl;
	cout << "count P_F / count: " << (double)count_page_F / (double)count << endl;
	return;
}

// Создание случайных обращений и запись в файл
int create_petition(int count, int count_page, const char file_name[]) {
	// Открытие файла
	ofstream fout(file_name);
	if (!fout) {
		cout << "Ошибка открытия файла" << endl;
		return -1;
	}

	int massive_repetition[20001] = {0};	// Массив учёта повторов
	int flag_repitition = 1;				// Флаг повтора

	srand(time(0));
	int start_page = 0;						// Генерация номеров страниц от 0 до count_page - 1 
	int end_page = count_page - 1;
	int start_time = 0;						// Генерация такта обращения от 0 до 2*count
	int end_time = 2*count;
	for (int i = 0; i < count; i++) {
		// Генерация случайных номеров страниц и такта обращения к RAM
		int num_page = rand() % (end_page - start_page + 1) + start_page;
		int time_petition = rand() % (end_time - start_time + 1) + start_time;

		// Исключение повторов
		while (flag_repitition == 1) {
			if (massive_repetition[time_petition] == 1)		flag_repitition = 1;
			else											flag_repitition = 0;
			if (flag_repitition == 0) {
				massive_repetition[time_petition] = 1;
			}
			if (flag_repitition == 1) {
				time_petition = rand() % (end_time - start_time + 1) + start_time;
			}
		}

		if (i < count - 1)
			fout << i << " " << num_page << " " << time_petition << " " << 0 << " , ";
		if (i == count - 1)
			fout << i << " " << num_page << " " << time_petition << " " << 0 << " ; ";

		// Восстановление флага
		flag_repitition = 1;
	}

	// Закрытие файла
	fout.close();
	return 0;
}


int main() {
	setlocale(LC_ALL, "Russian");

	// Генерация массива обращений по флагу
	if(need_create) create_petition(count_petitions, count_page, file_name);

	// Количество обращений и массив структур обращений
	int count = 0;
	petitions massive_petitions[10000];

	// Чтение из файла и печать
	read_to_massive(&massive_petitions, count, file_name);
	if (print_flag) print_massive(&massive_petitions, count);

	// Сортировка по времени входа и алгоритм FIFO
	sort_massive(&massive_petitions, count);
	cout << "После сортировки:" << endl;
	if (print_flag) print_massive(&massive_petitions, count);
	algoritm_FIFO(&massive_petitions, count, count_page_RAM, print_flag);
	algoritm_NFU(&massive_petitions, count, count_page_RAM, count_page, print_flag);

	return 0;
}
