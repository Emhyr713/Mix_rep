import tkinter as tk                # python 3
from tkinter import font as tkfont  # python 3
import hashlib

login = "NULL"
Role = "NULL"
welcoming = ""

class SampleApp(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        self.title_font = tkfont.Font(family='Helvetica', size=18, weight="bold", slant="italic")

        # the container is where we'll stack a bunch of frames
        # on top of each other, then the one we want visible
        # will be raised above the others
        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}
        for F in (StartPage, PageOne, PageTwo, PageThree):
            page_name = F.__name__
            frame = F(parent=container, controller=self)
            self.frames[page_name] = frame

            # put all of the pages in the same location;
            # the one on the top of the stacking order
            # will be the one that is visible.
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame("StartPage")

    def show_frame(self, page_name):
        '''Show a frame for the given page name'''
        frame = self.frames[page_name]
        frame.tkraise()


class StartPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="Добро пожаловать", font=controller.title_font)
        label.pack(side="top", fill="x", pady=10)

        button1 = tk.Button(self, text="Войти",
                            command=lambda: controller.show_frame("PageOne"))
        button2 = tk.Button(self, text="Зарегистрироваться",
                            command=lambda: controller.show_frame("PageTwo"))
        button1.pack(pady=50, padx=50, fill="x")
        button2.pack(pady=25, padx=50, fill="x")

class PageOne(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="Вход", font=controller.title_font)
        label.pack(side="top", fill="x", pady=10)

        lbl = tk.Label(self, text="Логин")  
        lbl.pack()
        input_login = tk.Entry(self, width=10)  
        input_login.pack(pady=10, padx=50, fill="x")

        lbl = tk.Label(self, text="Пароль")  
        lbl.pack()
        input_pass = tk.Entry(self, width=10)  
        input_pass.pack(pady=10, padx=50, fill="x")

        Inp = tk.Label(self, text="")  
        Inp.pack()

        button = tk.Button(self, text="Войти",
                           command = lambda:self.ClickButton(input_login.get(), input_pass.get(), Inp, input_login, input_pass, controller))
        button.pack(pady=60)

        button = tk.Button(self, text="Зарегистрироваться",
                           command=lambda: controller.show_frame("PageTwo"))
        button.pack(padx=75, fill="x")

        button = tk.Button(self, text="На главное меню",
                           command=lambda: controller.show_frame("StartPage"))
        button.pack(padx=75, fill="x")
    
    def ClickButton(self, log, password, Inp, input_login, input_pass, controller):
        # printLog = "Принят логин:{}".format(login)  
        # printPass = "Принят пароль:{}".format(password)  
        if(log == "" or password == ""):
            Inp.configure(text = "Введите логин или пароль") 
            return
        
        global login
        global Role
        global welcoming
        login = log
        Role = self.Check(log, password)

        if(Role != "wrong"):
            Inp.configure(text = "Успешная авторизация. Ваша роль: " + Role)
            controller.show_frame("PageThree")
            if(Role == "Admin"):
                welcoming.configure(text = "Здравствуйте, " + login + ", Создатель! Ваша роль: " + Role)
            else:
                welcoming.configure(text = "Здравствуйте, " + login + "! Ваша роль: " + Role)
            input_login.delete("0", tk.END)
            input_pass.delete("0",  tk.END)
            Inp.configure(text = "")
        else:
            Inp.configure(text = "Неправильный логин или пароль")

    def Check(self, log, password):
        file = open("accounts.txt", 'r')

        salt = "1234"
        password = password + salt

        hash_password = hashlib.md5(password.encode())
        hash_log = hashlib.blake2s(log.encode())

        while True:
            line = file.readline()
            words = line.split()

            # Если кончился файл
            if not line:
                Role = "wrong"
                break
            
            # Проверка по совпадения по логину, потом по паролю
            if(hash_log.hexdigest() == words[0]):
                if(hash_password.hexdigest() == words[1]):
                    Role = words[2]
                    break
                else:
                    Role = "wrong"
            else:
                Role = "wrong"

        file.close()
        print(Role)
        return Role

              
class PageTwo(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="Регистрация", font=controller.title_font)
        label.pack(side="top", fill="x", pady=10)

        lbl = tk.Label(self, text="Логин")  
        lbl.pack()
        input_login = tk.Entry(self, width=10)  
        input_login.pack(pady=10, padx=50, fill="x")

        lbl = tk.Label(self, text="Пароль")  
        lbl.pack()
        input_pass = tk.Entry(self,width=10)  
        input_pass.pack(pady=10, padx=50, fill="x")

        Inp = tk.Label(self, text="")  
        Inp.pack()

        button = tk.Button(self, text="Зарегистрироваться",
                           command=lambda: self.registration(input_login.get(), input_pass.get(),  Inp, input_login, input_pass, controller))
        button.pack(pady=15)


        button = tk.Button(self, text="На главное меню",
                           command=lambda: controller.show_frame("StartPage"))
        button.pack(pady=10)

    def registration(self, log, password, Inp, input_login, input_pass, controller):
        if(log == "" or password == ""):
            Inp.configure(text = "Не указан логин или пароль")
            return
        
        file_read_login = open("accounts.txt", 'r')
        uniq = 1

        hash_log = hashlib.blake2s(log.encode())

        while True:
            line = file_read_login.readline()
            words = line.split()
            # Если кончился файл
            if not line:
                break
            # Проверка по совпадения логина
            if(hash_log.hexdigest() == words[0]):
                uniq = 0
        file_read_login.close()

        # Если логин не уникальный
        if(uniq == 0):
            Inp.configure(text = "Пользователь с таким логином уже существует")
            return
        salt = "1234"
        password = password + salt
        file = open("accounts.txt", 'a')
        hash_password = hashlib.md5(password.encode())
        file.write(hash_log.hexdigest() + " " + hash_password.hexdigest() + " User \n")
        controller.show_frame("StartPage")

        input_login.delete("0", tk.END)
        input_pass.delete("0",  tk.END)
        Inp.configure(text = "")
        
        

class PageThree(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller

        global login
        global Role
        global welcoming
        
        label = tk.Label(self, text="Вы успешно вошли", font=controller.title_font)
        label.pack(side="top", fill="x", pady=10)
        
        welcoming = tk.Label(self, text="Здравствуйте, " + login + "Ваша роль: " + Role)
        welcoming.pack(side="top", fill="x", pady=10)

        button = tk.Button(self, text="На главное меню",
                        command=lambda: controller.show_frame("StartPage"))
        button.pack(pady=10)


if __name__ == "__main__":
    app = SampleApp()
    app.geometry('400x400')
    app.title('PythonGuides')
    app.mainloop()
