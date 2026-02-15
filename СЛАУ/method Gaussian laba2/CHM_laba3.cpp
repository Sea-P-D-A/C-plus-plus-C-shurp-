#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include "DenseMatrix.h"
#include <windows.h>  

// === ЭКСПЕРИМЕНТАЛЬНЫЕ ФУНКЦИИ ===

// 1. ОДИНОЧНЫЙ ЭКСПЕРИМЕНТ (п.2.3.1 условия)
// Вход: матрица A и точное решение x_exact
// Выход: абсолютная и относительная погрешности
void single_experiment(const DenseMatrix& matrix, const Vector& exact_solution,
    double& abs_error, double& rel_error) {
    Vector rhs = matrix * exact_solution;  // Вычисляем правую часть: b = A * x_exact
    Vector calculated_solution = matrix.solve_gauss(rhs);  // Решаем систему Ax = b

    Vector error_vector = calculated_solution - exact_solution;  // Вектор ошибок
    abs_error = error_vector.norm();  // Абсолютная погрешность = max|error|
    rel_error = calculated_solution.relative_error(exact_solution);  // Относительная погрешность
}

// 2. ЭКСПЕРИМЕНТ ПО ЗАВИСИМОСТИ ОТ РАЗМЕРА (п.2.3.2 условия)
// Исследует как растёт погрешность с увеличением n
void size_dependency_experiment() {
    std::vector<size_t> sizes = { 10, 20, 30, 50, 75, 100 };  // Логарифмическая шкала
    std::cout << "Размер\t|Абсолют.Погр.\t|Относит.Погр.\n";
    std::cout << "-----------------------------------------\n";

    for (size_t n : sizes) {
        DenseMatrix matrix(n);
        matrix.fill_random(-10.0, 10.0);  // Случайная матрица размера n

        Vector exact_solution(n);
        exact_solution.fill_random(-10.0, 10.0);  // Случайное точное решение

        double abs_error, rel_error;
        single_experiment(matrix, exact_solution, abs_error, rel_error);

        // Вывод в формате таблицы
        std::cout << n << "\t" << std::scientific << abs_error << "\t"
            << rel_error << "\n";
    }
}

// 3. ЭКСПЕРИМЕНТ ПО ЗАВИСИМОСТИ ОТ ОБУСЛОВЛЕННОСТИ (п.3 условия)
// Сравнивает точность для матриц разной обусловленности
void condition_experiment() {
    size_t n = 20;  // Фиксированный размер
    std::cout << "\nЭксперимент по обусловленности (n=" << n << "):\n";
    std::cout << "Тип матрицы\t|Абсолют.Погр.\t|Относит.Погр.\n";
    std::cout << "-------------------------------------------------\n";

    MatrixType types[] = { RANDOM_MATRIX, HILBERT_MATRIX, DIAG_DOMINANT_MATRIX };
    const char* type_names[] = { "Случайная", "Гильберта", "Диаг.Преобл." };

    for (int i = 0; i < 3; ++i) {
        try {
            DenseMatrix matrix;
            matrix.fill_by_type(types[i], n);  // Создаём матрицу заданного типа

            Vector exact_solution(n);
            exact_solution.fill_random(-10.0, 10.0);

            double abs_error = 0.0, rel_error = 0.0;

            try {
                single_experiment(matrix, exact_solution, abs_error, rel_error);
                std::cout << type_names[i] << "\t\t" << std::scientific << abs_error
                    << "\t" << rel_error << "\n";
            }
            catch (const std::exception& e) {
                // Обработка ошибок решения 
                std::cout << type_names[i] << "\t\t" << "ОШИБКА: " << e.what() << "\n";
                continue;
            }
        }
        catch (const std::exception& e) {
            // Обработка ошибок создания матрицы
            std::cout << type_names[i] << "\t\t" << "ОШИБКА создания: " << e.what() << "\n";
        }
    }
}


