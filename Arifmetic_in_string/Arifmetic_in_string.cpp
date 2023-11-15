#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <future>
#include <time.h>
#include <fstream>
using namespace std;

// Сравнение binint
int compare(string first_num, string second_num, int print_flag) {
	if (first_num == second_num) {
		if (print_flag == 1) cout << "first = second" << endl;
		return 0;
	}

	// Длины чисел
	int first_num_length = first_num.length();
	int second_num_length = second_num.length();

	int max_lenght = first_num_length;	// Установление максимальной длины
	int start_different = 0;			// Начало отличия чисел
	int flag_sign = 1;					// Флаг больше меньше равно (1 -1 0)

	if (print_flag == 1) cout << "first_num_length: " << first_num_length << endl;
	if (print_flag == 1) cout << "second_num_length: " << second_num_length << endl;


	// Если первое число меньше второго
	if (first_num_length <= second_num_length) {
		max_lenght = second_num_length;		// Максимальная длина

		// Если длины равны
		if (first_num_length == second_num_length) {
			while (start_different < first_num_length && first_num[start_different] == second_num[start_different]) {
				start_different++;
			}
		}

		// Если second больше first
		if (first_num[start_different] < second_num[start_different] || first_num_length < second_num_length) {
			if (print_flag == 1) cout << "first < second" << endl;
			flag_sign = -1;
		}
		else if (print_flag == 1) cout << "first > second" << endl;
	}

	return flag_sign;
}

// Сумма
string sum(string first_num, string second_num, int print_flag) {
	string result;			// Результат
	int flag_overflow = 0;	// Флаг переполнения

	// Длины чисел
	int first_num_length = first_num.length();
	int second_num_length = second_num.length();

	int max_lenght = first_num_length;
	if (first_num_length < second_num_length) max_lenght = second_num_length;

	int i = 1;
	while (i <= max_lenght || flag_overflow == 1) {

		// Подготовка операндов
		int a = 0;
		int b = 0;
		if (first_num_length >= i)	a = (int)first_num[first_num_length - i] - 48;
		if (second_num_length >= i)	b = (int)second_num[second_num_length - i] - 48;

		// Эхо-печать
		if (print_flag == 1) cout << i << ": a = " << a << " b = " << b << " flag_overflow = " << flag_overflow << endl;

		// Сложение двух чисел по ASCII коду минус 48 с учётом единицы переполнения
		int digit = a + b + flag_overflow;

		// Сброс флага
		flag_overflow = 0;

		// Если разряд переполнился, взять младший разряд
		if ((flag_overflow = digit / 10) > 0)	digit = digit % 10;

		// Запись в переменную результата
		//result.insert(result.begin(), char(digit + 48));

		result += char(digit + 48);

		i++;
	}

	reverse(result.begin(), result.end());

	if (print_flag == 1) cout << "count_iteration = " << i - 1 << endl;

	return result;
}

// Вычитание 
string sub(string first_num, string second_num, int print_flag) {
	// Длины чисел
	int first_num_length = first_num.length();
	int second_num_length = second_num.length();

	if (first_num == second_num)
		return "0";

	//// Если числа попадают в размер int
	//if (first_num_length <= 9 && second_num_length <= 9) {
	//	if (print_flag == 1) cout << "first_num_length = " << first_num_length << " second_num_length = " << second_num_length << endl;
	//	return to_string(atoi(first_num.c_str()) - atoi(second_num.c_str()));
	//}

	string result;			// Результат
	int flag_overflow = 0;	// Флаг переполнения
	int flag_sign = 0;

	if (print_flag == 1) cout << "first_num_length: " << first_num_length << " second_num_length: " << second_num_length << endl;

	int max_lenght = first_num_length;	// Установление максимальной длины
	int start_different = 0;			// Начало отличия чисел
	// Если вычитаемое меньше вычитателя
	if (first_num_length <= second_num_length) {
		max_lenght = second_num_length;		// Максимальная длина

		// Если длины равны
		if (first_num_length == second_num_length) {
			if (print_flag == 1) cout << "first_num_length == second_num_length" << endl;
			while (start_different < first_num_length && first_num[start_different] == second_num[start_different]) {
				start_different++;
			}
		}

		// Если second больше first, то свап
		if (first_num[start_different] < second_num[start_different] || first_num_length < second_num_length) {
			if (print_flag == 1) cout << "first < second" << endl;
			flag_sign = 1;						// Флаг знака (1 == "-")
			max_lenght = second_num_length;		// Максимальная длина 
			// Свап размеров чисел
			swap(first_num_length, second_num_length);
			// Свап чисел
			string temp = first_num;
			first_num = second_num;
			second_num = temp;
			result += "-";
		}
		else if (print_flag == 1) cout << "first > second" << endl;
	}

	int count = max_lenght - start_different;
	int i = 1;
	while (i <= count || flag_overflow == 1) {
		// Подготовка операндов
		int a = 0;
		int b = 0;
		if (first_num_length >= i)	a = (int)first_num[first_num_length - i] - 48;
		if (second_num_length >= i)	b = (int)second_num[second_num_length - i] - 48;
		
		// Эхо-печать
		if (print_flag == 1) cout << i << ": a = " << a << " b = " << b << " flag_overflow = " << flag_overflow << endl;

		// Сложение двух чисел по ASCII коду минус 48 с учётом единицы переполнения
		int digit = a - b - flag_overflow;

		if (print_flag == 1) cout << "digitA: " << digit << endl;

		// Сброс флага и если число получилось отрицательным, то прибавить 10
		flag_overflow = 0;
		if (digit < 0) {
			flag_overflow = 1;
			digit += 10;
		}

		if (print_flag == 1) cout << "digitB: " << digit << endl;

		// Конкотенация очередного разряда к результату
		result += char(digit + 48);

		i++;
	}
	
	while (result[result.length() - 1] == '0') {
		result.pop_back();
	}
		
	reverse(result.begin() + flag_sign, result.end());

	if (print_flag == 1) cout << "count_iteration = " << i - 1 << endl;
	return result;
}

