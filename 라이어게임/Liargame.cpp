#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class Category {
	vector<string> word;
public:
	Category(string theme);
	string get_word();
};

Category::Category(string theme) {
	ifstream fin;
	string line;

	if (theme == "무기") fin.open("무기.txt");
	else if (theme == "가수") fin.open("가수.txt");
	else if (theme == "배우") fin.open("배우.txt");
	else if (theme == "LOL챔피언") fin.open("LOL챔피언.txt");        //예외처리 try catch를 이용해 파일오픈실패 구현해보기
	
	while (getline(fin, line)) word.push_back(line);
	
	fin.close();
}

string Category::get_word() {
	int n = rand() % word.size();
	return word[n];
}

class player {
	string name;
	string identity;
	bool status;
	int vote;
public:
	player(string name, string dentity) {
		this->name = name;
		this->identity = identity;
		this->status = true;          // true면 생존, false면 아웃상태
		vote = 0;
	}
	string getname() { return name; }
	bool guess() {
		if (this->identity == "Liar") return true;
		else return false;
	}
	bool getstatus() { return status; }
	void die() { status = false; }
	void addvote() { vote++; }
	void reset() { vote = 0; }
	int getvote() { return vote; }
};

int main() {
	srand((unsigned)time(0));
	cout << "==========[ Liar Game ]==========" << endl;
	int num_players;
	cout << ">> 참가할 인원수는 몇 명입니까? : ";
	cin >> num_players;                               // 참가할 인원수 설정

	cin.ignore();                                     // 입력버퍼 비우기. 뒤에 나오는 getline을 위해
	player** players = new player * [num_players];    // 인원수에 따른 플레이어객체 동적 생성
	int liar = rand() % num_players;                  // 몇 변째 플레이어를 라이어로 할 것이냐!

	vector<string> namelist;                          // 입력받은 이름들을 저장하는 벡터를 선언
	for (int i = 0; i < num_players; i++) {           // 이 반복문 내에서 플레이어들의 이름을 입력받음
		string name;
		while (true) {
			int escape = 0;
			cout << "\n>> " << i + 1 << "번 째 플레이어의 이름 : ";
			getline(cin, name);
			for (int j = 0; j < i; j++) {
				if (name == namelist[j]) {            // 새로 입력받은 이름과 벡터 내에 저장된 이름 중 같은 것이 있다면 중복처리
					cout << ">> 이름이 중복됩니다! " << endl;
					escape = 1;
					break;
				}
			}
			if (escape == 0) break;
		}
		namelist.push_back(name);                     // 중복이 아니라면 입력받은 이름을 벡터로 푸쉬
		if (i == liar) players[i] = new player[1]{ player(name, "liar") };  // 플레이어를 라이어로
		else players[i] = new player[1]{ player(name, "Human") };           // 플레이어를 일반사람으로
	}

	string theme;
	
	while (true) {    // 이 반복문 내에서 주제를 입력받는다.
		cout << "\n==========[ 게임 주제 ]==========" << endl;
		cout << "목록 :무기 / 가수 / 배우 / LOL챔피언" << endl;
		cout << "\n>> 게임 주제는 뭘로 하시겠습니까? : ";
		cin >> theme;

		if (theme == "무기" || theme == "가수" || theme == "배우" || theme == "LOL챔피언") break;
		cout << ">> 잘못 입력하셨습니다.다시 골라주세요." << endl << endl;
	}

	Category *game = new Category(theme);            // 입력받은 주제로 카테고리 객체 동적 생성
	string word = game->get_word();
	delete game;                                     // 더 이상 필요없는 카테고리 객체 삭제

	cout << "\n=================================" << endl;
	cout << ">> '확인'을 입력하면 제시어를 확인할 수 있습니다" << endl;

	for (int i = 0; i < num_players; i++) {      // 이 반복문내에서 각 플레이어는 제시어를 확인함
		string* p = new string;
		while (true) {
			cout << ">> " << i + 1 << "번째 플레이어는 제시어를 확인해주세요 : ";
			cin >> *p;
			if (*p == "확인") break;
			else cout << ">> 다시 입력해주세요" << endl << endl;
		}
		if (i == liar) cout << ">> 당신은 라이어입니다" << endl << endl;
		else cout << ">> 제시어는 '" << word << "'입니다" << endl << endl;
		delete p;
	}

	cin.ignore();   // 입력버퍼 지우기. 뒤에 나오는 getline을 위함

	for (int round = 0; round < 3; round++) {    // 본게임 진행
		for (int i = 0; i < num_players; i++) players[i]->reset();
		cout << "=================================" << endl;
		cout << "< [" << round + 1 << "] Round >" << endl << endl;

		for (int i = 0; i < num_players; i++) {  // 이 반복문 내에선 설명 진행
			if (players[i]->getstatus() == false) continue;
			string describe;
			cout << ">> " << players[i]->getname() << "님은 제시어에 대해 설명해주세요 : ";
			getline(cin, describe);
			cout << ">> " << players[i]->getname() <<"님의 설명 : " << describe << endl << endl;
		}

		int max_index;
		while (true) {
			namelist.clear();
			for (int i = 0; i < num_players; i++) {         // 이 반복문 내에선 투표 진행
				if (players[i]->getstatus() == false) continue;
				string name;
				while (true) {
					int escape = 0;
					cout << ">> " << players[i]->getname() << "님은 라이어라 의심되는 플레이어를 투표해주세요 : ";
					getline(cin, name);
					for (int j = 0; j < num_players; j++) {
						if (name == players[j]->getname() && players[j]->getstatus() == true) {
							players[j]->addvote();
							escape = 1;
							break;
						}
						else if (name == players[j]->getname() && players[j]->getstatus() == false) {
							cout << ">> 해당 인원은 아웃된 상태입니다. 다시 골라주세요" << endl << endl;
							escape = 2;
							break;
						}
					}
					if (escape == 1) break;
					else if (escape == 2) continue;
					cout << ">> 해당 이름을 가진 인원이 존재하지 않습니다. 다시 골라주세요" << endl << endl;
				}
			}
			int max = 0;
			for (int j = 0; j < num_players; j++) {
				if (players[j]->getstatus() == false) continue;
				if (max < players[j]->getvote()) {
					max = players[j]->getvote();
					max_index = j;
				}
			}

			for (int j = 0; j < num_players; j++) {             // 투표 중복 검사 
				if (j == max_index || players[j]->getstatus() == false) continue;
				if (max == players[j]->getvote()) {
					namelist.push_back(players[j]->getname());
				}
			}

			if (namelist.size() > 0) {
				namelist.push_back(players[max_index]->getname());
				cout << ">> 투표 결과 ";
				for (int k = 0; k < namelist.size(); k++) cout << namelist[k] << ", ";
				cout << "님이 " << max << "표로 동률입니다. 투표를 다시 진행합니다." << endl << endl;
			}
			else break;
		}

		cout << "\n>> " << players[max_index]->getname() << "님이 가장 많은 투표를 받았습니다" << endl;
		if (max_index == liar) {
			cout << ">> " << players[max_index]->getname() << "님은 라이어가 맞습니다. 게임을 종료합니다" << endl;
			return 0;
		}
		else {
			string guess;
			cout << ">> " << players[max_index]->getname() << "님은 라이어가 아닙니다. 아웃되었습니다.." << endl << endl;
			players[max_index]->die();
			cout << ">> " << "라이어는 제시어가 무엇인지 추측해주세요 : ";
			getline(cin, guess);
			if (guess == word) {
				cout << ">> 정답입니다! 라이어의 승리로 게임을 종료합니다.";
				return 0;
			}
			else cout << ">> 오답입니다" << endl << endl;
		}
		if (round == 2) {
			cout << ">> 모든 라운드가 끝났습니다. 라이어의 승리입니다" << endl;
			return 0;
		}
	}
	for (int i = 0; i < num_players; i++) delete[] players[i];
	delete[]players;
}