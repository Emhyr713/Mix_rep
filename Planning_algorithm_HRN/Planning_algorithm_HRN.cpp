#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include <ctime>
using namespace std;

// magic number
int const MAX = 32;

int count_process = 20;
int print_flag = 1;

// Имя файлика
//const char file_name[] = "massive_thread.txt";
const char file_name[] = "massive_20.txt";
//const char file_name[] = "massive_50.txt";
//const char file_name[] = "massive_100.txt";
//const char file_name[] = "massive_1000.txt";
//const char file_name[] = "massive_10000.txt";

struct process{
	// По заданию
	int num_process;				// Номер процесса
	int from_bloking_to_readness;	// Из Б в Г
	int cpu_burst;					// Время выполнения процесса
	double priority;				// Приоритет

	// Дополнительно
	int offset_process = 0;			// Счётчик простаивания процесса (смещение)
	int process_completion_flag = 0;

	//// Двусвязный список
	//process *next;
	//process *prev;
};



// Чтение из файла в массив
int read_to_massive(process (*massive_process)[10000], int& count, const char file_name[]) {

	count = 0;
	ifstream fin(file_name);
	if (!fin) {
		cout << "Ошибка открытия файла" << endl;
		return -1;
	}

	// Для проверки символа конца строки / конца информации о процессе
	char check = ',';

	while(check != ';') {
		fin >> (*massive_process)[count].num_process;
		fin >> (*massive_process)[count].from_bloking_to_readness;
		fin >> (*massive_process)[count].cpu_burst;
		fin >> (*massive_process)[count].priority;
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

void print_massive(process (*massive_process)[10000], int count) {
	if (count == 0) {
		cout << "Массив пуст" << endl;
		return;
	}
	
	cout << "печать массива процессов:" << endl;
	cout << "count: " << count << endl;
	for (int i = 0; i < count; i++) {
		cout	<< setfill(' ')												<< setw(2)
				<< (*massive_process)[i].num_process << " "					<< setw(2)
				<< (*massive_process)[i].from_bloking_to_readness << " "	<< setw(2)
				<< (*massive_process)[i].cpu_burst << " "					<< setw(2)
				<< (*massive_process)[i].priority << endl;
	}

	return;
}

void shapka(process(*massive_process)[10000], int count) {
	setlocale(LC_ALL, "C");
	int setWide = 5;
	int setPrecision = 2;

	cout << "            NUM PROCESS" << endl;

	//		┌────┬────┬─────┐
	cout << "       " << char(218);			// ┌	
	for (int i = 0; i < count - 1; i++) {	// ────┰
		cout << setfill(char(196)) << setw(setWide) << char(194);
	}
	cout << setw(setWide) << char(191) << endl;			// ────┐

	cout << "       ";
	for (int i = 0; i < count; i++) {
		cout << setprecision(setPrecision) << setfill(' ') << char(179) << setw(3) << (*massive_process)[i].num_process << setw(2);
	}
	cout << char(179) << endl;


	//┌────┼────┼────┼────┐
	cout << "  " << char(218);
	for (int i = 0; i < count; i++) {	// ────┰
		cout << setfill(char(196)) << setw(setWide) << char(197);
	}
	cout << setw(setWide) << char(180);
}

void results(double count_ready, double count_runtime, double count, int algoritm) {
	// Итоги
	setlocale(LC_ALL, "Russian");
	if (algoritm == 0) cout << "FIFO:" << endl;
	else cout << "HRN:" << endl;
	cout << "________________________" << endl;
	cout << "Количество процессов: " << count << endl;
	cout << "Количество тактов проведённых в готовности процессами: " << setprecision(15) << count_ready << endl;
	cout << "Количество тактов исполнения процессов: " << setprecision(15) << count_runtime << endl;
	cout << "________________________" << endl;
	cout << "Среднее время выполнения: " << setprecision(15) << (count_ready + count_runtime) / count << endl;
	cout << "Среднее время готовности: " << setprecision(15) << count_ready / count << endl << endl;
}

int find_min_time_ready_FIFO(process(*massive_process)[10000], int count) {
	int flag = (*massive_process)[0].process_completion_flag;
	int j = 0;
	while (flag == 1 && j < count) {
		flag = (*massive_process)[j].process_completion_flag;
		j++;
	}
	if (j > 0) j--;
	int min_time_ready_process = j;
	int min_time_ready = (*massive_process)[j].from_bloking_to_readness;
	for (int i = 0; i < count; i++)
	{
		if ((*massive_process)[i].from_bloking_to_readness < min_time_ready && (*massive_process)[i].process_completion_flag == 0) {
			min_time_ready = (*massive_process)[i].from_bloking_to_readness;
			min_time_ready_process = i;
		}
	}
	return (*massive_process)[min_time_ready_process].num_process;
}

void algoritm_FIFO(process (*massive_process)[10000], int count, int print_flag) {
	if (print_flag == 1) shapka(massive_process, count);
	int setWide = 5;
	int setPrecision = 2;

	int takts = 0;
	int current_process = find_min_time_ready_FIFO(massive_process, count);	// текущий процесс
	int completed_process = 0;
	long long int count_ready = 0;		// счётчик готовности
	int count_runtime = 0;		// счётчик исполнения

	if (print_flag == 1) cout << "current_process: " << current_process << endl;

	while(completed_process < count) {
		// такты
		if (print_flag == 1) cout << "  " << setprecision(setPrecision) << setfill(' ') << char(179) << setw(3) << takts << setw(2) << char(179);

		for (int i = 0; i < count; i++) {
			if (print_flag == 1) cout  << setprecision(setPrecision) << "  ";
			if (takts < ((*massive_process)[i].from_bloking_to_readness) ||
				takts >((*massive_process)[i].from_bloking_to_readness + (*massive_process)[i].cpu_burst + (*massive_process)[i].offset_process - 1)) {
				if (print_flag == 1) cout << "\x1B[31mB\033[0m";
			}
				
			else {
				if (current_process == (*massive_process)[i].num_process) {
					if (print_flag == 1) cout << "\x1B[32mE\033[0m";
					count_runtime++;
				}
				else {
					if (print_flag == 1) cout << "\x1B[33mR\033[0m";
					(*massive_process)[i].offset_process++;
					count_ready++;
				}
			}

			if (print_flag == 1) cout << " " << char(179);
		}
		if (print_flag == 1) cout << endl;

		
		if (print_flag == 1) {
			//├────┼────┼────┼─────┤
			cout << "  " << char(195);
			for (int i = 0; i < count; i++) {
				cout << setfill(char(196)) << setw(setWide) << char(197);
			}
			cout << setw(setWide) << char(180);

			for (int i = 0; i < count; i++) {
				if ((*massive_process)[i].from_bloking_to_readness == takts + 1)
					cout << " ready: " << (*massive_process)[i].num_process << " / ";
			}
		}

		// Если в следующем такте время исполнения текущего процесса закончено, то переход к следующему такту
		if ((takts + 1) > ((*massive_process)[current_process].from_bloking_to_readness + (*massive_process)[current_process].cpu_burst + (*massive_process)[current_process].offset_process - 1)) {
			(*massive_process)[current_process].process_completion_flag = 1;
			current_process = find_min_time_ready_FIFO(massive_process, count);
			completed_process++;
			if (print_flag == 1) cout << "current_process: " << current_process << " ";
		}

		if (print_flag == 1) cout << endl;
		takts++;
	}

	results(count_ready, count_runtime, count, 0);

	return;
}

// Пузырьковая сортировка
void sort_massive(process (*massive_process)[10000], int count, int flag) {

	if(flag == 0) 

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count - i - 1 ; j++)
		{
			if ((*massive_process)[j + 1].from_bloking_to_readness < (*massive_process)[j].from_bloking_to_readness && flag == 0)
				swap((*massive_process)[j], (*massive_process)[j + 1]);
			if ((*massive_process)[j + 1].priority < (*massive_process)[j].priority && flag == 1)
				swap((*massive_process)[j], (*massive_process)[j + 1]);
		}
	}
}

