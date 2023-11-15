  #include <unistd.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  int main(int argc, char *argv[])
  {
       printf("--------------------------\n");
       char *massive = "";
       char *file_name;

       int data_processed;
       int file_pipes;

       if (argc < 2)
           file_name = "file1.txt";
       else
           file_name = argv[1];

       // Функция---------------------------
       FILE *file = fopen(file_name, "r");

       // Проверка на открытие файла
       if (file == NULL)
       {
           // файл не открыт
           printf("Файл не открыт.\n");
           return -1;
       }
       printf("Открыт %s файл\n", file_name);

       // Считываение строки
       char buff[256];
       fgets(buff, sizeof(buff), file);
       massive = buff;

       fclose(file);
       //------------------------------------------

       // Печать массива,
       printf("massivem: %s\n", massive);
      
       file_pipes = atoi(argv[2]);
       //printf("argv[2]: %s\n", argv[2]);

       data_processed = write(file_pipes, massive, strlen(massive));
       printf("Wrote %d bytes\n", data_processed);

       return 0;
  }
