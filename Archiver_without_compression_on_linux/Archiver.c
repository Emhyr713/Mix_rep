    #include <unistd.h>
    #include <stdio.h>
    #include <dirent.h>
    #include <string.h>
    #include <sys/stat.h>
    #include <stdlib.h>
     
    /* Определение размера файла и запись в bout*/
    int getFileSize(const char* file_name, FILE **bout)
    {
        int file_size = 0;
	FILE* fd = fopen(file_name, "rb");
	char byte[1];
	
	if(fd == NULL)
	{
		file_size = -1;
	}
	else
	{
		while(fread(byte,1,1,fd)==1)
		{
			file_size++;
			fwrite(byte,1,1,*bout);
		}
		fclose(fd);
	}
	    return file_size;
    }
   
   
   
    // Табуляции
    void tabs (int depth)
    {
        for (int i = 0; i < depth; i ++) printf("\t");
        return;
    }
   
    int print_info (int depth, char* Temp3, char* name, FILE **bout)
    {
        tabs (depth);
        printf("%s\n", Temp3);
        int file_size = getFileSize(name, bout);
        
     	////int64_t file_size = getFileSize(entry->name);
     	tabs (depth);
     	printf("File size: %d  bytes\n", file_size);
     	tabs (depth);
     	return (file_size);
    }
   
   void printdir(char *dir, int depth, char rep[100], FILE **info_files, FILE **info_direct, FILE **bout) 
   {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    //char *PN;
    
    // Проверка на возможность открыть файл
    if ((dp = opendir(dir)) == NULL) 
    {
     // выводит результат в заданный файловый поток stream
     fprintf(stderr, "cannot open directory: %s\n", dir);
     return;
    }
    
    // Изменяет текущий каталог на заданный
    chdir(dir);
    
    // Пока чтение директории не закончится
    while((entry = readdir(dp)) != NULL) 
    {
     // Возвращение информации о файле
     lstat(entry->d_name, &statbuf);
     
        //char *Temp0 = StartDir;
		char *Temp1 = rep;
		char *Temp2 = entry->d_name;
		char Temp3[100];
       
       	strcpy(Temp3, Temp1);
       	strcat(Temp3, "/"); 
       	strcat(Temp3, Temp2); 
     
     // Проверка на каталог
     if (S_ISDIR(statbuf.st_mode)) 
     {
      // Находит каталог, но игнорирует . и .. 
      // strcmp сравнение, если строки равны, то возвращает 0
      if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
       { continue; }
             
       print_info(depth, Temp3, entry->d_name, bout);  
       printf("Type: Directory\n\n");
       fprintf(*info_direct, "%s||", Temp3 );
       
      /* Рекурсивный вызов с новый отступом */
      printdir(entry->d_name, depth+1, Temp3, info_files, info_direct, bout);
     }
     // Для не каталога
     else 
     {

     	if((strcmp(entry->d_name, "info_files.txt")==0) || (strcmp(entry->d_name, "info_direct.txt")==0) || (strcmp(entry->d_name, "bout.txt") ==0))
        { continue; }
        
        int file_size = print_info(depth, Temp3, entry->d_name, bout);
     	printf("Type: File\n\n"); 
     	fprintf(*info_files, "%s|%d|%s||", entry->d_name, file_size, Temp3 );
     }
    }
    
    // Изменяет текущий каталог на заданный
    chdir("..");
    // Закрывает заданый каталог
    closedir(dp);
   }
   
   void enter_directory(char* dir, int mode)
   {
   
    	// Дефолт для ввода
   		if (mode == 1)
   		{
   			printf(" Введите директорию архивации\n");
   		}
   		// Дефолт для вывода
   		if (mode == 0)
   		{
   			printf(" Введите директорию разархивации\n");
   		}

   		while(1)
   		{
   			printf(" По умолчанию: %s\n", dir); 
   			printf(" 1) По умолчанию\n");
   			printf(" 2) Задать\n");
   			printf(" Выбор: ");
   			//cout << " Для выхода нажмите 0" << endl;
   	
   			int Choice;
   			char Input_dir[256];
   			scanf("%d", &Choice);
   			if(Choice == 1)
   			{
   				break;
   			} //if
   			if(Choice == 2)
   			{
   				printf(" Введите путь: ");
   				scanf("%s", Input_dir);
   				printf("\n\n");
   				if (opendir(Input_dir) == NULL)
   				{
						printf(" cannot open directory: %s\n", Input_dir);
						printf(" Попробуйте ещё раз\n");
						printf(" Для ввода по умолчанию нажмите не 2\n\n");
					}//if
    			else
    			{
						if((strcmp(Input_dir, "/home") == 0) || (strcmp(Input_dir, "/") == 0))
    				{
							printf(" Эту папку архивировать нельзя: %s\n", Input_dir);
							printf(" Попробуйте ещё раз\n");
							printf(" Для ввода по умолчанию нажмите не 2\n\n");
    				}//if
    				else
    				{
							strcpy(dir, Input_dir);
							break;
						}//else
    			}//else
    		}//if
    			else
   			{
   				printf(" Выбрано по умолчанию\n");
   				break;
   			}
   		}
   		printf("\n");
   }

	void files_archivation(char name_info_direct_bout[3][100], char* RestOfDir, char* StartDir)
	{
		//char Temp[256];
		
		//printf(" In function\n\n");
		//printf(" RestOfDir: %s\n", RestOfDir);
		//printf(" StartDir: %s\n\n", StartDir);
		
		char char_info_files[100] 	= "/info_files.txt";
        char char_info_direct[100] 	= "/info_direct.txt";
        char char_bout[100] 		= "/bout.txt";
        char char_main[100];
        
        strcpy(char_main, StartDir);
        strcat(char_main, ".zippo");

		
		//printf(" char_info_files: %s\n", char_info_files);
		//printf(" char_info_direct: %s\n", char_info_direct);
		//printf(" char_bout: %s\n", char_bout);
		//printf(" char_main: %s\n", char_main);
		
        for(int i = 0; i < 4;i++)
		{
			char Temp[256];
			strcpy(Temp, RestOfDir);			
			if (i != 3)
			{
				strcat(Temp, StartDir);
			}
			strcat(Temp, name_info_direct_bout[i]);
			strcpy(name_info_direct_bout[i], Temp);
		}
		
		printf("\n");
		
		strcat(name_info_direct_bout[0], char_info_files);
		strcat(name_info_direct_bout[1], char_info_direct);
		strcat(name_info_direct_bout[2], char_bout);
		strcat(name_info_direct_bout[3], char_main);
		
		//for(int i = 0; i < 4;i++)
		//{
			//printf(" name_info_direct_bout: %d %s\n", i, name_info_direct_bout[i]);
		//}

		return;
	}
	
    void arhivation ()
    {
		// Определение директории по умолчанию
    	char* dir_default;
    	char Temp[100] = "/home/nikita/Документы/labs/ModelForLab1";
    	dir_default = Temp;

        //Выбор директории 
        enter_directory(dir_default, 1);
        char *dir = dir_default;
        char StartDir[100];
        char RestOfDir[256];
        strcpy(Temp, dir);
        
        // Рабочие файлы (в конце удаляются)
        FILE *info_files;		// Хранятся названия файлов, размер, директория этого файла
        FILE *info_direct;		// Хранится директория папок
        FILE *bout;				// Хранится информация файлов
        // Файл архива
        FILE *main;				// Итоговый файл, в котором info_files, info_direct, bout
        
        // Инициализация и очистка массива
        char name_info_direct_bout[4][100];	
        for(int i = 0; i < 4; i++)
        {
        	strcpy(name_info_direct_bout[i], "");
        }
        
        printf(" Выбранная папка: %s\n", Temp);
                                
        // Определение конечной папки
        char sep [2]   = "/";
    	int  count1    = 0;
    	int  count2    = 0;
    		
    	while (dir[count2] != 0)
    			count2++;
    			
    	count1 = count2;
    			
    	while (dir[count1] != *sep)
    			count1--;	
    			
    	// Опеределение конечной папки			
    	for(int i = 0; i < (count2-count1 + 1);i++)
    	{
    		StartDir[i] = dir[count1+i];
    	}
    	  	
    	
    	// Опеределние директории конечной папки
    	if (count1 != 0)
    	{
    		//printf( " dir[%d] %s\n", count1, &dir[count1]);
    		for(int i = 0; i < count1; i++)
    		{
					RestOfDir[i] = dir[i];
    		}
    		RestOfDir[count1] = 0;
    	}
    	
    	// Определение директории для рабочих файлов и файла архива
		files_archivation(name_info_direct_bout, RestOfDir, StartDir );
        
		//return;
        
    	//printf( " StartDir %s\n", StartDir);
    	strcat(StartDir, "(zippo)");

		printf("Куда будут записываться файлы\n");
        for(int i = 0; i < 4; i++)
        {
        	printf(" %d %s\n", i, name_info_direct_bout[i]);
        }
        printf("\n");
        
    	
    	// Открытие файлов
    	info_files = fopen(name_info_direct_bout[0], "wb");
        info_direct = fopen(name_info_direct_bout[1], "wb");
        bout = fopen(name_info_direct_bout[2], "wb");
    	
    	        
        printf("Directory аrchiving of %s\n", dir);
        fprintf(info_direct, "%s||", StartDir);
        printdir(dir, 0, StartDir, &info_files, &info_direct, &bout);
        printf("done.\n");
        
        fprintf(info_files, "\n");
        fprintf(info_direct, "\n");
        
        fclose(info_files);
        fclose(info_direct);
        fclose(bout);
        
        char byte[1];  // единичный буфер для считывания одного байта

		// Открытие файлов для чтения        
		info_files = fopen(name_info_direct_bout[0], "rb");
		info_direct = fopen(name_info_direct_bout[1], "rb");
		bout = fopen(name_info_direct_bout[2], "rb");
        
        // Открытие файла для записи
        main = fopen(name_info_direct_bout[3], "wb");
        
        // Можно сделать через цикл

        // переписываем информацию с файлами в файл main
    	while(!feof(info_files))
    	{
        	if(fread(byte,1,1,info_files)==1) fwrite(byte,1,1,main);
        }
        
        // переписываем информацию с директориями в файл main
        while(!feof(info_direct))
    	{
       		if(fread(byte,1,1,info_direct)==1) fwrite(byte,1,1,main);
        }
        
        // Переписываем данные в файл main
        while(!feof(bout))
        {
            if(fread(byte,1,1,bout)==1) fwrite(byte,1,1,main);
        }
        
        // Закрытие
        fclose(info_files);
        fclose(info_direct);
        fclose(bout);
        
        // Удаление рабочих файлов
        for(int i = 0; i < 3; i++)
			remove(name_info_direct_bout[i]);

        // Закрытие файла с архивом
        fclose(main); 
    }
    
    //const int m = 3;
    
    //int files_info (FILE *main, char file[][m], int &n, int &m);
    //int files_direct (FILE **main, char* file1[100]);
    //void create_dir(char *Path_unrar, int count_dir);
    //void filling_folder(char *Path_unrar, FILE **main, char **file, int n, int &count);
    //void print_files (char* file[100], int count);
    
    void unrar ();
    
    
    int main(int argc, char* argv[]) 
    {
    
    	while(1)
    	{
        	int key;
        	printf("__________________________________________________\n");
        	printf(" Введите (1|0)\n");
       	 	printf(" 1) Архивировать\n");
        	printf(" 2) Разархивировать\n");
        	printf(" 3) Выйти\n");
       	 	printf(" Выбор: "); 
       	 	scanf("%d", &key);
        	printf("\n");
        
        
        // Можно сделать кэйсы
        	// Печать директории и запись информации в файл
        	if (key == 1)
				arhivation();
        	//  функция разархивации, подаётся путь разархивации и файл с данными 
        	if (key == 2)
				unrar();
			// Выход
			if (key == 3)
				break;
				
		}//while
            
        
        return 0;
   }
   
   void unrar ()
   {
     
   	//FILE *main = fopen("/home/nikita/Документы/main.zippo", "rb");
   	
   	FILE *main;				// файл архива
   	char DirUnrar[256];		// Директория файла
   	
   	// Попытка открыть файл
   	while(1)
   	{
   		// Ввод информации
   		printf(" Введите значение\n");
   		printf(" Путь до файла архива: ");
   		scanf("%s", DirUnrar);
   		//printf(" Длина строки %s: %ld\n", DirUnrar, strlen(DirUnrar));
   		printf("\n");
   		
   		//printf("            %d\n", (strcmp(&DirUnrar[strlen(DirUnrar)-6], ".zippo")));
   		
   		// Если доступ есть
   		if(access(DirUnrar, F_OK) == 0)
   		{
   			// Проверка на расширение
   			if((strcmp(&DirUnrar[strlen(DirUnrar)-6], ".zippo")) == 0)
   			{
   				main = fopen(DirUnrar, "rb");
   				printf(" Файл успешно открыт\n");
   				break;
   			}
   			else
   			{
   				// Нужна функция
   				int key;
   				printf(" Это либо папка, либо неправильное расширение\n");
   				printf("*Выйти (0)\n");
   				printf("*Попробовать ещё раз (1)\n");
   				printf("________________________\n");
   				printf("Выбор: \n");
   				scanf("%d", &key);
   			
   				if (key == 1) {}
   				else {return;}
   			}
   		}
   		// Если доступа нет
   		else
   		{
   			int key;
   			printf(" Невозможно открыть файл\n");
   			printf("*Выйти (0)\n");
   			printf("*Попробовать ещё раз (1)\n");
   			printf("________________________\n");
   			printf("Выбор: \n");
   			scanf("%d", &key);
   			
   			if (key == 1) {}
   			else {return;}
		}
	}
	
	main = fopen(DirUnrar, "rb");
   
    // Создание директории
   	char* dir_default;
   	char Temp[100] = "/home/nikita/Документы/labs/Resforlab1";
   	dir_default = Temp;
   
   	char str[10000];			// Типо буфера
   	char *first_str;			// Для первой строки из main
   	char *istr;

   	// Разделил первую строку
   	first_str = fgets (str, sizeof(str), main);
   	
	char sep [4]  = "|";

	
   	// Разделил на Name, Size, Path
   	istr = strtok (first_str, sep);
   	
   	char *file [100][3];
   	int count = 0;
   	
   	// Разделение на составляющие (Name, Size, Path)
   	while ((istr != NULL) && (istr[0] != 10) /*&& (istr[0] != 32)*/)
   	{
   		//cout << count << " ";
   		for (int i = 0; i < 3; i++)
   		{
   			file[count][i] = istr;
   			istr = strtok (NULL,sep);
   		}
   		count++;
   	}
	
   	// Разделил на директории
   	char str1[10000];
   	char *second_str;			// Для второй строки из main
   	char *istr1;
   	second_str = fgets (str1, sizeof(str1), main);
    
	istr1 = strtok (second_str, sep);
   	char *file1 [100];
   	int count_dir = 0;
    
    if(istr1 == NULL) scanf(" (istr1 == NULL)");
    
    printf(" Читаем архив...\n");
    printf(" Папочки:\n");
   	while ((istr1 != NULL) && (istr1[0] != 10) && (istr[0] != 32))
   	{
		printf("%d ", count_dir);
		file1[count_dir] = istr1;
		istr1 = strtok (NULL,sep);
		printf("%s\n", file1[count_dir]);
   		count_dir++;
   	}
   	printf("_____________\n");
   	printf("Totaly dir: %d\n\n", count_dir);
   	
   	
   	printf(" Файлики:\n");
   	// Просто печать что получилось
   	for (int i = 0; i < count; i++)
   	{
   		printf("%2d", i);

   		printf("%22s ", file[i][0]);
   		printf("%7s ",  file[i][1]);
   		printf("%55s ", file[i][2]);

   		printf("\n");
   	}
   	
   	printf("_____________\n");
   	printf("Totaly files: %d\n\n", count);
   	
   	
   	// Подтверждение разархивации
   	printf(" Разархивировать?\n");
   	printf(" Для продолжения введите 1\n");
   	printf(" _________________________\n");
   	printf(" Ввод:");
   	int k = 0;
   	scanf("%d", &k);
   	if(k == 1) {}
   	else {return;}
   	
   	
   	
   	printf(" Где будет директория?\n");
   	enter_directory(dir_default,2);
   	
   	char* DirOutPut = dir_default;
   	printf(" DirOutPut: %s\n", DirOutPut);
    
   	printf(" Распаковка сюда: %s\n", DirOutPut);
   	
   	printf(" Создание папок...\n");
   	for (int i = 0; i < count_dir; i++)
   	{
   	    char *CreateNewDir = DirOutPut;
   	    char* Temp = file1[i];
        char PathDir[100];
   	
   	    // конкотенация
        strcpy(PathDir, CreateNewDir);
        strcat(PathDir, Temp);
    
        printf("PathDir: %s\n", PathDir);
        mkdir(PathDir, 00777);
        
    }
    printf("\n");
      	    
   	    
   	printf(" Запись в папки...\n");
    char byte[1];

	// Создание файлов и побайтовая их перепись согласно количеству файлов
	for (int j = 0; j < count; j++)
	{
		char* PathSet = DirOutPut;
   		char* PathFile = file[j][2];
   		char PathRes[256];
   	
   		strcpy(PathRes, PathSet);
   		strcat(PathRes, PathFile);
   		
   		printf("PathRes: %s\n", PathRes);
    	int Size = atoi(file[j][1]);
    	//cout << "\tSize: " << Size << " bytes"<< endl;
	
	    FILE *Temp = fopen(PathRes, "wb");
	    
	    // Побайтовая перепить согласно заданному размеру
		for(int i = 0; i < Size; i++)
    	{
    		if(fread(byte,1,1, main) == 1)
    		{
    			fwrite(byte,1,1,Temp);
    		}
    	}
    	
    	fclose(Temp);
    }
        
    fclose(main);
    
    }
   
      /*  
        int n = 100;
        int k = 3;
        //int m = 3;
        
        char f_info[n][m];
        char *f_dir[n];
        

        
        int count = 0;
        int count_dir = 0;
   	
   	    //char files_info (&main)
   	    
   	    //cout << *main;
   	
   	    // Получение информации
   	    count = files_info(main, f_info, n, k);
   	    //count_dir = files_direct(main, f_dir);
   	    
   	    // Создание директорий
   	    //create_dir(Path_unrar, count_dir);
   	    
   	    // Заполнение папок
   	    //filling_folder(Path_unrar, main, f_info, count);
   	
   	    
   	    
   	    
   	    
   	    
   	    char str[10000];			// Типо буфера
   	    char *first_str;			// Для первой строки из main
   	    char *parts_3;				// Для разделения на части
   	
   	    // Разделил первую строку
   	    first_str = fgets (str, sizeof(str), *main);
   	    //cout << endl << "info_files:" << endl << first_str << endl;
   	
	    char sep [4]  = "|";
   	
   	    char *istr;

   	
   	    // Разделил на Name, Size, Path
   	    istr = strtok (first_str, sep);
   	
   	    //char *file [100][3];
   	    int count = 0;
   	
   	    // Разделение на составляющие (Name, Size, Path)
   	    while ((istr != NULL) && (istr[0] != 10))
   	    {
   	        	//cout << count << " ";
   		        for (int i = 0; i < 3; i++)
   		        {
   			        f_info[count][i] = istr;
   			        istr = strtok (NULL,sep);
   			        cout << f_info[count][i] << " ";
   		        }
		        //cout << endl;
   		        count++;
   	    }
   	    	
   	    
   	}
   	
   	
   	
   	int  files_info (FILE *main, char file[][m], int &n, int &m)
   	{
   	    char str[10000];			// Типо буфера
   	    char *first_str;			// Для первой строки из main
   	    char *parts_3;				// Для разделения на части
   	    
   	    
   	
   	    // Разделил первую строку
   	    first_str = fgets (str, sizeof(str), main);
   	    //cout << endl << "info_files:" << endl << first_str << endl;
   	
	    char sep [4]  = "|";
   	
   	    char *istr;

   	
   	    // Разделил на Name, Size, Path
   	    istr = strtok (first_str, sep);
   	
   	    //char *file [100][3];
   	    int count = 0;
   	
   	    // Разделение на составляющие (Name, Size, Path)
   	    while ((istr != NULL) && (istr[0] != 10) && (istr[0] != 32))
   	    {
   	        	//cout << count << " ";
   		        for (int i = 0; i < 3; i++)
   		        {
   			        file[count][i] = *istr;
   			        istr = strtok (NULL,sep);
   			        cout << file[count][i] << " ";
   		        }
		        //cout << endl;
   		        count++;
   	    }
   	    
   	    return count;
   	}
   	
   	int files_direct (FILE **main, char file1[100])
   	{
   	    // Разделил на директории
   	    char str1[10000];
   	    char *second_str;			// Для второй строки из main
   	    char *istr1;
   	    
   	    char sep [4]  = "|";
   	    second_str = fgets (str1, sizeof(str1), *main);
        //cout << endl << "info_direct:" << endl << second_str << endl;
    
        istr1 = strtok (second_str, sep);
   	    //char *file1 [100];
   	    int count_dir = 0;
    
        if(istr1 == NULL) cout << " (istr1 == NULL)";
    
        cout << "dir:" << endl;
   	    while ((istr1 != NULL) && (istr1[0] != 10) && (istr[0] != 32))
   	    {
   	        cout << count_dir << " ";
   	        file1[count_dir] = *istr1;
   		    istr1 = strtok (NULL,sep);
   		    cout << file1[count_dir] << endl;
   		    count_dir++;
   	    }
   	    cout << "_____________" << endl;
   	    cout << "Totaly dir: " << count_dir << endl << endl;
   	    
   	    return count_dir;
   	}
   	
   	void print_files (char* file[100], int n, int count)
   	{
   	    cout << " file" << endl;
   	    // Просто печать что получилось
   	    for (int i = 0; i < count; i++)
   	    {
   		    printf("%2d", i);
   		    //for (int j = 0; j < 3; j++)
   		    //{
   		        printf("%15d ", file[i][0]);
   		        printf("%7d ", file[i][1]);
   		        printf("%40d ", file[i][2]);
   			    //cout << file[i][j] << " ";
   		    //}
   		    cout << endl;
   	    }
   	
   	    cout << "_____________" << endl;
   	    cout << "Totaly files: " << &count << endl;
   	
   	    cout << endl;
   	
   	
   	    //return *file[];
   	}
   	
   	void create_dir(char *Path_unrar, char file1[100], int count_dir)
   	{
   	    cout << "Принятая директория: " << Path_unrar << endl;
   	    // Создание директории
   	
   	    cout << " Создание папок..." << endl;
   	    for (int i = 0; i < count_dir; i++)
   	    {
   	        char *CreateNewDir = Path_unrar;
   	        char* Temp = &file1[i];
            char PathDir[100];
   	
   	        // конкотенация
            strcpy(PathDir, CreateNewDir);
            strcat(PathDir, Temp);
    
            cout << "PathDir: " << PathDir << endl;
            mkdir(PathDir, 00777);
        
        }
        cout << endl;
    }
    

  	void filling_folder (char *Path_unrar, FILE **main, char file[][3], int n, int &count)
   	{   
   	    cout << " Запись в папки..." << endl;
        char byte[1];

	    // Создание файлов и побайтовая их перепись согласно количеству файлов
	    for (int j = 0; j < count; j++)
	    {
		    char PathSet[100] = "/home/nikita/Документы/labs/Resforlab1";
   		    char* PathFile = &(file[j][2]);
   		    char PathRes[100];
   	
   		    strcpy(PathRes, PathSet);
   		    strcat(PathRes, PathFile);
   		
   		    cout << "PathRes: " << PathRes << endl;
    	    int Size = atoi(&(file[j][1]));
    	    cout << "\tSize: " << Size << " bytes"<< endl;
	
	
	        FILE *Temp = fopen(PathRes, "wb");
	    
	        // Побайтовая перепить согласно заданному размеру
		    for(int i = 0; i < Size; i++)
    	    {
    		    if(fread(byte,1,1, *main) == 1)
    		    {
    			    fwrite(byte,1,1, Temp);
    		    }
    	    }
    	    
    	    fclose(Temp);
    	}
    	
    	
    }
       
   
   
   */
   
   
   
   
   
   
   
   
   
   
   
   
