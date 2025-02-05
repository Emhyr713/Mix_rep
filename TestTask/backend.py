from flask import Flask, request, jsonify
import requests
from config import IMEI_CHECK_API_URL, API_TOKEN

# Создание экземпляра Flask-приложения
app = Flask(__name__)

# Определение заголовков для запроса к API проверки IMEI
def check_imei(imei):
    headers = {
        'Authorization': f'Bearer {API_TOKEN}',     # Использование токена авторизации
        'Accept-Language': 'en',                    # Определение языка ответа
        'Content-Type': 'application/json'          # Формат передаваемых данных
    }
    
    # Формирование тела запроса
    body = {
        "deviceId": "123456789",    # Идентификатор устройства, может быть изменен
        "serviceId": imei           # Используемый IMEI для проверки
    }
    
    # Выполнение запроса к удаленному API проверки IMEI
    response = requests.post(IMEI_CHECK_API_URL, headers=headers, json=body)
    return response.json()          # Возвращение JSON-ответа


# Получение данных из запроса в формате JSON
@app.route('/check_imei', methods=['POST'])
def check_imei_api():
    data = request.get_json()
    imei = data.get("imei")         # Извлечение значения IMEI из запроса
    token = data.get("token")

    if not token or token != API_TOKEN:
        return jsonify({"error": "Неверный токен"}), 403
    
    # Проверка наличия IMEI и его корректности
    if not imei or not imei.isdigit():
        return jsonify({"error": "Неверный формат IMEI"}), 400
    
    # Вызов функции проверки IMEI
    imei_info = check_imei(imei)
    return jsonify(imei_info)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
