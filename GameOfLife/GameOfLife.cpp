#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

//constexpr auto row = 15;
//constexpr auto column = 15;
//constexpr auto temp = 26;
constexpr auto FILELOC = "../text.txt";

using namespace std;

int row, column;
double temp;

struct parameters {
	int generation;
	double temp;
	/*int food_count;*/
};

typedef struct parameters param;

param** createArray() {
	param** tab;
	tab = new param * [row];
	for (int i = 0; i < row; i++) {
		tab[i] = new param[column];
	}
	return tab;
}

void arrMove(param** tab, param** tabNext) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			tab[i][j] = tabNext[i][j];
		}
	}
}

void memFree(param** tab) {
	for (int j = 0; j < row; j++) {
		delete[] tab[j];
	}
	delete[] tab;
}

void writeToFile(param** tab, int &gennum) {
	int i, j;
	int inserted;
	
	char location[] = "./generations.txt";

	FILE* fp = fopen(location, "a");

	fprintf(fp, "Generacja: %d\n", gennum - 1);
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			inserted = tab[i][j].generation;
			fprintf(fp, "%d", inserted);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	fclose(fp);
}

void gridCheck(param** tab) {
	int x, y;
	
	cout << "Podaj wspolrzedne komorki, ktorej dane chcesz sprawdzic" << endl;
	cout << "Wiersz: ";
	cin >> x;
	cout << "Kolumna: ";
	cin >> y;

	cout << "Dane komorki [" << x << "][" << y << "] to:" << endl;
	cout << "Generacja: " << tab[x][y].generation << endl;
	cout << "Temperatura: " << tab[x][y].temp << endl;
}

void evenSpread(param** tab, double ctemp, int x, int y) {
	int i, j = 1;
	double sctemp = ctemp;
	tab[x][y].temp += ctemp;
	if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
	else ctemp = ctemp - ctemp * 0.1;

	i = x - 1;
	while (i >= 0) {
		int counter = 0;
		int k = y - j;
		int len = y + j;
		if (k < 0) k = 0;
		if (len == column) len = column - 1;
		while (k <= len) {
			tab[i][k].temp += ctemp;
			if (tab[i][k].generation != 0) counter++;
			/*printf("i = %d, k = %d\n", i, k);*/
			k++;
		}
		printf("counter = %d\n", counter);
		if (counter > 0) ctemp = ctemp - ctemp * 0.3;
		else ctemp = ctemp - ctemp * 0.1;
		j++;
		i--;
	}

	i = x + 1;
	j = 1;
	ctemp = sctemp;
	while (i < row) {
		int counter = 0;
		int k = y - j;
		int len = y + j;
		if (k < 0) k = 0;
		if (len == column) len = column - 1;
		while (k <= len) {
			tab[i][k].temp += ctemp;
			if (tab[i][k].generation != 0) counter++;
			/*printf("i = %d, k = %d\n", i, k);*/
			k++;
		}
		if (counter > 0) ctemp = ctemp - ctemp * 0.3;
		else ctemp = ctemp - ctemp * 0.1;
		j++;
		i++;
	}

	i = y - 1;
	j = 0;
	ctemp = sctemp;
	while (i >= 0) {
		int counter = 0;
		int k = x - j;
		int len = x + j;
		if (k < 0) k = 0;
		if (len == row) len = row - 1;
		while (k <= len) {
			tab[k][i].temp += ctemp;
			if (tab[k][i].generation != 0) counter++;;
			k++;
		}
		if (counter > 0) ctemp = ctemp - ctemp * 0.3;
		else ctemp = ctemp - ctemp * 0.1;
		j++;
		i--;
	}

	i = y + 1;
	j = 0;
	ctemp = sctemp;
	while (i < column) {
		int counter = 0;
		int k = x - j;
		int len = x + j;
		if (k < 0) k = 0;
		if (len == row) len = row - 1;
		while (k <= len) {
			tab[k][i].temp += ctemp;
			if (tab[k][i].generation != 0) counter++;;
			k++;
		}
		if (counter > 0) ctemp = ctemp - ctemp * 0.3;
		else ctemp = ctemp - ctemp * 0.1;
		j++;
		i++;
	}
}