// Умножение
string mult(string first_num, string second_num, int print_flag) {
	string result = "0";				// Результат
	string zeros;
	int first_num_length = first_num.length();
	int second_num_length = second_num.length();

	for (int i = second_num_length - 1; i >= 0; i--) {
		string summand = "";		// Подсумма
		int flag_overflow = 0;		// Флаг переполнения
		for (int j = first_num_length - 1; j >= 0; j--) {

			// Разряды чисел
			int a = (int)first_num[j] - 48;
			int b = (int)second_num[i] - 48;

			// Эхо-печать
			if (print_flag == 1) cout << "j: " << j << ": a = " << a << " b = " << b << endl;

			// Умножение двух чисел по ASCII коду с учётом единицы переполнения
			int digit = a * b + flag_overflow;

			// Если разряд переполнился, взять младший разряд
			if ((flag_overflow = digit / 10) > 0)	digit = digit % 10;

			// Запись в переменную результата
			summand+= char(digit + 48);
		}

		// Если флаг переполнения остался
		if (flag_overflow > 0)
			summand += char(flag_overflow + 48);

		// "Умножение" на 10 в зависимости от разряда множителя (i)
		reverse(summand.begin(), summand.end());
		summand += zeros;
		zeros = zeros + '0';
		if (print_flag == 1) cout << "i: " << i << " | summand: " << summand << endl << endl;

		// Суммирование подсуммы
		result = sum(result, summand, 0);
	}

	return result;
}

// Возведение в степень
string power(string first_num, unsigned long long n) {
	string result = "1";

	while (n != 0) {
		// Если нечётно то умножить на n
		if ((n & 1) != 0)
			result = mult(result, first_num, 0);
		first_num = mult(first_num, first_num, 0);	// Умножение само на себя	
		n >>= 1;									// Деление на 2
	}
	return result;
}

// Запуск возведения в степень через потоки
string power_th(string first_num, unsigned long long n) {
	string result1 = "1";
	string result2 = "1";
	string result3 = "1";
	string result4 = "1";

	unsigned long long mas_n[4] = {n, n, n, n};

	for (int i = 0; i < 4; i++) {
		mas_n[i] = n / 4;
	}
	mas_n[3] += n % 4;

	//for (int i = 0; i < 4; i++) {
	//	cout << " mas_n[i]: " << " " << mas_n[i];
	// 
	//}


	auto fut1 = async(power, first_num, mas_n[0]);
	auto fut2 = async(power, first_num, mas_n[1]);
	auto fut3 = async(power, first_num, mas_n[2]);
	auto fut4 = async(power, first_num, mas_n[3]);

	result1 = fut1.get();
	result2 = fut2.get();
	result3 = fut3.get();
	result4 = fut4.get();

	auto fut1_1 = async(mult, result1, result2, 0);
	auto fut2_1 = async(mult, result3, result4, 0);

	result1 = fut1_1.get();
	result2 = fut2_1.get();

	result1 = mult(result1, result2, 0);

	return result1;
}

