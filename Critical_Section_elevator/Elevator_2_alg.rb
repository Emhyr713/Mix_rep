require 'monitor'
require 'semaphore'

# Количество заявок
c = 5
#Структура заявки
$call_struct = Struct.new :floorStart, :floorEnd, :direction, :checkP

Semaph1  = Semaphore.new(1) # Мьютекс через семафор

# Списки заявок
$call_list        = []    # Все, которые были
$waiting_list     = []    # Не принятые
$processed_list   = []    # Принятые
$сompleted_list   = []    # Выполненные

# Счётчик заявок
$call_count       = 0
$waiting_calls    = 0     # На ожидании
$processed_calls  = 0     # Принятые
$сompleted_calls  = 0     # Выполненные

# Список этажей для текущего списка заявок
$processed_floor_list = []

class Call
  include MonitorMixin
  def initialize(floorStart, floorEnd)
    super()

    @current_call =  new_cond
    @call_floorSt = floorStart                # Начальный этаж вызова
    @call_floorEn = floorEnd                  # Конечный этаж вызова
    @call_checkP = 0                          # Контрольные точки

    if(@call_floorSt < @call_floorEn)         # Движение вверх
      @call_direction = "up"
    else (@call_floorSt > @call_floorEn)      # Движение вниз
      @call_direction= "down"
    end
  end

  # Геттеры
  def call_floorSt
    @call_floorSt
  end
  def call_floorEn
    @call_floorEn
  end
  def call_direction
    @call_direction
  end
  def call_checkP
    @call_direction
  end
  def call_struct
    $call_struct.new(@call_floorSt, @call_floorEn, @call_direction, @call_checkP)
  end

# Попытка вставить заявку в текущие
  def try_execution_сall(elevator)
    synchronize do
      # Если "режим" заявки не тот или лифт уже проехал, то заявка уходит в список ожидания
      if((@call_direction != elevator.mode) ||
        (@call_floorSt > elevator.current_floor && elevator.current_direct == "down" &&  elevator.mode == "down") ||
        (@call_floorSt < elevator.current_floor && elevator.current_direct == "up" && elevator.mode == "up"))
        if(@call_direction == elevator.mode)
          puts("* В ОЖИДАНИИ: (По этажу)        Заявка с #{@call_floorSt} на #{@call_floorEn}, направление: #{@call_direction}")
        else
          puts("* В ОЖИДАНИИ: (По направлению)  Заявка с #{@call_floorSt} на #{@call_floorEn}, направление: #{@call_direction}")
        end
        # Добавить в список ожидания
        $waiting_calls += 1
        $waiting_list << $call_list[$call_count-1]
        # Заблокировать
        @current_call.wait

        # Если направление совпало или поменялось, удалить из ожидания и добавить в активные
        $waiting_calls    -= 1
        $waiting_list.shift
        puts("* ПРОБУЖДЕНИЕ:")
        Semaph1.V
      end
    end

    # Добавить в список активных заявок
    puts("* ПРИНЯТА: Заявка с #{@call_floorSt} на #{@call_floorEn}, направление: #{@call_direction}")
    $processed_calls  += 1
    $processed_list << $call_struct.new(@call_floorSt, @call_floorEn, @call_direction, @call_checkP)


    # Запись и сортировка чекпоинтов
    $processed_floor_list << @call_floorSt
    $processed_floor_list << @call_floorEn
    $processed_floor_list = $processed_floor_list.uniq
    $processed_floor_list = $processed_floor_list.sort if (elevator.mode == "up")
    $processed_floor_list = $processed_floor_list.sort { |a, b| b <=> a } if (elevator.mode == "down")
    puts ("processed_floor_list = #{$processed_floor_list}")
  end

  def check_call(mode)
    flag = 0
    synchronize do
      if(mode == @call_direction)
        @current_call.signal
        flag = 1
      end
    end
    return flag
  end

end

