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

	if (theme == "����") fin.open("����.txt");
	else if (theme == "����") fin.open("����.txt");
	else if (theme == "���") fin.open("���.txt");
	else if (theme == "LOLè�Ǿ�") fin.open("LOLè�Ǿ�.txt");        //����ó�� try catch�� �̿��� ���Ͽ��½��� �����غ���
	
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
		this->status = true;          // true�� ����, false�� �ƿ�����
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
	cout << ">> ������ �ο����� �� ���Դϱ�? : ";
	cin >> num_players;                               // ������ �ο��� ����

	cin.ignore();                                     // �Է¹��� ����. �ڿ� ������ getline�� ����
	player** players = new player * [num_players];    // �ο����� ���� �÷��̾ü ���� ����
	int liar = rand() % num_players;                  // �� ��° �÷��̾ ���̾�� �� ���̳�!

	vector<string> namelist;                          // �Է¹��� �̸����� �����ϴ� ���͸� ����
	for (int i = 0; i < num_players; i++) {           // �� �ݺ��� ������ �÷��̾���� �̸��� �Է¹���
		string name;
		while (true) {
			int escape = 0;
			cout << "\n>> " << i + 1 << "�� ° �÷��̾��� �̸� : ";
			getline(cin, name);
			for (int j = 0; j < i; j++) {
				if (name == namelist[j]) {            // ���� �Է¹��� �̸��� ���� ���� ����� �̸� �� ���� ���� �ִٸ� �ߺ�ó��
					cout << ">> �̸��� �ߺ��˴ϴ�! " << endl;
					escape = 1;
					break;
				}
			}
			if (escape == 0) break;
		}
		namelist.push_back(name);                     // �ߺ��� �ƴ϶�� �Է¹��� �̸��� ���ͷ� Ǫ��
		if (i == liar) players[i] = new player[1]{ player(name, "liar") };  // �÷��̾ ���̾��
		else players[i] = new player[1]{ player(name, "Human") };           // �÷��̾ �Ϲݻ������
	}

	string theme;
	
	while (true) {    // �� �ݺ��� ������ ������ �Է¹޴´�.
		cout << "\n==========[ ���� ���� ]==========" << endl;
		cout << "��� :���� / ���� / ��� / LOLè�Ǿ�" << endl;
		cout << "\n>> ���� ������ ���� �Ͻðڽ��ϱ�? : ";
		cin >> theme;

		if (theme == "����" || theme == "����" || theme == "���" || theme == "LOLè�Ǿ�") break;
		cout << ">> �߸� �Է��ϼ̽��ϴ�.�ٽ� ����ּ���." << endl << endl;
	}

	Category *game = new Category(theme);            // �Է¹��� ������ ī�װ� ��ü ���� ����
	string word = game->get_word();
	delete game;                                     // �� �̻� �ʿ���� ī�װ� ��ü ����

	cout << "\n=================================" << endl;
	cout << ">> 'Ȯ��'�� �Է��ϸ� ���þ Ȯ���� �� �ֽ��ϴ�" << endl;

	for (int i = 0; i < num_players; i++) {      // �� �ݺ��������� �� �÷��̾�� ���þ Ȯ����
		string* p = new string;
		while (true) {
			cout << ">> " << i + 1 << "��° �÷��̾�� ���þ Ȯ�����ּ��� : ";
			cin >> *p;
			if (*p == "Ȯ��") break;
			else cout << ">> �ٽ� �Է����ּ���" << endl << endl;
		}
		if (i == liar) cout << ">> ����� ���̾��Դϴ�" << endl << endl;
		else cout << ">> ���þ�� '" << word << "'�Դϴ�" << endl << endl;
		delete p;
	}

	cin.ignore();   // �Է¹��� �����. �ڿ� ������ getline�� ����

	for (int round = 0; round < 3; round++) {    // ������ ����
		for (int i = 0; i < num_players; i++) players[i]->reset();
		cout << "=================================" << endl;
		cout << "< [" << round + 1 << "] Round >" << endl << endl;

		for (int i = 0; i < num_players; i++) {  // �� �ݺ��� ������ ���� ����
			if (players[i]->getstatus() == false) continue;
			string describe;
			cout << ">> " << players[i]->getname() << "���� ���þ ���� �������ּ��� : ";
			getline(cin, describe);
			cout << ">> " << players[i]->getname() <<"���� ���� : " << describe << endl << endl;
		}

		int max_index;
		while (true) {
			namelist.clear();
			for (int i = 0; i < num_players; i++) {         // �� �ݺ��� ������ ��ǥ ����
				if (players[i]->getstatus() == false) continue;
				string name;
				while (true) {
					int escape = 0;
					cout << ">> " << players[i]->getname() << "���� ���̾�� �ǽɵǴ� �÷��̾ ��ǥ���ּ��� : ";
					getline(cin, name);
					for (int j = 0; j < num_players; j++) {
						if (name == players[j]->getname() && players[j]->getstatus() == true) {
							players[j]->addvote();
							escape = 1;
							break;
						}
						else if (name == players[j]->getname() && players[j]->getstatus() == false) {
							cout << ">> �ش� �ο��� �ƿ��� �����Դϴ�. �ٽ� ����ּ���" << endl << endl;
							escape = 2;
							break;
						}
					}
					if (escape == 1) break;
					else if (escape == 2) continue;
					cout << ">> �ش� �̸��� ���� �ο��� �������� �ʽ��ϴ�. �ٽ� ����ּ���" << endl << endl;
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

			for (int j = 0; j < num_players; j++) {             // ��ǥ �ߺ� �˻� 
				if (j == max_index || players[j]->getstatus() == false) continue;
				if (max == players[j]->getvote()) {
					namelist.push_back(players[j]->getname());
				}
			}

			if (namelist.size() > 0) {
				namelist.push_back(players[max_index]->getname());
				cout << ">> ��ǥ ��� ";
				for (int k = 0; k < namelist.size(); k++) cout << namelist[k] << ", ";
				cout << "���� " << max << "ǥ�� �����Դϴ�. ��ǥ�� �ٽ� �����մϴ�." << endl << endl;
			}
			else break;
		}

		cout << "\n>> " << players[max_index]->getname() << "���� ���� ���� ��ǥ�� �޾ҽ��ϴ�" << endl;
		if (max_index == liar) {
			cout << ">> " << players[max_index]->getname() << "���� ���̾ �½��ϴ�. ������ �����մϴ�" << endl;
			return 0;
		}
		else {
			string guess;
			cout << ">> " << players[max_index]->getname() << "���� ���̾ �ƴմϴ�. �ƿ��Ǿ����ϴ�.." << endl << endl;
			players[max_index]->die();
			cout << ">> " << "���̾�� ���þ �������� �������ּ��� : ";
			getline(cin, guess);
			if (guess == word) {
				cout << ">> �����Դϴ�! ���̾��� �¸��� ������ �����մϴ�.";
				return 0;
			}
			else cout << ">> �����Դϴ�" << endl << endl;
		}
		if (round == 2) {
			cout << ">> ��� ���尡 �������ϴ�. ���̾��� �¸��Դϴ�" << endl;
			return 0;
		}
	}
	for (int i = 0; i < num_players; i++) delete[] players[i];
	delete[]players;
}