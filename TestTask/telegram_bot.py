from telegram import Update
from telegram.ext import Application, CommandHandler, MessageHandler, filters, CallbackContext
from config import API_TOKEN, TELEGRAM_API, DATA_FILE, FLASK_API_URL
from backend import check_imei
import json
import aiohttp

# Функция для сохранения данных в файл
def save_white_list(data):
    try:
        with open(DATA_FILE, 'w') as f:
            json.dump(data, f, indent=4)
    except Exception as e:
        print(f"Error saving data: {e}")

# Функция для загрузки данных из файла
def load_white_list():
    try:
        with open(DATA_FILE, 'r') as f:
            white_list = json.load(f)
            return white_list
    except (FileNotFoundError, json.JSONDecodeError):
        return []

# Функция добавления пользователя в белый список
def add_to_white_list(user_id):
    white_list = load_white_list()
    print(f"Текущий белый список: {white_list}")
    
    # Добавление пользователя в белый список.
    if str(user_id) not in white_list:
        white_list.append(str(user_id))
        
        # Сохраняем данные обратно в файл
        save_white_list(white_list)
        print(f"Пользователь {user_id} добавлен в белый список.")
    else:
        print(f"Пользователь {user_id} уже в белом списке.")

# Функция проверки авторизации пользователя
def is_authorized(user_id):
    white_list = load_white_list()
    return str(user_id) in white_list

# Функция для загрузки данных из файла
def load_data():
    try:
        with open(DATA_FILE, 'r') as f:
            return json.load(f)
    except (FileNotFoundError, json.JSONDecodeError):
        return {}

# Команда /start
async def start(update: Update, context: CallbackContext) -> None:
    await update.message.reply_text("Привет! Для того, чтобы использовать бота, введите свой API токен.")

# Отправка запроса на бекенд
async def check_imei(imei: str, token:str):
    async with aiohttp.ClientSession() as session:
        async with session.post(FLASK_API_URL, json={"imei": imei, "token":token}) as response:
            return await response.json()

# Обработка ввода пользователем
async def handle_message(update: Update, context: CallbackContext) -> None:
    user_id = update.message.from_user.id
    user_input = update.message.text.strip()

    # Если пользователь не авторизован, проверка токена
    if not is_authorized(user_id):
        if user_input == API_TOKEN:
            add_to_white_list(user_id)
            await update.message.reply_text("Вы успешно авторизованы и добавлены в белый список!")
        else:
            await update.message.reply_text("Вы не авторизованы для использования этого бота. Введите правильный токен.")
        return

    # Если пользователь авторизован, обработка IMEI
    if user_input.isdigit():
        imei_info = await check_imei(user_input, API_TOKEN)

        if "error" in imei_info:
            await update.message.reply_text("Ошибка: " + imei_info["error"])
        else:
            await update.message.reply_text(str(imei_info))
    else:
        await update.message.reply_text("Неверный формат IMEI.")

def main() -> None:
    """Запуск бота."""
    application = Application.builder().token(TELEGRAM_API).build()

    # Обработчик команды /start
    application.add_handler(CommandHandler("start", start))

    # Обработчик для всех сообщений
    application.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_message))

    # Запуск бота
    application.run_polling()

if __name__ == '__main__':
    main()
