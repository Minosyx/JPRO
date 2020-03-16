#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

constexpr auto ROW = 15;
constexpr auto COLUMN = 15;
constexpr auto FILELOC = "../text.txt";

using namespace std;

struct parameters {
	int generation;
	int temp;
	/*int food_count;*/
};

typedef struct parameters param;

void memFree(param** tab) {
	for (int j = 0; j < ROW; j++) {
		delete[] tab[j];
	}
	delete[] tab;
}

void tempShot(param** tab, int i, int j) {
	//zmiana temperatury przez użytkownika
}

void arrMove(param** tab, param** tabNext) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			tab[i][j].generation = tabNext[i][j].generation;
		}
	}
}

int countNeighbors(param** tab, int i, int j) {
	int neighbors = 0;
	if (tab[(ROW + (i - 1)) % ROW][j].generation == 1 || tab[(ROW + (i - 1)) % ROW][j].generation == 2) neighbors++;				//góra
	if (tab[(i + 1) % ROW][j].generation == 1 || tab[(i + 1) % ROW][j].generation == 2) neighbors++;								//dół
	if (tab[i][(COLUMN + (j - 1)) % COLUMN].generation == 1 || tab[i][(COLUMN + (j - 1)) % COLUMN].generation == 2) neighbors++;	//lewo
	if (tab[i][(j + 1) % COLUMN].generation == 1 || tab[i][(j + 1) % COLUMN].generation == 2) neighbors++;							//prawo
	if (tab[(ROW + (i - 1)) % ROW][(COLUMN + (j - 1)) % COLUMN].generation == 1 || tab[(ROW + (i - 1)) % ROW][(COLUMN + (j - 1)) % COLUMN].generation == 2) neighbors++;	//góra-lewo
	if (tab[(ROW + (i - 1)) % ROW][(j + 1) % COLUMN].generation == 1 || tab[(ROW + (i - 1)) % ROW][(j + 1) % COLUMN].generation == 2) neighbors++;							//góra-prawo
	if (tab[(i + 1) % ROW][(COLUMN + (j - 1)) % COLUMN].generation == 1 || tab[(i + 1) % ROW][(COLUMN + (j - 1)) % COLUMN].generation == 2) neighbors++;					//dół-lewo
	if (tab[(i + 1) % ROW][(j + 1) % COLUMN].generation == 1 || tab[(i + 1) % ROW][(j + 1) % COLUMN].generation == 2) neighbors++;											//dół-prawo

	return neighbors;
}

void nextGen(param** tab, param** tabNext) {
	int neighbors;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			neighbors = countNeighbors(tab, i, j);
			
			if (tab[i][j].generation < 2 && neighbors == 3) tabNext[i][j].generation = tab[i][j].generation + 1;
			else if (tab[i][j].generation > 0 && neighbors != 3) tabNext[i][j].generation = tab[i][j].generation - 1;
			else if (tab[i][j].generation == 2 && neighbors == 3) {
				tabNext[(ROW + (i - 1)) % ROW][j].generation = 1;
				tabNext[(i + 1) % ROW][j].generation = 1;
				tabNext[i][(COLUMN + (j - 1)) % COLUMN].generation = 1;
				tabNext[i][(j + 1) % COLUMN].generation = 1;
				tabNext[(ROW + (i - 1)) % ROW][(COLUMN + (j - 1)) % COLUMN].generation = 1;
				tabNext[(ROW + (i - 1)) % ROW][(j + 1) % COLUMN].generation = 1;
				tabNext[(i + 1) % ROW][(COLUMN + (j - 1)) % COLUMN].generation = 1;
				tabNext[(i + 1) % ROW][(j + 1) % COLUMN].generation = 1;
			}
			else tabNext[i][j].generation = tab[i][j].generation;
			tabNext[i][j].temp = tab[i][j].temp;

			/*if (neighbors != 0) {
				cout << neighbors << endl;
				cout << "i = " << i << " j = " << j << endl;
			}*/
		}
	}
}

//string iterate(int number) {
//	string hline = "";
//	stringstream ss;
//	for (int i = 0; i < number; i++) {
//		ss << i;
//	}
//	hline += ss.str()+'\n';
//	return hline;
//}

void printArr(param** tab, int* gennum) {
	cout << "Generacja: " << *gennum << endl;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			/*if (i == 0 && j == 0) cout << iterate(COLUMN);*/
			if (tab[i][j].generation == 0) cout << char(' ');
			else if (tab[i][j].generation == 1) cout << char(176);
			else if (tab[i][j].generation == 2) cout << char(178);
			if (j == COLUMN - 1) cout << endl;
		}
	}
	*gennum += 1;
}

param** createArray() {
	param** tab;
	tab = new param* [ROW];
	for (int i = 0; i < ROW; i++) {
		tab[i] = new param[COLUMN];
	}
	return tab;
}

void readFile(string filename, param** tab) {
	char c;
	int ic; // zmienna pomocnicza do zamiany char na int
	ifstream file(filename);
	if (file.is_open()) {
		int i = 0, j = 0;
		while (file >> c) {
			ic = c - '0';
			tab[i][j].generation = ic;
			tab[i][j].temp = 26;
			if (j == COLUMN - 1) {
				i++;
				j = 0;
			}
			else j++;
		}
	}
	file.close();
}

void centerString(const char* s, int console_width)
{
	int l = strlen(s);
	int pos = (int)((console_width - l) / 2);
	for (int i = 0; i < pos; i++)
		cout << " ";
	cout << s;
	cout << endl;
}

void printMenu(int console_width) {
	centerString("1. Wybierz uzytkownika", console_width);
	centerString("2. O programie", console_width);
	centerString("3. Wyjdz", console_width);
}

int main()
{
	string filename = FILELOC;
	param** tab;
	param** tabNext;
	char exit = ' ';
	int gennum = 0;

	tab = createArray();
	tabNext = createArray();

	readFile(filename, tab);
	printArr(tab, &gennum);

	while (1) {
		cin >> exit;
		if (exit == 'x') break;
		system("CLS");
		nextGen(tab, tabNext);
		printArr(tabNext, &gennum);
		arrMove(tab, tabNext);
	}

	memFree(tab);
	memFree(tabNext);


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

