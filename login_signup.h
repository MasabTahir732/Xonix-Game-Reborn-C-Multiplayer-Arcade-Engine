#pragma once
#include<iostream>
#include<fstream>
#include <sstream>
#include"SaveGame.h"
#include"Main Menu.h"
#include"Themes.h"
using namespace std;

class Node {
public:
	string Username;
	string Password;
	int TotalScore;
	float WinPercentage;
	Node* next;
	int CurrentScore;
	int RewardCounter;
	int PowerUpCount;

	Node() {
		next = nullptr;
		Username = "";
		Password = "";
		TotalScore = 0;
		WinPercentage = 0;
		PowerUpCount = 0;
		CurrentScore = 0;
		RewardCounter = 0;
	}

	void setUsername(string& temp) { Username = temp; }
	string getUsername() const { return Username; }
	void setPassword(string& temp) { Password = temp; }
	string getPassword() const { return Password; }
};

class Player {
private:
	Node* head;

public:
	Player() {
		head = nullptr;
	}

	void setUsername(string& temp) { if (head) head->Username = temp; }
	string getUsername() const { return head ? head->Username : ""; }
	void setPassword(string& temp) { if (head) head->Password = temp; }
	string getPassword() const { return head ? head->Password : ""; }

	bool usernameExists(const string& name) {
		Node* curr = head;
		while (curr) {
			if (curr->Username == name) return true;
			curr = curr->next;
		}
		return false;
	}

	int createAccount(string temp1, string temp2) {
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
		if (head == nullptr) head = newNode;
		else {
			curr = head;
			while (curr->next) curr = curr->next;
			curr->next = newNode;
		}
		savePlayersToFile();
		return 1;
	}

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

	void savePlayersToFile() {
		ofstream fout("Players.txt");
		if (!fout) {
			cout << "Error opening file for writing." << endl;
			return;
		}
		Node* temp = head;
		while (temp) {
			fout << temp->Username << " " << temp->Password << " " << temp->TotalScore << " " << temp->WinPercentage << endl;
			temp = temp->next;
		}
		fout.close();
	}

	void loadPlayersFromFile() {
		ifstream fin("Players.txt");
		if (!fin) {
			cout << "No saved players found.\n";
			return;
		}
		string username, password;
		int totalscore;
		float winpercentage;
		while (fin >> username >> password >> totalscore >> winpercentage) {
			Node* newNode = new Node;
			newNode->Username = username;
			newNode->Password = password;
			newNode->TotalScore = totalscore;
			newNode->WinPercentage = winpercentage;
			newNode->next = nullptr;
			if (head == nullptr) head = newNode;
			else {
				Node* curr = head;
				while (curr->next) curr = curr->next;
				curr->next = newNode;
			}
		}
		fin.close();
	}

	int getScore() const { return head ? head->CurrentScore : 0; }
	int getRewardCount() const { return head ? head->RewardCounter : 0; }
	int getPowerUpCount() {
		if (head && head->PowerUpCount > 0) {
			head->PowerUpCount--;
			return 1;
		}
		return 0;
	}

	void updateScore(int tilesCaptured) {
		if (!head) return;
		cout << "Reward counter: " << head->RewardCounter << endl;
		if (head->RewardCounter > 5) {
			if (tilesCaptured >= 5) {
				head->CurrentScore += tilesCaptured * 4;
				head->RewardCounter++;
			}
		}
		else if (head->RewardCounter > 3) {
			if (tilesCaptured >= 5) {
				head->CurrentScore += tilesCaptured * 2;
				head->RewardCounter++;
			}
		}
		else if (tilesCaptured >= 10) {
			head->CurrentScore += tilesCaptured * 2;
			head->RewardCounter++;
		}
		else {
			head->CurrentScore += tilesCaptured;
		}
		if (head->CurrentScore > 50) {
			head->PowerUpCount++;
			cout << "Press SPACE to use power-up.\n";
		}
	}

	void displayScore() const {
		if (head)
			cout << "Current Score: " << head->CurrentScore << endl;
	}

	void updateTotalScore() {
		if (!head) return; // No active player

		ifstream inputFile("Players.txt");
		if (!inputFile) {
			cout << "Error opening file for reading!" << endl;
			return;
		}

		stringstream updatedContent;
		string line;
		string uname = this->head->Username;

		while (getline(inputFile, line)) {
			stringstream ss(line);
			string fileUsername, filePassword;
			int fileTotalScore;
			float fileWinPercentage;

			ss >> fileUsername >> filePassword >> fileTotalScore >> fileWinPercentage;

			if (fileUsername == uname) {
				fileTotalScore += this->head->CurrentScore;
				this->head->TotalScore = fileTotalScore;
			}

			updatedContent << fileUsername << " " << filePassword << " "
				<< fileTotalScore << " " << fileWinPercentage << endl;
		}

		inputFile.close();

		ofstream outputFile("Players.txt");
		if (!outputFile) {
			cout << "Error opening file for writing!" << endl;
			return;
		}

		outputFile << updatedContent.str();
		outputFile.close();

	}

};
