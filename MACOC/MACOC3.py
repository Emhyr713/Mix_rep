import numpy as np
from random import randint, sample
from math import *
import random

from sklearn.metrics import adjusted_rand_score, normalized_mutual_info_score, silhouette_score, fowlkes_mallows_score
from sklearn.datasets import make_moons

import time

# Класс муравья
class Ant:
    def __init__(self, count_points, count_medoids):
        self.M_k = sample(range(count_points), count_medoids)           # Массив длиной count_medoids заполненный случайно идексами точек
        self.W_k = np.zeros((count_points, count_medoids), dtype=int)   # Матрица весов, заполненная нулями
        self.J_k = -1                                                   # Целевая функция
        self.S_k = [-1] * count_points                                  # Массив решений для каждой точки (записываются индексы медоидов)

# Расстояние между точкой и кластером
def calc_distance (point_i, point_j, r_minkovsky):
    point1 = np.array(point_i.data)
    point2 = np.array(point_j.data)
    return np.linalg.norm(point1 - point2, ord=r_minkovsky)

# Произведение феромонного следа на эвристическую функцию
def pheromone_x_heuristic(point_i, point_j, pheromon, alpha, beta, r_minkovsky):    
    a = pheromon ** alpha
    b = (1 / (calc_distance(point_i, point_j, r_minkovsky) + 1e-10)) ** beta
    return a * b

# Разведка муравьём и выбор случайного медоида
def Exploration(heuristic, count_medoids):
    sum_heuristic = np.sum(heuristic)                                   # Нахождение суммы элементов массива

    # print("heuristic: ", heuristic)
    # print("sum_heuristic: ", sum_heuristic)
   
    # Случайная выборка медоида
    probabilities = np.array([prob / sum_heuristic for prob in heuristic])        # Нормализация элементов массива 
    medoid = np.random.choice(range(count_medoids), p=probabilities)    # Выборка
    return medoid

def get_medoiod(ant, point, points, count_medoids,
                alpha, beta, probability_exploration,
                row_pheromones, r_minkovsky):

    # Нахождение массива эвристических значений для муравья от точки до каждого медоида
    heuristic = np.array([pheromone_x_heuristic(point, points[j_medoid],
                                                row_pheromones[j_medoid],
                                                alpha, beta, r_minkovsky)
                                                for j_medoid in ant.M_k])

    # Разведка, если q <= q0
    if(random.uniform(0, 1) >= probability_exploration):
        medoid = Exploration(heuristic, count_medoids)
    # Эксплуатация в противном случае
    else:
        medoid = np.argmax(heuristic)

    return medoid

# Вычисление целевой фуцнкции
def calc_objective_func(M_k, W_k, points, r_minkovsky,
                        count_points, count_medoids):
    J_k = 0
    for i_point in range(count_points):
        for j_medoid in range(count_medoids):
            J_k += W_k[i_point][j_medoid] * calc_distance(points[i_point], points[M_k[j_medoid]], r_minkovsky)
           
    return J_k

# Обновление матрицы феромонов
def update_matrix_pheromons(sorted_mass_J_k, ants, matrix_pheromones,
                            count_points, count_medoids,
                            elitist_ants, p):
    # Испарение феромонов
    matrix_pheromones *= (1 - p)

    for i in range(count_points):
        for j in range(count_medoids):
            for k in range(elitist_ants):
                ant = ants[sorted_mass_J_k[k][1]]                                       # k-й муравей с наилучшей объективной функцией
                current_J_k = sorted_mass_J_k[k][0]                                     # Текущая лучшая объективная функция
                current_medoid = ant.M_k[j]                                             # Номер точки для текущего медоида
                matrix_pheromones[i][current_medoid] += ant.W_k[i][j] * (1/current_J_k) # Увеличение троп, где прошли элитные муравьи
    
    return matrix_pheromones

