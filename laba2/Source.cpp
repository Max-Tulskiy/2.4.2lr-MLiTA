#include<iostream>
#include<cmath>
#include<vector>
#include<sstream>
#include<string>
#include<set>
#include<math.h>
#include <algorithm>

using namespace std;

int global_counter = 0;

void replacement(vector<vector<char>>& prom_vector, 
					vector<vector<char>>& vector_result, 
					int line, 
					int prom_i,
					int prom_j,
					int n) {

	for (int j = 0; j < n; j++) {
		if (j == prom_j) {
			vector_result[line][j] = '*';
		}
		else vector_result[line][j] = prom_vector[prom_i][j];
	}

}

void deleteStrings(set<int>& set,
					vector<vector<char>>& promvector,
					vector<vector<char>>& revect, 
					int n) {

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

void mergeConstituents(vector<vector<char>>& constituent, 
						vector<int>& vec,
						vector<vector<char>>& implicants, 
						int n) {
	int line = 0;
	int counter_of_recurse = 0;
	vector<vector<char>>prom_vector = constituent;
	vector<vector<char>>vector_result;
	set<int>set;

	vector_result.resize(vec.size() * 3, vector<char>(n));
	

	for (int i = 0; i < constituent.size(); i++) {
		for (int k = i + 1; k < constituent.size(); k++) {

			int counter = 0;
			int prom_i = 0;
			int prom_j = 0;
			int column_counter = 0;

			if (global_counter == 0) {
				for (int j = 0; j < n; j++) {
					if (prom_vector[i][j] != prom_vector[k][j])
					{
						counter++;
						prom_i = i;
						prom_j = j;
					}
				}
			}
			else
				for (int d = 0; d < n; d++) {
					if (prom_vector[k][d] == '*') {
						column_counter = d;
						break;
					}
				}

			if (prom_vector[i][column_counter] == '*' && prom_vector[i][column_counter] == prom_vector[k][column_counter]) {
				int a = 0;
				while (a < n) {
					if (prom_vector[i][a] != prom_vector[k][a])
					{
						counter++;
						prom_i = i;
						prom_j = a;
					}
					a++;
				}
			}

			if (counter == 1) {
				set.insert(i);
				set.insert(k);
				counter_of_recurse++;
				replacement(prom_vector, vector_result, line, prom_i, prom_j, n);
				line++;
			}
		}
	}

	vector<vector<char>>revect;

	if (counter_of_recurse != 0) {
		deleteStrings(set, prom_vector, revect, n);
		int j = 0;
		for (int i = 0; i < vector_result.size(); i++) {
			if (vector_result[i][j] != '\0')prom_vector.push_back(vector_result[i]);
		}

		int frequence = 3;
		for (int i = 0; i < frequence; i++) {
			deleteDuplicates(prom_vector, n);
		}

		cout << "Результат 'склеивания' "<< global_counter + 1<< " проход: " << endl;

		for (int i = 0; i < prom_vector.size(); i++) {
			for (int j = 0; j < n; j++) {
				cout << prom_vector[i][j];
			}
			cout << '\n';
		}
		cout << endl;
	}

	if (counter_of_recurse != 0) {
		global_counter++;
		mergeConstituents(prom_vector, vec, implicants, n);
	}
	else {
		implicants = prom_vector;
	}

}

void makeImplicance(vector<vector<char>>& implicants,
					vector<vector<char>>& constituent, 
					vector<vector<char>>& implicantmatrix, 
					int n, 
					vector<int>& pluses_in_string) {

	pluses_in_string.resize(implicants.size());
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
				pluses_in_string[i] += 1;
			}
		}
	}
}

void outputMatrix(vector<vector<char>>& implicants, 
				  vector<vector<char>>& constituent, 
				  vector<vector<char>>& implicant_matrix, 
				  int n) {

	cout << "			Импликантная матрица			" << endl << endl;
	cout << "      ";
	for (int i = 0; i < constituent.size(); i++) {
		for (int j = 0; j < n; j++) {
			cout << constituent[i][j];
		}
		cout << "   ";
	}
	cout << endl;
	for (int i = 0; i < implicants.size(); i++) {
		int merged_vector_counter = 0;
		int k = 0;
		while (merged_vector_counter < n) {
			cout << implicants[i][merged_vector_counter];
			merged_vector_counter++;
		}
		cout << "    ";
		while (k < constituent.size()) {
			cout << implicant_matrix[i][k];
			cout << "      ";
			k++;
		}
		cout << endl;
	}
	cout << endl;
}


