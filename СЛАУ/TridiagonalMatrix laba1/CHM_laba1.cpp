#include <iostream>
#include "TridiagonalMatrix.h"

//рандомайзер + степени двойки в тесте пункт 4 


void experiment_results(const TridiagonalMatrix& matrix, const Vector& exact_solution,
    double& abs_error_sweep, double& rel_error_sweep,
    double& abs_error_unsustainable, double& rel_error_unsustainable)
{
    Vector rhs = matrix * exact_solution;
    Vector sweep_matr = matrix.sweep_method(rhs, 2);
    Vector unsustainable_matr = matrix.unsustainable_method(rhs);

    Vector error_sweep = sweep_matr - exact_solution;
    Vector error_unsustainable = unsustainable_matr - exact_solution;

    abs_error_sweep = error_sweep.norm();
    abs_error_unsustainable = error_unsustainable.norm();

    rel_error_sweep = 0.0;
    rel_error_unsustainable = 0.0;

    double epsilon = std::sqrt(std::numeric_limits<double>::epsilon());
    for (size_t i = 1; i <= exact_solution.size(); ++i)
    {
        double exact_val = abs(exact_solution(i));
        double rel_error_s = abs(error_sweep(i));
        double rel_error_u = abs(error_unsustainable(i));

        if (exact_val > epsilon)
        {
            rel_error_s /= exact_val;
            rel_error_u /= exact_val;
        }

        rel_error_sweep = max(rel_error_sweep, rel_error_s);
        rel_error_unsustainable = max(rel_error_unsustainable, rel_error_u);
    }
}

void size_dependecy_experiment()
{
    std::vector<size_t> sizes = { 16,32,64,128,256,512,1024, 8192 };
    std::cout << "Размер\t|Абсолют.Прог.\t|Относит.Прог.\t|Абсолют.Неуст.\t|Относит.Неуст." << '\n';
    std::cout << "---------------------------------------------------------------------------" << '\n';

    for (size_t n : sizes)
    {
        Vector a(n - 1), b(n), c(n - 1);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist_a_c(-1.0, 1.0);
        std::uniform_real_distribution<double> dist_b(2.0, 3.0);

        for (size_t i = 0; i < n - 1; ++i)
        {
            a[i] = dist_a_c(gen);
            c[i] = dist_a_c(gen);
        }

        for (size_t i = 0; i < n; ++i)
            b[i] = dist_b(gen);

        TridiagonalMatrix matrix1;
        matrix1.fill_random_conditional(WELL_CONDITIONED, n);


        TridiagonalMatrix matrix(a, b, c);
        Vector exact_solution(n);
        std::uniform_real_distribution<double> dist_sol(-10.0, 10.0);

        for (size_t i = 0; i < n; ++i)
            exact_solution(i + 1) = dist_sol(gen);

        double abs_sweep, rel_sweep, abs_unsustainable, rel_unsustainable;
        experiment_results(matrix1, exact_solution, abs_sweep, rel_sweep, abs_unsustainable, rel_unsustainable);

        std::cout << n << "\t" << abs_sweep << "\t" << rel_sweep << "\t"
            << abs_unsustainable << "\t" << rel_unsustainable << '\n';
    }
}

int main()
{
    SetConsoleOutputCP(1251);

    std::cout << "Решение трехдиагональных матриц\n";
    std::cout << "1. Ручной ввод\n";
    std::cout << "2. Ввод из файла\n";
    std::cout << "3. Ввод из random\n";
    std::cout << "4. Тест на зависимость от размера\n";
    std::cout << "Выбрать пункт меню: ";

    int choice{};
    std::cin >> choice;

    if (choice == 1 || choice == 2 || choice == 3)
    {
        TridiagonalMatrix matrix;
        Vector resulths;
        Vector solution;

        if (choice == 1)
        {
            std::cout << "Введите матрицу (индексы начинаются с 1):\n";
            matrix.read_from_console();
            resulths.read_from_console();
        }
        if (choice == 3) {
            int a;
            std::cout << "1.Хорошо обусловленная\n";
            std::cout << "2.Плохо обусловленная\n";
            std::cout << "Вариант: ";
            std::cin >> a;
            if (a == 1) {
                std::cout << "Ведите размер матрицы: ";
                std::cin >> a;
                matrix.fill_random_conditional(WELL_CONDITIONED,a);
                resulths.fill_random_values(1,10);
            }
            else {
                std::cout << "Ведите размер матрицы: ";
                std::cin >> a;
                matrix.fill_random_conditional(ILL_CONDITIONED, a);
                resulths.fill_random_values(1, 10);
            }

        }
        else
        {
            std::string matrix_filename, rhs_filename;
            std::cout << "Введите имя файла с матрицей: ";
            std::cin >> matrix_filename;
            matrix.read_from_file(matrix_filename);

            std::cout << "Введите имя файла с правой частью: ";
            std::cin >> rhs_filename;
            resulths.read_from_file(rhs_filename);

            if (resulths.size() != matrix.size())
            {
                std::cout << "Ошибка: размер правой части не совпадает с размером матрицы!\n";
                return 1;
            }
        }

        // Показываем введенные данные
        std::cout << "\nВведенная матрица:\n";
        matrix.print_console();

        std::cout << "\nПравая часть: ";
        resulths.print_console();

        // Выбор метода решения
        std::cout << "\nВыберите метод решения:\n";
        std::cout << "1. Метод прогонки\n";
        std::cout << "2. Неустойчивый метод\n";
        std::cout << "3. Оба метода\n";
        std::cout << "Выбор: ";

        int method_choice{};
        std::cin >> method_choice;

        if (method_choice == 1 || method_choice == 3)
        {
            Vector sweep_solution = matrix.sweep_method(resulths, 1);
            std::cout << "\nРешение методом прогонки: ";
            sweep_solution.print_console();
        }

        if (method_choice == 2 || method_choice == 3)
        {
            Vector unsustainable_solution = matrix.unsustainable_method(resulths);
            std::cout << "\nРешение неустойчивым методом: ";
            unsustainable_solution.print_console();
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
                outfile << "Размер: " << matrix.size() << "\n";

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
                for (size_t i = 1; i <= resulths.size(); ++i)
                    outfile << resulths(i) << " ";

                if (method_choice == 1 || method_choice == 3)
                {
                    Vector sweep_solution = matrix.sweep_method(resulths, 1);
                    outfile << "\n\nРешение методом прогонки: ";
                    for (size_t i = 1; i <= sweep_solution.size(); ++i)
                        outfile << sweep_solution(i) << " ";
                }

                if (method_choice == 2 || method_choice == 3)
                {
                    Vector unsustainable_solution = matrix.unsustainable_method(resulths);
                    outfile << "\n\nРешение неустойчивым методом: ";
                    for (size_t i = 1; i <= unsustainable_solution.size(); ++i)
                        outfile << unsustainable_solution(i) << " ";
                }

                outfile.close();
                std::cout << "Результаты сохранены в файл: " << output_filename << "\n";
            }
        }
    }
    else
        if (choice == 4)
            size_dependecy_experiment();
        else
            std::cout << "Неверный выбор!\n";

    std::cin.ignore();
    std::cin.get();
    return 0;
}