// Остаток от деления и деление
string big_mod(string first_num, string second_num, int flag_operation, int print_flag) {
	// Дефолные значения, если Второе число больше
	string quotient = "0";			// Частое
	string remainder = first_num;	// Остаток

	// Если числа равны
	if (compare(first_num, second_num, 0) == 0) {
		quotient = "1";
		remainder = "0";
	}
	
	// Если Первое число больше
	if (compare(first_num, second_num, 0) > 0) {
		int first_num_length = first_num.length();		// Максимальная длина у Первого
		string sub_first_num = "";
		quotient = "";

		for (int i = 0; i < first_num_length; i++) {

			string mult_second_num = second_num;
			sub_first_num += first_num[i];		

			if (print_flag == 1) cout << "i: " << i << endl;
			if (print_flag == 1) cout << "sub_first_num: " << sub_first_num << endl;
			if (print_flag == 1) cout << "mult_second_num: " << mult_second_num << endl;

			// Если 0, то "умножить на 10" частное
			if (sub_first_num == "0") {
				quotient += "0";
				sub_first_num = "";
				continue;
			}
			// Пока часть Первого числа меньше Второго
			/*while (compare(sub_first_num, second_num, 0) <= 0 && (i+1) < first_num_length) {*/
			while (compare(sub_first_num, second_num, 0) < 0 && (i + 1) < first_num_length) {
				i++;
				sub_first_num += first_num[i];
				//cout << "first_num[i]: " << first_num[i] << " sub_first_num: " << sub_first_num << " " << sub_first_num.length() << endl;
				if(quotient != "") quotient += "0";
			}

			// Пока произведение Второго числа меньше части Первого
			int j = 1;
			while (compare(sub_first_num, mult_second_num, 0) > 0) {
				j++;
				mult_second_num = mult(second_num, to_string(j), 0);
			}

			// Если "Промахнулся"
			if (compare(sub_first_num, mult_second_num, 0) < 0) {
				mult_second_num = sub(mult_second_num, second_num, 0);
				j--;
			}

			if (print_flag == 1) cout << "j: " << j << endl;
			if (print_flag == 1) cout << "sub_first_num: " << sub_first_num << endl;
			if (print_flag == 1) cout << "mult_second_num: " << mult_second_num << endl;

			// Записать к частному
			quotient += to_string(j);

			// Вычисление остатка
			remainder = sub(sub_first_num, mult_second_num, 0);

			if (print_flag == 1) cout << "remainder: " << remainder << endl;
			if (print_flag == 1) cout << "quotient: " << quotient << endl;

			// Если промежуточный остаток не равен нулю, то приписать к следующей части Первого числа
			sub_first_num = remainder;
			if (sub_first_num == "0") sub_first_num = "";
		}

	}
	// Если нужно частое
	if (flag_operation == 0)	return quotient;
	// Если нужен остаток
	else						return remainder;
}

// Определение корня
string big_sqrt(string first_num, int print_flag) {
	string sqrt_result = "1";
	string border_right = "0";
	string border_left = "1";
	int n = 0;

	for (int i = 0; i < ((first_num.length() / 2)); i++)
		border_left += "0";

	string temp = border_left;

	for (int i = 0; i < ((first_num.length() / 2) + 1) ; i++) {
		
		while (compare(mult(border_left, border_left, 0), first_num, 0) < 0) {
			border_left = sum(border_left, temp, 0);
		}
		if (compare(mult(border_left, border_left, 0), first_num, 0) > 0) {
			border_left = sub(border_left, temp, 0);
		}
		if (print_flag == 1) cout << "border_right: " << border_right << " border_left: " << border_left << endl;
		temp.pop_back();
	}

	sqrt_result = sub(border_left, temp, 0);
	while (compare(mult(sqrt_result, sqrt_result, 0), first_num, 0) < 0) {
		sqrt_result = sum(sqrt_result, "1", 0);
	}
	if (compare(mult(sqrt_result, sqrt_result, 0), first_num, 0) > 0)
		sqrt_result = sub(sqrt_result, "1", 0);

	//cout << "sqrt: " << sqrt << endl;
	return sqrt_result;
}

