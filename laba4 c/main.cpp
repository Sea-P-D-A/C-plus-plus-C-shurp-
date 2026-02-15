/*«Сотрудник» представляет собой структуру с полями : имя; фамилия, отчество, дата
трудоустройства(использовать одну из предлагаемых языком структур); дата
увольнения(может не быть); должность — перечисление, пол — перечисление,
заработная плата.Выборка сотрудников производится путем указания
пользователем подходящего интервала по одному из критериев : фамилия, зарплата.
Выборка всех сотрудников - мужчин и выборка всех сотрудников - женщин.Выборка
всех сотрудников, которые числились в штате в указанный пользователем день.
Шаблонный класс должен иметь дополнительный метод сортировки.Добавить
в меню возможность отсортировать сотрудников по имени и по фамилии в прямом
и обратном порядке.*/
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <ctime>
#include "container.h"
#include "Person.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    Container<Person> people;

    while (true) {
        int option;
        std::string inputName;
        std::tm date = {};

        std::cout << "\n1. Добавить\n2. Удалить\n3. Изменить\n4. Сохранить в файл\n"
            << "5. Вывести\n6. Считать из файла\n7. Выборка\n"
            << "8. Сортировка\n9. Выход\nВыберите опцию: ";
        std::cin >> option;

        if (option == 9) break;

        switch (option) {
        case 1: {
            std::string name, surname, patronymic;
            std::cout << "Введите ФИО через пробел: ";
            std::cin.ignore();
            std::getline(std::cin, name, ' ');
            std::getline(std::cin, surname, ' ');
            std::getline(std::cin, patronymic);

            int day, month, year, salary;
            std::cout << "Введите день, месяц, год (через пробел): ";
            std::cin >> day >> month >> year;
            std::cout << "Введите ЗП: ";
            std::cin >> salary;

            Gender gender;
            std::cout << "1. Мужской / 2. Женский: ";
            std::cin >> option;
            gender = (option == 1) ? Gender::Male : Gender::Female;

            JobTitle jobTitle;
            std::cout << "Выберите должность (A, B, C, D, I, F, G): ";
            std::string jobTitleStr;
            std::cin >> jobTitleStr;
            jobTitle = static_cast<JobTitle>(jobTitleStr[0] - 'A');

            std::cout << "Добавить дату увольнения? (1 - Да, 0 - Нет): ";
            std::cin >> option;
            if (option == 1) {
                int year1, month1, day1;
                std::cout << "Введите день, месяц, год увольнения: ";
                std::cin >> day1 >> month1 >> year1;
                people.add(Person(name, surname, patronymic, year, month, day, gender, jobTitle, salary, year1, month1, day1));
            }
            else {
                people.add(Person(surname, name, patronymic, year, month, day, gender, jobTitle, salary));
            }
            break;
        }
        case 2: {
            std::cout << "Введите ФИО для удаления: ";
            std::cin.ignore();
            std::getline(std::cin, inputName);
            if (!people.remove_if([inputName](const Person& p) { return p.get_fio() == inputName; })) {
                std::cout << "Не найдено для удаления.\n";
            }
            else {
                std::cout << "Удалено успешно.\n";
            }
            break;
        }
        case 3: {
            std::cout << "ФИО изменяемого объекта: ";
            std::cin.ignore();
            std::getline(std::cin, inputName);
            std::cout << "\nИзменяемый параметр:\n1. ЗП\n2. Пол\n3. Должность\n4. Дата увольнения\n5. ФИО\n";
            std::cin >> option;
            switch (option) {
            case 1: {
                int newSalary;
                std::cout << "Введите новую зарплату: ";
                std::cin >> newSalary;
                people.update_if([inputName](Person& p) { return p.get_fio() == inputName; },
                    [newSalary](Person& p) { p.set_salary(newSalary); });
                break;
            }
            case 2: {
                Gender newGender;
                std::cout << "1. Мужской / 2. Женский: ";
                std::cin >> option;
                newGender = (option == 1) ? Gender::Male : Gender::Female;
                people.update_if([inputName](Person& p) { return p.get_fio() == inputName; },
                    [newGender](Person& p) { p.set_Gender(newGender); });
                break;
            }
            case 3: {
                JobTitle newJobTitle;
                std::cout << "Выберите новую должность (A, B, C, D, I, F, G): ";
                std::string jobTitleStr;
                std::cin >> jobTitleStr;
                newJobTitle = static_cast<JobTitle>(jobTitleStr[0] - 'A');
                people.update_if([inputName](Person& p) { return p.get_fio() == inputName; },
                    [newJobTitle](Person& p) { p.set_JobTitle(newJobTitle); });
                break;
            }
            case 4: {
                int newYear, newMonth, newDay;
                std::cout << "Введите новую дату увольнения (год, месяц, день): ";
                std::cin >> newYear >> newMonth >> newDay;
                people.update_if([inputName](Person& p) { return p.get_fio() == inputName; },
                    [newYear, newMonth, newDay](Person& p) { p.add_rem(newYear, newMonth, newDay); });
                break;
            }
            case 5: {
                std::string newName, newSurname, newPatronymic;
                std::cout << "Введите новое ФИО через пробел: ";
                std::cin.ignore();
                std::getline(std::cin, newName, ' ');
                std::getline(std::cin, newSurname, ' ');
                std::getline(std::cin, newPatronymic);
                people.update_if([inputName](Person& p) { return p.get_fio() == inputName; },
                    [newName, newSurname, newPatronymic](Person& p) { p.set_str(newName, newSurname, newPatronymic); });
                break;
            }
            }
            break;
        }
        case 4: {
            people.saveToFile("save.txt");
            break;
        }
        case 5: {
            std::cout << "\n";
            people.print();
            break;
        }
        case 6: {
            people.remove_all();
            std::ifstream file123("save.txt");
            people.loadFromFile(file123);
            break;
        }
        case 7: {
            Container<Person> filtered;
            std::cout << "1. Фильтр по ЗП\n2. Фильтр по фамилии\n3. Фильтр по полу\n4. Фильтр по дате\n";
            std::cin >> option;
            switch (option) {
            case 1: {
                int minSalary, maxSalary;
                std::cout << "Введите минимальную и максимальную ЗП: ";
                std::cin >> minSalary >> maxSalary;
                filtered = people.get_if([minSalary, maxSalary](const Person& p) {
                    return p.get_salary() >= minSalary && p.get_salary() <= maxSalary;
                    });
                break;
            }
            case 2: {
                std::string surnameStart, surnameEnd;
                std::cout << "Введите начальную и конечную фамилию: ";
                std::cin >> surnameStart >> surnameEnd;
                filtered = people.get_if([surnameStart, surnameEnd](const Person& p) {
                    return p.get_surname() >= surnameStart && p.get_surname() <= surnameEnd;
                    });
                break;
            }
            case 3: {
                int genderOption;
                std::cout << "Выберите пол (1 - Мужской, 2 - Женский): ";
                std::cin >> genderOption;
                Gender gender = (genderOption == 1) ? Gender::Male : Gender::Female;
                filtered = people.get_if([gender](const Person& p) {
                    return p.get_gen() == gender;
                    });
                break;
            }
            case 4: {
                std::cout << "Введите дату (год, месяц, день): ";
                int year, month, day;
                std::cin >> year >> month >> day;
                std::tm date = { 0, 0, 0, day, month - 1, year - 1900 };
                filtered = people.get_if([date](const Person& p) {
                    return p.is_employed_on(date);
                    });
                break;
            }
            }
            filtered.print();
            break;
        }
        case 8: {
            std::cout << "Выберите параметр сортировки:\n1. По имени в прямом порядке\n2. По имени в обратном порядке\n3. По фамилии в прямом порядке\n4. По фамилии в обратном порядке\n";
            std::cin >> option;
            switch (option) {
            case 1:
                people.sort_by([](const Person& p1, const Person& p2) {
                    return p1.get_name() < p2.get_name();
                    });
                break;
            case 2:
                people.sort_by([](const Person& p1, const Person& p2) {
                    return p1.get_name() > p2.get_name();
                    });
                break;
            case 3:
                people.sort_by([](const Person& p1, const Person& p2) {
                    return p1.get_surname() < p2.get_surname();
                    });
                break;
            case 4:
                people.sort_by([](const Person& p1, const Person& p2) {
                    return p1.get_surname() > p2.get_surname();
                    });
                break;
            }
            break;
        }
        default:
            std::cout << "Неизвестная опция.\n";
            break;
        }
    }

    return 0;
}
