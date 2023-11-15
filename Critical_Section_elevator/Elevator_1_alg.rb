require 'semaphore'

Elevator_occupancy  = Semaphore.new(1) # Мьютекс через семафор

direct = ["up", "down", "no"]
passenger = Struct.new :floorStart, :floorEnd, :direction

# Список текущих и выполненных заявок
waiting_list = []
сompleted_list  = []

# Счёткчик ожидающих и выполненных заявок
waiting_calls = 0
сompleted_calls = 0

# Количество заявок
c = 3

 # Заявки
calls = [
  passenger.new(5, 7, direct[0]),
  passenger.new(3, 8, direct[0]),
  passenger.new(8, 1, direct[1])
]

def echoPrint(сompleted_calls, waiting_calls)
  puts ("+--------------------------------+")
  puts ("сompleted_calls #{сompleted_calls}"  )
  puts ("waiting_calls #{waiting_calls}")
  puts ("+--------------------------------+")
end

#Поток генерирует 3 вызова
calls_Elevator = Thread.new do
  i = 0
  c.times do
    # Разное время между независимыми заявками
    sleep rand * 5
    Thread.new do

      # Формирование заявки
      floorSt = rand(1..10)
      floorEn = floorSt
      while(floorEn == floorSt)
        floorEn = rand(1..10)
      end

      if(floorSt < floorEn)     # Движение вверх
        direction_Temp = 0
      else (floorSt > floorEn)  # Движение вниз
        direction_Temp = 1
      end

      # Текущая заявка
      current_call = passenger.new(floorSt, floorEn, direct[direction_Temp])

      # Вызвал
      puts " #{i} Чел вызвал с #{floorSt}, едет на #{floorEn}, направление #{current_call.direction}"

      # Мьютекс на критическую секцию
      Elevator_occupancy.P

      # Запись в список заявок
      waiting_list << current_call
      waiting_calls += 1
      i += 1

      # Снятие мьютекса
      Elevator_occupancy.V

      # Эхо-печать
      echoPrint(сompleted_calls, waiting_calls)

    end
  end
end

#Поток обрабатывает 3 вызова
calls_work = Thread.new do
  i = 0
  c.times do
    # Ожидание заявок при их отсутствии
    while (waiting_calls == 0)
      puts ("*Ждёт заявки* waiting_calls = #{waiting_calls}")
      sleep 1
    end

    # Едет
    sleep 1 * (waiting_list[сompleted_calls].floorStart - waiting_list[сompleted_calls].floorEnd).abs
    # Приехал
    puts " Чел приехал с #{waiting_list[сompleted_calls].floorStart}, на #{waiting_list[сompleted_calls].floorEnd}, направление #{waiting_list[сompleted_calls].direction} #{сompleted_calls}"

    # Занесение в список обработанных заявок
    сompleted_list  << waiting_list[сompleted_calls]

    # Мьютекс на критическую секцию
    Elevator_occupancy.P

    # Подсчёт обработанных заявок
    сompleted_calls += 1
    waiting_calls -= 1

    # Снятие мьютекса
    Elevator_occupancy.V

    # Эхо-печать
    echoPrint(сompleted_calls, waiting_calls)

  end
end

calls_Elevator.join
calls_work.join

# сompleted_list = сompleted_list.sort_by {| a | [a.floorEnd, a.direction] }
# i = 0
# c.times do
#   puts("#{сompleted_list[i].floorEnd} " )
#   i += 1
# end
# сompleted_list.sort_by { |a| [a.floorEnd]}
