#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include "container.h"

// Перечисления для пола и должности
enum class Gender { Male, Female };
enum class JobTitle { A, B, C, D, I, F, G };

// Преобразование должности в строку
std::string jobTitleToString(JobTitle jobTitle) {
    switch (jobTitle) {
    case JobTitle::A: return "A";
    case JobTitle::B: return "B";
    case JobTitle::C: return "C";
    case JobTitle::D: return "D";
    case JobTitle::I: return "I";
    case JobTitle::F: return "F";
    case JobTitle::G: return "G";
    default: return "Неизвестная должность";
    }
}

// Преобразование пола в строку
std::string genderToString(Gender gender) {
    return gender == Gender::Male ? "Мужчина" : "Женщина";
}

// Класс Person для хранения информации о сотруднике
class Person {
private:
    std::string name, surname, patronymic;
    std::tm add = {};
    std::tm rem = {};
    Gender gender;
    JobTitle jobTitle;
    int salary;
    bool hasRemovalDate;

public:
    // Конструктор
    Person(const std::string& name = "", const std::string& surname = "", const std::string& patronymic = "",
        int year = 0, int month = 0, int day = 0, Gender gender = Gender::Male, JobTitle jobTitle = JobTitle::A, int salary = 0,
        int year1 = 0, int month1 = 0, int day1 = 0)
        : name(name), surname(surname), patronymic(patronymic), gender(gender),
        jobTitle(jobTitle), salary(salary), hasRemovalDate(year1 != 0) {
        // Заполнение даты
        if (month > 12) month = 12;
        if (month1 > 12) month1 = 12;
        if (month < 0) month = 1;
        if (month1 < 0) month1 = 1;
        if (day < 0) day = 1;
        if (day1 < 0) day1 = 1;
        {
            if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
                if (day > 31) {
                    day = 1;
                    if (month != 12) month += 1;
                    else { year += 1; month = 1; day = 1; }
                }
            if (month == 4 || month == 6 || month == 9 || month == 11)
                if (day > 30) {
                    day = 1;
                    month += 1;
                }
            if (month == 2)
                if (day > 29) {
                    day = 1;
                    month += 1;
                }

            if (month1 == 1 || month1 == 3 || month1 == 5 || month1 == 7 || month1 == 8 || month1 == 10 || month1 == 12)
                if (day1 > 31) {
                    day1 = 1;
                    if (month1 != 12) month1 += 1;
                    else { year1 += 1; month1 = 1; day1 = 1; }
                }
            if (month1 == 4 || month1 == 6 || month1 == 9 || month1 == 11)
                if (day1 > 30) {
                    day1 = 1;
                    month1 += 1;
                }
            if (month1 == 2)
                if (day1 > 29) {
                    day1 = 1;
                    month1 += 1;
                }
        }

        add.tm_year = year;//- 1900; // корректировка года
        add.tm_mon = month; //- 1;    // корректировка месяца
        add.tm_mday = day;

        if (hasRemovalDate) {
            rem.tm_year = year1 - 1900; // корректировка года
            rem.tm_mon = month1 - 1;    // корректировка месяца
            rem.tm_mday = day1;
        }
    }

    // Метод для вывода информации о сотруднике
    void print() const {
        std::cout << "Имя: " << name << ", Фамилия: " << surname << ", Отчество: " << patronymic
            << ", Дата приёма на работу: " << add.tm_mday << ":" << (add.tm_mon + 1) << ":" << (add.tm_year + 1900)
            << ", Пол: " << genderToString(gender)
            << ", Должность: " << jobTitleToString(jobTitle)
            << ", Зарплата: " << salary << " руб.";

        if (hasRemovalDate) {
            std::cout << ", Дата увольнения: " << rem.tm_mday << ":" << (rem.tm_mon + 1) << ":" << (rem.tm_year + 1900);
        }
        std::cout << std::endl;
    }

    std::string get_fio() const {
        return surname + " " + name + " " + patronymic;
    }

    Gender get_gen() const { return gender; }
    int get_salary() const { return salary; }
    std::string get_surname() const { return surname; }
    std::string get_name() const { return name; }

    // Метод для сериализации данных сотрудника в строку
    std::string serialize() const {
        std::ostringstream oss;
        oss << "Имя: " << name << " , Фамилия: " << surname << " , Отчество: " << patronymic
            << " , Дата приёма на работу: " << add.tm_mday << " / " << (add.tm_mon) << " / " << (add.tm_year)
            << " , Пол: " << genderToString(gender)
            << " , Должность: " << jobTitleToString(jobTitle)
            << " , Зарплата: " << salary << " руб.";
        if (hasRemovalDate) {
            oss << ", Дата увольнения: " << rem.tm_mday << " / " << (rem.tm_mon) << " / " << (rem.tm_year);
        }
        return oss.str();
    }

    /*// Метод для сериализации данных сотрудника в строку
    std::string serialize() const {
        std::ostringstream oss;
        oss << "Имя: " << name << ", Фамилия: " << surname << ", Отчество: " << patronymic
            << ", Дата приёма на работу: " << add.tm_mday << "/" << (add.tm_mon + 1) << "/" << (add.tm_year + 1900)
            << ", Пол: " << genderToString(gender)
            << ", Должность: " << jobTitleToString(jobTitle)
            << ", Зарплата: " << salary << " руб.";
        if (hasRemovalDate) {
            oss << ", Дата увольнения: " << rem.tm_mday << "/" << (rem.tm_mon + 1) << "/" << (rem.tm_year + 1900);
        }
        return oss.str();
    }*/

