/*Двусторонняя очередь.Увеличить все чётные числа на количество нечётных */

#include <iostream>
#include <fstream>
#include <deque>
#include <random>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <iterator>


class Container {
private:
    std::deque<int> list;
    int diapazon; // Член класса для хранения диапазона

    // Функция генерации случайного числа, использующая член класса diapazon
    int randomInRange(int diapazon) {
        //srand(time(NULL));
        return rand() % (2 * diapazon + 1) - diapazon;
    }

public:
    // Конструктор для инициализации диапазона
    Container(int diapazon) : diapazon(diapazon) {}

    // Метод для создания файла с случайными числами
    std::fstream* random_file(const std::string& file_name, int size, int diapazon) {
        // Создание динамического объекта файла
        std::fstream* file = new std::fstream(file_name + ".txt", std::ios::out | std::ios::trunc);

        if (file->is_open()) {
            // Запись чисел из вектора в файл
            for (int i = 0; i < size; i++) {
                int number = randomInRange(diapazon); // Генерация случайного числа
                *file << number << std::endl; // Запись числа в файл
            }
            file->close(); // Закрытие файла после записи
            file->open(file_name + ".txt", std::ios::in); // Переоткрытие файла для чтения
        }
        else {
            std::cerr << "Не удалось открыть файл " << file_name + ".txt" << std::endl;
            delete file; // Освобождение ресурсов в случае неудачи
            return nullptr;
        }

        // Возврат указателя на файл
        return file;
    }

    // Метод для создания файла с случайными числами, используя std::generate_n
    std::fstream* random_file_generate(const std::string& file_name, int size, int diapazon) {
        // Создание динамического объекта файла
        std::fstream* file = new std::fstream(file_name + ".txt", std::ios::out | std::ios::trunc);
        //std::istream_iterator<int> fileIT(*file);
        if (file->is_open()) {
            // Вектор для хранения случайных чисел
            std::vector<int> numbers(size);

            // Генерация случайных чисел и заполнение вектора
            std::generate_n(numbers.begin(), size, [diapazon, file]()/*->auto */ { *file << rand() % (2 * diapazon + 1) - diapazon << std::endl; return 0; });
            //std::generate_n(fileIT,size, [diapazon]()->int { return rand() % (2 * diapazon + 1) - diapazon; });
            // Запись чисел из вектора в файл
            //for (int num : numbers) {
            //    *file << num << std::endl; // Записываем каждое число в новой строке
          //  }
            file->close(); // Закрытие файла после записи
            file->open(file_name + ".txt", std::ios::in); // Переоткрытие файла для чтения
        }
        else {
            std::cerr << "Не удалось открыть файл " << file_name + ".txt" << std::endl;
            delete file; // Освобождение ресурсов в случае неудачи
            return nullptr;
        }

        // Возврат указателя на файл
        return file;
    }

    // Метод для чтения с файла
    std::deque<int> file_add(std::fstream* file) {
        list.clear();
        if (file->is_open()) {
            file->seekg(0); // Перемещаем указатель в начало файла для чтения
            int a;
            while (*file >> a) { // Чтение из файла и проверка успешности операции
                list.push_back(a); // Добавление прочитанного значения в контейнер
            }
            file->close(); // Закрываем файл после использования
        }
        else {
            std::cerr << "Файл не открыт для чтения." << std::endl;
        }
        return list;
    }
    void file_add(std::string file_name) {
        list.clear();
        std::fstream file;
        file.open(file_name + ".txt", std::ios::in);

        if (file.is_open()) {
            int a;
            while (file >> a) { // Чтение из файла и проверка успешности операции
                list.push_back(a); // Добавление прочитанного значения в контейнер
            }
            file.close(); // Закрываем файл после использования
        }
        else {
            std::cerr << "Файл не открыт для чтения." << std::endl;
        }
    }
    
