#pragma once
#include "Vector.h"

enum MatrixCondition { WELL_CONDITIONED, ILL_CONDITIONED };

double machine_number() {
    double precision = 1.0;
    double next = 1.0 + precision;
    double beta = 2.0;
    while (next > 1.0) {
        precision = precision / beta;
        next = 1.0 + precision;
    }
    return precision = precision * beta;
}


class TridiagonalMatrix
{
private:
    Vector left;
    Vector midle;
    Vector right;
public:
    TridiagonalMatrix() {};
    TridiagonalMatrix(const Vector& lower, const Vector& main, const Vector& upper) : left(lower), midle(main), right(upper)
    {
        if (left.size() != midle.size() - 1 || right.size() != midle.size() - 1)
            throw std::runtime_error("Недопустимые размеры диагоналей\n");
    }

    // Считывание из файла
    void read_from_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
            throw  std::runtime_error("Ошибка открытия файла: " + filename);
        size_t n;
        file >> n;
        left.resize(n - 1);
        midle.resize(n);
        right.resize(n - 1);
        for (size_t i = 0; i < n - 1; ++i)
            file >> left[i];
        for (size_t i = 0; i < n; ++i)
            file >> midle[i];
        for (size_t i = 0; i < n - 1; ++i)
            file >> right[i];
        file.close();
    }

    // Считывание с экрана
    void read_from_console()
    {
        std::cout << "Ведите размер матрицы: ";
        size_t n;
        std::cin >> n;
        left.resize(n - 1);
        midle.resize(n);
        right.resize(n - 1);
        std::cout << "Введите " << n - 1 << " элемент для нижней диагонали: ";
        for (size_t i = 0; i < n - 1; ++i)
            std::cin >> left[i];
        std::cout << "Введите " << n << " элемент для главной диагонали: ";
        for (size_t i = 0; i < n; ++i)
            std::cin >> midle[i];
        std::cout << "Введите " << n - 1 << " элемент для верхней диагонали: ";
        for (size_t i = 0; i < n - 1; ++i)
            std::cin >> right[i];
    }

    // Заполнение случайными числами
    /*void fill_random(double min_val, double max_val)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(min_val, max_val);
        for (size_t i = 0; i < left.size(); ++i)
            left[i] = dist(gen);
        for (size_t i = 0; i < midle.size(); ++i)
            midle[i] = dist(gen);
        for (size_t i = 0; i < right.size(); ++i)
            right[i] = dist(gen);
    }*/

    // Вывод в файл
    void print_to_file(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
            throw  std::runtime_error("Ошибка открытия файла: " + filename);
        file << midle.size() << '\n';
        for (size_t i = 0; i < left.size(); ++i)
            file << left[i] << " ";
        file << '\n';
        for (size_t i = 0; i < midle.size(); ++i)
            file << midle[i] << " ";
        file << '\n';
        for (size_t i = 0; i < right.size(); ++i)
            file << right[i] << " ";
        file << '\n';
        file.close();
    }

    // Вывод на экран
    void print_console() const
    {
        size_t n = midle.size();
        for (size_t i = 1; i <= n; ++i)
        {
            for (size_t j = 1; j <= n; ++j)
            {
                if (j == i - 1 && i > 1)
                    std::cout << left_(i) << " ";
                else if (j == i)
                    std::cout << midle_(i) << " ";
                else if (j == i + 1 && i < n)
                    std::cout << right_(i) << " ";
                else
                    std::cout << "0 ";
            }
            std::cout << '\n';
        }
    }

    // Умножение матрицы на вектор
    Vector operator*(const Vector& vec) const
    {
        if (vec.size() != midle.size())
            throw std::runtime_error("Размеры матрицы и вектора не совпадают");

        size_t n = midle.size();
        Vector result(n);

        // Первый элемент (i=1)
        result(1) = midle_(1) * vec(1) + right_(1) * vec(2);

        // Средние элементы (i=2 до n-1)
        for (size_t i = 2; i <= n - 1; ++i)
            result(i) = left_(i) * vec(i - 1) + midle_(i) * vec(i) + right_(i) * vec(i + 1);

        // Последний элемент (i=n)
        result(n) = left_(n) * vec(n - 1) + midle_(n) * vec(n);

        return result;
    }

    size_t size() const { return midle.size(); }

    // Геттеры для диагоналей (с индексацией с 1)
    double left_(size_t i) const
    {
        if (i < 2 || i > midle.size())
            throw std::out_of_range("Индекс вне диапазона");
        return left[i - 2];
    } // a2..an

    double midle_(size_t i) const
    {
        if (i < 1 || i > midle.size())
            throw std::out_of_range("Индекс вне диапазона");
        return midle[i - 1];
    } // b1..bn

    double right_(size_t i) const
    {
        if (i < 1 || i > midle.size() - 1)
            throw std::out_of_range("Индекс вне диапазона");
        return right[i - 1];
    } // c1..cn-1

    // Сеттеры для внутреннего использования
    double& left_(size_t i)
    {
        if (i < 2 || i > midle.size())
            throw std::out_of_range("Индекс вне диапазона");
        return left[i - 2];
    }

    double& midle_(size_t i)
    {
        if (i < 1 || i > midle.size())
            throw std::out_of_range("Индекс вне диапазона");
        return midle[i - 1];
    }

    double& right_(size_t i)
    {
        if (i < 1 || i > midle.size() - 1)
            throw std::out_of_range("Индекс вне диапазона");
        return right[i - 1];
    }

    // Публичные геттеры для доступа к диагоналям
    const Vector& get_left() const { return left; }
    const Vector& get_midle() const { return midle; }
    const Vector& get_right() const { return right; }


    // Метод прогонки
    Vector sweep_method(const Vector& d, int flag) const
    {
        if (d.size() != midle.size())
            throw std::runtime_error("Размеры матрицы и вектора не совпадают");

        size_t n = midle.size();
        Vector x(n);
        Vector l(n + 1);
        Vector m(n + 1);

        double machin_number = machine_number();


        if (std::abs(midle_(1)) < machin_number || std::abs(midle_(1)) == 0) //
            throw std::runtime_error("b[1] равно нулю или слишком мало");

        // Первый шаг: i = 1
        l(2) = right_(1) / midle_(1);
        m(2) = d(1) / midle_(1);

        // Прямой ход для i = 2 до n-1
        for (size_t i = 2; i <= n - 1; ++i)
        {
            double denominator = midle_(i) - left_(i) * l(i);
            if (std::abs(denominator) < machin_number)
                throw std::runtime_error("denominator равен нулю или слишком мал");

            l(i + 1) = right_(i) / denominator;
            m(i + 1) = (d(i) - left_(i) * m(i)) / denominator;
        }


        // Последний шаг: i = n
        double denominator = midle_(n) - left_(n) * l(n);
        if (std::abs(denominator) < machin_number)
            throw std::runtime_error("denominator равен нулю или слишком мал");

        x(n) = (d(n) - left_(n) * m(n)) / denominator;
        m(n + 1) = (d(n) - left_(n) * m(n)) / denominator;

        if (flag == 1) {
            std::cout << "============ИТОГ ВЫЧЕСЛЕНИЙ=========== \n";

            // Итоги вычеслений
            for (size_t i = 2; i < l.size(); i++) {
                for (size_t j = 1; j < l.size(); j++) {
                    if (j == i)
                        std::cout << l(i) << " ";
                    if (j == i - 1)
                        std::cout << "1 ";
                    else
                        std::cout << "0 ";
                }
                std::cout << '\n';
            }

            std::cout << "Вектор правой части (М): ";
            m.print_console();
        }

        // Обратный ход
        for (int i = n - 1; i >= 1; --i)
            x(i) = m(i + 1) - l(i + 1) * x(i + 1);

        return x;
    }

    // Неустойчивый метод
    Vector unsustainable_method(const Vector& d) const
    {
        double machin_number = machine_number();


        if (d.size() != midle.size())
            throw std::runtime_error("Размеры матрицы и вектора не совпадают");

        size_t n = midle.size();
        Vector x(n), y(n), z(n);

        for (size_t i = 1; i <= n - 1; ++i)
            if (std::abs(right_(i)) < machin_number || std::abs(right_(i)) == 0)
                throw std::runtime_error("c[i] равно нулю или слишком мало");

        y(1) = 0.0;
        y(2) = d(1) / right_(1);

        for (size_t i = 2; i <= n - 1; ++i)
            y(i + 1) = (d(i) - left_(i) * y(i - 1) - midle_(i) * y(i)) / right_(i);

        z(1) = 1.0;
        z(2) = -midle_(1) / right_(1);

        

        for (size_t i = 2; i <= n - 1; ++i)
            z(i + 1) = -(left_(i) * z(i - 1) + midle_(i) * z(i)) / right_(i);

        double denominator = (left_(n) * z(n - 1) + midle_(n) * z(n));
        if (std::abs(denominator) < machin_number || std::abs(denominator) == 0)
            throw std::runtime_error("denominator равен нулю или слишком мал");

        //for(test : z)
        //std::cout << z(1);

        /*std::cout << "вектор Z: ";
        for (size_t i = 0;i < z.size();i++) {
            std::cout << z[i] << " ";
        }
        std::cout << "\n вектор Y: ";
        for (size_t i = 0; i < y.size(); i++) {
            std::cout << y[i] << " ";
        }*/

        double K = (d(n) - left_(n) * y(n - 1) - midle_(n) * y(n)) / denominator;

        for (size_t i = 1; i <= n; ++i)
            x(i) = y(i) + K * z(i);

        return x;
    }

    void fill_random_conditional(MatrixCondition condition, size_t size) {
        // Простая инициализация - убираем сложные random-объекты
        srand(time(0));  // Инициализируем генератор текущим временем

        size_t n = size;
        left.resize(n - 1);
        midle.resize(n);
        right.resize(n - 1);

        // 1. СНАЧАЛА генерируем ВСЕ диагонали случайными числами
        for (size_t i = 0; i < n; ++i) {
            midle[i] = 100.0 + rand() % 100;  // Центральная: 100-200
        }
        for (size_t i = 0; i < n - 1; ++i) {
            left[i] = 100.0 + rand() % 100;   // Нижняя: 100-200
        }
        for (size_t i = 0; i < n - 1; ++i) {
            right[i] = 100.0 + rand() % 100;  // Верхняя: 100-200
        }




        switch (condition) {
        case WELL_CONDITIONED:
            // ХОРОШО: большая диагональ, маленькие боковые
            for (size_t i = 1; i < midle.size() - 1; i++) {
                double left_int = left[i - 1];
                double right_int = right[i];
                double sum = left_int + right_int;

                if (midle[i] <= sum) {
                    // На сколько нужно уменьшить боковые
                    double excess = sum / midle[i];
                    double scale_factor = excess * 2; // Домножаем на 0.5 для гарантии
                    // Умножаем боковые элементы
                    midle[i] *= scale_factor;
                }
            }
            if (midle[0] <= right[0]) {
                double scale_factor = (midle[0] / right[0]) * 2;
                midle[0] *= scale_factor;
            }
            if (midle[midle.size() - 1] <= left[left.size() - 1]) {
                double scale_factor = (midle[midle.size() - 1]  / left[left.size() - 1])*2;
                midle[midle.size() - 1] *= scale_factor;
            }
            break;

        case ILL_CONDITIONED:
            // ПЛОХО: все элементы почти одинаковые
            for (size_t i = 1; i < midle.size() - 1; i++) {
                double left_int = left[i - 1];
                double right_int = right[i];
                double sum = left_int + right_int;

                if (midle[i] > sum) {
                    // На сколько нужно уменьшить боковые
                    double excess = sum - midle[i];
                    double scale_factor = (midle[i] * 2.0) / sum; // Домножаем на 0.5 для гарантии

                    // Умножаем боковые элементы
                    left[i - 1] *= scale_factor;
                    right[i] *= scale_factor;
                }
            }
            if (midle[0] > right[0]) {
                double scale_factor = (midle[0] * 2.0) / right[0];
                right[0] *= scale_factor;
            }
            if (midle[midle.size() - 1] > left[left.size() - 1]) {
                double scale_factor = (midle[midle.size() - 1] * 2.0) / left[left.size() - 1];
                left[left.size() - 1] *= scale_factor;
            }
            break;
        }
    }

};

