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
	n->data.dataType = UndefinedType; //
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
	bool delRight = n->data.dataType == ObjClassObject ? false : true;
	if (n) delete n;
	if (left) delete left;
	if (right && delRight) delete right;
}

void Tree::deleteSubtree(Tree* from)
{
	if (!from) return;
	deleteSubtree(from->left); from->left = nullptr;
	deleteSubtree(from->right); from->right = nullptr;
	delete from;
}

void Tree::removeSubtree(Tree* from)
{
	if (from->right) {
		deleteSubtree(from->right);
		from->right = nullptr;
	}
}

void Tree::setLeft(Tree* from, Node* node)
{
	left = new Tree(node, from, nullptr, nullptr, sc);
}
 
void Tree::setRight(Tree* from, Node* node)
{
	right = new Tree(node, from, nullptr, nullptr, sc);
}

void Tree::setRight(Tree* from, Tree* r)
{
	from->right = r;
}

Tree* Tree::getLeft(Tree* from)
{
	return from->left;
}

Tree* Tree::getRight(Tree* from)
{
	return from->right;
}

Tree* Tree::findUp(Tree* From, TypeLex id)
{
	Tree* i = From; //Текущая вершина поиска
	while ((i != nullptr) && (id != i->n->id))
		i = i->parent; //Поднимаемся наверх по связям
	return i;
}

void Tree::printTree(Tree* from, const std::string& prefix, bool isLeft)
{
	if (!from) return;

	cout << prefix;
	cout << (isLeft ? "|-- " : "L_ ") << from->n->id << endl;

	string newPrefix = prefix + (isLeft ? "|   " : "    ");

	if (from->left || from->right) { 
		if (from->left) {
			printTree(from->left, newPrefix, true);
		}
		else {
			cout << newPrefix << "|-- [null]" << endl;
		}
		if (from->right) {
			printTree(from->right, newPrefix, false);
		}
		else {
			cout << newPrefix << "L_ [null]" << endl;
		}
	}
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

Tree* Tree::findRightLeft(Tree* From, TypeLex id)
{
	Tree* i = From->right; //Текущая вершина поиска
	while ((i != nullptr) && (id != i->n->id))
		i = i->left;
	//Обходим только соседей по левым связям
	return i;
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
	b->data.dataType = data; //
	current->setLeft(current, b);
	current = current->left;
	return current;
}

Tree* Tree::semIncludeClassObject(TypeLex id, TypeObject obj, DataType data, Tree* r)
{
	dupControl(current, id);
	Node* b = new Node();
	b->id = id;
	b->objType = obj;
	b->data.dataType = data; //
	current->setLeft(current, b);
	current = current->left;
	current->setRight(current, r);
	return current;
}

Tree* Tree::semGetVar(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Идентификатор не определён");
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода");
	if (v->n->objType == ObjClassObject)
		sc->error("Неверное использование объекта класса");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main");
	return v;
}

Tree* Tree::semGetClass(TypeLex a)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
		sc->error("Отсутствует описание класса с таким именем");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода");
	if (v->n->objType == ObjVar)
		sc->error("Неверное использование идентификатора переменной");
	if (v->n->objType == ObjClassObject)
		sc->error("Неверное использование объекта класса");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main");
	return v;
}

Tree* Tree::semGetMethod(TypeLex a, Tree* from)
{
	Tree* v = findUp(current, a);
	if (v == nullptr) 
	{
		v = findRightLeft(from, a);
			if (v == nullptr) sc->error("Отсутствует описание метода");
	}
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса");
	if (v->n->objType == ObjVar)
		sc->error("Неверное использование идентификатора переменной");
	if (v->n->objType == ObjClassObject)
		sc->error("Неверное использование объекта класса");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main");
	return v;
}

Tree* Tree::semGetClassObject(TypeLex a, Tree* from)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
	{
		v = findRightLeft(from, a);
		if (v == nullptr) sc->error("Объект класса не определён");
	}
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса");
	if (v->n->objType == ObjVar)
		sc->error("Неверное использование идентификатора переменной");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main");
	return v;
}

Tree* Tree::semGetVarOrCO(TypeLex a, Tree* from)
{
	Tree* v = findUp(current, a);
	if (v == nullptr)
	{
		v = findRightLeft(from, a);
		if (v == nullptr) sc->error("Объект класса или переменная не определен(-а)");
	}
	if (v->n->objType == ObjClass)
		sc->error("Неверное использование имени класса");
	if (v->n->objType == ObjClassMethod)
		sc->error("Неверное использование вызова метода");
	if (v->n->objType == ObjMain)
		sc->error("Недопустимое использование main");
	return v;
}

void Tree::dupControl(Tree* Addr, TypeLex a)
{
	Tree* v = findUpOneLevel(Addr, a);
	if (v == nullptr) return;
	if (v->n->objType == ObjVar)
		sc->error("Дублирование идентификатора переменной не допускается");
	else if (v->n->objType == ObjClass)
		sc->error("Дублирование идентификатора класса не допускается");
	else if (v->n->objType == ObjClassMethod)
		sc->error("Дублирование идентификатора метода класса не допускается");
	else if (v->n->objType == ObjClassObject)
		sc->error("Дублирование идентификатора объекта класса не допускается");
	else if (v->n->objType == ObjMain)
		sc->error("Дублирование main не допускается");
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