void makeKNF(vector<vector<char>>& constituents, 
			 vector<vector<char>>& implicantmatrix, 
			 vector<char>& KNF, 
			 int n) {

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

void outDNF(vector<vector<char>>& implicants,
			vector<vector<char>>& implicantmatrix,
			int n,
			vector<int>&strNumbers) {

	vector<char> resul;
	for (int i = 0; i < strNumbers.size(); i++) {
		for (int j = 0; j < n; j++) {
			if (implicants[strNumbers[i]][j] == '0') {
				resul.push_back('!');
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

void sorting(string& prom_result) {
	
	sort(prom_result.begin(), prom_result.end());

	for (int i = 0; i < prom_result.length() - 1; i++) {
		if (prom_result[i] == prom_result[i + 1]) {
			prom_result.erase(prom_result.begin() + i + 1);
		}
	}
}

// ПЕРЕМНОЖЕНИЕ  импликант
void multiply(vector<string>& terms) { 
	for (int i = 0; i < terms.size(); i++) {
		for (int k = i + 1; k < terms.size(); k++) {
			if (terms[i] != "9" && terms[k] != "9") {
				string out;
				string one = terms[i];
				string two = terms[k];
				set<string>sett;
				char prom;
				for (int one_i = 0; one_i < one.size(); one_i++) { 
					string res_prom;
					string one_prom;
					while (one[one_i] != '+' && one[one_i] != one[one.length()]) {
						one_prom.push_back(one[one_i]);
						one_i++;
					}
					for (int two_i = 0; two_i < two.size(); two_i++) {
						string two_prom;
						while (two[two_i] != '+' && two[two_i] != two[two.length()]) {
							two_prom.push_back(two[two_i]);
							two_i++;
						}
						if (one_prom != two_prom) {
							res_prom = one_prom + two_prom; 
							sorting(res_prom);
							sett.insert(res_prom);
						}
					}
				}
				for (auto it : sett) {
					out += it;
					out.push_back('+');
				}
				out.pop_back();
				terms[i] = out;
				terms[k] = "9";
			}
			i++;
		}
	}
	terms.erase(remove_if(terms.begin(), 
							terms.end(),
							[](const string& str) {
								return str == "9"; 
							}),
							terms.end());
	if (terms.size() != 1) multiply(terms);
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

			if (one.length() == two.length()) { 
				
				string str;
				char prom;

				if (isCorrect(one, two, prom))
				{
					str.push_back(prom);
					str.push_back('+');

					for (int iter = 0; iter < one.length(); iter++) {
						if (one[iter] != prom && one[iter] != '+') {
							str.push_back(one[iter]);
						}
						if (two[iter] != prom && two[iter] != '+') {
							str.push_back(two[iter]);
						}
					}
					strif[i] = str;
					i++;
					strif[k] = "9";

				}
			}
		}
	}

	strif.erase(remove_if(strif.begin(), 
						  strif.end(),
						  [](const string& str) {
							return str == "9";
						  } ),
						  strif.end());

	int counter = 0;
	for (int i = 0; i < strif.size(); i++) {
		string stroka = strif[i];
		if (stroka.length() > 1) counter++;
	}
	if (counter >= 1) {
		multiply(strif); // рекурсиво вызываю фунцию, если длина массива не равна 1 (в 1 ячейке: ab+bcd...)
	}

	if (counter == 0) {
		flag = false;
	}

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

void simplExplression(vector<char>& resul, 
						int size, 
						string& answer) {  // для корректировки вектора с уравнением
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
	strif.erase(remove_if(strif.begin(), 
							strif.end(),
							[](const string& str) 
							{return str.empty(); } ),
							strif.end());

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
	cout << "Введите номера единичных наборов (через пробел)" << endl;
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
	mergeConstituents(constituent, vec, implicants, n);

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


	cout << "КНФ = ";
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