// Поиск максимального приоритета
int find_max_process_priority(process (*massive_process)[10000], int count) {
	int flag = (*massive_process)[0].process_completion_flag;
	int j = 0;
	while (flag == 1 && j < count) {
		flag = (*massive_process)[j].process_completion_flag;
		j++;
	}
	if (j > 0) j--;
	int max_priority = -1;
	int max_priority_process = j;
	for (int i = j; i < count; i++)
	{
		if ((*massive_process)[i].priority > max_priority && (*massive_process)[i].process_completion_flag == 0) {
			max_priority = (*massive_process)[i].priority;
			max_priority_process = i;
		}
	}
	return (*massive_process)[max_priority_process].num_process;
}

// Поиск минимального времени перехода Б->Г
int find_min_time_ready(process(*massive_process)[10000], int count) {
	int min_time_ready_process = 0;
	int min_time_ready = 10000;

	for (int i = 0; i < count; i++)
	{
		if ((*massive_process)[i].from_bloking_to_readness < min_time_ready) {
			min_time_ready = (*massive_process)[i].from_bloking_to_readness;
			min_time_ready_process = i;
		}

	}
	return (*massive_process)[min_time_ready_process].num_process;
}


void algoritm_HRN(process(*massive_process)[10000], int count, int print_flag) {
	if (print_flag == 1 ) shapka(massive_process, count);
	int setWide = 5;
	int setPrecision = 2;

	int takts = 0;
	int current_process = find_min_time_ready(massive_process, count);	// текущий процесс
	int completed_process = 0;
	long long int count_ready = 0;		// счётчик готовности
	int count_runtime = 0;		// счётчик исполнения

	if (print_flag == 1) cout << "current_process: " << current_process << endl;

	while (completed_process < count) {
		// такты
		if (print_flag == 1) cout << "  " << setprecision(setPrecision) << setfill(' ') << char(179) << setw(3) << takts << setw(2) << char(179);
		for (int i = 0; i < count; i++) {
			if (print_flag == 1) cout << setprecision(setPrecision) << "  ";

			if (takts < ((*massive_process)[i].from_bloking_to_readness) ||
				takts >((*massive_process)[i].from_bloking_to_readness + (*massive_process)[i].cpu_burst + (*massive_process)[i].offset_process - 1)) {
				if (print_flag == 1) cout << "\x1B[31mB\033[0m";
			}
				
			else {
				if (current_process == (*massive_process)[i].num_process) {
					if (print_flag == 1) cout << "\x1B[32mE\033[0m";
					count_runtime++;
				}
				else {
					if (print_flag == 1) cout << "\x1B[33mR\033[0m";
					(*massive_process)[i].offset_process++;
					count_ready++;
				}
			}

			if (print_flag == 1) cout << setw(2) << char(179);
		}

		if (print_flag == 1) cout << endl;

		if (print_flag == 1) {

			//├────┼────┼────┼─────┤
			cout << "  " << char(195);
			for (int i = 0; i < count; i++) {
				cout << setfill(char(196)) << setw(setWide) << char(197);
			}
			cout << setw(setWide) << char(180);

			for (int i = 0; i < count; i++) {
				if ((*massive_process)[i].from_bloking_to_readness == takts + 1)
					cout << " ready: " << (*massive_process)[i].num_process << " / ";
			}

		}

		// Если в следующем такте время исполнения текущего процесса закончено, то переход к следующему такту
		if ((takts + 1) > ((*massive_process)[current_process].from_bloking_to_readness + (*massive_process)[current_process].cpu_burst + (*massive_process)[current_process].offset_process - 1)) {
			// Расчёт приоритета
			(*massive_process)[current_process].process_completion_flag = 1;
			completed_process++;
			for (int i = 0; i < count; i++) {
				if ((*massive_process)[i].process_completion_flag == 0 && (takts + 1) > (*massive_process)[i].from_bloking_to_readness)
					(*massive_process)[i].priority = ((double)(*massive_process)[i].offset_process + (double)(*massive_process)[i].cpu_burst) / (double)(*massive_process)[i].cpu_burst;
			}
			current_process = find_max_process_priority(massive_process, count);
			if (print_flag == 1) cout << " current_process: " << current_process;
		}
		if (print_flag == 1) cout << endl;
		takts++;
	}

	results(count_ready, count_runtime, count, 1);

	return;
}

