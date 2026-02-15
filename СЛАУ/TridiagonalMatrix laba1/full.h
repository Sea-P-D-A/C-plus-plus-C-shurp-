#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include<random>

enum MatrixCondition { WELL_CONDITIONED, ILL_CONDITIONED };

class Vector {
private:
    std::vector<double> data;
public:
    Vector() {};
    // explicit - запрещает неявное преобразование Vector x = 5;
    explicit Vector(size_t size) : data(size, 0.0) {};
    Vector(const std::vector<double>& values) : data(values) {};

    // Чтение из файла
    void read_from_file(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + file_name);
        data.clear();
        double value{};
        while (file >> value)
            data.push_back(value);
        file.close();
    }

    // Чтнение с консоли
    void read_from_console()
    {
        std::cout << "Введите размер вектора: ";
        size_t size;
        std::cin >> size;
        data.resize(size);
        std::cout << "\nВведите " << size << " элементов:\n";
        for (int i = 0; i < size; ++i)
            std::cin >> data[i];
    }

    // Заполнение случайными числами
    void fill_random_values(double min_val, double max_val)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min_val, max_val);
        for (auto& val : data)
            val = dis(gen);
    }

    // Вывод в файл
    void print_to_file(const std::string& file_name) {
        std::ofstream file(file_name);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + file_name);
        for (const double& value : data)
            file << value << " ";
        file.close();
    }

    // Вывод в консоль
    void print_to_console() {
        for (const double& value : data)
            std::cout << value << " ";
        std::cout << "\n";
    }

    // Сумма векторов
    Vector operator+ (const Vector& other) {
        if (data.size() != other.data.size())
            throw std::runtime_error("� азмеры векторов не совпадают");
        Vector result(data.size());
        for (rsize_t i = 0; i < data.size(); i++)
            result.data[i] = data[i] + other.data[i];
        return result;
    }
    // � азность векторов
    Vector operator- (const Vector& other) {
        if (data.size() != other.data.size())
            throw std::runtime_error("� азмеры векторов не совпадают");
        Vector result(data.size());
        for (rsize_t i = 0; i < data.size(); i++)
            result.data[i] = data[i] - other.data[i];
        return result;
    }
    // Доступ к элементам (индексация с 0)
    double& operator[](size_t index)
    {
        if (index >= data.size())
            throw std::out_of_range("Индекс вектора вне диапозона");
        return data[index];
    }

    const double& operator[](size_t index) const
    {
        if (index >= data.size())
            throw std::out_of_range("Индекс вектора вне диапозона");
        return data[index];
    }

    // Доступ к элементам (индексация с 1)
    double& operator()(size_t index)
    {
        if (index < 1 || index > data.size())
            throw std::out_of_range("Индекс вектора вне диапозона");
        return data[index - 1];
    }

    const double& operator()(size_t index) const
    {
        if (index < 1 || index > data.size())
            throw std::out_of_range("Индекс вектора вне диапозона");
        return data[index - 1];
    }

    double scalar_multi(const Vector& other) const {
        if (data.size() != other.data.size())
            throw std::runtime_error("� азмеры векторов не равны\n");
        double result = 0.0;
        for (size_t i = 0; i < data.size(); ++i)
            result += data[i] * other.data[i];
        return result;
    }

    // Норма (максимальная по модулю компонента)
    double norm() const {
        double max_val = 0.0;
        for (const double& value : data)
            max_val = max(max_val, abs(value));
        return max_val;
    }

    // Получение размера
    size_t size()const { return data.size(); }

    // Заполнение вектора заданного размера нулями и изменение размера
    void resize(size_t new_size, double value = 0.0) {
        data.resize(new_size, value);
    }

};

class TridiagonalMatrix {
private:
    Vector left, midle, right;
public:
    TridiagonalMatrix() {};
    TridiagonalMatrix(const Vector& lower, const Vector& main, const Vector& upper) : left(lower), midle(main), right(upper)
    {
        if (left.size() != midle.size() - 1 || right.size() != midle.size() - 1)
            throw std::runtime_error("Недопустимые размеры диагоналей\n");
    }

    // Чтение из файла
    void read_from_file(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + file_name);
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
    // Чтнение с консоли
    void read_from_console()
    {
        std::cout << "Ведите размер матрицы: ";
        size_t n;
        std::cin >> n;
        left.resize(n - 1);
        midle.resize(n);
        right.resize(n - 1);
        std::cout << "Введите " << n - 1 << " элемент для левой диагонали: ";
        for (size_t i = 0; i < n - 1; ++i)
            std::cin >> left[i];
        std::cout << "Введите " << n << " элемент для центральной диагонали: ";
        for (size_t i = 0; i < n; ++i)
            std::cin >> midle[i];
        std::cout << "Введите " << n - 1 << " элемент для правой диагонали: ";
        for (size_t i = 0; i < n - 1; ++i)
            std::cin >> right[i];
    }

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
    void print_to_console() const
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

    // Геттеры для векторов
    const Vector& get_left() const { return left; }
    const Vector& get_midle() const { return midle; }
    const Vector& get_right() const { return right; }