void directionalSpread(param** tab, double ctemp, int x, int y) { 
	int choice;
	double ctempPlus;
	double ctempMinus;
	cout << "Wybierz kierunek:" << endl;
	cout << "1. Gora\n2. Dol\n3. Lewo\n4. Prawo" << endl;
	cout << "Twoj wybor: ";
	cin >> choice;

	tab[x][y].temp += ctemp;
	if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
	else ctemp = ctemp - ctemp * 0.1;
	ctempPlus = ctemp;
	ctempMinus = ctemp;

	switch (choice) {
	case 1:
		x -= 1;
		while (x >= 0) {
			tab[x][(column + (y - 1)) % column].temp += ctempMinus;
			tab[x][y].temp += ctemp;
			tab[x][(y + 1) % column].temp += ctempPlus;
			if (tab[x][(column + (y - 1)) % column].generation != 0) ctempMinus = ctempMinus - ctempMinus * 0.3;
			else ctempMinus = ctempMinus - ctempMinus * 0.1;
			if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
			else ctemp = ctemp - ctemp * 0.1;
			if (tab[x][(y + 1) % column].generation != 0) ctempPlus = ctempPlus - ctempPlus * 0.3;
			else ctempPlus = ctempPlus - ctempPlus * 0.1;
			x -= 1;
		}
		break;
	case 2:
		x += 1;
		while (x < row) {
			tab[x][(column + (y - 1)) % column].temp += ctempMinus;
			tab[x][y].temp += ctemp;
			tab[x][(y + 1) % column].temp += ctempPlus;
			if (tab[x][(column + (y - 1)) % column].generation != 0) ctempMinus = ctempMinus - ctempMinus * 0.3;
			else ctempMinus = ctempMinus - ctempMinus * 0.1;
			if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
			else ctemp = ctemp - ctemp * 0.1;
			if (tab[x][(y + 1) % column].generation != 0) ctempPlus = ctempPlus - ctempPlus * 0.3;
			else ctempPlus = ctempPlus - ctempPlus * 0.1;
			x += 1;
		}
		break;
	case 3:
		y -= 1;
		while (y >= 0) {
			tab[(row + (x - 1)) % row][y].temp += ctempMinus;
			tab[x][y].temp += ctemp;
			tab[(x + 1) % row][y].temp += ctempPlus;
			if (tab[(row + (x - 1)) % row][y].generation != 0) ctempMinus = ctempMinus - ctempMinus * 0.3;
			else ctempMinus = ctempMinus - ctempMinus * 0.1;
			if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
			else ctemp = ctemp - ctemp * 0.1;
			if (tab[(x + 1) % row][y].generation != 0) ctempPlus = ctempPlus - ctempPlus * 0.3;
			else ctempPlus = ctempPlus - ctempPlus * 0.1;
			y -= 1;
		}
		break;
	case 4:
		y += 1;
		while (y < column) {
			tab[(row + (x - 1)) % row][y].temp += ctempMinus;
			tab[x][y].temp += ctemp;
			tab[(x + 1) % row][y].temp += ctempPlus;
			if (tab[(row + (x - 1)) % row][y].generation != 0) ctempMinus = ctempMinus - ctempMinus * 0.3;
			else ctempMinus = ctempMinus - ctempMinus * 0.1;
			if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
			else ctemp = ctemp - ctemp * 0.1;
			if (tab[(x + 1) % row][y].generation != 0) ctempPlus = ctempPlus - ctempPlus * 0.3;
			else ctempPlus = ctempPlus - ctempPlus * 0.1;
			y += 1;
		}
		break;
	}

}