int create_process(int count, const char file_name[]) {
	ofstream fout(file_name);
	if (!fout) {
		cout << "Ошибка открытия файла" << endl;
		return -1;
	}

	srand(time(0));
	int start = 1;
	int end = 500;
	for (int i = 0; i < count; i++) {
		int time_ready = rand() % (end - start + 1) + start;
		int cpu_burst = rand() % (end - start + 1) + start;

		if(i < count - 1)
			fout << i << " " << time_ready << " " << cpu_burst << " " << 0 << " , ";
		if (i == count - 1)
			fout << i << " " << time_ready << " " << cpu_burst << " " << 0 << " ; ";
	}

	fout.close();
	return 0;
}

int main() {
	setlocale(LC_ALL, "Russian");

	int count = 0;							// Счётчик процессов

	// Инициализация списка
	process massive_process[10000];				// Новый адрес

	// Чтение из файла
	read_to_massive(&massive_process, count, file_name);

	// Реализация Алгоритма FCFS (FIFO)
	algoritm_FIFO(&massive_process, count, 0);

	process massive_process_1[10000];				// Новый адрес
	// Чтение из файла
	read_to_massive(&massive_process_1, count, file_name);

	// Реализация Алгоритма HRN
	algoritm_HRN(&massive_process_1, count, print_flag);


	return 0;
}