string bi_sqrt(string first_num, int print_flag) {
	string sqrt_result = "1";
	string border_min = "1";
	string border_max = "1";
	string mid;

	// Нахождение границ
	if (first_num.length() == 2) {
		border_min = "0";
		border_max = "10";
	}
	else {
		for (int i = 0; i < ((first_num.length() / 2) - 1); i++) {
			border_min += "0";
			border_max = border_min + "0";
		}
	}

	while (compare(border_max, border_min, 0) >= 0) { // Инструкции продолжаем поиск

		string sum_max_min = sum(border_min, border_max, 0);
		mid = big_mod(sum_max_min, "2", 0, 0);
		if (print_flag == 1) {
			cout << "border_min: " << border_min << " border_max: " << border_max << endl;
			cout << "sum_max_min: " << sum_max_min << endl;
			cout << "mid: " << mid << endl;
		}
		// Если значение для поиска равно среднему значению, вернуть это значение напрямую
		if (mult(mid, mid, 0) == first_num) {
			return mid;
		}
		// Если значение в середине массива больше, чем значение, которое нужно найти
		else if (compare(mult(mid, mid, 0), first_num, 0) > 0) {
			border_max = sub(mid, "1", 0);	// Нужно искать в  меньшей половине
		}
		else {
			border_min = sum(mid, "1", 0);	// Нужно искать в  большей половине
		}
	}

	return mid;
}

// Проверка на простое число
int checking_prime(unsigned long long a) {
	unsigned long i1, i2, i3, i4, i5, i6, i7, i8, bound;
	if (a == 0 || a == 1)
		return 0;
	if (a == 2 || a == 3 || a == 5 || a == 7 || a == 11 || a == 13 || a == 17 || a == 19 || a == 23 || a == 29)
		return 1;
	if (a % 2 == 0 || a % 3 == 0 || a % 5 == 0 || a % 7 == 0 || a % 11 == 0 || a % 13 == 0 || a % 17 == 0 || a % 19 == 0 || a % 23 == 0 || a % 29 == 0)
		return 0;
	bound = sqrt((double)a);
	i1 = 31; i2 = 37; i3 = 41; i4 = 43; i5 = 47; i6 = 49; i7 = 53; i8 = 59;
	while (i8 <= bound && a % i1 && a % i2 && a % i3 && a % i4 && a % i5 && a % i6 && a % i7 && a % i8)
	{
		i1 += 30; i2 += 30; i3 += 30; i4 += 30; i5 += 30; i6 += 30; i7 += 30; i8 += 30;
	}
	if (i8 <= bound ||
		i1 <= bound && a % i1 == 0 ||
		i2 <= bound && a % i2 == 0 ||
		i3 <= bound && a % i3 == 0 ||
		i4 <= bound && a % i4 == 0 ||
		i5 <= bound && a % i5 == 0 ||
		i6 <= bound && a % i6 == 0 ||
		i7 <= bound && a % i7 == 0) {
		return 0;
	}
		
	return 1;
}
// Поиск простого числа
int search_prime(unsigned long long int offset) {
	vector <int> vector_prime;	// Вектор простых неповторяющихся чисел
	unsigned long long int result = 0;

	if (offset <= 3) return offset;

	int count = 3;
	int i = 0;
	while (count < offset) {
		unsigned long long int num_1 = 6 * i - 1;
		unsigned long long int num_2 = 6 * i + 1;

		if (checking_prime(num_1)) {
			count++;
			if (count == offset) {
				result = num_1;
				break;
			}
		}

		if (checking_prime(num_2)) {
			count++;
			if (count == offset) {
				result = num_2;
				break;
			}
		}

		i++;
	}
	return result;

}

// НОД чисел a и b
int gcd(int a, int b)
{
	if (b != 0)
		return gcd(b, a % b);
	else
		return a;
}

// НОД чисел a и b
string gcd_str(string a, string b)
{
	if (compare(b, "0", 0) != 0)
		return gcd_str(b, big_mod(a, b, 1, 0));
	else
		return a;
}

// Генерация чисел длиной от len_start до len_end
string generate_num(int len_start, int len_end) {
	string result = "";
	//int digit_0 = 0;
	//int digit_9 = 9;
	srand(time(NULL));
	int length = len_start + rand() % (len_end - len_start + 1);

	for (int i = 0; i < length; i++) {
		if(i == 0) result += to_string(1 + rand() % (11));
		else result += to_string(rand() % (10));
	}
	//cout << "result: " << result << endl;
	return result;
}

// Генерация взаимнопростых чисел
string* generate_coprime(string* nums) {
	string first_num = generate_num(15, 20);
	string second_num = generate_num(15, 20);

	while (gcd_str(first_num, second_num) != "1"){
		first_num = generate_num(15, 20);
		second_num = generate_num(15, 20);
	}
	//cout << "coprime: " << endl;
	//cout << "first_num: " << first_num << endl;
	//cout << "second_num: " << second_num << endl;
	//cout << "gcd_str(" << first_num << ", " << second_num << "): " << gcd_str(first_num, second_num) << endl;
	nums[0] = first_num;
	nums[1] = second_num;
	return nums;
}


