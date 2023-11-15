#include <iostream>
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>                                // Для RGBTRIPLE
#include <thread>                                   // Поток
#include <ctime>

using namespace std;
using namespace cv;

Mat res_im;                                          // Результат

// Вычисляет X градиент
int xGradient(int buff[3][3])
{
    return buff[0][0] + 2 * buff[1][0] + buff[2][0]
         - buff[0][2] - 2 * buff[1][2] - buff[2][2];
}

// Вычисляет Y градиент
int yGradient(int buff[3][3])
{
    return buff[0][0] + 2 * buff[0][1] + buff[0][2]
         - buff[0][2] - 2 * buff[1][2] - buff[2][2];
}

int Gradient(Mat image, int x, int y, int height, int width, int key)
{
    int buff[3][3];     // Буфер
  
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (((x - 1 + i) < 0) || ((y - 1 + j) < 0) || ((x - 1 + i) > width) || ((y - 1 + j) > height))
                buff[i][j] = 0;
            else
                buff[i][j] = image.at<uchar>(y - 1 + j, x - 1 + i);
        }
    }

    // По ключу определяется градиент (X / Y)
    if (key == 0)
        return xGradient(buff);
    else
        return yGradient(buff);
}

void f_thread(Mat image, int delta_y, int height, int width, int threads, int iterartor)
{
    Mat res = image.clone();

    int rest = 0;
    int terty = 0;
    int gx, gy, sum;

    if ((height - delta_y * iterartor) < delta_y)
        rest = height - delta_y * iterartor - 4;

    // Проход по координатам
    for (int y = 1; y < (delta_y + rest); y++) {
        for (int x = 1; x < width; x++) {
            gx = Gradient(image, x, y, delta_y + rest, width, 0);       // Вычисление Gx
            gy = Gradient(image, x, y, delta_y + rest, width, 1);       // Вычисление Gy
            sum = sqrt(gx * gx + gy * gy);                              // √(Gx^2 + Gy^2)
            if (sum > 255)      sum = 255;                              // Если переполнение
            if (sum < 0)        sum = 0;                                // Если недополнение
            terty = iterartor * (delta_y - 1) + y;
            res_im.at<uchar>(terty, x) = sum;
        }// for columns
    }// for rows

    return;
}

int main()
{
    //const char * image = "lena.jpg";
    const char *image = "Stasya.jpg";

    // Чтение изображения
    Mat bw_im = imread(image, IMREAD_GRAYSCALE);        // Открыть в ЧБ
    Mat color_im = imread(image, IMREAD_COLOR);         // Открыть в цвете
    res_im = bw_im.clone();                             // Результат

    int height = bw_im.rows;                            // Высота изображения
    int width = bw_im.cols;                             // Ширина изображения

    cout << "image: " << height << " x " << width << endl;
    
    // Проверка на открытие
    if (!bw_im.data)
        return -1;

    // Разделение на строчки
    const int n_threads = 32;                            // Количество потоков
    int delta_y = height / n_threads;                    // Размер обрабатываемых сегментов

    for (int i = 0; i < n_threads; i++)
    {
        Mat part_im;
        part_im = bw_im(Rect(0, i * delta_y, width, delta_y));
        thread t (f_thread, part_im, delta_y+1, height, width, n_threads, i);
        t.join();
    }

    // Вывод изображения с фильтром
    Sleep(1000);                                         // Дождаться обработки
    namedWindow("final");
    imshow("final", res_im);
    waitKey();
    return 0;
}