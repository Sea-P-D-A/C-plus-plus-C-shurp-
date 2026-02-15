#pragma once
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>

// Шаблонный класс Container для работы с различными типами данных
template <class T>
class Container {
private:
    std::list<T> elements;

public:
    // Добавить элемент
    void add(const T& element) {
        elements.push_back(element);
    }

    // Печать всех элементов
    void print() const {
        for (const auto& element : elements) {
            std::cout << element << std::endl;
        }
    }

    // Удалить элемент
    bool remove(const T& element) {
        auto it = std::find(elements.begin(), elements.end(), element);
        if (it != elements.end()) {
            elements.erase(it);
            return true;
        }
        return false;
    }

    void remove_all() {
        elements.clear();
    }

    // Сохранение всех данных в файл
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
            return;
        }
        for (const auto& element : elements) {
            file << element << std::endl;
        }
    }

     //Считывание всех данных из файла
    //void loadFromFile(const std::string& filename) {
    //    std::ifstream file(filename);
    //    if (!file) {
    //        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
    //        return;
    //    }
    //    file.seekg(0, std::ios::beg);
    //    file.clear();
    //    //читать из стринг стрима
    //    T element;
    //    while (file >> element /*!file.eof() && file.good() */) {
    //        //file >> element;
    //        add(element);
    //    }
    //}

    void loadFromFile(std::istream& in) {
        std::copy(std::istream_iterator<T>(in), std::istream_iterator<T>(), std::inserter(elements, elements.end()));
    }

   /* //void loadFromFile(const std::string& filename) {
    //    std::ifstream file(filename);
    //    if (!file) {
    //        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
    //        return;
    //    }

    //    file.seekg(0, std::ios::beg);
    //    file.clear();

    //    std::stringstream buffer;
    //    buffer << file;

    //    T element;
    //    //while (!buffer.end) {
    //    //    buffer >> element;
    //    //    add(element);
    //    //}
    //    
    //    //проверить что стринг стрим не пустая строка 

    //    while (!buffer.eof()) {
    //        buffer >> element;
    //        add(element);
    //        buffer << file;
    //    }

    //    //while (buffer >> element) {
    //    //    add(element);
    //    //}
    //}*/

    // Сортировка элементов в прямом порядке
    void sortAscending() {
        elements.sort();
    }

    // Сортировка элементов в обратном порядке
    void sortDescending() {
        elements.sort(std::greater<T>());
    }

    // Ввод элементов контейнера с использованием потоковых итераторов на экран и в файл
    friend std::istream& operator>>(std::istream& in, Container<T>& container) {
        T element;
        while (in >> element) {
            container.add(element);
        }
        return in;
    }

    // Вывод элементов контейнера с использованием потоковых итераторов на экран и в файл
    friend std::ostream& operator<<(std::ostream& out, const Container<T>& container) {
        for (const auto& element : container.elements) {
            out << element << std::endl;
        }
        return out;
    }

    // Универсальные методы с лямбда-функциями

    // Удалить элемент по условию
    template<typename Predicate>
    bool remove_if(Predicate predicate) {
        auto it = std::find_if(elements.begin(), elements.end(), predicate);
        if (it != elements.end()) {
            elements.erase(it);
            return true;
        }
        return false;
    }

    // Обновить элемент по условию
    template<typename Predicate, typename Updater>
    bool update_if(Predicate predicate, Updater updater) {
        auto it = std::find_if(elements.begin(), elements.end(), predicate);
        if (it != elements.end()) {
            updater(*it);
            return true;
        }
        return false;
    }

    // Получить подмножество элементов по условию
    template<typename Predicate>
    Container<T> get_if(Predicate predicate) const {
        Container<T> result;
        for (const auto& element : elements) {
            if (predicate(element)) {
                result.add(element);
            }
        }
        return result;
    }

    // Сортировка с использованием лямбда-функций
    template<typename Comparator>
    void sort_by(Comparator comparator) {
        elements.sort(comparator);
    }
};

// Пример использования специализированного метода print
template<>
void Container<std::string>::print() const {
    for (const auto& element : elements) {
        std::cout << element << std::endl;
    }
}
