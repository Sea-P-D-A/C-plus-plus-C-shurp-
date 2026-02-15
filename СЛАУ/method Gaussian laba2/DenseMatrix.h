#pragma once
#include "Vector.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>
#include <stdexcept>

//относительная погрешность в 5 + сделать чтобы в матрице гилберта можно было получить пятый пункт меню
//  + сделать пятыйй пункт на основе 4 с зависимостями от размерности

// Типы матриц для экспериментов
enum MatrixType { RANDOM_MATRIX, HILBERT_MATRIX, DIAG_DOMINANT_MATRIX };

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

class DenseMatrix {
private:
    std::vector<std::vector<double>> data;  // Двумерный массив для хранения матрицы
    size_t rows, cols;  // Количество строк и столбцов

public:
    // Конструкторы
    DenseMatrix() : rows(0), cols(0) {}  // Пустая матрица
    DenseMatrix(size_t n) : rows(n), cols(n), data(n, std::vector<double>(n, 0.0)) {}  // Квадратная n×n
    DenseMatrix(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<double>(c, 0.0)) {}  // Прямоугольная r×c

    // Геттеры
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    bool isSquare() const { return rows == cols; }  // Проверка на квадратность

    // Доступ к элементам с проверкой границ
    double& operator()(size_t i, size_t j) {
        if (i >= rows || j >= cols)
            throw std::out_of_range("Индекс матрицы вне диапазона");
        return data[i][j];
    }

