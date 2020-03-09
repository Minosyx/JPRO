#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

constexpr auto ROW = 15;
constexpr auto COLUMN = 15;

using namespace std;

void memFree(char** tab) {
	for (int j = 0; j < ROW; j++) {
		delete[] tab[j];
	}
	delete[] tab;
}

void printArr(char** tab) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			cout << tab[i][j];
			if (j == COLUMN - 1) cout << endl;
		}
	}
}

char** createArray() {
	char** tab;
	tab = new char* [ROW];
	for (int i = 0; i < ROW; i++) {
		tab[i] = new char[COLUMN];
	}
	/*cout << tab << endl;*/
	return tab;
}

void readFile(string filename, char** tab) {
	char c;
	/*cout << "test " << tab;*/
	ifstream file(filename);
	if (file.is_open()) {
		int i = 0, j = 0;
		while (file >> c) {
			tab[i][j] = c;
			if (j == COLUMN - 1) {
				i++;
				j = 0;
			}
			else j++;
		}
	}
	file.close();
}

void centerstring(const char* s, int console_width)
{
	int l = strlen(s);
	int pos = (int)((console_width - l) / 2);
	for (int i = 0; i < pos; i++)
		cout << " ";
	cout << s;
	cout << endl;
}

void printMenu(int console_width) {
	centerstring("1. Wybierz uzytkownika", console_width);
	centerstring("2. O programie", console_width);
	centerstring("3. Wyjdz", console_width);
}

int main()
{
	string filename = "D:/JPRO/text.txt";
	char** tab;
	char** tabNext;

	tab = createArray();
	tabNext = createArray();

	readFile(filename, tab);
	printArr(tab);
	memFree(tab);

	/*tab[5][5] = 176;*/

	/*int choice;
	int console_width = 120;

	printMenu(console_width);
	cin >> choice;

	system("CLS");

	switch (choice) {
	case 1:	cout << "Wybrales 1 opcje" << endl;
		break;
	case 2: cout << "Wybrales 2 opcje" << endl;
		break;
	case 3: cout << "Do widzenia" << endl;
		exit(0);
	}*/
}


