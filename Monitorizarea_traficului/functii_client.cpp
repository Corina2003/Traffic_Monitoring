#include "functii_client.h"
#include <tinyxml2.h>
#include <iostream>
std::vector<Drum> drumuri;
std::vector<Cartier> cartiere;
std::vector<std::pair<int, int>> ruta;
//std::vector<int> ruta_lungime;
bool realizare_matrice(bool (&matrice_cartiere)[M_MAX][M_MAX], int (&matrice_lungime)[M_MAX][M_MAX], int (&matrice_viteza)[M_MAX][M_MAX])
{
    tinyxml2::XMLDocument drumuri_document;
    drumuri_document.LoadFile("Drumuri.xml");
    if (drumuri_document.LoadFile("Drumuri.xml") != 0)
    {
        perror("Eroare la deschiderea Drumuri.xml (in functii_client.h)");
        return false;
    }
    tinyxml2::XMLElement *radacina_drumuri = drumuri_document.FirstChildElement("drumuri");
    for (tinyxml2::XMLElement *copil = radacina_drumuri->FirstChildElement("drum"); copil != nullptr; copil = copil->NextSiblingElement("drum"))
    {
        Drum drum;
        drum.id = std::stoi(copil->FirstChildElement("id")->GetText());
        drum.id_cartier1 = std::stoi(copil->FirstChildElement("id_cartier1")->GetText());
        drum.id_cartier2 = std::stoi(copil->FirstChildElement("id_cartier2")->GetText());
        drum.nume = copil->FirstChildElement("nume")->GetText();
        drum.lungime = std::stoi(copil->FirstChildElement("lungime")->GetText());
        drum.limita = std::stoi(copil->FirstChildElement("limita")->GetText());
        // std::cout << "drumuri ";
        std::cout << drum.nume;
        drumuri.push_back(drum);
    }
    tinyxml2::XMLDocument cartiere_document;
    if (cartiere_document.LoadFile("Cartiere.xml") != 0)
    {
        perror("Eroare la deschiderea Drumuri.xml (in functii_client.h)");
        return false;
    }
    tinyxml2::XMLElement *radacina_cartiere = cartiere_document.FirstChildElement("cartiere");
    for (tinyxml2::XMLElement *copil = radacina_cartiere->FirstChildElement("cartier"); copil != nullptr; copil = copil->NextSiblingElement("cartier"))
    {
        Cartier cartier;
        cartier.id = std::stoi(copil->FirstChildElement("id")->GetText());
        cartier.nume = copil->FirstChildElement("nume")->GetText();
        std::cout << cartier.nume;
        cartiere.push_back(cartier);
    }
    // for(int k=0;k<drumuri.size();k++){
    for (int i = 1; i < M_MAX; i++)
    {

        for (int j = 1; j < M_MAX; j++)
        {
            for (const Drum &drum : drumuri)
            {
                if ((drum.id_cartier1 == cartiere[i].id) && (drum.id_cartier2 == cartiere[j].id))
                {
                    matrice_cartiere[i][j] = true;
                    matrice_cartiere[j][i] = true;
                    matrice_lungime[i][j] = drum.lungime;
                    matrice_viteza[i][j] = drum.limita;
                    matrice_lungime[j][i] = drum.lungime;
                    matrice_viteza[j][i] = drum.limita;
                    // break;
                }
            }
            // if (!gasit)
            // {
            //     matrice_cartiere[i][j] = false;
            //     matrice_lungime[i][j] = 0; // Sau o valoare invalidă, în funcție de context
            //     matrice_viteza[i][j] = 0;  // Sau o valoare invalidă, în funcție de context
            // }
        }
    }
    for (int i = 1; i < M_MAX; i++)
    {
        for (int j = 1; j < M_MAX; j++)
        {
            std::cout << matrice_cartiere[i][j] << " ";
            //<< matrice_lungime[i][j] << " "
        }
        std::cout << '\n';
    }
    return true;
}
bool localizare(std::string Cartier1, std::string Cartier2, int &start, int &finish)
{
    tinyxml2::XMLDocument drumuri_document;
    drumuri_document.LoadFile("Drumuri.xml");
    if (drumuri_document.LoadFile("Drumuri.xml") != 0)
    {
        perror("Eroare la deschiderea Drumuri.xml (in functii_client.h)");
        return false;
    }
    tinyxml2::XMLElement *radacina_drumuri = drumuri_document.FirstChildElement("drumuri");
    for (tinyxml2::XMLElement *copil = radacina_drumuri->FirstChildElement("drum"); copil != nullptr; copil = copil->NextSiblingElement("drum"))
    {
        Drum drum;
        drum.id = std::stoi(copil->FirstChildElement("id")->GetText());
        drum.id_cartier1 = std::stoi(copil->FirstChildElement("id_cartier1")->GetText());
        drum.id_cartier2 = std::stoi(copil->FirstChildElement("id_cartier2")->GetText());
        drum.nume = copil->FirstChildElement("nume")->GetText();
        drum.lungime = std::stoi(copil->FirstChildElement("lungime")->GetText());
        drum.limita = std::stoi(copil->FirstChildElement("limita")->GetText());
        // std::cout << "drumuri ";
        // std::cout << drum.nume;
        drumuri.push_back(drum);
    }
    tinyxml2::XMLDocument cartiere_document;
    if (cartiere_document.LoadFile("Cartiere.xml") != 0)
    {
        perror("Eroare la deschiderea Drumuri.xml (in functii_client.h)");
        return false;
    }
    tinyxml2::XMLElement *radacina_cartiere = cartiere_document.FirstChildElement("cartiere");
    for (tinyxml2::XMLElement *copil = radacina_cartiere->FirstChildElement("cartier"); copil != nullptr; copil = copil->NextSiblingElement("cartier"))
    {
        Cartier cartier;
        cartier.id = std::stoi(copil->FirstChildElement("id")->GetText());
        cartier.nume = copil->FirstChildElement("nume")->GetText();
        // std::cout << cartier.nume;
        cartiere.push_back(cartier);
    }
    start = 0;
    finish = 0;
    if ((!Cartier1.empty()) && (!Cartier2.empty()))
    {
        Cartier1[0] = std::toupper(Cartier1[0]);
        Cartier2[0] = std::toupper(Cartier2[0]);
    }
    else
    {
        return false;
    }
    for (int i = 0; i < cartiere.size(); i++)
    {
        if (cartiere[i].nume == Cartier1)
        {
            start = cartiere[i].id;
            // std::cout << start;
        }
        else if (cartiere[i].nume == Cartier2)
        {
            finish = cartiere[i].id;
            // std::cout << finish;
        }
    }
    std::cout << Cartier1 << Cartier2 << start << finish << " ";
    if (start == 0 || finish == 0)
    {
        return false;
    }

    return true;
}
int nod_nevizitat(int distante[], bool vizitat[])
{
    int min = INT_MAX, min_index;

    for (int v = 1; v < M_MAX; v++)
    {
        if (vizitat[v] == false && distante[v] <= min)
        {
            min = distante[v];
            min_index = v;
        }
    }
    return min_index;
}
void afisare_drum(int drum_minim[], int destinatie, int matrice_lungime[M_MAX][M_MAX])
{
    // std::vector<std::pair<int, int>> drum;
    int i = 0;
    ruta.clear();
    while (destinatie != -1)
    {
        int nod_precedent = drum_minim[destinatie];
        if (nod_precedent != -1)
        {
            // distanta[i]=matrice_lungime[nod_precedent][destinatie];
            ruta.emplace_back(nod_precedent, destinatie);
        }
        destinatie = nod_precedent;
        // i++;
    }

    std::reverse(ruta.begin(), ruta.end());
    std::cout<<"\n";
    std::cout << "Drumul este: ";
    for (const auto &pair : ruta)
    {
        // distanta[i]=matrice_lungime[pair.first][pair.second];
        // i++;
        std::cout << pair.first << " -> " << pair.second << " ";
    }
    std::cout<<"\n";
}
int traseu(bool (&matrice_cartiere)[M_MAX][M_MAX], int (&matrice_lungime)[M_MAX][M_MAX], int start, int finish)
{
    int distante[M_MAX];
    bool vizitat[M_MAX];
    int drum_minim[M_MAX];

    for (int i = 0; i < M_MAX; i++)
    {
        distante[i] = INT_MAX;
        vizitat[i] = false;
        drum_minim[i] = -1;
    }

    distante[start] = 0;

    for (int count = 1; count < M_MAX - 2; count++)
    {
        int u = nod_nevizitat(distante, vizitat);
        vizitat[u] = true;

        for (int v = 1; v < M_MAX; v++)
        {
            if (!vizitat[v] && matrice_cartiere[u][v] && distante[u] != INT_MAX && distante[u] + matrice_lungime[u][v] < distante[v])
            {
                distante[v] = distante[u] + matrice_lungime[u][v];
                drum_minim[v] = u;
            }
        }
    }

    // std::cout<<" "<<drum_minim[finish]<<" ";
    afisare_drum(drum_minim, finish, matrice_lungime);
    return 1;
}
int calculeaza_distanta(int matrice_lungime[M_MAX][M_MAX], int (&vector)[50])
{
    int i = 0;
    int distanta=0;
    for (const auto &pair : ruta)
    {
        vector[i]=matrice_lungime[pair.first][pair.second];
        //std::cout<<vector[i];
        //std::cout << pair.first << " -> " << pair.second << " ";
        i++;
        distanta+=vector[i];
    }
    return distanta;
}