// Vector.h
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class Vector {
private:
    std::vector<double> data;  // Хранение компонент вектора
public:
    Vector() {}  // Конструктор по умолчанию
    explicit Vector(size_t size) : data(size, 0.0) {}  // Конструктор с заданным размером
    Vector(const std::vector<double>& values) : data(values) {}  // Конструктор из std::vector

    // === МЕТОДЫ ВВОДА/ВЫВОДА ===

    // Чтение из файла (формат: числа через пробел)
    void read_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + filename);
        data.clear();
        double value{};
        while (file >> value)  // Читаем пока есть числа
            data.push_back(value);
        file.close();
    }

    // Чтение с консоли
    void read_from_console() {
        std::cout << "Введите размер вектора: ";
        size_t size{};
        std::cin >> size;
        data.resize(size);
        std::cout << "\nВведите " << size << " элементов:\n";
        for (size_t i = 0; i < size; ++i)
            std::cin >> data[i];
    }

    // Заполнение случайными числами в диапазоне [min_val, max_val]
    void fill_random(double min_val, double max_val) {
        std::random_device rd;  // Источник случайности
        std::mt19937 gen(rd());  // Генератор
        std::uniform_real_distribution<> dis(min_val, max_val);  // Равномерное распределение
        for (auto& val : data)
            val = dis(gen);
    }

    // Запись в файл
    void print_to_file(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Ошибка открытия файла: " + filename);
        for (const double& value : data)
            file << value << " ";
        file << '\n';
        file.close();
    }

    // Вывод на консоль
    void print_console() const {
        for (const double& value : data)
            std::cout << value << " ";
        std::cout << '\n';
    }

    // === АРИФМЕТИЧЕСКИЕ ОПЕРАЦИИ ===

    // Сложение векторов
    Vector operator+(const Vector& other) const {
        if (data.size() != other.data.size())
            throw std::runtime_error("Размеры векторов не равны\n");
        Vector result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    // Вычитание векторов
    Vector operator-(const Vector& other) const {
        if (data.size() != other.data.size())
            throw std::runtime_error("Размеры векторов не равны\n");
        Vector result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            result.data[i] = data[i] - other.data[i];
        return result;
    }

    // Скалярное произведение векторов
    double dot(const Vector& other) const {
        if (data.size() != other.data.size())
            throw std::runtime_error("Размеры векторов не равны\n");
        double result = 0.0;
        for (size_t i = 0; i < data.size(); ++i)
            result += data[i] * other.data[i];
        return result;
    }

    // === НОРМЫ И ПОГРЕШНОСТИ ===

    // Норма: максимальная по модулю компонента (ТОЧНО по условию п.2.1)
    double norm() const {
        double max_val = 0.0;
        for (const double& value : data)
            max_val = std::max(max_val, std::abs(value));
        return max_val;
    }

    // Относительная погрешность (ТОЧНО по условию п.2.3.1)
    double relative_error(const Vector& exact) const {
        if (data.size() != exact.data.size())
            throw std::runtime_error("Размеры векторов не равны");

        double max_rel_error = 0.0;
        double epsilon = std::sqrt(std::numeric_limits<double>::epsilon());  // корень(машинная точность)

        for (size_t i = 0; i < data.size(); ++i) {
            double exact_val = std::abs(exact[i]);  // Модуль точного значения
            double abs_error = std::abs(data[i] - exact[i]);  // Абсолютная погрешность

            if (exact_val < epsilon) {
                // Если точное решение близко к нулю, берем абсолютную погрешность
                max_rel_error = std::max(max_rel_error, abs_error);
            }
            else {
                // Иначе вычисляем относительную: |приближённое - точное| / |точное|
                max_rel_error = std::max(max_rel_error, abs_error / exact_val);
            }
        }

        return max_rel_error;
    }

    // === ДОСТУП К ЭЛЕМЕНТАМ ===

    // Индексация с 0 (для C++ стиля)
    double& operator[](size_t index) {
        if (index >= data.size())
            throw std::out_of_range("Индекс вектора вне диапазона");
        return data[index];
    }

    const double& operator[](size_t index) const {
        if (index >= data.size())
            throw std::out_of_range("Индекс вектора вне диапазона");
        return data[index];
    }

    // Индексация с 1 (для математического стиля, как в вашей первой лабе)
    double& operator()(size_t index) {
        if (index < 1 || index > data.size())
            throw std::out_of_range("Индекс вектора вне диапазона");
        return data[index - 1];
    }

    const double& operator()(size_t index) const {
        if (index < 1 || index > data.size())
            throw std::out_of_range("Индекс вектора вне диапазона");
        return data[index - 1];
    }

    // Получение размера вектора
    size_t size() const { return data.size(); }

    // Изменение размера вектора
    void resize(size_t new_size, double value = 0.0) {
        data.resize(new_size, value);
    }
};