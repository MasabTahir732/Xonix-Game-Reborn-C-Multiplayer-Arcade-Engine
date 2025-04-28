#pragma once
#include<iostream>

using namespace std;

class Player {
private:
	string Username;
	string Password;
	int PlayerScore;
    float WinPercentage;
public:
	Player() {
		Username = "";
		Password = "";
		PlayerScore = 0; WinPercentage = 0;
	}
	void setUsername(string& temp) { Username = temp; }
	string getUsername()const { return Username; }
	void setPassword(string& temp) { Password = temp; }
	string getPassword()const { return Password; }
	void createAccount(string temp1,string temp2){
		this->setUsername(temp1); 
		this->setPassword(temp2);
	}
	bool check(string temp1,string temp2,Player*& PlayerList,int currentPlayers ){
		for (int i = 0; i < currentPlayers; ++i) {
			if (temp1 == PlayerList[i].getUsername()) {
				if (temp2 == PlayerList[i].getPassword()) {
					cout << PlayerList[i].getUsername() << endl;
					cout << PlayerList[i].getPassword() << endl;
					return true;
				}
			}
		}
		cout << "Account Doesn't exits. Signup first\n";
		return false;
	}

};