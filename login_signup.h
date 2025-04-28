#pragma once
#include<iostream>
#include<fstream>
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
};


class Player {
private:
	Node* head;

public:
	Player() {
		head = nullptr;
	}
	void setUsername(string& temp) { this->head->Username = temp; }
	string getUsername()const { return this->head->Username ; }
	void setPassword(string& temp) { this->head->Password = temp; }
	string getPassword()const { return this->head->Password; }
	void createAccount(string temp1, string temp2) {
		if (head == nullptr) {
			head = new Node;
			head->Username = temp1;
			head->Password = temp2;
		}
		else {
			Node* curr = head;
			while (curr->next != nullptr) {
				curr = curr->next;
			}
			curr->next = new Node;
			curr->next->Username = temp1;
			curr->next->Password = temp2;
		}
	}

	bool check(string temp1,string temp2){
		Node* curr = head;
		while (curr) {
			if (curr->Username == temp1 && curr->Password == temp2) {
				cout << "Login successful!\n";
				cout << "Username: " << curr->Username << endl;
				cout << "Password: " << curr->Password << endl;
				return true;
			}
			curr = curr->next;
		}
		cout << "Account doesn't exist. Please sign up.\n";
		return false;
	}
	void loadPlayersFromFile(const string& filename) {
		ifstream fin(filename);
		if (!fin) {
			cout << "Error opening file: " << filename << endl;
			return;
		}
		string username, password;
		while (fin >> username >> password) {
			createAccount(username, password);
		}
		fin.close();
	}

	void savePlayersToFile(const string& filename) {
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

};