    // метод для вывода в консоль
    void print_console() {
        for (const int &i : list) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

    // метод для вывода в файл
    void print_file(std::string file_name) {
        int i = 0;
        std::fstream file;
        file.open(file_name + ".txt", std::ios::out);
        while (i < list.size()) {
            file << list[i++] << std::endl;
        }
    }

    void list_front(int num) {
        list.push_front(num);
    }
    void list_back(int num) {
        list.push_back(num);
    }
    void list_pop_front() {
        list.pop_front();
    }
    void list_pop_back() {
        list.pop_back();
    }

    // Функция вычисления суммы чисел в контейнере
    int sum() {
        return std::accumulate(list.begin(), list.end(), 0);
    }

    // Функция вычисления среднего арифметического чисел в контейнере
    double average() {
        if (list.empty()) return 0.0; // Проверка на пустой контейнер
        return static_cast<double>(sum()) / list.size();
    }


    void modify(int n) {
        if (n == 1) { modify1(list.begin(), list.end()); }
        if (n == 2) { modify2(list.begin(), list.end()); }
        if (n == 3) { modify3(list.begin(), list.end()); }
    }
    void modify1(std::deque<int>::iterator begin, std::deque<int>::iterator end) {
        // Подсчёт количества нечётных чисел в заданной части контейнера
        int oddCount = 0;
        for (std::deque<int>::iterator it = begin; it != end; ++it) {
            if (*it % 2 != 0) {
                ++oddCount;
            }
        }

        // Увеличение чётных чисел на количество нечётных
        for (std::deque<int>::iterator it = begin; it != end; ++it) {
            if (*it % 2 == 0) {
                *it += oddCount;
            }
        }
    }
    void modify2(std::deque<int>::iterator begin, std::deque<int>::iterator end) {
        // Подсчёт количества нечётных чисел
        int oddCount = 0;
        std::for_each(begin, end, [&oddCount](int n) {
            if (n % 2 != 0) {
                ++oddCount;
            }
            });

        // Увеличение чётных чисел на количество нечётных
        std::for_each(begin, end, [oddCount](int& n) {
            if (n % 2 == 0) {
                n += oddCount;
            }
            });
    }
    void modify3(std::deque<int>::iterator begin, std::deque<int>::iterator end) {
        // Подсчёт количества нечётных чисел
        int oddCount = std::count_if(begin, end, [](int n) { return n % 2 != 0; });

        // Увеличение чётных чисел на количество нечётных
        std::transform(begin, end, begin, [oddCount](int n) {
            if (n % 2 == 0) {
                return n + oddCount; // Увеличиваем чётные числа
            }
            return n; // Нечётные числа оставляем без изменений
            });
    }
};

int main()
{

    setlocale(LC_ALL, "Rus");
    std::cout << "Введите диапазон: ";
    int dip = 5;
    std::cin >> dip;
    Container first(dip);
    std::deque<int> a;
    std::fstream* file = new std::fstream('j' + ".txt", std::ios::out | std::ios::in);
    std::string file_name = "";
    int size = 0;
    int exit = -1;

    while (exit != 9) {
        std::cout << "1.заполнить очередь с файла\n2.создать файл и считать с него очередь\n3.добавить значение в очередь вручную\n4.удалить элемент из очереди\n5.напечатать очередь\n6.сохранить очередь в файл\n7.выполнить задание\n8.сумма и среднее арифметическое\n";
        std::cout << "9.exit\n";
        std::cin >> exit;
        if (exit == 1) {
            system("cls");
            std::cout << "Введите название файла: ";
            std::cin >> file_name;
            first.file_add(file_name);
            file_name = "";
            exit = -1;
        }
        if (exit == 2) {
            std::cout << "random/generate: ";
            std::cin >> exit;
            std::cout << "\nВведите название файла:";
            std::cin >> file_name;
            std::cout << "\nВведите количество цирф:";
            std::cin >> size;
            if (exit == 1) { first.file_add(first.random_file(file_name, size, dip)); }
            if (exit == 2) { first.file_add(first.random_file_generate(file_name, size, dip)); }
            file_name = "";
            exit = -1;
        }
        if (exit == 3) {
            std::cout << "front/back: ";
            std::cin >> exit;

            if (exit == 1) {
                std::cout << "Введите добовляемое число:";
                std::cin >> exit;
                first.list_front(exit);
                exit = -1;
            }
            if (exit == 2) {
                std::cout << "Введите добовляемое число:";
                std::cin >> exit;
                first.list_back(exit);
            }
            exit = -1;
        }
        if (exit == 4) {
            std::cout << "front/back: ";
            std::cin >> exit;

            if (exit == 1) { first.list_pop_front(); }
            if (exit == 2) { first.list_pop_back(); }
            exit = -1;
        }
        if (exit == 5) { first.print_console(); std::cin >> exit; exit = -1; }
        if (exit == 6) {
            std::cout << "Название файла: ";
            std::cin >> file_name;
            first.print_file(file_name);
        }
        if (exit == 7) {
            std::cout << "for/transform/for_each: ";
            std::cin >> exit;
            first.print_console();
            first.modify(exit);
            first.print_console();
            std::cin >> exit;
            exit = -1;
        }
        if (exit == 8) {
            std::cout << "Сумма: " << first.sum();
            std::cout << "\nСреднее арифметическое: " << first.average();
            std::cin >> exit;
            exit = -1;
        }
        system("cls");
    }
}
