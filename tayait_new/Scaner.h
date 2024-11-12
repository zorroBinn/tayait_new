#pragma once
#include "Defs.h"
#include <fstream>

class Scaner {
private:
    std::string text; //Исходный (сканируемый) текст
    size_t uk; //Текущая позиция в тексте
    int line; //Номер строки для выдачи ошибки
    int posInLine; //Позиция в строке для выдачи ошибки

public:
    Scaner(const std::string& filename); //Конструктор
    bool readFile(const std::string& filename); //Чтение исходного файла
    size_t getUK() const; //Запоминание текущей позиции
    void setUK(size_t newUK); //Восстановление позиции
    int nextLexeme(TypeLex& l); //Функция выделения очередной лексемы
    void error(const std::string& msg) const; //Функция выдачи сообщения об ошибке
    bool isEnd() const; //Проверка конца файла
    int getLine();
    void setLine(int line);
    int getPosInLine();
    void setPosInLine(int posInline);
};

