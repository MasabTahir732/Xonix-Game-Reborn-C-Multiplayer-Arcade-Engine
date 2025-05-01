#pragma once
#include<iostream>
#include<fstream>
#include"SaveGame.h"
#include"Main Menu.h"
#include"Themes.h"
using namespace std;
class Node {
public:
	string Username;
	string Password;
	int PlayerScore;
	float WinPercentage;
	Node* next;
public:
	Node() {
		next = nullptr;
		Username = "";
		Password = "";
		PlayerScore = 0; WinPercentage = 0;
	}
	void setUsername(string& temp) { Username = temp; }
	string getUsername()const { return Username; }
	void setPassword(string& temp) { Password = temp; }
	string getPassword()const { return Password; }
};


class Player {
private:
	Node* head;

public:
	Player() {
		head = nullptr;
	}
	void setUsername(string& temp) { this->head->Username = temp; }
	string getUsername() const {
		return head ? head->Username : "";
	}

	void setPassword(string& temp) { this->head->Password = temp; }
	string getPassword()const { return this->head->Password; }
	bool usernameExists(const string& name) {
		Node* curr = head;
		while (curr) {
			if (curr->Username == name)
				return true;
			curr = curr->next;
		}
		return false;
	}
	int createAccount(string temp1,string temp2) {

	
	

		Node* curr = head;
		while (curr) {
			if (curr->Username == temp1) {
				cout << "Account already exists. Please login.\n";
				return 0;
			}
			curr = curr->next;
		}

		Node* newNode = new Node;
		newNode->Username = temp1;
		newNode->Password = temp2;
		newNode->next = nullptr;

		if (head == nullptr) {
			head = newNode;
		}
		else {
			curr = head;
			while (curr->next)
				curr = curr->next;
			curr->next = newNode;
		}

		savePlayersToFile();
		return 1;
	}

	/*Player* createAccount() {
		string temp1, temp2;
		cout << "enter Username: ";
		cin >> temp1;
		cout << "enter Password: ";
		cin >> temp2;

		// Check if list is empty
		if (head == nullptr) {
			head = new Node;
			head->Username = temp1;
			head->Password = temp2;
			savePlayersToFile();  // ✅ save after creating
			return this;
		}

		// Check if username already exists
		Node* curr = head;
		while (curr != nullptr) {
			if (curr->Username == temp1) {
				cout << "Account already exists. Please login now.\n";
				this->Login();
				return this;
			}
			if (curr->next == nullptr) break;
			curr = curr->next;
		}

		// Add new node at end
		curr->next = new Node;
		curr->next->Username = temp1;
		curr->next->Password = temp2;
		savePlayersToFile();  // ✅ save after adding
		return this;
	}*/

	Node* Login(string temp1, string temp2) {
		Node* curr = head;
		while (curr) {
			if (curr->Username == temp1 && curr->Password == temp2) {
				cout << "Login successful!\n";
				return curr;
			}
			else if (curr->Username == temp1) {
				cout << "Wrong Password.\n";
				return nullptr;
			}
			curr = curr->next;
		}

		
		return nullptr; 
	}


	/*Player* Login() {
		string temp1, temp2;
		cout << "enter Username: ";
		cin >> temp1;
		cout << "enter Password: ";
		cin >> temp2;
		Node* curr = head;
		while (curr) {
			if (curr->Username == temp1 && curr->Password == temp2) {
				cout << "Login successful!\n";
				cout << "Username: " << curr->Username << endl;
				cout << "Password: " << curr->Password << endl;
				return this;
			}
			else if (curr->Username == temp1 && curr->Password != temp2) {
				cout << "Wrong Password.\n";
				Login();
				return this;
			}
			curr = curr->next;
		}
		cout << "Account doesn't exist. Please sign up.\n";
		this->createAccount();
		return this;
	}*/
	void savePlayersToFile() {
		const string filename = "Players.txt";
		ofstream fout(filename);
		if (!fout) {
			cout << "Error opening file for writing: " << filename << endl;
			return;
		}
		Node* temp = head;
		while (temp != nullptr) {
			fout << temp->Username << " " << temp->Password << endl;
			temp = temp->next;
		}
		fout.close();
	}
	void loadPlayersFromFile() {
		const string filename = "Players.txt";
		ifstream fin(filename);
		if (!fin) {
			cout << "No saved players found.\n";
			return;
		}
		string username, password;
		while (fin >> username >> password) {
			Node* newNode = new Node;
			newNode->Username = username;
			newNode->Password = password;
			newNode->next = nullptr;

			if (head == nullptr) {
				head = newNode;
			}
			else {
				Node* curr = head;
				while (curr->next != nullptr)
					curr = curr->next;
				curr->next = newNode;
			}
		}
		fin.close();
	}



};