    // Умножение матрицы на вектор
    Vector operator*(const Vector& vec) const
    {
        if (vec.size() != midle.size())
            throw std::runtime_error("� азмеры матрицы и вектора не совпадают");

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

    void fill_random_conditional(MatrixCondition condition, size_t size) {
        // Простая инициализация - убираем сложные random-объекты
        srand(time(0));  // Инициализируем генератор текущим временем

        size_t n = size;
        left.resize(n - 1);
        midle.resize(n);
        right.resize(n - 1);

        switch (condition) {
        case WELL_CONDITIONED:
            // ХО� ОШО: большая диагональ, маленькие боковые
            for (size_t i = 0; i < n; ++i) {
                midle[i] = 100.0 + rand() % 100;  // Большие числа: 100-200
            }
            for (size_t i = 0; i < n - 1; ++i) {
                left[i] = 0.1 + (rand() % 10) * 0.01;   // Маленькие: 0.1-0.2
                right[i] = 0.1 + (rand() % 10) * 0.01;
            }
            break;

        case ILL_CONDITIONED:
            // ПЛОХО: все элементы почти одинаковые
            double base = 1.0 + (rand() % 10) * 0.1;  // Базовое число: 1.0-2.0

            for (size_t i = 0; i < n; ++i) {
                midle[i] = base + 0.000000000001;
            }
            for (size_t i = 0; i < n - 1; ++i) {
                left[i] = base + 0.000000000002;
                right[i] = base + 0.000000000003;
            }

            break;
        }
    }
    size_t size() const { return midle.size(); }


};

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


int main()
{
    setlocale(LC_ALL, "rus");

    std::cout << "Машинная точность: " << machine_number() << "\n";
    Vector a;
    TridiagonalMatrix b;

    std::cout << "� ешение трехдиагональных матриц\n";
    std::cout << "1. � учной ввод\n";
    std::cout << "2. Ввод из файла\n";
    std::cout << "3. Тест на зависимость от размера\n";
    std::cout << "Выбрать пункт меню: ";

    int choice{};
    std::cin >> choice;

    if (choice == 1 || choice == 2)
    {
        TridiagonalMatrix matrix;
        Vector resultHS;
        Vector solution;

        if (choice == 1)
        {
            std::cout << "Введите матрицу (индексы начинаются с 1):\n";
            matrix.read_from_console();

            resultHS.read_from_console();
        }
        else
        {
            std::string matrix_filename, rhs_filename;
            std::cout << "Введите имя файла с матрицей: ";
            std::cin >> matrix_filename;
            matrix.read_from_file(matrix_filename);

            std::cout << "Введите имя файла с правой частью: ";
            std::cin >> rhs_filename;
            resultHS.read_from_file(rhs_filename);

            if (resultHS.size() != matrix.size())
            {
                std::cout << "Ошибка: размер правой части не совпадает с размером матрицы!\n";
                return 1;
            }
        }

        // Показываем введенные данные
        std::cout << "\nВведенная матрица:\n";
        matrix.print_to_console();

        std::cout << "\nПравая часть: ";
        resultHS.print_to_console();

        // Выбор метода решения
        std::cout << "\nВыберите метод решения:\n";
        std::cout << "1. Метод прогонки\n";
        std::cout << "2. Неустойчивый метод\n";
        std::cout << "3. Оба метода\n";
        std::cout << "Выбор: ";

        int method_choice;
        std::cin >> method_choice;

        if (method_choice == 1 || method_choice == 3)
        {
            // Vector sweep_solution = matrix.sweep_method(resultHS);
            std::cout << "\n� ешение методом прогонки: ";
            // sweep_solution.print_console();
        }

        if (method_choice == 2 || method_choice == 3)
        {
            // Vector unsustainable_solution = matrix.unsustainable_method(resultHS);
            std::cout << "\n� ешение неустойчивым методом: ";
            //unsustainable_solution.print_console();
        }

        // Сохранение результатов в файл
        std::cout << "\nСохранить результаты в файл? (y/n): ";
        char save_choice;
        std::cin >> save_choice;

        if (save_choice == 'y' || save_choice == 'Y')
        {
            std::string output_filename;
            std::cout << "Введите имя файла для сохранения: ";
            std::cin >> output_filename;

            std::ofstream outfile(output_filename);
            if (outfile.is_open())
            {
                outfile << "Матрица:\n";
                outfile << "� азмер: " << matrix.size() << "\n";

                // Используем публичные геттеры вместо методов a_, b_, c_
                const Vector& left_diag = matrix.get_left();
                const Vector& midle_diag = matrix.get_midle();
                const Vector& right_diag = matrix.get_right();

                outfile << "Нижняя диагональ: ";
                for (size_t i = 0; i < left_diag.size(); ++i)
                    outfile << left_diag[i] << " ";

                outfile << "\nГлавная диагональ: ";
                for (size_t i = 0; i < midle_diag.size(); ++i)
                    outfile << midle_diag[i] << " ";

                outfile << "\nВерхняя диагональ: ";
                for (size_t i = 0; i < right_diag.size(); ++i)
                    outfile << right_diag[i] << " ";

                outfile << "\n\nПравая часть: ";
                for (size_t i = 1; i <= resultHS.size(); ++i)
                    outfile << resultHS(i) << " ";

                if (method_choice == 1 || method_choice == 3)
                {
                   /*Vector sweep_solution = matrix.sweep_method(resultHS);
                    outfile << "\n\n� ешение методом прогонки: ";
                    for (size_t i = 1; i <= sweep_solution.size(); ++i)
                        outfile << sweep_solution(i) << " "; */
                }

                if (method_choice == 2 || method_choice == 3)
                {
                   /* Vector unsustainable_solution = matrix.unsustainable_method(resultHS);
                    outfile << "\n\n� ешение неустойчивым методом: ";
                    for (size_t i = 1; i <= unsustainable_solution.size(); ++i)
                        outfile << unsustainable_solution(i) << " "; */
                }

                outfile.close();
                std::cout << "� езультаты сохранены в файл: " << output_filename << "\n";
            }
        }
    }

}
