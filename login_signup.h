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
	int PrefTheme;
	Node* friendsHead;
	Node() {
		next = nullptr;
		Username = "";
		Password = "";
		TotalScore = 0;
		WinPercentage = 0;
		PowerUpCount = 0;
		CurrentScore = 0;
		RewardCounter = 0;
		PrefTheme = 1;
		friendsHead = nullptr;

	}
    
	void setUsername(string& temp) { Username = temp; }
	string getUsername() const { return Username; }
	void setPassword(string& temp) { Password = temp; }
	string getPassword() const { return Password; }
	void operator=(Node* val) {
		this->Username = val->Username;
		this->Password = val->Password;
		this->TotalScore = val->TotalScore;
		this->WinPercentage = val->WinPercentage;
		this->PowerUpCount = val->PowerUpCount;
		this->CurrentScore = val->CurrentScore;
		this->RewardCounter = val->RewardCounter;
		this->PrefTheme = val->PrefTheme;
		
	}
};

class Player {
private:
	Node* head;

public:
	Player() {
		head = nullptr;
	}
	Node* returnHead() {
		return head;
	}
	Player* CopyFunc(Node* val) {
		head->operator=(val);
		return this;
	}
	Player(const Node* node) {
		if (node != nullptr) {
			head = new Node();
			*head = const_cast<Node*>(node);  
		}
		else {
			head = nullptr;
		}
	}
	Player(const Player& other) {
		if (other.head != nullptr) {
			head = new Node();
			*head = other.head; 
		}
		else {
			head = nullptr;
		}
	}
	Player& operator=(const Player& other) {
		if (this != &other) {
			delete head;
			if (other.head != nullptr) {
				head = new Node();
				*head = other.head;  
			}
			else {
				head = nullptr;
			}
		}
		return *this;
	}
	Node* findNodeByUsername(const std::string& uname) {
		Node* temp = head;  
		while (temp != nullptr) {
			if (temp->getUsername() == uname) {
				return temp;
			}
			temp = temp->next;
		}
		return nullptr;
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
	int getPref() {
		return head->PrefTheme;
	}
	void setPref(int val) {
		head->PrefTheme = val;
	}
	void savePlayersToFile() {
		ofstream fout("Players.txt");
		if (!fout) {
			cout << "Error opening file for writing." << endl;
			return;
		}
		Node* temp = head;
		while (temp) {
			fout << temp->Username << " " << temp->Password << " " << temp->TotalScore << " " << temp->WinPercentage<<" "<< temp->PrefTheme << endl;
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
		int prefVal;
		while (fin >> username >> password >> totalscore >> winpercentage >> prefVal) {
			Node* newNode = new Node;
			newNode->Username = username;
			newNode->Password = password;
			newNode->TotalScore = totalscore;
			newNode->WinPercentage = winpercentage;
			newNode->next = nullptr;
			newNode->PrefTheme = prefVal;
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

	//void saveFriendsToFile(Node* head) {
	//	ofstream fout("Friends.txt");
	//	if (!fout) {
	//		cout << "Error opening Friends.txt for writing!" << endl;
	//		return;
	//	}

	//	Node* player = head;
	//	while (player) {
	//		fout << player->Username << ": ";
	//		Node* friendNode = player->friendsHead;
	//		while (friendNode) {
	//			fout << friendNode->Username << " ";
	//			friendNode = friendNode->next;
	//		}
	//		fout << endl;
	//		player = player->next;
	//	}

	//	fout.close();
	//}
	/*void addFriend(Node* player, Node* friendNode) {
		if (!player || !friendNode) return;


		Node* temp = player->friendsHead;
		while (temp) {
			if (temp->Username == friendNode->Username) {
				cout << player->Username << " is already friends with " << friendNode->Username << endl;
				return;
			}
			temp = temp->next;
		}

		Node* newFriend = new Node();
		newFriend->Username = friendNode->Username;
		newFriend->next = player->friendsHead;
		player->friendsHead = newFriend;

		cout << player->Username << " added " << friendNode->Username << " as a friend." << endl;
	}*/

	//void loadFriendsFromFile(Node* head) {
	//	ifstream fin("Friends.txt");
	//	if (!fin) {
	//		cout << "No saved friends found." << endl;
	//		return;
	//	}

	//	string line;
	//	while (getline(fin, line)) {
	//		if (line.empty()) continue;

	//		istringstream iss(line);
	//		string username;
	//		getline(iss, username, ':');

	//		Node* player = findNodeByUsername(username);
	//		if (!player) continue;

	//		string friendUsername;
	//		while (iss >> friendUsername) {
	//			Node* friendNode = findNodeByUsername(friendUsername);
	//			if (friendNode) {
	//			
	//				addFriend(player, friendNode); 
	//			}
	//		}
	//	}

	//	fin.close();
	//}

	void displayScore(sf::RenderWindow& window, Themes* currentTheme) const {
		if (!head) return;  // no player

		sf::Font font;
		if (!font.loadFromFile(currentTheme->font)) {
			cerr << "Failed to load font in displayScore!\n";
			return;
		}

		sf::Text scoreText;
		scoreText.setFont(font);
		scoreText.setCharacterSize(20);
		scoreText.setFillColor(currentTheme->textColor);
		scoreText.setString("Score: " + to_string(head->CurrentScore));
		scoreText.setPosition(330,10); 
		sf::Text powerupText;
		powerupText.setFont(font);
		powerupText.setCharacterSize(17);
		powerupText.setFillColor(sf::Color::Red);
		powerupText.setString("PowerUp available (Press Space to Use)");
		powerupText.setPosition(250, 30);


		window.draw(scoreText);
		if (head->PowerUpCount != 0) {
			window.draw(powerupText);
		}
		
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
			int preftheme;

			ss >> fileUsername >> filePassword >> fileTotalScore >> fileWinPercentage >> preftheme;

			if (fileUsername == uname) {
				fileTotalScore += this->head->CurrentScore;
				this->head->TotalScore = fileTotalScore;
			}

			updatedContent << fileUsername << " " << filePassword << " "
				<< fileTotalScore << " " << fileWinPercentage <<" " << preftheme << endl;
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
	void updatePrefTheme(int newThemeId) {
		if (!head) return;

		ifstream inputFile("Players.txt");
		if (!inputFile) {
			cout << "Error opening file for reading!" << endl;
			return;
		}

		stringstream updatedContent;
		string line;
		string uname = this->head->Username;

		while (getline(inputFile, line)){
			if (line.empty()) {
				updatedContent << endl;
				continue;
			}

			istringstream ss(line);
			string fileUsername, filePassword;
			int fileTotalScore;
			float fileWinPercentage;
			int filePreferredTheme;

	
			if (!(ss >> fileUsername >> filePassword >> fileTotalScore >> fileWinPercentage >> filePreferredTheme)) {
	
				updatedContent << line << endl;
				continue;
			}

	
			if (fileUsername == uname) {
				filePreferredTheme = newThemeId;
				this->head->PrefTheme = newThemeId;
			}

			updatedContent << fileUsername << " " << filePassword << " "
				<< fileTotalScore << " " << fileWinPercentage << " "
				<< filePreferredTheme << endl;
		}

		inputFile.close();

	
		ofstream outputFile("Players_temp.txt");
		if (!outputFile) {
			cout << "Error opening temp file for writing!" << endl;
			return;
		}

		outputFile << updatedContent.str();
		outputFile.close();
\
		if (remove("Players.txt") != 0) {
			cout << "Error deleting original Players.txt!" << endl;
			return;
		}
		if (rename("Players_temp.txt", "Players.txt") != 0) {
			cout << "Error renaming temp file to Players.txt!" << endl;
		}
	}


};