void tempShot(param** tab) {
	int tchoice, x, y;
	double ctemp;
	cout << "Podaj rodzaj rozchodzenia sie temperatury (rownomierna = 1, kierunkowa = 2): ";
	cin >> tchoice;
	cout << "Podaj wspolrzedne z siatki" << endl;
	cout << "Wiersz: ";
	cin >> x;
	cout << "Kolumna: ";
	cin >> y;

	cout << "Podaj o ile zmienic temperature tej komorki: ";
	cin >> ctemp;

	switch (tchoice) {
	case 1: 
		evenSpread(tab, ctemp, x, y);
		break;
	case 2:
		directionalSpread(tab, ctemp, x, y);
		break;
	default: 
		cout << "Dokonano niepoprawnego wyboru" << endl;
	}

}

int countNeighbors(param** tab, int i, int j) {
	int neighbors = 0;
	if (tab[(row + (i - 1)) % row][j].generation == 1 || tab[(row + (i - 1)) % row][j].generation == 2) neighbors++;				//góra
	if (tab[(i + 1) % row][j].generation == 1 || tab[(i + 1) % row][j].generation == 2) neighbors++;								//dół
	if (tab[i][(column + (j - 1)) % column].generation == 1 || tab[i][(column + (j - 1)) % column].generation == 2) neighbors++;	//lewo
	if (tab[i][(j + 1) % column].generation == 1 || tab[i][(j + 1) % column].generation == 2) neighbors++;							//prawo
	if (tab[(row + (i - 1)) % row][(column + (j - 1)) % column].generation == 1 || tab[(row + (i - 1)) % row][(column + (j - 1)) % column].generation == 2) neighbors++;	//góra-lewo
	if (tab[(row + (i - 1)) % row][(j + 1) % column].generation == 1 || tab[(row + (i - 1)) % row][(j + 1) % column].generation == 2) neighbors++;							//góra-prawo
	if (tab[(i + 1) % row][(column + (j - 1)) % column].generation == 1 || tab[(i + 1) % row][(column + (j - 1)) % column].generation == 2) neighbors++;					//dół-lewo
	if (tab[(i + 1) % row][(j + 1) % column].generation == 1 || tab[(i + 1) % row][(j + 1) % column].generation == 2) neighbors++;											//dół-prawo

	return neighbors;
}

void nextGen(param** tab, param** tabNext) {
	int neighbors;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			neighbors = countNeighbors(tab, i, j);
			
			if (tab[i][j].generation < 2 && neighbors == 3) tabNext[i][j].generation = tab[i][j].generation + 1;
			else if (tab[i][j].generation > 0 && neighbors != 3) tabNext[i][j].generation = tab[i][j].generation - 1;
			else if (tab[i][j].generation == 2 && (neighbors == 3 || neighbors == 2)) {
				tabNext[(row + (i - 1)) % row][j].generation = 1;
				tabNext[(i + 1) % row][j].generation = 1;
				tabNext[i][(column + (j - 1)) % column].generation = 1;
				tabNext[i][(j + 1) % column].generation = 1;
				tabNext[(row + (i - 1)) % row][(column + (j - 1)) % column].generation = 1;
				tabNext[(row + (i - 1)) % row][(j + 1) % column].generation = 1;
				tabNext[(i + 1) % row][(column + (j - 1)) % column].generation = 1;
				tabNext[(i + 1) % row][(j + 1) % column].generation = 1;
				tabNext[i][j].generation = 0; // temporary
			}
			else tabNext[i][j].generation = tab[i][j].generation;
			tabNext[i][j].temp = tab[i][j].temp;

			/*if (i == 7 && j == 5) { // debugowanie
				cout << neighbors << endl;
				cout << "i = " << i << " j = " << j << endl;
			}*/
		}
	}
}

