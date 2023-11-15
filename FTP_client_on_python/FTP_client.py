from ftplib import FTP
#Открытие файла для записи
logfile = open('directory.txt', 'w+')

#Подключение к серверу
ftp = FTP('ftp.vim.org')
ftp.login()

#Начальная папка
start_dir = "/"
#Ограничитель глубины каталога
limit_level = 3

#Табы
def tabs(depths) -> str:
   tabs_sum = ""
   for i in range(depths):
       tabs_sum += '\t'
   return tabs_sum

#Определение папок
def print_dir(path_dir, depths):
   if(limit_level == depths):
       return
   #Выбор папки
   ftp.cwd(path_dir)
   list_files_down = []
   ftp.retrlines("LIST",list_files_down.append)
   for str in list_files_down:
       #Если директория
       if(str[0] == 'd'):
           print(tabs(depths) + str.split()[-1] + "/")
           logfile.write(tabs(depths) + str.split()[-1] + "/" + '\n');
           print_dir(path_dir + "/" + str.split()[-1], depths+1)

#main()           
print_dir(start_dir, 0)
#Выход
ftp.quit()
logfile.close()