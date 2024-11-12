#pragma once
#include "Scaner.h"

enum TypeObject {
	ObjEmpty = 0,
	ObjVar,
	ObjClass,
	ObjClassObject,
	ObjClassMethod,
	ObjMain,
};

enum DataType {
	UndefinedType = 0,
	IntType,
	DoubleType,
	BoolType
};

struct Node {
	TypeLex id; //Идентификатор объекта
	TypeObject objType; //Тип объекта
	DataType dataType; //Тип данных
};

class Tree {
private:
	Node* n;
	Tree* parent, * left, * right;
	Scaner* sc;
	static Tree* current; //Текущий элемент дерева

public:
	Tree(Scaner* sc);
	Tree(Node* node, Tree* parent, Tree* left, Tree* right, Scaner* sc); //Конструктор создает узел с заданными связями и данными
	~Tree();

	void setLeft(Tree* from, Node* node); //Создать левого потомка от текущей вершины
	void setRight(Tree* from, Node* node); //Создать правого потомка от текущей вершины

	Tree* findUp(Tree* From, TypeLex id); //Поиск данных в дереве от заданной вершины From до его корня вверх по связям
	Tree* findUp(TypeLex id); //Поиск данных в дереве, начиная от текущей вершины this до его корня вверх по связям
	Tree* findUpOneLevel(Tree* From, TypeLex id); //Поиск элемента id вверх по дереву от текущей вершины From. Поиск осуществляется на одном уровне вложенности по левым связям

	void printTree(Tree* from);

	void setCurrent(Tree* cur); //Установить текущий узел дерева
	Tree* getCurrent(); //Получить значение текущего узла дерева

	Tree* semInclude(TypeLex id, TypeObject obj, DataType data); //Занесение идентификатора в таблицу
	Tree* semGetVar(TypeLex a); //Найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree* semGetClass(TypeLex a); //Найти в таблице класс с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree* semGetMethod(TypeLex a); //Найти в таблице метод класса с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree* semGetClassObject(TypeLex a); //Найти в таблице объект класса с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree* semGetVarOrCO(TypeLex a); //Найти в таблице переменную или объект класса с именем a и вернуть ссылку на соответствующий элемент дерева
	void dupControl(Tree* Addr, TypeLex a); //Проверка идентификатора а на повторное описание внутри блока
	Tree* createNewScope(); //Создать новую область видимости
	DataType getDatatypeFromTypeLex(TypeLex a); //Получить тип данных лексемы
};