    // Метод для десериализации данных сотрудника из строки
    static Person deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string nameLabel, surnameLabel, patronymicLabel, addLabel, remLabel, genderLabel, jobTitleLabel, salaryLabel, temp;
        std::string name, surname, patronymic;
        std::string low;
        int day, month, year, day1, month1, year1;
        std::string genderStr, jobTitleStr;
        int salary;

        iss >> nameLabel >> name >> surnameLabel >> surname >> patronymicLabel >> patronymic
            >> addLabel >> day >> temp >> month >> temp >> year
            >> genderLabel >> genderStr
            >> jobTitleLabel >> jobTitleStr
            >> salaryLabel >> salary;

        Person p(name, surname, patronymic, year, month, day,
            genderStr == "Мужчина" ? Gender::Male : Gender::Female,
            static_cast<JobTitle>(jobTitleStr[0] - 'A'), salary);

        if (iss >> remLabel >> day1 >> temp >> month1 >> temp >> year1) {
            p.add_rem(year1, month1, day1);
        }

        return p;
    }

    // Проверка был ли человек в штате
    bool is_employed_on(const std::tm& date) const {
        auto targetDate = std::mktime(const_cast<std::tm*>(&date));
        auto startDate = std::mktime(const_cast<std::tm*>(&add));

        if (targetDate < startDate) {
            return false; // Дата цели раньше даты начала работы
        }

        if (hasRemovalDate) {
            auto endDate = std::mktime(const_cast<std::tm*>(&rem));
            if (targetDate > endDate) {
                return false; // Дата цели после даты увольнения
            }
        }

        return true; // Сотрудник был в штате в указанный день
    }

    // Сеттеры
    void set_str(int a, const std::string& str) {
        switch (a) {
        case 1:
            name = str;
            break;
        case 2:
            surname = str;
            break;
        case 3:
            patronymic = str;
            break;
        }
    }

    // Добавление даты увольнения
    void add_rem(int year1, int month1, int day1) {
        if (month1 > 12) month1 = 12;
        if (month1 < 0) month1 = 1;
        if (day1 < 0) day1 = 1;
        if (month1 == 1 || month1 == 3 || month1 == 5 || month1 == 7 || month1 == 8 || month1 == 10 || month1 == 12)
            if (day1 > 31) {
                day1 = 1;
                if (month1 != 12) month1 += 1;
                else { year1 += 1; month1 = 1; day1 = 1; }
            }
        if (month1 == 4 || month1 == 6 || month1 == 9 || month1 == 11)
            if (day1 > 30) {
                day1 = 1;
                month1 += 1;
            }
        if (month1 == 2)
            if (day1 > 29) {
                day1 = 1;
                month1 += 1;
            }

        std::tm date;
        date.tm_year = year1 - 1900; // корректировка года
        date.tm_mon = month1 - 1;    // корректировка месяца
        date.tm_mday = day1;

        auto targetDate = std::mktime(const_cast<std::tm*>(&date));
        auto startDate = std::mktime(const_cast<std::tm*>(&add));

        if (targetDate > startDate) {
            rem.tm_year = year1; // корректировка года
            rem.tm_mon = month1;    // корректировка месяца
            rem.tm_mday = day1;
        }
        else
            std::cerr << "Ошибка: Дата увольнения не может быть раньше или равна дате приёма на работу.\n";
    }

    void set_Gender(Gender newGender) {
        gender = newGender;
    }

    void set_JobTitle(JobTitle newJobTitle) {
        jobTitle = newJobTitle;
    }

    void set_salary(int a) {
        salary = a;
    }

    void set_str(const std::string& str, const std::string& str2, const std::string& str3) {
        name = str2;
        surname = str;
        patronymic = str3;
    }

    // Перегрузка операторов сравнения для сортировки
    bool operator<(const Person& other) const {
        return surname < other.surname;
    }

    bool operator>(const Person& other) const {
        return surname > other.surname;
    }

    bool operator==(const Person& other) const {
        return name == other.name && surname == other.surname && patronymic == other.patronymic;
    }

    // Перегрузка операторов ввода и вывода
    friend std::istream& operator>>(std::istream& in, Person& person) {
        std::string nameLabel, surnameLabel, patronymicLabel, addLabel, remLabel, genderLabel, jobTitleLabel, salaryLabel, temp;
        std::string name, surname, patronymic;
        std::string low;
        int day, month, year, day1, month1, year1;
        std::string genderStr, jobTitleStr;
        int salary;

        in >> nameLabel >> name >> temp >> surnameLabel >> surname >> temp >> patronymicLabel >> patronymic >> temp
            >> addLabel >> addLabel >> addLabel >> addLabel >> day >> temp >> month >> temp >> year
            >> temp >> genderLabel >> genderStr >> temp
            >> jobTitleLabel >> jobTitleStr >> temp
            >> salaryLabel >> salary;

        Person p(name, surname, patronymic, year, month, day,
            genderStr == "Мужчина" ? Gender::Male : Gender::Female,
            static_cast<JobTitle>(jobTitleStr[0] - 'A'), salary);

        if (in >> remLabel >> day1 >> temp >> month1 >> temp >> year1) {
            p.add_rem(year1, month1, day1);
        }

        person = p; // Присваиваем считанный объект person

        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Person& person) {
        out << person.serialize();
        return out;
    }
};