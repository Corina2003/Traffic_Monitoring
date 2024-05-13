#pragma once
#include <string.h>
#include <tinyxml2.h>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <ctime>
#include <algorithm>
#define M_MAX 14
#define D_MAXIM 17
using namespace tinyxml2;

struct Peco{
    double benzina;
    double motorina;
    std::string nume;
};

struct User{
    int index;
    int id_start;
    int id_finish;
    int id_loc1;
    int id_loc2;
    double distantaParcursa;
    bool info_vreme=false;
    bool info_sport=false;
    bool info_PECO=false; 
    bool info_accident= true;
    int idThread;
    bool conectat = false;
    int viteza=0;
    int distanta_totala=0;
    //auto t_start;
    //auto t_finish;
    bool inceput_traseu=false;
    int socketul_fiecarui_client;
    std::string nume_utilizator;
    
    bool logout();// ramane aici
    bool accident(std::string, std::string,std::vector<User>&, int, int);
};

struct Drum{
    Peco peco;
    int id;
    int id_cartier1;
    int id_cartier2;
    std::string nume;
    int lungime;
    int limita;
};

struct Cartier{
    int id;
    std::string nume;
};
//extern std::vector<std::pair<int, int>> ruta;
extern std::vector<Drum> drumuri;
extern std::vector<Cartier> cartiere;
bool verificare_numar(char*);
bool localizare(std::string Cartier1, std::string Cartier2, int &start, int &finish);
bool realizare_matrice(bool (&matrice_cartiere)[M_MAX][M_MAX],int (&matrice_lungime)[M_MAX][M_MAX],int (&matrice_viteza)[M_MAX][M_MAX]);
int traseu(bool (&matrice_cartiere)[M_MAX][M_MAX],int (&matrice_lungime)[M_MAX][M_MAX],int start, int finish, std::vector<std::pair<int, int>>& ruta);
std::string login(std::string nume,std::string cheie, User &client);
int calculeaza_distanta(int matrice_lungime[M_MAX][M_MAX], int (&vector)[50], const std::vector<std::pair<int, int>>& ruta);
//int calculeaza_timp(int (&vector)[50]);
double parcursa(int viteza, double durata);
std::string cartierul_meu(int distanta, int (&vector)[50], bool pret, const std::vector<std::pair<int, int>>& ruta, int &id_first, int &id_second);
std::string get_vreme(int oraReala);
std::string get_sport(int oraReala);
std::string vezi_aglomeratie(int (&matrice)[M_MAX][M_MAX], int precedenta1, int precedenta2, int curenta1, int curenta2);
//std::string get_PECO(int distanta, int (&vector)[50]);
std::string transmitere_accident(int id1, int id2);
std::string inregistrare(std::string nume, std::string parola);