# Класс лифта
class Elevator
  # Инициализация
  def initialize (max_floor)
    @current_floor = 1
    @mode = "up"
    @current_direct = "up"
    @max_floor = max_floor
  end

  # Движение на заданный этаж
  def go_to_floor()
    if($processed_floor_list.length > 0)

      if($processed_floor_list[0] > @current_floor)
        @current_direct = "up"
        while($processed_floor_list[0] != @current_floor)
          @current_floor += 1
          puts("Лифт поднялся с #{@current_floor - 1} на #{@current_floor}")
          sleep 1
        end

      else
        @current_direct = "down"
        while($processed_floor_list[0] != @current_floor)
          @current_floor -= 1
          puts("Лифт опустился с #{@current_floor + 1} на #{@current_floor}")
          sleep 1
        end
      end

    end
  end

  # Смена направления
  def edit_mode
    if(@mode == "up")
      @mode = "down"
    else
      @mode = "up"
    end
    puts("Теперь режим: #{@mode}")
  end

  # Сеттер
  def set_mode(mode)
    @mode = mode
    puts("Теперь режим: #{@mode}")
  end

  # Геттеры
  def current_floor
    @current_floor
  end
  def current_direct
    @current_direct
  end
  def mode
    @mode
  end
end

# Создание объектов
elevator = Elevator.new(20)

# Добавление заявки в процесс
def add_call(floorSt, floorEn, elevator)
  # Создание заявки
  current_call = Call.new(floorSt, floorEn)
  # Подсчёт заявок всего и внесение в список
  $call_count += 1
  $call_list << current_call
  # Попытка сразу же активировать
  $call_list[$call_count-1].try_execution_сall(elevator)
end

# massive = [ [8, 3], [4, 5], [3, 7], [5, 2], [8, 9]]
# massive = [ [7, 2], [6, 5], [9, 7], [5, 2], [9, 1]]
# massive = [ [9, 3], [3, 1], [8, 6], [5, 3], [9, 1]]
# massive = [ [9, 1], [9, 1], [9, 1],[9, 1], [2, 1]]
massive = [ [9, 1], [5, 1], [7, 1], [4, 1], [2, 1]]

# Создание объектов
calls_thread = Thread.new do
  i = 0
  5.times do
    sleep (i+1)*0.75
    i += 1
    Thread.new do
      add_call(massive[i-1][0], massive[i-1][1], elevator)
    end
  end
end

# Сортировка для дальнейшей работы
def sort_waiting(mode)
  if(mode == "up")
    $waiting_list = $waiting_list.sort_by {| a | [!a.call_struct.direction, a.call_struct.floorStart] }
  else
    $waiting_list = $waiting_list.sort_by {| a | [a.call_struct.direction, !a.call_struct.floorStart] }
  end
end

# Попытка пробуждения заявок
def try_wake_up(elevator)
  # Предварительная сортировка
  sort_waiting(elevator.mode)

  i = 0
  try_count = 0
  $waiting_calls.times do
    # puts("Заявки на выполнение: #{i} #{$waiting_list[i].call_struct}")
    try_count = try_count + $waiting_list[i].check_call(elevator.mode)
    i += 1
  end
  return try_count
end

# Работа лифта
work_elevator = Thread.new do
  while ($сompleted_calls < 5)

    while ($processed_calls == 0)

      if($waiting_calls > 0)
        elevator.edit_mode
        if (try_count = try_wake_up(elevator)) > 0
          puts("Смена направления пробудила заявок: #{try_count}")
          Semaph1.P
        end
      end
      Semaph1.P
      puts ("ОЖИДАНИЕ waiting_calls = #{$waiting_calls}, processed_calls = #{$processed_calls}, сompleted_calls =  #{$сompleted_calls}, mode =  #{elevator.mode}")
      Semaph1.V
      sleep 1
    end

    offset = 0
    while($processed_floor_list.length > 0)
      elevator.go_to_floor()

      j = 0
      length = $processed_list.length
      length.times do
        # Если совпадает с floorStart или floorEnd, то отметить чекпоинт
        if (($processed_list[j].floorStart == $processed_floor_list[0]) || ($processed_list[j].floorEnd == $processed_floor_list[0]))
          $processed_list[j].checkP += 1
          puts("Подобрал человека ") if  ($processed_list[j].checkP == 1)
          puts("Высадил человека") if  ($processed_list[j].checkP == 2)
        end

        # Если чекпоинтов досточно, заявка выполнена
        if($processed_list[j].checkP == 2 )
          $processed_calls -= 1
          $сompleted_calls += 1
          $processed_list.delete_at(j)
          j -= 1
        end
        j += 1
      end

      # Удалить этаж из списка текущих этажей
      $processed_floor_list.shift
      puts ("processed_floor_list = #{$processed_floor_list}")
    end
    puts ("waiting_calls = #{$waiting_calls}, processed_calls = #{$processed_calls}, сompleted_calls =  #{$сompleted_calls}")
  end
end

work_elevator.join
calls_thread.join
