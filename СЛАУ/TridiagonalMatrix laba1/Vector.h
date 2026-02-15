#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include<random>
#include<Windows.h>



class Vector
{
private:
    std::vector<double> data;
public:
    Vector() {};
    explicit Vector(size_t size) : data(size, 0.0) {};
    Vector(const std::vector<double>& values) : data(values) {};

    // Считывание из файла
    void read_from_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
            throw  std::runtime_error("Ошибка открытия файла: " + filename);
        data.clear();
        double value{};
        while (file >> value)
            data.push_back(value);
        file.close();
    }

    // Считывание с экрана
    void read_from_console()
    {
        std::cout << "Введите размер вектора: ";
        size_t size{};
        std::cin >> size;
        data.resize(size);
        std::cout << "\nВведите " << size << " элементов:\n";
        for (size_t i = 0; i < size; ++i)
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
    void print_to_file(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
            throw  std::runtime_error("Ошибка открытия файла: " + filename);
        for (const double& value : data)
            file << value << " ";
        file << '\n';
        file.close();
    }

    // Вывод на экран
    void print_console() const
    {
        for (const double& value : data)
            std::cout << value << " ";
        std::cout << '\n';
    }

    // Операции с векторами
    Vector operator+(const Vector& other) const
    {
        if (data.size() != other.data.size())
            throw std::runtime_error("Размеры векторов не равны\n");
        Vector result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    Vector operator-(const Vector& other) const
    {
        if (data.size() != other.data.size())
            throw std::runtime_error("Размеры векторов не равны\n");
        Vector result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            result.data[i] = data[i] - other.data[i];
        return result;
    }

    double scalar_multi(const Vector& other) const
    {
        if (data.size() != other.data.size())
            throw std::runtime_error("Размеры векторов не равны\n");
        double result = 0.0;
        for (size_t i = 0; i < data.size(); ++i)
            result += data[i] * other.data[i];
        return result;
    }

    // Норма (максимальная по модулю компонента)
    double norm() const
    {
        double max_val = 0.0;
        for (const double& value : data)
            max_val = max(max_val, abs(value)); // Поиск максимального модуля
        return max_val;
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

    size_t size() const { return data.size(); }

    // Заполнение вектора заданного размера
    void resize(size_t new_size, double value = 0.0)
    {
        data.resize(new_size, value);
    }
};