// 5. Эксперимент: Сравнение разных типов матриц при изменении размера
void compare_matrices_by_size_experiment() {
    std::vector<size_t> sizes = { 8, 16, 32, 64, 128, 256, 512, 1024 };  // Разные размеры
    MatrixType types[] = {
        RANDOM_MATRIX,          // Обычная
        DIAG_DOMINANT_MATRIX,   // Хорошо обусловленная
        HILBERT_MATRIX  // Плохо обусловленная
    };

    const char* type_names[] = { "Случайная", "Диаг.преобл.", "Плохо обусп." };

    std::cout << "\n=== ЗАВИСИМОСТЬ ТОЧНОСТИ ОТ РАЗМЕРА И ТИПА МАТРИЦЫ ===\n\n";

    // Таблица для каждого типа матрицы
    for (int t = 0; t < 3; ++t) {
        std::cout << "Тип матрицы: " << type_names[t] << "\n";
        std::cout << "Размер\t|Абс.погр.\t|Отн.погр.\t|Невязка\t|Успех\n";
        std::cout << "----------------------------------------------------------------\n";

        for (size_t n : sizes) {
            try {
                // Создаём матрицу заданного типа и размера
                DenseMatrix matrix;
                matrix.fill_by_type(types[t], n);

                // Генерируем точное решение
                Vector exact_solution(n);
                exact_solution.fill_random(-5.0, 5.0);

                // Вычисляем правую часть: b = A * x_exact
                Vector rhs = matrix * exact_solution;

                // Решаем систему
                Vector calculated_solution = matrix.solve_gauss(rhs);

                // Вычисляем погрешности
                Vector error = calculated_solution - exact_solution;
                double abs_error = error.norm();
                double rel_error = calculated_solution.relative_error(exact_solution);

                // Вычисляем невязку: r = A*x_calc - b
                Vector residual = matrix * calculated_solution - rhs;
                double residual_norm = residual.norm();

                // Выводим результаты
                std::cout << n << "\t" << std::scientific
                    << abs_error << "\t"
                    << rel_error << "\t"
                    << residual_norm << "\t"
                    << "OK\n";
            }
            catch (const std::exception& e) {
                // Если ошибка (например, матрица вырождена)
                std::cout << n << "\t---\t\t---\t\t---\t\tОШИБКА: "
                    << e.what() << "\n";
            }
        }
        std::cout << "\n";
    }

    // Сводная таблица (максимальные погрешности для каждого типа)
    std::cout << "\n=== СВОДНАЯ ТАБЛИЦА (макс. относительная погрешность) ===\n";
    std::cout << "Размер\t|Случайная\t|Диаг.преобл.\t|Плохо обусп.\n";
    std::cout << "--------------------------------------------------------\n";

    for (size_t n : sizes) {
        std::cout << n << "\t";

        // Для каждого типа матрицы
        for (int t = 0; t < 3; ++t) {
            try {
                DenseMatrix matrix;
                matrix.fill_by_type(types[t], n);

                Vector exact_solution(n);
                exact_solution.fill_random(-5.0, 5.0);

                Vector rhs = matrix * exact_solution;
                Vector solution = matrix.solve_gauss(rhs);

                double rel_error = solution.relative_error(exact_solution);

                if (t == 2) {  // Последний столбец
                    std::cout << std::scientific << rel_error << "\n";
                }
                else {
                    std::cout << std::scientific << rel_error << "\t";
                }
            }
            catch (...) {
                if (t == 2) {
                    std::cout << "---\n";
                }
                else {
                    std::cout << "---\t";
                }
            }
        }
    }
}






