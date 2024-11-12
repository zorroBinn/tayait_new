#include "Semant.h"
#include <iostream>
#include <string>
using namespace std;
#define max(a,b) a<b? b : a

Tree* Tree::current;

Tree::Tree(Scaner* sc)
{
	n = new Node();
	n->id = "<>";
	n->objType = ObjEmpty;
	n->dataType = UndefinedType;
	parent = nullptr;
	left = nullptr;
	right = nullptr;
	this->sc = sc;
}

Tree::Tree(Node* node, Tree* parent, Tree* left, Tree* right, Scaner* sc)
{
	n = node;
	this->parent = parent;
	this->left = left;
	this->right = right;
	this->sc = sc;
}

Tree::~Tree()
{
	if (n) delete n;
	if (left) delete left;
	if (right) delete right;
}

void Tree::setLeft(Tree* from, Node* node)
{
	left = new Tree(node, from, nullptr, nullptr, sc);
}

void Tree::setRight(Tree* from, Node* node)
{
	right = new Tree(node, from, nullptr, nullptr, sc);
}

Tree* Tree::findUp(Tree* From, TypeLex id)
{
	Tree* i = From; //Текущая вершина поиска
	while ((i != nullptr) && (id != i->n->id))
		i = i->parent; //Поднимаемся наверх по связям
	return i;
}

Tree* Tree::findUp(TypeLex id)
{
	return findUp(this, id);
}

void Tree::printTree(Tree* from)
{
	cout << "Вершина с данными: " << from->n->id << endl;
	if (from->left != nullptr)
		cout << "слева данные: " << from->left->n->id << " <--- ";
	if (from->right != nullptr)
		cout << "---> справа данные: " << from->right->n->id;
	cout << endl;
	if (from->left != nullptr)
		from->left->printTree(from->left);
	if (from->right != nullptr)
		from->right->printTree(from->right);
}

Tree* Tree::findUpOneLevel(Tree* From, TypeLex id)
{
	Tree* i = From; //Текущая вершина поиска
	while ((i->parent != nullptr) && (i->parent->right != i))
	{
		if (id == i->n->id) return i; //Нaшли совпадающий идентификатор
		i = i->parent; //Поднимаемся наверх по связям
	}
	return nullptr;
}

void Tree::setCurrent(Tree* cur)
{
	current = cur;
}

Tree* Tree::getCurrent()
{
	return current;
}

Tree* Tree::semInclude(TypeLex id, TypeObject obj, DataType data)
{
	dupControl(current, id);
	Node* b = new Node();
	b->id = id;
	b->objType = obj;
	b->dataType = data;
	current->setLeft(current, b);
	current = current->left;
	return current;
}

Tree* Tree::semGetVar(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Идентификатор не определён.");
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса.");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода.");
	if (v->n->objType == ObjClassObject)
		sc->error("Неверное использование объекта класса.");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main.");
	return v;
}

Tree* Tree::semGetClass(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Отсутствует описание класса с таким именем.");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода.");
	if (v->n->objType == ObjVar)
		sc->error("Неверное использование идентификатора переменной");
	if (v->n->objType == ObjClassObject)
		sc->error("Неверное использование объекта класса.");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main.");
	return v;
}

Tree* Tree::semGetMethod(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Отсутствует описание метода.");
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса.");
	if (v->n->objType == ObjVar)
		sc->error("Неверное использование идентификатора переменной");
	if (v->n->objType == ObjClassObject)
		sc->error("Неверное использование объекта класса.");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main.");
	return v;
}

Tree* Tree::semGetClassObject(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Объект класса не определён.");
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса.");
	if (v->n->objType == ObjVar)
		sc->error("Неверное использование идентификатора переменной");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода.");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main.");
	return v;
}

Tree* Tree::semGetVarOrCO(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Объект класса или переменная не определен(-а).");
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса.");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода.");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main.");
	return v;
}

void Tree::dupControl(Tree* Addr, TypeLex a)
{
	if (findUpOneLevel(Addr, a) == nullptr) return;
	if (Addr->n->objType == ObjVar)
		sc->error("Дублирование идентификатора переменной не допускается.");
	else if (Addr->n->objType == ObjClass)
		sc->error("Дублирование идентификатора класса не допускается.");
	else if (Addr->n->objType == ObjClassMethod)
		sc->error("Дублирование идентификатора метода класса не допускается.");
	else if (Addr->n->objType == ObjClassObject)
		sc->error("Дублирование идентификатора объекта класса не допускается.");
	else if (Addr->n->objType == ObjMain)
		sc->error("Дублирование main не допускается.");
}

Tree* Tree::createNewScope()
{
	current->left = new Tree(sc);
	current->left->parent = current;
	current = current->left;
	Tree* chPnt = current;
	current->right = new Tree(sc);
	current->right->parent = current;
	current = current->right;
	return chPnt;
}

DataType Tree::getDatatypeFromTypeLex(TypeLex a)
{
	if (a == "int") return IntType;
	if (a == "bool") return BoolType;
	if (a == "double") return DoubleType;
	return UndefinedType;
}