//Проверка на простое число
int checking_prime_str(string num) {
	string i1, i2, i3, i4, i5, i6, i7, i8, bound;
	if (num == "0" || num == "1")
		return 0;
	if (num == "2" || num == "3" || num == "5" || num == "7" || num == "11" ||
		num == "13" || num == "17" || num == "19" || num == "23" || num == "29")
		return 1;


	char last_simb = num[num.length() - 1];
	//cout << "num: " << num << endl;
	//cout << "last_simb: " << last_simb << endl;
	//cout << "last_simb == " << last_simb << " : " << bool(last_simb == last_simb) << endl;
	if (last_simb == '0' || last_simb == '2' || last_simb == '4' || last_simb == '5' || last_simb == '6' || last_simb == '8') {
		//cout << " ----Nen" << endl << endl;
		return 0;
	}


	//if (big_mod(num, "2", 1, 0) == "0" || big_mod(num, "3", 1, 0) == "0" ||
	//	big_mod(num, "5", 1, 0) == "0" || big_mod(num, "7", 1, 0) == "0" ||
	//	big_mod(num, "11", 1, 0) == "0" || big_mod(num, "13", 1, 0) == "0" ||
	//	big_mod(num, "17", 1, 0) == "0" || big_mod(num, "19", 1, 0) == "0" ||
	//	big_mod(num, "23", 1, 0) == "0" || big_mod(num, "29", 1, 0) == "0") {
	//	return 0;
	//}

	if ( big_mod(num, "3", 1, 0) == "0" || big_mod(num, "7", 1, 0) == "0" ||
		big_mod(num, "11", 1, 0) == "0" || big_mod(num, "13", 1, 0) == "0" ||
		big_mod(num, "17", 1, 0) == "0" || big_mod(num, "19", 1, 0) == "0" ||
		big_mod(num, "23", 1, 0) == "0" || big_mod(num, "29", 1, 0) == "0") {
		return 0;
	}
		
	bound = big_sqrt(num, 0);
	//cout << "bound: " << bound << endl;
	i1 = "31"; i2 = "37"; i3 = "41"; i4 = "43"; i5 = "47"; i6 = "49"; i7 = "53"; i8 = "59";
	while (compare(i8, bound, 0) <= 0 &&
		big_mod(num, i1, 1, 0) != "0" &&
		big_mod(num, i2, 1, 0) != "0" &&
		big_mod(num, i3, 1, 0) != "0" &&
		big_mod(num, i4, 1, 0) != "0" &&
		big_mod(num, i5, 1, 0) != "0" &&
		big_mod(num, i6, 1, 0) != "0" &&
		big_mod(num, i7, 1, 0) != "0" &&
		big_mod(num, i8, 1, 0) != "0")
	{
		i1 = sum(i1, "30", 0);
		i2 = sum(i2, "30", 0);
		i3 = sum(i3, "30", 0);
		i4 = sum(i4, "30", 0);
		i5 = sum(i5, "30", 0);
		i6 = sum(i6, "30", 0);
		i7 = sum(i7, "30", 0);
		i8 = sum(i8, "30", 0);

	}
	if (compare(i8, bound, 0) <= 0 ||
		compare(i1, bound, 0) <= 0 && big_mod(num, i1, 1, 0) == "0" ||
		compare(i2, bound, 0) <= 0 && big_mod(num, i2, 1, 0) == "0" ||
		compare(i3, bound, 0) <= 0 && big_mod(num, i3, 1, 0) == "0" ||
		compare(i4, bound, 0) <= 0 && big_mod(num, i4, 1, 0) == "0" ||
		compare(i5, bound, 0) <= 0 && big_mod(num, i5, 1, 0) == "0" ||
		compare(i6, bound, 0) <= 0 && big_mod(num, i6, 1, 0) == "0" ||
		compare(i7, bound, 0) <= 0 && big_mod(num, i7, 1, 0) == "0") {
		return 0;
	}

	return 1;
}

