  #include <unistd.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <sys/types.h>
  #include <sys/wait.h>

   int main()
   {
       int data_processed1;
       int data_processed2;
       int file_pipes1[2];
       int file_pipes2[2];
       pid_t fork_result1;
       pid_t fork_result2;
       char Temp1[BUFSIZ + 1];
       char Temp2[BUFSIZ + 1];
       char buffer[BUFSIZ + 1];

       memset(buffer, '\0', sizeof(buffer));
       if (pipe(file_pipes1) == 0 && pipe(file_pipes2) == 0)
       {
           fork_result1 = fork();
           if (fork_result1 == (pid_t)-1)
           {
               fprintf(stderr, "Fork failure");
               exit(EXIT_FAILURE);
           }
           if (fork_result1 == 0)          // Форк1
           {
               sprintf(buffer, "%d", file_pipes1[1]);
               (void)execl("laba4_1_1", "laba4_1_1", "file1.txt", buffer, (char*)0);
               exit(EXIT_FAILURE);
           }
           else
           {
               fork_result2 = fork();

               if (fork_result2 == 0)      // Форк2
               {
                   sprintf(buffer, "%d", file_pipes2[1]);
                   (void)execl("laba4_1_1", "laba4_1_1", "file2.txt", buffer, (char*)0);
                   exit(EXIT_FAILURE);
               }
               else                   // Родительский процесс
               {
                   wait(0);
                   wait(0);
                   printf("++++++++++++++++++++++++++\n");
                   printf("Родительский процесс\n");

                   data_processed1 = read(file_pipes1[0], Temp1, BUFSIZ);
                   data_processed2 = read(file_pipes2[0], Temp2, BUFSIZ);
                   printf("read %d bytes: %s\n", data_processed1, Temp1);
                   printf("read %d bytes: %s\n", data_processed2, Temp2);
               }
           }
       }

       printf("Temp1: %s\n",  Temp1);
       printf("Temp2: %s\n", Temp2);

       char Temp3[BUFSIZ + 1];

       FILE *result = fopen("result", "w+");

       int i = 0;
       while(strlen(Temp1) > i)
       {
           if (strlen(Temp1) < i)
               Temp1[i] = 0;
           if (strlen(Temp2) < i)
               Temp2[i] = 0;

           Temp3[i] = Temp1[i] ^ Temp2[i];
           printf("Temp3[%d]: %d ^ %d = %d\n", i,  (int)Temp1[i], (int)Temp2[i], Temp3[i]);
           fprintf(result, "%c", Temp3[i]);
           i++;
       }
       printf("Temp3:");
       for(int j = 0; j < i;j++)
           printf("%d|%c  ", (int)Temp3[j], Temp3[j]);
       printf("\n");

       exit(EXIT_SUCCESS);
  }
