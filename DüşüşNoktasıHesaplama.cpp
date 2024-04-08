
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




int getNextRandom(int lBound, int uBaund);      //Rastgele sayý oluþturur.
void placeChar(int surface[][WIDTH], char surfaceMask[][WIDTH], char c, int x, int y, char track);    //Yükseklikleri algýlayarak char yerleþtirir.
void printMask(char surfaceMask[][WIDTH]);   //Harflerinn gidiþ yollarýnýn gösterildiði arka planý yazdýrýr.
void print(int surface[][WIDTH]);        //Yüzeyi yazdýrýr.



int main()
{
	setlocale(LC_ALL, "Turkish");
	srand(time(NULL));       //"Rand()" Komutunun rastgele deðerler oluþturmasý için anlýk olarak zaman ile iliþkilendirir.
	//Merkezi bulur.
	int centerX = WIDTH / 2, centerY = HEIGHT / 2, radius = 0;

	//Harflerinn gidiþ yollarýnýn gösterildiði arka planý hazýrlar.
	char surfaceMask[HEIGHT][WIDTH] = { 0 };

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			surfaceMask[i][j] = ' ';

	}

	//2 Boyutlu dizi ( Enlem,Boylam,Yükseklik(Dizideki deðerler)).
	int surface[HEIGHT][WIDTH];
	ifstream dosya("Veri.csv");
	string metin;

	cout << "Arazi Yükseklikleri:\n";

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



	//14 ila 21 arasýnda rastgele yarýçap deðeri oluþturur.
	radius = getNextRandom(15, 21);

	//Ýsmi char'a atar ve uzunluðunu ölçer.
	char myName[] = "NIYAZIAKDEMIR";
	int myNameLeght = strlen(myName);

	//Yüzeyi yazdýrýr.
	print(surface);


	//Tam açýyý harf sayýsýna bölerek kaç derecede 1 harf koyulacagýný hesaplar.
	double dTheta = 2 * PI / myNameLeght, theta = 0;
	int x = 0, y = 0;

	//x ve y degerlerini deðiþtirerek gerekli yerlere c veya 'O' koyar.
	for (int i = 0; i < myNameLeght; i++) {
		x = cos(theta) * radius + centerX;
		y = sin(theta) * radius + centerY;
		char c = myName[i];
		placeChar(surface, surfaceMask, c, x, y, 'O');

		theta += dTheta;
	}
	printMask(surfaceMask);
	

}



int getNextRandom(int lBound, int uBaund) {    //15-20 Deðerleri arasýnda rastgele yarýçap deðeri oluþturur.
	return rand() % (uBaund - lBound) + lBound;
}

//North,West,East,South diye adlandýrýlan yön bölgelerini 'x' ve 'y' degerlerine 1 ekleyip azaltarak kontrol eder.
void placeChar(int surface[][WIDTH], char surfaceMask[][WIDTH], char c, int x, int y, char track) {
	int minX = x, minY = y;
	int Sx = x, Sy = y + 1;                                  //   N  
	int Ex = x + 1, Ey = y;                                 //  W   E
	int Nx = x, Ny = y - 1;                                  //   S  
	int Wx = x - 1, Wy = y;
	
	//Kontrol edilen bölgelerde sýrasýyla;
	//Kenarda olup olmadýgýný,
	//O bölgede büyük harf olup olmadýðýný,
	//Mevcut bulunduðu bölgeyle gitmek istediði bölgenin yüksekliklerini kontrol eder.
	//Son olarak mevcut konumundaki yüksekliðinden düþük olan bölgelerde ayný olan yüksekliklerden hangisine gideceðini rastgele 1 veya 2 deðerini seçerek karar verir.

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

		//Merkezle minimum deðeri karþýlaþtýrýr ve yazdýrýr
	if (surface[minY][minX] == surface[y][x]) {
		cout << "Char: " << c << "\tx: " << x << "\ty: " << y << endl;
		surfaceMask[y][x] = c;
	}

	//Harflerin gittiði kordinatlarý yazdýrýr
	else {
		cout << "Char: o->" << c << "\tx: " << x << "\ty: " << y << endl;
		surfaceMask[y][x] = track;
		placeChar(surface, surfaceMask, c, minX, minY, 'X');
	}
}


//Harflerinn gidiþ yollarýnýn gösterildiði arka planý yazdýrýr
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

//Yüzeyi yazdýrýr
void print(int surface[][WIDTH]) {
	for (int i = 0; i < HEIGHT; i++) {
		cout << "|";
		for (int j = 0; j < WIDTH; j++)
			cout << surface[i][j] << ' ';
		cout << "|" << endl;

	}
}