// Поиск простого числа
string search_prime_str(unsigned long long int offset) {
	vector <int> vector_prime;	// Вектор простых неповторяющихся чисел
	string result = "0";

	if (offset <= 3) return to_string(offset);
	if (offset <= 4) return "5";
	if (offset <= 5) return "7";

	int count = 6;
	string i = "2";
	while (count < offset) {
		string num_1 = mult("6", i, 0);
		num_1 = sub(num_1, "1", 0);
		string num_2 = mult("6", i, 0);
		num_2 = sum(num_2, "1", 0);


		if (checking_prime_str(num_1)) {
			//cout << count << " num_1: " << num_1 << endl;
			count++;
			if (count == offset) {
				result = num_1;
				break;
			}
		}

		if (checking_prime_str(num_2)) {
			//cout << count << " num_2: " << num_2 << endl;
			count++;
			if (count == offset) {
				result = num_2;
				break;
			}
		}

		i = sum(i, "1", 0);
	}
	return result;

}

// Генератор простого числа
string generate_prime() {
	string num = generate_num(15, 20);

	while (checking_prime_str(num) != 1) {
		num = generate_num(15, 20);
	}
	
	return num;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	string first_num	= "18014398241046527";
	string second_num	= "56468745";

	// Открытие файлов для чтения
	ifstream fin_Arifmetic("Input_nums_Arifmetic.txt");
	ifstream fin_prime("Input_nums_check_prime.txt");
	ifstream fin_pow("Input_nums_pow.txt");

	// Переменные для хранения чисел в виде строки
	string num_Arifmetic_1, num_Arifmetic_2;
	unsigned long long int num_prime_int;
	string num_prime_str;
	int count_prime;
	string num_pow_1, num_pow_2;

	// Запись в переменные
	fin_Arifmetic >> num_Arifmetic_1 >> num_Arifmetic_2;
	fin_prime >> num_prime_int >> num_prime_str >> count_prime;
	fin_pow >> num_pow_1 >> num_pow_2;

	// Закрытие файлов
	fin_Arifmetic.close();
	fin_prime.close();
	fin_pow.close();


	//cout << "num_Arifmetic_1: " << num_Arifmetic_1 << " num_Arifmetic_2: " << num_Arifmetic_2 << endl;
	//cout << "num_prime_int_1: " << num_prime_int_1 << " num_prime_int_2: " << num_prime_int_2 << endl;
	//cout << "num_prime_str_1: " << num_prime_str_1 << " num_prime_str_2: " << num_prime_str_2 << endl;
	//cout << "num_pow_1: " << num_pow_1 << " num_pow_2: " << num_pow_2 << endl;


	// Сложение, вычитание, умножение, деление, взятие корня
	ofstream fout_Arifmetic("Output_Arifmetic.txt");
	fout_Arifmetic << "num_Arifmetic_1: " << num_Arifmetic_1 << endl;
	fout_Arifmetic << "num_Arifmetic_2: " << num_Arifmetic_2 << endl;
	fout_Arifmetic << "_____________________________" << endl;
	fout_Arifmetic << "sum: " << sum(num_Arifmetic_1, num_Arifmetic_2, 0) << endl;
	fout_Arifmetic << "expectation: " << atoi(num_Arifmetic_1.c_str()) + atoi(num_Arifmetic_2.c_str()) << endl;
	fout_Arifmetic << "_____________________________" << endl;
	fout_Arifmetic << "sub: " << sub(num_Arifmetic_1, num_Arifmetic_2, 0) << endl;
	fout_Arifmetic << "expectation: " << atoi(num_Arifmetic_1.c_str()) - atoi(num_Arifmetic_2.c_str()) << endl;
	fout_Arifmetic << "_____________________________" << endl;
	fout_Arifmetic << "mult: " << mult(num_Arifmetic_1, num_Arifmetic_2, 0) << endl;
	fout_Arifmetic << "expectation: " << atoi(num_Arifmetic_1.c_str()) * atoi(num_Arifmetic_2.c_str()) << endl;
	fout_Arifmetic << "_____________________________" << endl;
	fout_Arifmetic << "big_bod / : " << big_mod(num_Arifmetic_1, num_Arifmetic_2, 0, 0) << endl;
	fout_Arifmetic << "big_bod % : " << big_mod(num_Arifmetic_1, num_Arifmetic_2, 1, 0) << endl;
	fout_Arifmetic << "expectation /: " << atoi(num_Arifmetic_1.c_str()) / atoi(num_Arifmetic_2.c_str()) << endl;
	fout_Arifmetic << "expectation %: " << atoi(num_Arifmetic_1.c_str()) % atoi(num_Arifmetic_2.c_str()) << endl;
	fout_Arifmetic << "_____________________________" << endl;
	fout_Arifmetic << "bi_sqrt: " << bi_sqrt(num_Arifmetic_1, 0) << endl;
	fout_Arifmetic << "expectation: = " << sqrt(atoi(num_Arifmetic_1.c_str())) << endl;
	fout_Arifmetic << "_____________________________" << endl;
	fout_Arifmetic << "big_sqrt: " << big_sqrt(num_Arifmetic_1, 0) << endl;
	fout_Arifmetic << "expectation: = " << sqrt(atoi(num_Arifmetic_1.c_str())) << endl;
	fout_Arifmetic.close();


	cout << "num_Arifmetic_1: " << num_Arifmetic_1 << endl;
	cout << "num_Arifmetic_2: " << num_Arifmetic_2 << endl;
	cout << "_____________________________" << endl;
	cout << "sum: " << sum(num_Arifmetic_1, num_Arifmetic_2, 0) << endl;
	cout << "expectation: " << atoi(num_Arifmetic_1.c_str()) + atoi(num_Arifmetic_2.c_str()) << endl;
	cout << "_____________________________" << endl;
	cout << "sub: " << sub(num_Arifmetic_1, num_Arifmetic_2, 0) << endl;
	cout << "expectation: " << atoi(num_Arifmetic_1.c_str()) - atoi(num_Arifmetic_2.c_str()) << endl;
	cout << "_____________________________" << endl;
	cout << "mult: " << mult(num_Arifmetic_1, num_Arifmetic_2, 0) << endl;
	cout << "expectation: " << atoi(num_Arifmetic_1.c_str()) * atoi(num_Arifmetic_2.c_str()) << endl;
	cout << "_____________________________" << endl;
	cout << "big_bod / : " << big_mod(num_Arifmetic_1, num_Arifmetic_2, 0, 0) << endl;
	cout << "big_bod % : " << big_mod(num_Arifmetic_1, num_Arifmetic_2, 1, 0) << endl;
	cout << "expectation /: " << atoi(num_Arifmetic_1.c_str()) / atoi(num_Arifmetic_2.c_str()) << endl;
	cout << "expectation %: " << atoi(num_Arifmetic_1.c_str()) % atoi(num_Arifmetic_2.c_str()) << endl;
	cout << "_____________________________" << endl;
	cout << "bi_sqrt: " << bi_sqrt(num_Arifmetic_1, 0) << endl;
	cout << "expectation: = " << sqrt(atoi(num_Arifmetic_1.c_str())) << endl;
	cout << "_____________________________" << endl;
	cout << "big_sqrt: " << big_sqrt(num_Arifmetic_1, 0) << endl;
	cout << "expectation: = " << sqrt(atoi(num_Arifmetic_1.c_str())) << endl;

	// Возведение в степень
	ofstream fout_pow("Output_pow.txt");

	fout_pow << "num_pow_1: " << num_pow_1 << endl;
	fout_pow << "num_pow_2: " << num_pow_2 << endl;
	fout_pow << "_____________________________" << endl;
	fout_pow << "power: " << power(num_pow_1, atoi(num_pow_2.c_str())) << endl;
	fout_pow << "expectation: " << pow(atoi(num_pow_1.c_str()), atoi(num_pow_2.c_str())) << endl;
	fout_pow.close();

	cout << "_____________________________" << endl;
	cout << "num_pow_1: " << num_pow_1 << endl;
	cout << "num_pow_2: " << num_pow_2 << endl;
	cout << "_____________________________" << endl;
	cout << "power: " << power(num_pow_1, atoi(num_pow_2.c_str())) << endl;
	cout << "expectation: " << pow(atoi(num_pow_1.c_str()), atoi(num_pow_2.c_str())) << endl;

	//Проверка на простое число и генерация протого числа
	ofstream fout_check_prime("Output_check_prime.txt");
	fout_check_prime << "num_prime_int_1: " << num_prime_int << endl;
	fout_check_prime << "num_prime_str_1: " << num_prime_str << endl;
	fout_check_prime << "count_prime: " << count_prime << endl;
	fout_check_prime << "_____________________________" << endl;
	fout_check_prime << "checking_prime: " << checking_prime(num_prime_int) << endl;
	fout_check_prime << "search_prime: " << search_prime(count_prime) << endl;
	fout_check_prime << "_____________________________" << endl;
	fout_check_prime << "checking_prime_str: " << checking_prime_str(num_prime_str) << endl;
	fout_check_prime << "search_prime_str: " << search_prime_str(count_prime) << endl;
	fout_check_prime.close();

	cout << "_____________________________" << endl;
	cout << "num_prime_int_1: " << num_prime_int << endl;
	cout << "num_prime_str_1: " << num_prime_str << endl;
	cout << "count_prime: " << count_prime << endl;
	cout << "_____________________________" << endl;
	cout << "checking_prime: " << checking_prime(num_prime_int) << endl;
	cout << "search_prime: " << search_prime(count_prime) << endl;
	cout << "_____________________________" << endl;
	cout << "checking_prime_str: " << checking_prime_str(num_prime_str) << endl;
	cout << "search_prime_str: " << search_prime_str(count_prime) << endl;

	// Генерация простых чисел
	ofstream fout_coprime("Output_coprime.txt");
	string* nums = new string[2];
	fout_coprime << "generate_comprime: " << generate_coprime(nums) << endl;
	fout_coprime << "first_num_coprime: " << nums[0] << endl;
	fout_coprime << "second_num_coprime: " << nums[1] << endl;
	fout_coprime << "gcd_str(" << nums[0] << ", " << nums[1] << "): " << gcd_str(nums[0], nums[1]) << endl;
	fout_coprime << "gcd_str(1242345, 145): " << gcd_str("1242345", "145") << endl;
	fout_coprime.close();

	cout << "_____________________________" << endl;
	cout << "generate_comprime: " << generate_coprime(nums) << endl;
	cout << "first_num_coprime: " << nums[0] << endl;
	cout << "second_num_coprime: " << nums[1] << endl;
	cout << "gcd_str(" << nums[0] << ", " << nums[1] << "): " << gcd_str(nums[0], nums[1]) << endl;
	cout << "gcd_str(1242345, 145): " << gcd_str("1242345", "145") << endl;



	//cout << "first_num: " << first_num << endl;
	//cout << "second_num: " << second_num << endl;
	//cout << "_____________________________" << endl;
	//cout << "sum: " << sum(first_num, second_num, 0) << endl;
	//cout << "expectation: " << atoi(first_num.c_str()) + atoi(second_num.c_str()) << endl;
	//cout << "_____________________________" << endl;
	//cout << "sub: " << sub(first_num, second_num, 0) << endl;
	//cout << "expectation: " << atoi(first_num.c_str()) - atoi(second_num.c_str()) << endl;
	//cout << "_____________________________" << endl;
	//cout << "mult: " << mult(first_num, second_num, 0) << endl;
	//cout << "expectation: " << atoi(first_num.c_str()) * atoi(second_num.c_str()) << endl;
	// 
	//cout << "_____________________________" << endl;
	//cout << "power: " << power(first_num, atoi(second_num.c_str())) << endl;
	//cout << "expectation: " << pow(atoi(first_num.c_str()), atoi(second_num.c_str())) << endl;

	//cout << "_____________________________" << endl;
	//cout << "big_bod / : " << big_mod(first_num, second_num, 0, 0) << endl;
	//cout << "big_bod % : " << big_mod(first_num, second_num, 1, 0) << endl;
	//cout << "expectation /: " << atoi(first_num.c_str()) / atoi(second_num.c_str()) << endl;
	//cout << "expectation %: " << atoi(first_num.c_str()) % atoi(second_num.c_str()) << endl;
	//cout << "_____________________________" << endl;
	//cout << "big_sqrt: " << big_sqrt(first_num, 0) << endl;
	//cout << "expectation: = " << sqrt(atoi(first_num.c_str())) << endl;
	//cout << "_____________________________" << endl;
	//cout << "checking_prime: " << checking_prime(18014398241046527) << endl;
	//cout << "search_prime: " << search_prime(2000) << endl;
	//cout << "_____________________________" << endl;
	//cout << "checking_prime_str: " << checking_prime_str("2147483647") << endl;
	//cout << "search_prime_str: " << search_prime_str(2000) << endl;
	//cout << "_____________________________" << endl;
	//cout << "bi_sqrt: " << bi_sqrt(first_num, 0) << endl;
	//cout << "expectation: = " << sqrt(atoi(first_num.c_str())) << endl;
	//cout << "_____________________________" << endl;
	//cout << "gcd: " << gcd(1242345, 145) << endl;
	//cout << "gcd_str: " << gcd_str("1242345", "145") << endl;
	//cout << "_____________________________" << endl;
	//cout << "generate_num: " << generate_num(10, 15) << endl;
	//cout << "_____________________________" << endl;
	//cout << "generate_comprime: " << endl;
	//cout << generate_coprime() << endl;
	//cout << "_____________________________" << endl;
	//cout << "generate_mprime: " << generate_prime() << endl;
	
	return 0;
}
