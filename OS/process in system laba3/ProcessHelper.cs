using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public static class ProcessHelper
{
    // Флаги для создания снимка системы (snapshot)
    [Flags]
    internal enum SnapshotFlags : uint
    {
        HeapList = 0x00000001,    // Включает информацию о кучах (heaps) процесса
        Process = 0x00000002,     // Включает информацию о процессах
        Thread = 0x00000004,      // Включает информацию о потоках
        Module = 0x00000008,      // Включает информацию о модулях (16-битные приложения)
        Module32 = 0x00000010,    // Включает информацию о модулях (32-битные приложения)
        Inherit = 0x80000000,     // Дочерние процессы наследуют снимок
        All = 0x0000001F,         // Включает всю доступную информацию
        NoHeaps = 0x40000000      // Исключает информацию о кучах для ускорения
    }

    // Импорт функции из Windows API для создания снимка процессов/модулей
    // CreateToolhelp32Snapshot - создает "снимок" указанных системных ресурсов
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern IntPtr CreateToolhelp32Snapshot(
        [In] UInt32 dwFlags,          // Флаги, определяющие что включать в снимок
        [In] UInt32 th32ProcessID     // ID процесса (0 для всех процессов)
    );

    // Извлекает информацию о первом процессе из снимка
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool Process32First(
        [In] IntPtr hSnapshot,        // Дескриптор снимка, полученный из CreateToolhelp32Snapshot
        ref PROCESSENTRY32 lppe       // Ссылка на структуру для заполнения информацией о процессе
    );

    // Извлекает информацию о следующем процессе из снимка
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool Process32Next(
        [In] IntPtr hSnapshot,        // Дескриптор снимка
        ref PROCESSENTRY32 lppe       // Ссылка на структуру для заполнения
    );

    // Извлекает информацию о первом модуле из снимка
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool Module32First(
        [In] IntPtr hSnapshot,        // Дескриптор снимка модулей
        ref MODULEENTRY32 lpme        // Ссылка на структуру для заполнения информацией о модуле
    );

    // Извлекает информацию о следующем модуле из снимка
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool Module32Next(
        [In] IntPtr hSnapshot,        // Дескриптор снимка модулей
        ref MODULEENTRY32 lpme        // Ссылка на структуру для заполнения
    );

    // Закрывает дескриптор системного объекта (освобождает ресурсы)
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool CloseHandle(
        [In] IntPtr hObject           // Дескриптор объекта для закрытия
    );

    // Структура для хранения информации о процессе
    // [StructLayout(LayoutKind.Sequential)] - гарантирует, что поля располагаются 
    // в памяти последовательно в том же порядке, как объявлены в структуре
    // Это важно для корректной маршализации (преобразования) между управляемым и 
    // неуправляемым кодом
    [StructLayout(LayoutKind.Sequential)]
    public struct PROCESSENTRY32
    {
        public uint dwSize;           // Размер структуры в байтах (должен быть установлен перед использованием)
        public uint cntUsage;         // Счетчик ссылок на процесс (сколько раз процесс используется)
        public uint th32ProcessID;    // Уникальный идентификатор процесса (PID)
        public IntPtr th32DefaultHeapID; // Идентификатор основной кучи процесса
        public uint th32ModuleID;     // Идентификатор основного модуля процесса
        public uint cntThreads;       // Количество потоков выполнения в процессе
        public uint th32ParentProcessID; // Идентификатор родительского процесса
        public int pcPriClassBase;    // Базовый приоритет класса процесса
        public uint dwFlags;          // Флаги процесса (зарезервировано)

        // Имя исполняемого файла процесса
        // [MarshalAs] - указывает как маршализировать строку между управляемым и неуправляемым кодом
        // ByValTStr - строка фиксированной длины, передаваемая по значению
        // SizeConst = 260 - максимальная длина пути в Windows (MAX_PATH)
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string szExeFile;
    }

    // Структура для хранения информации о модуле (DLL, EXE)
    [StructLayout(LayoutKind.Sequential)]
    public struct MODULEENTRY32
    {
        public uint dwSize;           // Размер структуры в байтах
        public uint th32ModuleID;     // Идентификатор модуля
        public uint th32ProcessID;    // Идентификатор процесса, которому принадлежит модуль
        public uint GlblcntUsage;     // Глобальный счетчик использования модуля
        public uint ProccntUsage;     // Счетчик использования в текущем процессе
        public IntPtr modBaseAddr;    // Базовый адрес модуля в памяти
        public uint modBaseSize;      // Размер модуля в байтах
        public IntPtr hModule;        // Дескриптор модуля

        // Имя модуля
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string szModule;

        // Полный путь к файлу модуля
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string szExePath;
    }

    // Метод для получения списка процессов в указанном диапазоне ID
    public static List<PROCESSENTRY32> GetProcessesInRange(uint minId, uint maxId)
    {
        var processes = new List<PROCESSENTRY32>();
        IntPtr snapshotHandle = IntPtr.Zero;  // Дескриптор снимка системы

        try
        {
            // Создаем снимок всех процессов в системе
            // (uint)SnapshotFlags.Process - флаг указывает, что нужна информация о процессах
            // 0 - получить информацию обо всех процессах
            snapshotHandle = CreateToolhelp32Snapshot((uint)SnapshotFlags.Process, 0);

            // Проверяем успешность создания снимка
            if (snapshotHandle == IntPtr.Zero || snapshotHandle.ToInt64() == -1)
                throw new ApplicationException($"Failed to create snapshot. Error code: {Marshal.GetLastWin32Error()}");

            // Создаем и инициализируем структуру для информации о процессе
            PROCESSENTRY32 procEntry = new PROCESSENTRY32();
            procEntry.dwSize = (uint)Marshal.SizeOf(typeof(PROCESSENTRY32));  // Устанавливаем размер структуры

            // Получаем информацию о первом процессе в снимке
            if (Process32First(snapshotHandle, ref procEntry))
            {
                do
                {
                    // Проверяем, попадает ли ID процесса в заданный диапазон
                    if (procEntry.th32ProcessID >= minId && procEntry.th32ProcessID <= maxId)
                    {
                        processes.Add(procEntry);  // Добавляем процесс в список
                    }
                }
                while (Process32Next(snapshotHandle, ref procEntry));  // Переходим к следующему процессу
            }
        }
        finally
        {
            // Гарантированно освобождаем дескриптор снимка в блоке finally
            if (snapshotHandle != IntPtr.Zero && snapshotHandle.ToInt64() != -1)
                CloseHandle(snapshotHandle);
        }

        return processes;
    }

    // Метод для поиска самого большого модуля в указанном процессе
    public static MODULEENTRY32? GetLargestModule(uint processId)
    {
        IntPtr snapshotHandle = IntPtr.Zero;
        MODULEENTRY32? largestModule = null;  // Nullable тип для хранения результата

        try
        {
            // Создаем снимок модулей для конкретного процесса
            snapshotHandle = CreateToolhelp32Snapshot((uint)SnapshotFlags.Module, processId);
            if (snapshotHandle == IntPtr.Zero || snapshotHandle.ToInt64() == -1)
                return null;  // Не удалось создать снимок

            // Инициализируем структуру для информации о модуле
            MODULEENTRY32 moduleEntry = new MODULEENTRY32();
            moduleEntry.dwSize = (uint)Marshal.SizeOf(typeof(MODULEENTRY32));

            // Получаем информацию о первом модуле
            if (Module32First(snapshotHandle, ref moduleEntry))
            {
                do
                {
                    // Сравниваем размер текущего модуля с самым большим найденным
                    if (!largestModule.HasValue || moduleEntry.modBaseSize > largestModule.Value.modBaseSize)
                    {
                        largestModule = moduleEntry;  // Обновляем самый большой модуль
                    }
                }
                while (Module32Next(snapshotHandle, ref moduleEntry));  // Переходим к следующему модулю
            }
        }
        finally
        {
            // Гарантированно освобождаем ресурсы
            if (snapshotHandle != IntPtr.Zero && snapshotHandle.ToInt64() != -1)
                CloseHandle(snapshotHandle);
        }

        return largestModule;
    }
}