int main() {
    SetConsoleOutputCP(1251);  // Русская кодировка в консоли Windows

    std::cout << "Лабораторная №3: Метод Гаусса (схема единственного деления)\n";
    std::cout << "============================================================\n";

    // ГЛАВНОЕ МЕНЮ (аналогично вашей первой лабе)
    while (true) {
        std::cout << "\nМЕНЮ:\n";
        std::cout << "1. Решить систему (ручной ввод)\n";
        std::cout << "2. Решить систему (из файла)\n";
        std::cout << "3. Сгенерировать и решить систему\n";
        std::cout << "4. Эксперимент: зависимость от размера\n";
        std::cout << "5. Эксперимент: зависимость от обусловленности\n";
        std::cout << "6. Выход\n";
        std::cout << "Выберите пункт: ";

        int choice;
        std::cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            DenseMatrix matrix;
            Vector rhs;

            // ПУНКТ 1: Ручной ввод
            if (choice == 1) {
                std::cout << "Введите матрицу:\n";
                matrix.read_from_console();
                std::cout << "Введите правую часть:\n";
                rhs.read_from_console();
            }
            // ПУНКТ 2: Ввод из файла
            else if (choice == 2) {
                std::string matrix_file, rhs_file;
                std::cout << "Введите имя файла с матрицей: ";
                std::cin >> matrix_file;
                matrix.read_from_file(matrix_file);

                std::cout << "Введите имя файла с правой частью: ";
                std::cin >> rhs_file;
                rhs.read_from_file(rhs_file);
            }
            // ПУНКТ 3: Генерация системы
            else if (choice == 3) {
                size_t n;
                std::cout << "Введите размер системы: ";
                std::cin >> n;

                int type_choice;
                std::cout << "Выберите тип матрицы:\n";
                std::cout << "1. Случайная\n";
                std::cout << "2. Матрица Гильберта (плохо обусловленная)\n";
                std::cout << "3. С диагональным преобладанием (хорошо обусловленная)\n";
                std::cout << "Выбор: ";
                std::cin >> type_choice;

                MatrixType type;
                switch (type_choice) {
                case 1: type = RANDOM_MATRIX; break;
                case 2: type = HILBERT_MATRIX; break;
                case 3: type = DIAG_DOMINANT_MATRIX; break;
                default:
                    std::cout << "Неверный выбор!\n";
                    continue;
                }

                matrix.fill_by_type(type, n);  // Генерируем матрицу
                rhs.resize(n);
                rhs.fill_random(-10.0, 10.0);  // Генерируем правую часть

                // Показываем сгенерированные данные
                std::cout << "\nСгенерированная матрица:\n";
                matrix.print_console();
                std::cout << "\nПравая часть: ";
                rhs.print_console();
            }

            // РЕШЕНИЕ СИСТЕМЫ
            try {
                Vector solution = matrix.solve_gauss(rhs);  // Метод Гаусса
                std::cout << "\nРешение: ";
                solution.print_console();

                // Проверка точности через невязку: r = Ax - b
                Vector residual = matrix * solution - rhs;
                std::cout << "Норма невязки: " << residual.norm() << "\n";

                // СОХРАНЕНИЕ РЕЗУЛЬТАТОВ
                std::cout << "\nСохранить результаты? (y/n): ";
                char save_choice;
                std::cin >> save_choice;

                if (save_choice == 'y' || save_choice == 'Y') {
                    std::string filename;
                    std::cout << "Введите имя файла: ";
                    std::cin >> filename;

                    std::ofstream file(filename);
                    file << "Матрица (" << matrix.getRows() << "x" << matrix.getCols() << "):\n";
                    matrix.print_to_file("temp_matrix.txt");  // Сохраняем матрицу во временный файл

                    // Копируем временный файл в итоговый
                    std::ifstream temp("temp_matrix.txt");
                    std::string line;
                    while (std::getline(temp, line)) {
                        file << line << "\n";
                    }
                    temp.close();

                    // Сохраняем правую часть и решение
                    file << "\nПравая часть: ";
                    for (size_t i = 0; i < rhs.size(); ++i) {
                        file << rhs[i] << " ";
                    }
                    file << "\n\nРешение: ";
                    for (size_t i = 0; i < solution.size(); ++i) {
                        file << solution[i] << " ";
                    }
                    file << "\n";
                    file.close();

                    std::cout << "Результаты сохранены в " << filename << "\n";
                }
            }
            catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }
        // ПУНКТ 4: Эксперимент по размеру
        else if (choice == 4) {
            size_dependency_experiment();
        }
        // ПУНКТ 5: Эксперимент по обусловленности
        else if (choice == 5) {
            //condition_experiment();
            compare_matrices_by_size_experiment();
        }
        // ПУНКТ 6: Выход
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Неверный выбор!\n";
        }
    }

    std::cout << "\nПрограмма завершена.\n";
    return 0;
}