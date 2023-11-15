import socket
import time

log_file = open("log_file_py.txt", "w")
#info_server = open("Param_server.txt", "r")

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ip_server = '192.168.23.229'
port_server = 3030
#sock.connect(('127.0.0.1', port_server))

# Считывание ip и порта с файла, подключение и печать в логфайл
#ip_server = str(info_server.readline().rstrip())
#port_server = int(info_server.readline())
sock.connect((ip_server, port_server))
log_file.write(time.ctime() + " connecting_to " + '\t\t' + ip_server + " " + str(port_server) + '\n')

print("Подключение установлено")

# Имитация расстояния
time.sleep(3)

# Отправка строки серверу, печать в логфайл
send_to_server = "Kilmishkin Nikita Vladimirovich M3O-310B-20"
sock.send(send_to_server.encode())
log_file.write(time.ctime() + " it_was_sent " + '\t\t' + send_to_server + '\n')

# Получение ответа от сервера, печать в логфайл
print("Ждёт ответа")
time.sleep(2)
data = sock.recv(1024).decode('cp1251')
log_file.write(time.ctime() + " it_was_received " + '\t' + data + '\n')
print("Ответ получен")
print("Ответ: ", data)

#Отключение от сервера
time.sleep(2)
#sock.send("".encode())
sock.close()

log_file.close()
info_server.close()