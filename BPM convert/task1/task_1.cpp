#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push,1) // Выравнивание структуры
struct BITMAPFILEHEADER {
    uint16_t bfType;      // Тип файла (BM)
    uint32_t bfSize;      // Размер файла в байтах
    uint16_t bfReserved1; // Зарезервировано
    uint16_t bfReserved2; // Зарезервировано
    uint32_t bfOffBits;   // Смещение до начала данных
};
struct BITMAPINFOHEADER {
    uint32_t biSize;          // Размер этого заголовка
    int32_t  biWidth;         // Ширина изображения
    int32_t  biHeight;        // Высота изображения
    uint16_t biPlanes;        // Количество цветовых плоскостей (обычно 1)
    uint16_t biBitCount;      // Количество бит на пиксель
    uint32_t biCompression;    // Тип сжатия (0 = непостроенный)
    uint32_t biSizeImage;     // Размер изображения в байтах
    int32_t  biXPelsPerMeter;  // Горизонтальное разрешение
    int32_t  biYPelsPerMeter;  // Вертикальное разрешение
    uint32_t biClrUsed;       // Количество используемых цветов
    uint32_t biClrImportant;   // Количество важных цветов
};
#pragma pack(pop)

int ConvertToGrayscale(const std::string& inputFilePath);

int main()
{
    setlocale(LC_ALL, "Rus");
    std::string filename;

    filename = "input3.bmp";
    int result = ConvertToGrayscale(filename);
    if (result == 1) {
        std::cout << "good" << std::endl;
    }
}

int ConvertToGrayscale(const std::string& inputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return -1;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    inputFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    inputFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    //std::cout << std::hex << std::uppercase << static_cast<uint16_t>(fileHeader.bfType) << std::endl;

    /*if (fileHeader.bfType != 0x4D42 || infoHeader.biBitCount != 24) {
        std::cerr << "Ошибка: файл не является 24-битным BMP" << std::endl;
        return;
    }*/

    int rowSize = (infoHeader.biBitCount / 8) * infoHeader.biWidth;
    std::vector<uint8_t> rowData(rowSize);

    // Создаем новый файл для записи изображения в градациях серого
    std::ofstream outputFile("output.bmp", std::ios::binary);
    outputFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    outputFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // Чтение и преобразование пикселей
    for (int i = 0; i < infoHeader.biHeight; ++i) {
        inputFile.read(reinterpret_cast<char*>(rowData.data()), rowSize);

        for (int j = 0; j < infoHeader.biWidth; ++j) {
            uint8_t blue = rowData[j * 3];       // Синий компонент
            uint8_t green = rowData[j * 3 + 1];  // Зеленый компонент
            uint8_t red = rowData[j * 3 + 2];     // Красный компонент

            // Вычисляем яркость пикселя по формуле яркости
            uint8_t gray = static_cast<uint8_t>(0.299 * red + 0.587 * green + 0.114 * blue);

            // Устанавливаем все три компонента цвета в одно значение (оттенок серого)
            rowData[j * 3] = rowData[j * 3 + 1] = rowData[j * 3 + 2] = gray;
        }

        // Записываем преобразованные данные в выходной файл
        outputFile.write(reinterpret_cast<char*>(rowData.data()), rowSize);
    }

    inputFile.close();
    outputFile.close();

    return 1;
}
