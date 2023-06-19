#include<iostream>
#include<cmath>
#include<vector>
#include<sstream>
#include<string>
#include<set>
#include<math.h>
#include <algorithm>

using namespace std;
int globalcounter = 0;

void zamena(vector<vector<char>>& promvector, vector<vector<char>>& vectorresult, int stroka, int promi, int promj, int n) {

	for (int j = 0; j < n; j++) {
		if (j == promj) {
			vectorresult[stroka][j] = '*';
		}
		else vectorresult[stroka][j] = promvector[promi][j];
	}

}

void delstrings(set<int>& set, vector<vector<char>>& promvector, vector<vector<char>>& revect, int n) {

	int i = 0;
	for (auto it = set.begin(); it != set.end(); ++it) {
		int elem = *it;
		if (i != elem) {
			revect.push_back(promvector[i]);
			i = elem + 1;
		}
		else i++;
	}
	promvector.clear();
	promvector = revect;
	revect.clear();
}



void deleteDuplicates(vector<vector<char>>& promvector, int n) {
	for (int i = 0; i < promvector.size(); i++) {

		for (int k = i + 1; k < promvector.size(); k++) {
			int deleteCounter = 0;
			for (int j = 0; j < n; j++) {
				if (promvector[i][j] == promvector[k][j]) deleteCounter++;
				if (deleteCounter == n) promvector.erase(promvector.begin() + k);
			}
		}
	}
}

void merg(vector<vector<char>>& constituent, vector<int>& vec, vector<vector<char>>& implicants, int n) {
	int recursecounter = 0;
	vector<vector<char>>promvector = constituent;
	set<int>set;
	vector<vector<char>>vectorresult;
	vectorresult.resize(vec.size() * 3, vector<char>(n));
	int stroka = 0;

	for (int i = 0; i < constituent.size(); i++) {
		for (int k = i + 1; k < constituent.size(); k++) {

			int counter1 = 0;
			int promi = 0;
			int promj = 0;
			int cx = 0;
			if (globalcounter == 0) {
				for (int j = 0; j < n; j++) {
					if (promvector[i][j] != promvector[k][j]) // ПРОХОД 1 РАЗ
					{
						counter1++;
						promi = i;
						promj = j;
					}
				}
			}
			else
				for (int d = 0; d < n; d++) {
					if (promvector[k][d] == '*') {
						cx = d;
						break;
					}
				}
			if (promvector[i][cx] == '*' && promvector[i][cx] == promvector[k][cx]) { //ПРОХОД 2 раз
				int a = 0;
				while (a < n) {
					if (promvector[i][a] != promvector[k][a])
					{
						counter1++;
						promi = i;
						promj = a;
					}
					a++;
				}
			}
			if (counter1 == 1) {
				set.insert(i);
				set.insert(k);
				recursecounter++;
				zamena(promvector, vectorresult, stroka, promi, promj, n);
				stroka++;
			}
		}
	}

	vector<vector<char>>revect;
	if (recursecounter != 0) {
		delstrings(set, promvector, revect, n);
		int j = 0;
		for (int i = 0; i < vectorresult.size(); i++) {
			if (vectorresult[i][j] != '\0')promvector.push_back(vectorresult[i]);
		}

		int frequence = 3;
		for (int i = 0; i < frequence; i++) {
			deleteDuplicates(promvector, n);
		}

		cout << "Результат склеивания: " << endl;

		for (int i = 0; i < promvector.size(); i++) {
			for (int j = 0; j < n; j++) {
				cout << promvector[i][j];
			}
			cout << '\n';
		}
		cout << endl;
	}
	if (recursecounter != 0) {
		globalcounter++;
		merg(promvector, vec, implicants, n);
	}
	else implicants = promvector;

}


