using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using static ProcessHelper;  // Импорт статических методов из класса ProcessHelper

//Из процессов с идентификаторами из заданного диапазона выбрать те,
//у которых модуль максимальной длины. 

namespace OS_laba3
{
    public partial class Form1 : Form
    {
        // Конструктор главной формы
        public Form1()
        {
            InitializeComponent();
            InitializeDataGridView();
        }

        // Метод для настройки колонок в DataGridView
        private void InitializeDataGridView()
        {
            resultsDataGridView.Columns.Clear();  // Очистка существующих колонок

            // Добавление колонок для отображения информации о процессах и модулях
            resultsDataGridView.Columns.Add("ProcessId", "ID процесса");
            resultsDataGridView.Columns.Add("ProcessName", "Имя процесса");
            resultsDataGridView.Columns.Add("ModuleName", "Самый большой модуль");
            resultsDataGridView.Columns.Add("ModuleSize", "Размер модуля (байты)");
            resultsDataGridView.Columns.Add("ModulePath", "Путь к модулю");
        }

        // Обработчик события нажатия кнопки сканирования
        private void scanButton_Click(object sender, EventArgs e)
        {
            // Парсинг минимального ID процесса из текстового поля
            if (!uint.TryParse(minIdTextBox.Text, out uint minId) ||
                // Парсинг максимального ID процесса из текстового поля
                !uint.TryParse(maxIdTextBox.Text, out uint maxId))
            {
                MessageBox.Show("Пожалуйста, введите корректные числовые ID процессов");
                return;
            }

            // Проверка корректности диапазона ID
            if (minId > maxId)
            {
                MessageBox.Show("Минимальный ID процесса не может быть больше максимального");
                return;
            }

            // Запуск сканирования процессов в указанном диапазоне
            ScanProcesses(minId, maxId);
        }

        // Основной метод сканирования процессов
        private void ScanProcesses(uint minId, uint maxId)
        {
            // Блокировка кнопки сканирования на время выполнения операции
            scanButton.Enabled = false;
            // Отображение прогресс-бара
            progressBar.Visible = true;
            // Очистка предыдущих результатов
            resultsDataGridView.Rows.Clear();

            try
            {
                // Получение списка процессов в указанном диапазоне ID
                var processes = ProcessHelper.GetProcessesInRange(minId, maxId);

                // Настройка прогресс-бара: максимальное значение = количество процессов
                progressBar.Maximum = processes.Count;
                // Сброс текущего значения прогресс-бара
                progressBar.Value = 0;

                // Список для хранения информации о процессах и их модулях
                var results = new List<ProcessModuleInfo>();

                // Перебор всех найденных процессов
                foreach (var process in processes)
                {
                    // Получение информации о самом большом модуле для текущего процесса
                    var largestModule = ProcessHelper.GetLargestModule(process.th32ProcessID);

                    // Если модуль найден, добавляем информацию в список результатов
                    if (largestModule.HasValue)
                    {
                        results.Add(new ProcessModuleInfo
                        {
                            ProcessId = process.th32ProcessID,      // ID процесса
                            ProcessName = process.szExeFile,        // Имя исполняемого файла
                            LargestModule = largestModule.Value     // Информация о самом большом модуле
                        });
                    }

                    // Увеличение значения прогресс-бара
                    progressBar.Value++;
                    // Обработка pending сообщений UI для предотвращения "зависания" интерфейса
                    Application.DoEvents();
                }

                // Отображение результатов сканирования
                DisplayResults(results);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при сканировании процессов: {ex.Message}");
            }
            finally
            {
                // Восстановление состояния UI независимо от результата выполнения
                scanButton.Enabled = true;      // Разблокировка кнопки
                progressBar.Visible = false;    // Скрытие прогресс-бара
            }
        }

        // Метод для отображения результатов сканирования
        private void DisplayResults(List<ProcessModuleInfo> results)
        {
            // Проверка наличия результатов
            if (!results.Any())
            {
                MessageBox.Show("В указанном диапазоне не найдено процессов с модулями");
                return;
            }

            // Поиск максимального размера модуля среди всех процессов
            var maxSize = results.Max(r => r.LargestModule.modBaseSize);

            // Фильтрация процессов: оставляем только те, у которых размер модуля равен максимальному
            var processesWithMaxModule = results.Where(r => r.LargestModule.modBaseSize == maxSize).ToList();

            // Добавление отфильтрованных результатов в DataGridView
            foreach (var result in processesWithMaxModule)
            {
                resultsDataGridView.Rows.Add(
                    result.ProcessId,                       // ID процесса
                    result.ProcessName,                     // Имя процесса
                    result.LargestModule.szModule,          // Имя самого большого модуля
                    result.LargestModule.modBaseSize,       // Размер модуля в байтах
                    result.LargestModule.szExePath          // Полный путь к модулю
                );
            }

            // Вывод информационного сообщения с количеством найденных процессов
            MessageBox.Show($"Найдено {processesWithMaxModule.Count} процесс(ов) с максимальным размером модуля ({maxSize} байт)");
        }
    }

    // Вспомогательный класс для хранения информации о процессе и его самом большом модуле
    public class ProcessModuleInfo
    {
        public uint ProcessId { get; set; }           // ID процесса
        public string ProcessName { get; set; }       // Имя процесса
        public MODULEENTRY32 LargestModule { get; set; }  // Структура с информацией о самом большом модуле
    }
}