string iterate(int number) { //	opisanie kolumn
	string hline = "";
	stringstream ss;

	ss << " ";
	for (int i = 0; i < number; i++) {
		if (i > 9) ss << i % 10;
		else ss << i;
		ss << " ";
	}
	hline += ss.str()+'\n';
	return hline;
}

string createSpacer(int number) { // tworzenie dynamicznych przerw do zachowania wygladu gridu
	string spacer = "";
	stringstream ss;

	for (int i = 0; i < number - 1; i++) {
		ss << " ";
	}
	spacer = ss.str();
	return spacer;
}

void printArr(param** tab, int &gennum) {
	int counter = 0;
	int crow;
	crow = (int)ceil(log10(row + 1));

	cout << "Generacja: " << gennum << endl;
	for (int i = 0; i < row; i++) {
		int countlog = (int)ceil(log10(i + 1));
		if (i == 0) {
			cout << createSpacer(crow - countlog) << iterate(column);
			cout << createSpacer(crow - countlog) << i;
		}
		else {
			if (countlog < crow) {
				cout << createSpacer(crow - countlog + 1);
			}
			cout << i;
		}
		for (int j = 0; j < column; j++) {
			if (j > 0) cout << " ";
			if (tab[i][j].generation == 0) cout << char(' ');
			else if (tab[i][j].generation == 1) {
				cout << char(176);
				counter++;
			}
			else if (tab[i][j].generation == 2) {
				cout << char(178);

			}
			if (j == column - 1) cout << endl;
		}
	}
	gennum += 1;
	if (counter == 0) {
		cout << "Brawo! Udalo ci sie powstrzymac rozwoj bakterii." << endl;
		cout << "Nacisnij dowolny klawisz aby wyjsc z programu" << endl;
		getchar();
		getchar();
		exit(0);
	}

	/*for (int i = 0; i < row; i++) { // debugowanie
		for (int j = 0; j < column; j++) {
			printf("[%d][%d].gen = %d\n", i, j, tab[i][j].generation);
		}
	}*/
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
			tab[i][j].temp = temp;
			if (j == column - 1) {
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

void printOptions() {
	cout << "Dostepne opcje:" << endl;
	cout << "1. Zmien temperature wybranej komorki" << endl;
	cout << "2. Sprawdz dane wybranej komorki" << endl;
	cout << "3. Stworz nowa generacje" << endl;
	cout << "4. Zapisz generacje do pliku" << endl;
	cout << "x. Wyjdz z programu" << endl;
	cout << "Podaj opcje: ";
}

int main()
{
	string filename = FILELOC;
	param** tab;
	param** tabNext;
	char choice = ' ';
	int gennum = 0;
	int is_active = 1;

	cout << "Podaj ilosc wierszy siatki do gry: ";
	cin >> row;
	cout << "Podaj ilosc kolumn siatki do gry: ";
	cin >> column;
	cout << "Podaj poczatkowa temperature dla bakterii: ";
	cin >> temp;
	
	system("CLS");

	tab = createArray();
	tabNext = createArray();

	readFile(filename, tab);
	printArr(tab, gennum);

	FILE* fp = fopen("./generations.txt", "w");
	fclose(fp);

	while (is_active) {
		printOptions();
		option:
		cin >> choice;
		switch (choice) {
		case 'x':
			is_active = 0;
			break;
		case '1':
			tempShot(tab);
			break;
		case '2':
			gridCheck(tab);
			cout << "Podaj opcje: ";
			goto option;
		case '3':
			break;
		case '4':
			writeToFile(tab, gennum);
			cout << "Podaj opcje: ";
			goto option;
		}
		if (is_active) {
			cout << "Nacisnij dowolny klawisz by przejsc dalej" << endl;
			getchar();
			getchar();
			system("CLS");
			nextGen(tab, tabNext);
			printArr(tabNext, gennum);
			arrMove(tab, tabNext);
		}
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
		choice(0);
	}*/
}

