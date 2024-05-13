#pragma once
#include <string.h>
#include <iostream>
#include <vector>
#include <tinyxml2.h>
#include <iostream>
#include <algorithm>
#define M_MAX 14
// struct Matrici_client{
//     bool matrice_cartiere[13][13] = {false};
//     int matrice_lungime[13][13] = {0};
// //int matrice_viteza[13][13] = {0};
// };
struct Drum{
    int id;
    int id_cartier1;
    int id_cartier2;
    std::string nume;
    int lungime;
    int limita;
};
struct Client{
    int id_start;
    int id_finish;
    bool inceput_traseu=false;
    int viteza =0;
};
struct Cartier{
    int id;
    std::string nume;
};

extern std::vector<std::pair<int,int>> ruta;
//extern std::vector<int> ruta_lungime;
bool realizare_matrice(bool (&matrice_cartiere)[M_MAX][M_MAX],int (&matrice_lungime)[M_MAX][M_MAX],int (&matrice_viteza)[M_MAX][M_MAX]);
extern std::vector<Drum> drumuri;
extern std::vector<Cartier> cartiere;
bool localizare(std::string Cartier1, std::string Cartier2, int &start, int &finish);
int traseu(bool (&matrice_cartiere)[M_MAX][M_MAX],int (&matrice_lungime)[M_MAX][M_MAX],int start, int finish);
int nod_nevizitat(int distante[], bool vizitat[]);
int calculeaza_distanta(int matrice_lungime[M_MAX][M_MAX], int (&vector)[50]);
int calculeaza_timp(int (&vector)[50]);
