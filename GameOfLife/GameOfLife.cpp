#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

constexpr auto FILELOC = "./text.txt";

using namespace std;

struct parameters {
	int generation;
	double temp;
};

typedef struct parameters param;

param** createArray(const int &row, const int &column) {
	param** tab;
	tab = new param * [row];
	for (int i = 0; i < row; i++) {
		tab[i] = new param[column];
	}
	return tab;
}

param** cCreateArray(const int &row, const int &column) {
	param **tab = (param**)malloc(sizeof(param*) * row);
	for (int i = 0; i < row; i++) {
		tab[i] = (param*)malloc(sizeof(param) * column);
	}
	return tab;
}

void arrMove(param** tab, param** tabNext, const int &row, const int &column) { // podmiana tablic po wygenerowaniu nowej generacji
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			tab[i][j] = tabNext[i][j];
		}
	}
}

void memFree(param** tab, const int &row) {
	for (int j = 0; j < row; j++) {
		delete[] tab[j];
	}
	delete[] tab;
}

void cMemFree(param** tab, const int &row) {
	for (int j = 0; j < row; j++) {
		free(tab[j]);
	}
	free(tab);
}

void writeToFile(param** tab, int &gennum, const int &row, const int &column) { // zapisa aktualnej generacji do pliku
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

void gridCheck(param** tab) { // odczyt danych komórki
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

void evenSpread(param** tab, double ctemp, int x, int y, const int &row, const int &column) {
	int i, j = 1;
	double sctemp = ctemp;
	tab[x][y].temp += ctemp;
	if (tab[x][y].generation != 0) ctemp = ctemp - ctemp * 0.3;
	else ctemp = ctemp - ctemp * 0.1;

	/* 
	Dzielę grid na cztery części i poruszam się od wskazanego punktu do zewnątrz. 
	Zmiana temperatury zależy od tego czy w iteracji wcześniej istnieje żywa bakteria.
	*/

	i = x - 1;
	while (i >= 0) {
		int counter = 0;
		int k = y - j;
		int len = y + j;
		if (k < 0) k = 0;
		if (len >= column) len = column - 1;
		while (k <= len) {
			tab[i][k].temp += ctemp;
			if (tab[i][k].generation != 0) counter++;
			k++;
		}
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
		if (len >= column) len = column - 1;
		while (k <= len) {
			tab[i][k].temp += ctemp;
			if (tab[i][k].generation != 0) counter++;
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
		if (len >= row) len = row - 1;
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
		if (len >= row) len = row - 1;
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

void directionalSpread(param** tab, double ctemp, int x, int y, const int &row, const int &column) {
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

	/*
	Zmiana temperatury w podanej linii. Zaczynamy od podanego punktu, a w następnej iteracji linia rozszerza się do 3 pól.
	Wartość o jaką zmienia się temperatura zależy od tego czy komórka wcześniej była żywa.
	*/

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

void tempShot(param** tab, const int &row, const int &column) { // funkcja wywołująca strzał temperaturą
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
		evenSpread(tab, ctemp, x, y, row, column);
		break;
	case 2:
		directionalSpread(tab, ctemp, x, y, row, column);
		break;
	default: 
		cout << "Dokonano niepoprawnego wyboru" << endl;
	}

}

int countNeighbors(param** tab, int i, int j, const int &row, const int &column) { // funkcja zliczająca ilość żywych sąsiadów komórki
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

void nextGen(param** tab, param** tabNext, const int &row, const int &column, const double &temperature) {
	int neighbors;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			/*
			Generowanie następnego pokolenia na podstawie temperatury i ilości żywych sąsiadów
			*/
			neighbors = countNeighbors(tab, i, j, row, column);

			if (tab[i][j].generation == 2 && tab[i][j].temp > temperature + temperature * 0.4) {
				tabNext[(row + (i - 1)) % row][j].generation = 2;
				tabNext[(i + 1) % row][j].generation = 2;
				tabNext[i][(column + (j - 1)) % column].generation = 2;
				tabNext[i][(j + 1) % column].generation = 2;
				tabNext[(row + (i - 1)) % row][(column + (j - 1)) % column].generation = 1;
				tabNext[(row + (i - 1)) % row][(j + 1) % column].generation = 1;
				tabNext[(i + 1) % row][(column + (j - 1)) % column].generation = 1;
				tabNext[(i + 1) % row][(j + 1) % column].generation = 1;
				tabNext[i][j].generation = tab[i][j].generation;
			}
			else if (tab[i][j].generation == 2 && (tab[i][j].temp < temperature + temperature * 0.4 && tab[i][j].temp > temperature - temperature * 0.4)) {
				tabNext[(row + (i - 1)) % row][j].generation = 1;
				tabNext[(i + 1) % row][j].generation = 1;
				tabNext[i][(column + (j - 1)) % column].generation = 1;
				tabNext[i][(j + 1) % column].generation = 1;
				tabNext[(row + (i - 1)) % row][(column + (j - 1)) % column].generation = 1;
				tabNext[(row + (i - 1)) % row][(j + 1) % column].generation = 1;
				tabNext[(i + 1) % row][(column + (j - 1)) % column].generation = 1;
				tabNext[(i + 1) % row][(j + 1) % column].generation = 1;
				tabNext[i][j].generation = 0;
			}
			else if (tab[i][j].generation == 2 && tab[i][j].temp < temperature - temperature * 0.4) {
				tabNext[(row + (i - 1)) % row][j].generation = (tab[(row + (i - 1)) % row][j].generation == 0) ? 0 : (tab[(row + (i - 1)) % row][j].generation - 1);
				tabNext[(i + 1) % row][j].generation = (tab[(i + 1) % row][j].generation == 0) ? 0 : (tab[(i + 1) % row][j].generation - 1);
				tabNext[i][(column + (j - 1)) % column].generation = (tab[i][(column + (j - 1)) % column].generation == 0) ? 0 : (tab[i][(column + (j - 1)) % column].generation - 1);
				tabNext[i][(j + 1) % column].generation = (tab[i][(j + 1) % column].generation == 0) ? 0 : (tab[i][(j + 1) % column].generation - 1);
				tabNext[i][j].generation = tab[i][j].generation - 1;
			}
			else if (tab[i][j].generation < 2 && tab[i][j].temp > temperature + temperature * 0.4) tabNext[i][j].generation = tab[i][j].generation + 1;
			else if ((tab[i][j].generation < 2 && tab[i][j].generation > 0) && tab[i][j].temp < temperature - temperature * 0.4) tabNext[i][j].generation = tab[i][j].generation - 1;
			else if (tab[i][j].generation == 0 && tab[i][j].temp < temperature - temperature * 0.4) tabNext[i][j].generation = tab[i][j].generation;
			else if (tab[i][j].generation < 2 && (tab[i][j].temp < temperature + temperature * 0.4 && tab[i][j].temp > temperature - temperature * 0.4 && neighbors < 3)) tabNext[i][j].generation = tab[i][j].generation;
			else if (tab[i][j].generation < 2 && (tab[i][j].temp < temperature + temperature * 0.4 && tab[i][j].temp > temperature - temperature * 0.4 && (neighbors >= 3 && neighbors <= 6))) tabNext[i][j].generation = tab[i][j].generation + 1;
			else if (tab[i][j].generation < 2 && (tab[i][j].temp < temperature + temperature * 0.4 && tab[i][j].temp > temperature - temperature * 0.4 && neighbors >= 6))
				tabNext[i][j].generation = tab[i][j].generation == 0 ? 0 : tab[i][j].generation - 1;

			else tabNext[i][j].generation = tab[i][j].generation;
			if (tab[i][j].temp > temperature)
				tabNext[i][j].temp = tab[i][j].temp - fabs(tab[i][j].temp - temperature) * 0.1;
			else if (tab[i][j].temp < temperature)
				tabNext[i][j].temp = tab[i][j].temp + fabs(tab[i][j].temp - temperature) * 0.1;
			else tabNext[i][j].temp = tab[i][j].temp;
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

string createSpacer(int number) { // tworzenie przerw do zachowania wygladu gridu
	string spacer = "";
	stringstream ss;

	for (int i = 0; i < number - 1; i++) {
		ss << " ";
	}
	spacer = ss.str();
	return spacer;
}

void printArr(param** tab, int &gennum, const int &row, const int &column) { // wypisanie tablicy na w konsoli
	int counter = 0;
	int crow;
	crow = (int)ceil(log10(row + 1));	// obliczanie ilości znaków potrzebnych, by zapisać numer wiersza przy gridzie

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
				counter++;

			}
			if (j == column - 1) cout << endl;
		}
	}
	gennum += 1;
	if (counter == 0) { // Zakończenie programu jeśli ilość żywych komórek wynosi 0
		cout << "Brawo! Udalo ci sie powstrzymac rozwoj bakterii." << endl;
		cout << "Nacisnij klawisz ENTER aby wyjsc z programu" << endl;
		getchar();
		exit(0);
	}
}

void readFile(string filename, param** tab, const int &column, const double &temperature) { // wczytanie gridu z pliku i zapisanie go do tablicy
	char c;
	int ic; // zmienna pomocnicza do zamiany char na int
	ifstream file(filename);
	if (file.is_open()) {
		int i = 0, j = 0;
		while (file >> c) {
			ic = c - '0';
			tab[i][j].generation = ic;
			tab[i][j].temp = temperature;
			if (j == column - 1) {
				i++;
				j = 0;
			}
			else j++;
		}
	}
	file.close();
}

void printChoice() {
	cout << "Podaj opcje: ";
}

void printOptions() {
	cout << "Dostepne opcje:" << endl;
	cout << "1. Zmien temperature wybranej komorki" << endl;
	cout << "2. Sprawdz dane wybranej komorki" << endl;
	cout << "3. Stworz nowa generacje" << endl;
	cout << "4. Zapisz generacje do pliku" << endl;
	cout << "x. Wyjdz z programu" << endl;
	printChoice();
}

int main()
{
	int row, column;
	double temperature;
	string filename = FILELOC;
	param** tab;
	param** tabNext;
	string choice = "";
	int gennum = 0;
	int is_active = 1;

	cout << "Podaj ilosc wierszy siatki do gry: ";
	cin >> row;
	cout << "Podaj ilosc kolumn siatki do gry: ";
	cin >> column;
	cout << "Podaj poczatkowa temperature dla bakterii (stan rownowagi): ";
	cin >> temperature;
	
	system("CLS");

	tab = cCreateArray(row, column);
	if (!tab) {
		return -1;
	}
	tabNext = cCreateArray(row, column);
	if (!tabNext) {
		return -1;
	}

	readFile(filename, tab, column, temperature);
	printArr(tab, gennum, row, column);

	FILE* fp = fopen("./generations.txt", "w");
	fclose(fp);		// czyszczenie pliku z zapisanymi generacjami przy uruchomieniu programu po uprzednim jego zamknięciu

	while (is_active) { // menu
		printOptions();
		option:
		cin >> choice;
		switch (choice[0]) {
		case 'x':
			is_active = 0;
			break;
		case '1':
			tempShot(tab, row, column);
			break;
		case '2':
			gridCheck(tab);
			printChoice();
			goto option;
		case '3':
			break;
		case '4':
			writeToFile(tab, gennum, row, column);
			printChoice();
			goto option;
		default:
			cout << "Wybrana opcja jest niepoprawna!\n";
			printChoice();
			goto option;
		}
		if (is_active) {
			cout << "Nacisnij klawisz ENTER by przejsc dalej" << endl;
			getchar();
			getchar();
			system("CLS");
			nextGen(tab, tabNext, row, column, temperature);
			printArr(tabNext, gennum, row, column);
			arrMove(tab, tabNext, row, column);
		}
	}

	cMemFree(tab, row);
	cMemFree(tabNext, row);
}