    const double& operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols)
            throw std::out_of_range("Индекс матрицы вне диапазона");
        return data[i][j];
    }

    // === МАТРИЧНЫЕ ОПЕРАЦИИ ===

    // Умножение матрицы на вектор: A * x = b
    Vector operator*(const Vector& vec) const {
        if (cols != vec.size())
            throw std::runtime_error("Размеры матрицы и вектора не совпадают");

        Vector result(rows);  // Результат размера rows
        for (size_t i = 0; i < rows; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < cols; ++j) {
                sum += data[i][j] * vec[j];  // Сумма произведений
            }
            result[i] = sum;
        }
        return result;
    }

    // Сложение матриц
    DenseMatrix operator+(const DenseMatrix& other) const {
        if (rows != other.rows || cols != other.cols)
            throw std::runtime_error("Размеры матриц не совпадают");

        DenseMatrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] + other(i, j);
            }
        }
        return result;
    }

    // Вычитание матриц
    DenseMatrix operator-(const DenseMatrix& other) const {
        if (rows != other.rows || cols != other.cols)
            throw std::runtime_error("Размеры матриц не совпадают");

        DenseMatrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] - other(i, j);
            }
        }
        return result;
    }

    // === ВВОД/ВЫВОД ===

    // Чтение из файла (формат: сначала rows cols, потом элементы построчно)
    void read_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + filename);

        file >> rows >> cols;  // Считываем размеры
        data.resize(rows, std::vector<double>(cols));

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                file >> data[i][j];  // Считываем элементы
            }
        }
        file.close();
    }

    // Чтение с консоли
    void read_from_console() {
        std::cout << "Введите количество строк: ";
        std::cin >> rows;
        std::cout << "Введите количество столбцов: ";
        std::cin >> cols;

        data.resize(rows, std::vector<double>(cols));
        std::cout << "Введите элементы построчно:\n";
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cin >> data[i][j];
            }
        }
    }

    // Заполнение случайными числами
    void fill_random(double min_val, double max_val) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min_val, max_val);

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = dis(gen);
            }
        }
    }

    // Матрица Гильберта: A[i][j] = 1/(i+j+1) - ПЛОХО ОБУСЛОВЛЕННАЯ
    void fill_hilbert() {
        if (!isSquare())
            throw std::runtime_error("Матрица Гильберта должна быть квадратной");

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = 1.0 / (i + j + 1.0);  // Знаменатель растёт, элементы малы
            }
        }
    }

    void fill_hilbert_modified() {
        if (!isSquare())
            throw std::runtime_error("Матрица должна быть квадратной");

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                // Классическая: 1/(i+j+1)
                // Модификация: 1/(i+j+0.5) - элементы больше, медленнее убывают
                // + добавляем случайную компоненту к диагонали
                if (i == j) {
                    // На диагонали: 1/(2i+1) + маленькая случайная добавка
                    data[i][j] = 1.0 / (2.0 * i + 1.0) + 0.001;
                }
                else {
                    // Вне диагонали: 1/(i+j+1) - классическая
                    data[i][j] = 1.0 / (i + j + 1.0);
                }
            }
        }
    }


    // Матрица с диагональным преобладанием - ХОРОШО ОБУСЛОВЛЕННАЯ
    void fill_diag_dominant() {
        if (!isSquare())
            throw std::runtime_error("Матрица должна быть квадратной");

        fill_random(-1.0, 1.0);  // Сначала заполняем случайными
        for (size_t i = 0; i < rows; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < cols; ++j) {
                if (i != j) sum += std::abs(data[i][j]);  // Сумма модулей недиагональных
            }
            data[i][i] = sum + 1.0;  // Диагональный элемент > суммы остальных
        }
    }

    // Запись в файл
    void print_to_file(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + filename);

        file << rows << " " << cols << "\n";  // Сохраняем размеры
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                file << std::scientific << std::setprecision(10) << data[i][j] << " ";
            }
            file << "\n";
        }
        file.close();
    }

    // Вывод на консоль
    void print_console() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(10) << data[i][j] << " ";  // Форматированный вывод
            }
            std::cout << "\n";
        }
    }

    // === МЕТОД ГАУССА===
    // Схема единственного деления (без выбора ведущего элемента)
    Vector solve_gauss(const Vector& b) const {
        // Проверки
        if (!isSquare())
            throw std::runtime_error("Матрица должна быть квадратной");
        if (b.size() != rows)
            throw std::runtime_error("Размер вектора не совпадает с размером матрицы");

        size_t n = rows;

        // Копируем матрицу и вектор, чтобы не портить оригиналы
        std::vector<std::vector<double>> A = data;  // Копия матрицы
        Vector x(n);  // Вектор для решения
        std::vector<double> B = std::vector<double>(b.size());  // Копия правой части
        for (size_t i = 0; i < b.size(); ++i) B[i] = b[i];


        double a = machine_number();
        // ПРЯМОЙ ХОД: приведение к верхнетреугольному виду
        for (size_t k = 0; k < n; ++k) {
            // Проверка диагонального элемента (может быть 0)
            if (std::abs(A[k][k]) < a) {
                throw std::runtime_error("Нулевой диагональный элемент в методе Гаусса");
            }

            // Нормировка k-й строки: делим на диагональный элемент
            double pivot = A[k][k];
            for (size_t j = k; j < n; ++j) {
                A[k][j] /= pivot;  // Делим все элементы строки
            }
            B[k] /= pivot;  // Делим правую часть

            // Исключение переменной x_k из всех последующих строк
            for (size_t i = k + 1; i < n; ++i) {
                double factor = A[i][k];  // Множитель для вычитания
                for (size_t j = k; j < n; ++j) {
                    A[i][j] -= factor * A[k][j];  // Вычитаем k-ю строку
                }
                B[i] -= factor * B[k];  // Вычитаем правую часть
            }
        }

        // ОБРАТНЫЙ ХОД: нахождение решения снизу вверх
        for (int i = n - 1; i >= 0; --i) {  // От последней строки к первой
            x[i] = B[i];  // Начальное значение = правая часть
            for (size_t j = i + 1; j < n; ++j) {
                x[i] -= A[i][j] * x[j];  // Вычитаем уже найденные переменные
            }
        }

        return x;
    }

    // Заполнение матрицы заданного типа и размера
    void fill_by_type(MatrixType type, size_t size) {
        rows = cols = size;  // Для экспериментов используем квадратные матрицы
        data.resize(rows, std::vector<double>(cols, 0.0));

        switch (type) {
        case RANDOM_MATRIX:
            fill_random(-10.0, 10.0);  // Обычная случайная матрица
            break;
        case HILBERT_MATRIX:
            fill_hilbert_modified();  // Плохо обусловленная (для эксперимента)
            break;
        case DIAG_DOMINANT_MATRIX:
            fill_diag_dominant();  // Хорошо обусловленная (для эксперимента)
            break;
        }
    }
};