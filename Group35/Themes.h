#pragma once

//Masab Tahir 23i-0006
//Muhammad Arslan 23i-0572
//Xonix Game
#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<string>
#include <fstream>
using namespace std;
using namespace ::sf;
struct Themes {
	int id;
	sf::Color backgroundColor;
	sf::Color textColor;
	string font;
	Themes* llink;
	Themes* rlink;
	int bfactor;
	string image;

	Themes() {
		id = -999;
		backgroundColor = Color::Black;
		textColor = Color::White;
		font="fonts/arial.ttf";
		llink = rlink = nullptr;
		bfactor = 0;
	}

	Themes(int idVal, Color bg, Color txt, string fontPath,string imagePath) {
		id = idVal;
		backgroundColor = bg;
		textColor = txt;
		font = fontPath;
		llink = rlink = nullptr;
		bfactor = 0;
		image = imagePath;
	}
};

class AVLTree {
    Themes* root;

	void rotateToLeft(Themes*& root) {
		Themes* p;
		if (root == NULL || root->rlink == NULL) return;
		p = root->rlink;
		root->rlink = p->llink;
		p->llink = root;
		root = p;
	}

	void rotateToRight(Themes*& root) {
		Themes* p;
		if (root == NULL || root->llink == NULL) return;
		p = root->llink;
		root->llink = p->rlink;
		p->rlink = root;
		root = p;
	}

	void balanceFromLeft(Themes*& root) {
		Themes* p = root->llink;
		switch (p->bfactor) {
		case -1:
			root->bfactor = 0;
			p->bfactor = 0;
			rotateToRight(root);
			break;
		case 1: {
			Themes* w = p->rlink;
			switch (w->bfactor) {
			case -1:
				root->bfactor = 1;
				p->bfactor = 0;
				break;
			case 0:
				root->bfactor = 0;
				p->bfactor = 0;
				break;
			case 1:
				root->bfactor = 0;
				p->bfactor = -1;
			}
			w->bfactor = 0;
			rotateToLeft(p);
			root->llink = p;
			rotateToRight(root);
			break;
		}
		}
	}

	void balanceFromRight(Themes*& root) {
		Themes* p = root->rlink;
		switch (p->bfactor) {
		case -1: {
			Themes* w = p->llink;
			switch (w->bfactor) {
			case -1:
				root->bfactor = 0;
				p->bfactor = 1;
				break;
			case 0:
				root->bfactor = 0;
				p->bfactor = 0;
				break;
			case 1:
				root->bfactor = -1;
				p->bfactor = 0;
			}
			w->bfactor = 0;
			rotateToRight(p);
			root->rlink = p;
			rotateToLeft(root);
			break;
		}
		case 1:
			root->bfactor = 0;
			p->bfactor = 0;
			rotateToLeft(root);
			break;
		}
	}

	void insertIntoAVL(Themes*& root, Themes* newNode, bool& isTaller) {
		if (root == NULL) {
			root = newNode;
			isTaller = true;
		}
		else if (newNode->id < root->id) {
			insertIntoAVL(root->llink, newNode, isTaller);
			if (isTaller) {
				switch (root->bfactor) {
				case -1:
					balanceFromLeft(root);
					isTaller = false;
					break;
				case 0:
					root->bfactor = -1;
					isTaller = true;
					break;
				case 1:
					root->bfactor = 0;
					isTaller = false;
				}
			}
		}
		else if (newNode->id> root->id) {
			insertIntoAVL(root->rlink, newNode, isTaller);
			if (isTaller) {
				switch (root->bfactor) {
				case -1:
					root->bfactor = 0;
					isTaller = false;
					break;
				case 0:
					root->bfactor = 1;
					isTaller = true;
					break;
				case 1:
					balanceFromRight(root);
					isTaller = false;
				}
			}
		}
	}

	Themes* searchNode(Themes* root, int key) {
		if (!root || root->id == key) return root;
		if (key < root->id) return searchNode(root->llink, key);
		else return searchNode(root->rlink, key);
	}

	void inOrder(Themes* node) {
		if (!node) return;
		inOrder(node->llink);
		cout << node->id<< " ";
		inOrder(node->rlink);
	}

	

	Themes* findMin(Themes* node) {
		while (node->llink) node = node->llink;
		return node;
	}
	int countThemesHelper(Themes* node) {
		if (!node) return 0;
		return 1 + countThemesHelper(node->llink) + countThemesHelper(node->rlink);
	}
	

public:
	AVLTree() {
		root = nullptr;
	}

	void insert(Themes* newItem) {
		bool isTaller = false;
		Themes* newNode = new Themes;
		newNode->id = newItem->id;
		newNode->font = newItem->font;
		newNode->textColor = newItem->textColor;
		newNode->backgroundColor = newItem->backgroundColor;
		newNode->image = newItem->image;
		insertIntoAVL(root, newNode, isTaller);
	}

	Themes* search(int key) {
		Themes* res = searchNode(root, key);
		if (res) {
			cout << "Found" << endl;
			return res;
		}
		else cout << "Not Found" << endl;
		return nullptr;
	}

	void displayInOrder() {
		inOrder(root);
		cout << endl;
	}

	int countThemes() {
		return countThemesHelper(root);
	}
	Themes* getRoot() {
		return root;
	}


};
