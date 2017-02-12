#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <functional>  

using namespace std;
struct pair_{
	string left;
	string right;
};

bool operator<(pair_ a, pair_ b){
		if (a.left < b.left) return true;
		else return false;
}

int n;
vector <string> text;
vector <pair_> pairs;
set <string> words_exclusion;
multimap <int, string, greater<>> often_words;

void input(){
	ifstream file("text.txt");
	string word;

	while (!file.eof()){
		file >> word;
		text.push_back(word);
	}
	file.close();

	file.open("word-exclusion.txt");
	while (!file.eof()){
		file >> word;
		words_exclusion.insert(word);
	}
	file.close();

	file.open("n.txt");
	file >> n;
	file.close();
}

void create_often_words(){
	map <string, int> words;
	for (auto i : text)
		//���� ����� �� ����������� ������-�����������
		if(!words_exclusion.count(i)){
			//���� ��� ��� ������ �����, ��������� � �������
			if (!words.count(i)) words.insert({ i, 1 });
			else { //����� ���� ��� ����� � ������� 
				auto &elem = words.find(i);
				//� ����������� ������� ����� ���� �� �������
				elem->second++;
			} 
		}
	//���� ������� �� ����
	if (words.size()){
		//��������� ����� �������, ������������� �� ���������� ���� � �������
		//� ������� ��������, ����� ����� ���� �������� N �������� 
		//����� ������������� ����
		for (auto i : words) 
			often_words.insert({ i.second, i.first });
		//������� ������, ���������� ��� ��������� �������
		words.erase(words.begin(), words.end());
	}
}

void add_pair(map <pair_, int> &a, string left, string right){
	pair_ p;
	p.left = left;
	p.right = right;
	if (!a.count(p)) a.insert({ p, 1 });
	else {
			auto &elem = a.find(p);
			elem->second++;
	}
}

void create_pairs(){
	map <pair_, int> a;
	map <int, pair_, greater<>> b;
	int k = 0;
	//�������� �� N ����� ������������� ������ � ������
	for (auto i = begin(often_words); k < n; k++, i++) {
		auto j = begin(text);
		//�������� �� ����� ������
		while (j != end(text)){	
			//���� ��������� �� ������� ����� ������������� ����� � ������
			for (;  j != end(text) && i->second != *j ; j++);

			if (j != end(text)){ 
				//���� ��� �������� ������ ������ ������
				if (j == begin(text)) {
					//���� ������ �� ���� ����� �� ����������� ������-�����������
					if (!words_exclusion.count(*(j+1)))
						//��������� ������ ���� ���� � �������
						add_pair(a, *j, *(j + 1));
				}

				//���� ��� �������� ��������� ������ ������
				else if (j == end(text)-1) {
					if (!words_exclusion.count(*(j - 1))) 
						add_pair(a, *(j - 1), *j);
				}

				//����� �������� ���� � �����, � ������
				else {
					if (!words_exclusion.count(*(j - 1))) add_pair(a, *(j - 1), *j);
					if (!words_exclusion.count(*(j + 1))) add_pair(a, *j, *(j + 1));
				}

				j++;
			}
		}

		//���� ������� ��� ��������� ��� ������� �����
		if (a.size()){
			//������������� ��� �� �������� ���������� ���� � ����� �������
			for (auto i : a) b.insert({ i.second, i.first });
			//��������� ����� ����� ������������� ���� � ������ ����� ���
			pairs.push_back(begin(b)->second);
			//����������� ������
			a.erase(a.begin(), a.end());
			b.erase(b.begin(), b.end());
		}
	}
}

void output() {
	ofstream file("result.txt");
	for (auto i : pairs)
		file << i.left << ' ' << i.right << endl;
	file.close();
}

void main(){
	input();
	create_often_words();
	create_pairs();
	output();
}