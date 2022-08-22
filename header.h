#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <float.h>
#include <cstdlib>
#include <iomanip>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <filesystem>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

typedef vector<string> vs;
typedef vector<Point2f> vp;
typedef vector<vs> vvs;
typedef vector<int> vi;
typedef map<string, int> msi;
typedef vector<double> vd;



struct node													
{
	string splitOn;
	string label;
	bool isLeaf;
	vector<string> childrenValues;
	vector<node*> children;
};

struct figure
{
    string label;
    string cornerCnt;
    string hWRatio;
    string parallelCnt;
    string rAngleCnt;
};


vvs pruneTable(vvs&, string&, string);						// Skrój tabelê bazuj¹c¹ na kolumnie lub nazwie atrybutu i wartoœæ tego atrybutu
node* buildDecisionTree(vvs& table, node* nodePtr, vvs& tableInfo);// Buduj drzewko decyzyjne bazuj¹ce na tabeli.
bool isHomogeneous(vvs&);									// Zwraca true jesli tabela zawiera te same labele klasy
vi countDistinct(vvs&, int);								// Zwraca wektor z intów zawier¹j¹cy iloœæ wyst¹pieñ unikalnych atrybutów
string decideSplittingColumn(vvs&);							// Zwraca kolumne w której nale¿y podzieliæ tabele
int returnColumnIndex(string&, vvs&);						// Zwraca indeks kolumny w podtabeli
bool tableIsEmpty(vvs&);									// Sprawdza czy tabela jest pusta
void printDecisionTree(node*, string);								// Rekurencyjnie drukuje drzewo decyzyjne
string testDataOnDecisionTree(vs&, node*, vvs&, string);	// Przepuszcza dane przez drzewo decyzyjne
int returnIndexOfVector(vs&, string);						// Zwraca index wektora
vvs generateTableInfo(vvs& dataTable);						// Sortuje informacje o danych w wektorze zawieraj¹cym wektory stringów
string returnMostFrequentClass(vvs& dataTable);				// Zwraca najczêœciej wystêpuj¹c¹ klasê z danych do nauki
figure processImage(string imageToLearn);					// Procesuje zdjecia zwraca obiekt figure zawierajacy dane o zdjeciu
int getRightAnglesCnt(vp corners);							// Sprawdza iloœæ k¹tów ostrych
int getParallelCnt(vector<Point2f> corners);				// Sprawdza iloœæ równoleg³ych par œcian
float getAngleABC(Point2f a, Point2f b, Point2f c);			// Zwraca k¹t zawarty miêdzy punktami ABC
bool comparePoints(Point2f p1, Point2f p2);					// Porównuje który punkt znajdujê siê w wiekszym k¹cie zawartym miêdzy punktem 0,0 a punktami p1 i p2
double getClockwiseAngle(Point2f p);						// Zwraca k¹t pomiedzy punktem 0,0 a punktem p
vp getGoodFeaturesToTrack();								// Zwraca punkty bêd¹ce wieszcho³kami figury
string calculateHWRatio(vp corners);						// Liczy stosunek wysokoœci do szerokoœci figury
float distance(Point2f a, Point2f b);						// Liczy odleglosc z punktu a do b
string getFileName(string strPath);							// Zwraca nazwa pliku dla przykladu test01.jpg to test
vs figureToVS(figure fig);									// Zmienia strukture figure na wektor stringów
vs getAllFilesNamesWithinFolder(string folder);				// Bierze wszystkie pliki z folderu
vs getHeaders();											// Zwraca nag³ówki dla struktury figure

