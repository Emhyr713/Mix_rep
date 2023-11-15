#include <iostream>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <fstream>

using namespace std;
using namespace cv;

void Sobel_Filter(Mat image, Mat res_im, int begin, int end)
{
    for (int y = begin; y < end; y++)
    {
        for (int x = 1; x < image.cols - 1; x++)
        {
            int gx = (image.at<uchar>(y - 1, x + 1) + 2 * image.at<uchar>(y, x + 1) + image.at<uchar>(y + 1, x + 1)) -
                (image.at<uchar>(y - 1, x - 1) + 2 * image.at<uchar>(y, x - 1) + image.at<uchar>(y + 1, x - 1));
            int gy = (image.at<uchar>(y + 1, x - 1) + 2 * image.at<uchar>(y + 1, x) + image.at<uchar>(y + 1, x + 1)) -
                (image.at<uchar>(y - 1, x - 1) + 2 * image.at<uchar>(y - 1, x) + image.at<uchar>(y - 1, x + 1));
            
            int sum = sqrt(gx * gx + gy * gy);           // √(Gx^2 + Gy^2)
            if (sum > 255)      sum = 255;               // Если переполнение
            if (sum < 0)        sum = 0;                 // Если недополнение
            res_im.at<uchar>(y, x) = sum;                // Запись результата в пиксель
        }
    }
}

int main()
{           
    int n_thread = 32;                                    // Количество потоков

    ofstream fout("test_records.txt", ios::app);

    const char *image = "kotya.jpg";
    //const char *image = "Stasya.jpg";

    // Открытие картинок
    Mat bw_im = imread(image, IMREAD_GRAYSCALE);         // ЧБ
    Mat original = imread(image, IMREAD_COLOR);          // Оригинал
    Mat res_im = bw_im.clone();                          // Результат

    // Заполнение чёрным
    for (int y = 0; y < bw_im.rows; y++)
        for (int x = 0; x < bw_im.cols; x++)
            res_im.at<uchar>(y, x) = 0;

    // Разбиение на отрезки
    int delta_y = (bw_im.rows - 1) / n_thread;
    int y1 = 1;
    int y2 = delta_y;

    clock_t time_start = clock();                       // Начало отсчёта
    for (int i_thread = 0; i_thread < n_thread; i_thread++)
    {
        if (i_thread == n_thread - 1)
            thread{ Sobel_Filter, bw_im, res_im, y1, y2 }.join();
        else
            thread{ Sobel_Filter, bw_im, res_im, y1, y2 }.detach();

        // Переход к следующему отрезку
        y1 = y2;
        y2 += delta_y;
    }
    clock_t time_end = clock();                         // Конец отсчёта
    double seconds = (double)(time_end - time_start) / CLOCKS_PER_SEC;
    cout << n_thread << " threads " << " -- " << seconds * 1000 << " ms" << endl;
    fout << n_thread << " threads "  << " -- " << seconds * 1000 << " ms" << endl;

    namedWindow("Результат");
    imshow("Результат", res_im);

    namedWindow("Оригинал");
    imshow("Оригинал ", original);

    waitKey();

    return 0;
}