def statistic_macoc(alg_MACOC):
    n_repeat = 5
    statistic = []

    # Без вывод без статистики
    def without_stat(points, count_medoids,
                       count_ants, elitist_ants,
                       n_iter, alpha, beta,
                       probability_exploration, p, init_pheromone,
                       r_minkovsky,
                       parameter=None):
        
        start_time = time.time()
        
        result, J_k_acoc = alg_MACOC(points, count_medoids,
                                        count_ants, elitist_ants,
                                        n_iter, alpha, beta,
                                        probability_exploration, p, init_pheromone,
                                        r_minkovsky,
                                        parameter=None)
        
        end_time = time.time()
        print("time: ", end_time - start_time)

        X = [point.data for point in points]
        print("silhouette_score: ", silhouette_score(X, result))

        return result

    def calc_statistic(points, count_medoids,
                       count_ants, elitist_ants,
                       n_iter, alpha, beta,
                       probability_exploration, p, init_pheromone,
                       r_minkovsky,
                       parameter = None,
                       reference_solution = None):
        
        start_time_list, end_time_list, result_list, J_k_list = [], [], [], []

        for i in range(n_repeat):
            start_time_list.append(time.time())

            result, J_k_acoc = alg_MACOC(points, count_medoids,
                                        count_ants, elitist_ants,
                                        n_iter, alpha, beta,
                                        probability_exploration, p, init_pheromone,
                                        r_minkovsky,
                                        parameter=None)
            result_list.append(result)
            J_k_list.append(J_k_acoc)

            end_time_list.append(time.time())

        average_time = np.mean(np.array(end_time_list)) -  np.mean(np.array(start_time_list))
        average_result = np.round(np.mean([result for result in result_list], axis=0))
        average_J_k = np.mean(np.array(J_k_list))

        X = [point.data for point in points]

        # a_r_s Мера сходства между истинными и предсказанными кластерами, учитывающая случайное согласование.
        # s_s   Мера, которая оценивает, насколько объект в кластере похож на свои соседние кластеры.
        # f_m_s Мера, оценивающая точность и полноту между истинными и предсказанными кластерами.

        if(parameter != None):
            if(len(reference_solution) == 0):
                statistic.append([round(parameter, 5), average_time, average_J_k,
                                # adjusted_rand_score(reference_solution, average_result),
                                silhouette_score(X, average_result),
                                # normalized_mutual_info_score(reference_solution, average_result),
                                # fowlkes_mallows_score(reference_solution, average_result)
                                ])
            else:
                statistic.append([round(parameter, 5), average_time, average_J_k,
                adjusted_rand_score(reference_solution, average_result),
                silhouette_score(X, average_result),
                normalized_mutual_info_score(reference_solution, average_result),
                fowlkes_mallows_score(reference_solution, average_result)
                ])

        if parameter is not None:
            print(round(parameter, 5), average_time, average_J_k, silhouette_score(X, average_result))
        else:
            print(average_time, average_J_k, silhouette_score(X, average_result))

        return result
    
    def get_statistic():
        return statistic
    
    def clear_statistic():
        del statistic [:]
        return
        
    calc_statistic.get_statistic = get_statistic
    calc_statistic.clear_statistic = clear_statistic
    calc_statistic.without_stat = without_stat
    return calc_statistic

@statistic_macoc
def alg_MACOC (points, count_medoids,
                count_ants, elitist_ants,
                n_iter, alpha, beta,
                probability_exploration, p, init_pheromone,
                r_minkovsky,
                parameter=None,
                reference_solution = None):

    best_solution = -1                  # Лучшее решение
    result_solutions = []               # Список соответствия точек и кластеров (индекс: номер точки, значение: номер кластера)
    count_points = len(points)          # Количество объектов
    count_medoids = int(count_medoids)

    # Начальное распределение феромонов
    matrix_pheromones = np.random.uniform(init_pheromone - 0.05 , init_pheromone + 0.05, size=(count_points, count_points))

    for i_iter in range(n_iter):
       
        ants = []                       # Объявление count_ants муравьёв
        massive_J_k = []                # Массив объективных функций

        for i_ant in range(count_ants):
            ants.append(Ant(count_points, count_medoids))
            cur_ant = ants[-1]

            for i_point in range(count_points):
                # Выбор кластера для текущей точки
                current_medoid = get_medoiod(cur_ant, points[i_point], points, count_medoids,
                                             alpha, beta, probability_exploration,
                                             matrix_pheromones[i_point, :], r_minkovsky)

                # Занесение данных
                cur_ant.W_k[i_point][current_medoid] = 1      # В матрицу весов
                cur_ant.S_k[i_point] = current_medoid         # В матрицу решений

            # Прошёл по всем точкам
             
            # Вычисление объективной функции J_k 
            cur_ant.J_k = calc_objective_func(cur_ant.M_k, cur_ant.W_k, points, r_minkovsky,
                                              count_points, count_medoids)
            massive_J_k.append([cur_ant.J_k, i_ant])        # Значение целевой функции и номер муравья
        # Просчитались все муравьи

        # Отсотировать и упаковать в массив целевые функции
        massive_J_k = sorted(massive_J_k, key=lambda x: x[0])

        # Если найдено более лучшее решение, то запомнить его
        if(massive_J_k[0][0] < best_solution or best_solution < 0):
            best_solution = massive_J_k[0][0]
            result_solutions = ants[massive_J_k[0][1]].S_k
            # print(i_iter, " ", best_solution)

        matrix_pheromones = update_matrix_pheromons(massive_J_k, ants, matrix_pheromones,
                                                    count_points, count_medoids,
                                                    elitist_ants, p)

    return np.array(result_solutions), best_solution


# Отдельный запуск
class Point:
    def __init__(self, data):
        self.data = data

count_points = 100
seed = 42
points = []
X, y = make_moons(n_samples=count_points, noise=0.05, random_state=seed)
points.extend(Point([x, y]) for (x, y) in zip(X[:, 0], X[:, 1]))
count_medoids = 2
count_ants = 10
elitist_ants = 1
n_iter = 1
alpha = 1
beta = 2
probability_exploration = 0.0001
p = 0.1
init_pheromone = 0.75
r_minkovsky = 2



# alg_MACOC(points, count_medoids, count_ants, elitist_ants, n_iter, alpha, beta, probability_exploration, p, init_pheromone, r_minkovsky)