void makeImplicance(vector<vector<char>>& implicants, vector<vector<char>>& constituent, vector<vector<char>>& implicantmatrix, int n, vector<int>& plusesinString) {

	plusesinString.resize(implicants.size());
	for (int i = 0; i < implicants.size(); i++) {
		for (int k = 0; k < constituent.size(); k++) {
			int counter = 0;
			for (int j = 0; j < n; j++) {
				if (implicants[i][j] != constituent[k][j]) counter++;
				if (implicants[i][j] == '*') counter--;;
			}
			if (counter > 0) implicantmatrix[i][k] = '-';
			else {
				implicantmatrix[i][k] = '+';
				plusesinString[i] += 1;
			}
		}
	}
}

void outputMatrix(vector<vector<char>>& implicants, vector<vector<char>>& constituent, vector<vector<char>>& implicantmatrix, int n) {
	cout << "       ";
	for (int i = 0; i < constituent.size(); i++) {
		for (int j = 0; j < n; j++) {
			cout << constituent[i][j];
		}
		cout << "   ";
	}
	cout << endl;
	for (int i = 0; i < implicants.size(); i++) {
		int mergedvectorCounter = 0;
		int k = 0;
		while (mergedvectorCounter < n) {
			cout << implicants[i][mergedvectorCounter];
			mergedvectorCounter++;
		}
		cout << "    ";
		while (k < constituent.size()) {
			cout << implicantmatrix[i][k];
			cout << "      ";
			k++;
		}
		cout << endl;
	}
	cout << endl;
}

void makeKNF(vector<vector<char>>& constituents, vector<vector<char>>& implicantmatrix, vector<char>& KNF, int n) {

	for (int j = 0; j < constituents.size(); j++) {
		int plusCounter = 0;
		int openBracket = 0;
		for (int k = 0; k < implicantmatrix.size(); k++) {
			if (implicantmatrix[k][j] == '+')
			{
				auto iter = KNF.end();
				if (plusCounter > 0) {

					if (openBracket == 0) {
						KNF.insert(iter - 1, '(');
						openBracket++;
					}
					KNF.push_back('+');
				}
				KNF.push_back('a' + k);
				plusCounter++;
			}
		}
		if (plusCounter > 1) KNF.push_back(')');
		if (j < constituents.size() - 1)KNF.push_back('*');
	}
	if (KNF[KNF.size() - 1] == '*') KNF.pop_back();
	cout << endl;
}

void outDNF(vector<vector<char>>& implicants, vector<vector<char>>& implicantmatrix, int n, vector<int>&strNumbers) {
	vector<char> resul;
	for (int i = 0; i < strNumbers.size(); i++) {
		for (int j = 0; j < n; j++) {
			if (implicants[strNumbers[i]][j] == '0') {
				resul.push_back('^');
				resul.push_back('X');
				resul.push_back(j + '1');
			}
			if (implicants[strNumbers[i]][j] == '1') {
				resul.push_back('X');
				resul.push_back(j + '1');
			}
		}
		if (i < strNumbers.size() - 1)resul.push_back('+');
	}
	for (int i = 0; i < resul.size(); i++) {
		cout << resul[i];
	}
	cout << endl;
 }

void sorting(string& resProm) {
	sort(resProm.begin(), resProm.end());
	for (int i = 0; i < resProm.length() - 1; i++) {
		if (resProm[i] == resProm[i + 1]) resProm.erase(resProm.begin() + i + 1);
	}
}

