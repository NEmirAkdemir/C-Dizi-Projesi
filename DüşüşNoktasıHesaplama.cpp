
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
//Global sabitler.
#define WIDTH 71
#define HEIGHT 51
#define PI 3.14
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red   */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue  */




int getNextRandom(int lBound, int uBaund);      //Rastgele say� olu�turur.
void placeChar(int surface[][WIDTH], char surfaceMask[][WIDTH], char c, int x, int y, char track);    //Y�kseklikleri alg�layarak char yerle�tirir.
void printMask(char surfaceMask[][WIDTH]);   //Harflerinn gidi� yollar�n�n g�sterildi�i arka plan� yazd�r�r.
void print(int surface[][WIDTH]);        //Y�zeyi yazd�r�r.



int main()
{
	setlocale(LC_ALL, "Turkish");
	srand(time(NULL));       //"Rand()" Komutunun rastgele de�erler olu�turmas� i�in anl�k olarak zaman ile ili�kilendirir.
	//Merkezi bulur.
	int centerX = WIDTH / 2, centerY = HEIGHT / 2, radius = 0;

	//Harflerinn gidi� yollar�n�n g�sterildi�i arka plan� haz�rlar.
	char surfaceMask[HEIGHT][WIDTH] = { 0 };

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			surfaceMask[i][j] = ' ';

	}

	//2 Boyutlu dizi ( Enlem,Boylam,Y�kseklik(Dizideki de�erler)).
	int surface[HEIGHT][WIDTH];
	ifstream dosya("Veri.csv");
	string metin;

	cout << "Arazi Y�kseklikleri:\n";

	for (int i = 0; i < HEIGHT; i++) {
		if (getline(dosya, metin)) {
			istringstream ss(metin);
			for (int j = 0; j < WIDTH; j++) {
				string eleman;
				if (getline(ss, eleman, ';')) {
					surface[i][j] = stoi(eleman);
				}
			}
		}
	}
	dosya.close();



	//14 ila 21 aras�nda rastgele yar��ap de�eri olu�turur.
	radius = getNextRandom(15, 21);

	//�smi char'a atar ve uzunlu�unu �l�er.
	char myName[] = "NIYAZIAKDEMIR";
	int myNameLeght = strlen(myName);

	//Y�zeyi yazd�r�r.
	print(surface);


	//Tam a��y� harf say�s�na b�lerek ka� derecede 1 harf koyulacag�n� hesaplar.
	double dTheta = 2 * PI / myNameLeght, theta = 0;
	int x = 0, y = 0;

	//x ve y degerlerini de�i�tirerek gerekli yerlere c veya 'O' koyar.
	for (int i = 0; i < myNameLeght; i++) {
		x = cos(theta) * radius + centerX;
		y = sin(theta) * radius + centerY;
		char c = myName[i];
		placeChar(surface, surfaceMask, c, x, y, 'O');

		theta += dTheta;
	}
	printMask(surfaceMask);
	

}



int getNextRandom(int lBound, int uBaund) {    //15-20 De�erleri aras�nda rastgele yar��ap de�eri olu�turur.
	return rand() % (uBaund - lBound) + lBound;
}

//North,West,East,South diye adland�r�lan y�n b�lgelerini 'x' ve 'y' degerlerine 1 ekleyip azaltarak kontrol eder.
void placeChar(int surface[][WIDTH], char surfaceMask[][WIDTH], char c, int x, int y, char track) {
	int minX = x, minY = y;
	int Sx = x, Sy = y + 1;                                  //   N  
	int Ex = x + 1, Ey = y;                                 //  W   E
	int Nx = x, Ny = y - 1;                                  //   S  
	int Wx = x - 1, Wy = y;
	
	//Kontrol edilen b�lgelerde s�ras�yla;
	//Kenarda olup olmad�g�n�,
	//O b�lgede b�y�k harf olup olmad���n�,
	//Mevcut bulundu�u b�lgeyle gitmek istedi�i b�lgenin y�ksekliklerini kontrol eder.
	//Son olarak mevcut konumundaki y�ksekli�inden d���k olan b�lgelerde ayn� olan y�ksekliklerden hangisine gidece�ini rastgele 1 veya 2 de�erini se�erek karar verir.

	//South
	if (Sy < HEIGHT && (surfaceMask[Sy][Sx] < 'A' || surfaceMask[Sy][Sx] > 'Z') && surface[minY][minX] > surface[Sy][Sx]) {
		minX = Sx;
		minY = Sy;
	}
	//East
	if (Ex < WIDTH && (surfaceMask[Ey][Ex] < 'A' || surfaceMask[Ey][Ex] > 'Z'))
		if ((surface[minY][minX] > surface[Ey][Ex]) || (surface[minY][minX] == surface[Ey][Ex] && getNextRandom(1, 3) == 2)) {
			minX = Ex;
			minY = Ey;
		}
	//North
	if (Ny >= 0 && (surfaceMask[Ny][Nx] < 'A' || surfaceMask[Ny][Nx] > 'Z'))
		if ((surface[minY][minX] > surface[Ny][Nx]) || (surface[minY][minX] == surface[Ny][Nx] && getNextRandom(1, 3) == 2)) {
			minX = Nx;
			minY = Ny;
		}
	//West
	if (Wx >= 0 && (surfaceMask[Wy][Wx] < 'A' || surfaceMask[Wy][Wx] > 'Z'))
		if ((surface[minY][minX] > surface[Wy][Wx]) || (surface[minY][minX] == surface[Wy][Wx] && getNextRandom(1, 3) == 2)) {
			minX = Wx;
			minY = Wy;
		}

		//Merkezle minimum de�eri kar��la�t�r�r ve yazd�r�r
	if (surface[minY][minX] == surface[y][x]) {
		cout << "Char: " << c << "\tx: " << x << "\ty: " << y << endl;
		surfaceMask[y][x] = c;
	}

	//Harflerin gitti�i kordinatlar� yazd�r�r
	else {
		cout << "Char: o->" << c << "\tx: " << x << "\ty: " << y << endl;
		surfaceMask[y][x] = track;
		placeChar(surface, surfaceMask, c, minX, minY, 'X');
	}
}


//Harflerinn gidi� yollar�n�n g�sterildi�i arka plan� yazd�r�r
void printMask(char surfaceMask[][WIDTH]) {
	for (int i = 0; i < HEIGHT; i++) {
		cout << "|";
		for (int j = 0; j < WIDTH; j++)
			if (surfaceMask[i][j] == 'X') {
				cout << setw(1) << RED << surfaceMask[i][j] << RESET;
			}
			else
				if (surfaceMask[i][j] == 'O') {
					cout << setw(1) << BLUE << surfaceMask[i][j] << RESET;
				}
				else
					cout << setw(1) << GREEN << surfaceMask[i][j] << RESET;
		cout << "|" << endl;

	}
}

//Y�zeyi yazd�r�r
void print(int surface[][WIDTH]) {
	for (int i = 0; i < HEIGHT; i++) {
		cout << "|";
		for (int j = 0; j < WIDTH; j++)
			cout << surface[i][j] << ' ';
		cout << "|" << endl;

	}
}