void multiply(vector<string>& strif) { /// ПЕРЕМНОЖЕНИЕ  a+bc и d+ce
	for (int i = 0; i < strif.size(); i++) {
		for (int k = i + 1; k < strif.size(); k++) {
			if (strif[i] != "9" && strif[k] != "9") {
				string out;
				string one = strif[i];
				string two = strif[k];
				set<string>sett;
				char prom;
				for (int onei = 0; onei < one.size(); onei++) { // умножаю a+bc и d+ce
					string resProm;
					string oneProm;
					while (one[onei] != '+' && one[onei] != one[one.length()]) {
						oneProm.push_back(one[onei]);
						onei++;
					}
					for (int twoi = 0; twoi < two.size(); twoi++) {
						string twoProm;
						while (two[twoi] != '+' && two[twoi] != two[two.length()]) {
							twoProm.push_back(two[twoi]);
							twoi++;
						}
						if (oneProm != twoProm) {
							resProm = oneProm + twoProm; // ad или bce
							sorting(resProm);
							sett.insert(resProm);
						}
					}
				}
				for (auto it : sett) {
					out += it;
					out.push_back('+');
				}
				out.pop_back();
				strif[i] = out;
				strif[k] = "9";
			}
			i++;
		}
	}
	strif.erase(remove_if(strif.begin(), strif.end(), [](const string& str) {return str == "9"; }), strif.end());
	if (strif.size() != 1) multiply(strif);
}

int isCorrect(string& one, string& two, char& prom) {
	int counter = 0;
	for (int i = 0; i < one.length(); i++) {
		for (int j = 0; j < two.length(); j++) {
			if (one[i] == two[j] && one[i] != '+') {
				counter++;
				prom = one[i];
			}
		}
	}
	if (counter == 1) return true;
	else return false;
}

void reducing(vector<string>& strif, bool& flag) {
	for (int i = 0; i < strif.size(); i++) {
		for (int k = i + 1; k < strif.size(); k++) {
			string one = strif[i];
			string two = strif[k];
			if (one == "9" || two == "9") continue;

			if (one == two) {
				strif[k] = '9';
				continue;
			}

			if (one.length() < two.length() && two.length() == 3) {
				int counter = 0;
				for (int c = 0; c < two.length(); c++)
				{
					if (two[c] == one[0]) counter++;
				}
				if (counter == 1) strif[k] = '9';
			}

			if (one.length() == two.length()) {  //случай a+b и a+c
				string str;
				char prom;
				if (isCorrect(one, two, prom))
				{
					str.push_back(prom);
					str.push_back('+');
					for (int iter = 0; iter < one.length(); iter++) {
						if (one[iter] != prom && one[iter] != '+') str.push_back(one[iter]);
						if (two[iter] != prom && two[iter] != '+') str.push_back(two[iter]);
					}
					strif[i] = str;
					i++;
					strif[k] = "9";

				}
			}
		}
	}

	strif.erase(remove_if(strif.begin(), strif.end(), [](const string& str) {return str == "9"; }), strif.end());
	int spcou = 0;
	for (int i = 0; i < strif.size(); i++) {
		string st = strif[i];
		if (st.length() > 1) spcou++;
	}
	if (spcou >= 1)multiply(strif); // рекурсиво вызываю фунцию, если длина массива не равна 1 (в 1 ячейке: ab+bcd...)
	if (spcou == 0) flag = false;

}

void rowtoColumn(vector<string>& strif, vector<string>& vec) {

	string str = strif[0];
	for (int i = 0; i < str.length(); i++) {
		string prom;
		if (str[i] != '+') {
			while (str[i] != '+' && i != str.length()) {
				prom.push_back(str[i]);
				i++;
			}
			vec.push_back(prom);
		}
	}
}

int findMinimal(vector<string>& strif, vector<string>& vec) {
	rowtoColumn(strif, vec);
	int min = INT_MAX;
	for (int i = 0; i < vec.size(); i++)
	{
		string str = vec[i];
		if (str.length() < min) min = str.length();
	}
	return min;
}

void findMDNF(vector<string>& strif, vector<string>& MDNF) {
	vector<string>vec;
	int min = findMinimal(strif, vec);
	for (int i = 0; i < vec.size(); i++) {
		string str = vec[i];
		if (str.length() == min) MDNF.push_back(vec[i]);
	}

}


void simplExplression(vector<char>& resul, int size, string& answer) {  // для корректировки вектора с уравнением
	vector<string>strif;
	strif.resize(resul.size());
	int iCounter = 0;

	for (int i = 0; i < resul.size(); i++) {
		if (resul[i] == '(') {
			int promi = i + 1;
			string str;
			while (resul[promi] != ')') {
				str.push_back(resul[promi]);
				promi++;
			}
			strif[iCounter] = str;
			iCounter++;
			i = promi;
			continue;
		}

		if (resul[i] == '*') continue;
		else {
			string str;
			str.push_back(resul[i]);
			strif[iCounter] = str;
			iCounter++;
			continue;
		}
	}
	strif.erase(remove_if(strif.begin(), strif.end(), [](const string& str) {return str.empty(); }), strif.end()); //удаление пустых строк

	for (int i = 0; i < strif.size(); i++) {
		for (int k = i + 1; k < strif.size(); k++) {
			string one = strif[i];
			string two = strif[k];
			if (one.length() > two.length() && !two.empty())swap(strif[i], strif[k]);   // сортировка по возрастанию переменных
		}
	}
	vector<string> MDNF;
	vector<string>output;
	bool flag = true;
	
	reducing(strif, flag);
	if (flag) {
		findMDNF(strif, MDNF);
		output = MDNF;
		cout << strif <<" = ";
		//cout << " = ";
		for (int i = 0; i < output.size(); i++) {
			string prom = output[i];
			answer.append(prom);
			if (i != output.size() - 1) answer.append("+");
		}
	}
	else {
		for (int i = 0; i < strif.size(); i++) {
			answer.append(strif[i]);
		}
	}
}

int main() {
	setlocale(LC_ALL, "rus");
	char ch;
	int num = 0;
	cout << "Введите номера единичных наборов" << endl;
	vector<int>vec;

	while (true) {
		ch = cin.peek();
		if (ch == '\n') break;
		if (ch == ' ') {
			cin.ignore();
			continue;
		}
		cin >> num;
		vec.push_back(num);
		continue;
	}
	int max = *max_element(vec.begin(), vec.end());
	int n;

	for (int i = 0; i < 100; i++) {
		if (max < pow(2, i)) {
			n = i;
			break;
		}
		else continue;
	}

	vector<vector<char>>vect;
	vect.resize(pow(2, n), vector<char>(n));

	vector<vector<char>>constituent;
	constituent.resize(vec.size(), vector<char>(n));

	for (int i = 0; i < vec.size(); i++) {
		int prom = vec[i];
		for (int j = n - 1; j >= 0; j--) {
			if (prom == 0) {
				constituent[i][j] = '0';
				continue;
			}
			if (prom % 2 == 0)
			{
				constituent[i][j] = '0';
				prom /= 2;
				continue;
			}
			else {
				constituent[i][j] = '1';
				prom /= 2;
			}
		}
	}
	vector<vector<char>>implicants;
	cout << endl;
	merg(constituent, vec, implicants, n);

	vector<vector<char>>implicantmatrix;
	implicantmatrix.resize(implicants.size(), vector<char>(constituent.size()));

	vector<int>plusesinString;
	makeImplicance(implicants, constituent, implicantmatrix, n, plusesinString);
	
	vector<char>KNF;
	outputMatrix(implicants, constituent, implicantmatrix, n); 
	makeKNF(constituent, implicantmatrix, KNF, n);
	
	int size = implicants.size();
	string answer;
	simplExplression(KNF, size, answer);


	//ВЫВОД 
	cout << "K = ";
	for (int i = 0; i < KNF.size(); i++) {
		cout << KNF[i];
	}
	cout << " = " << answer << endl << endl;

	
	vector<int> strNumbers;
	for (int i = 0; i < answer.length(); i++) {
		if ('a' < answer[i] < 'z') {
			while (answer[i] != '+' && i != answer.length()) {
				strNumbers.push_back(answer[i] - 'a');
				i++;
			}
			cout << "Fmin: ";
			outDNF(implicants, implicantmatrix, n, strNumbers);
			strNumbers.clear();
		}
	}

}

