#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <tinyxml2.h>
#include <ctime>
#include <chrono>
#include <random>
#include <thread>
// #include "functii_client.h"
#define M_MAX 14
#define MAX_LEN 1500
int port;
bool matrice_cartiere[14][14] = {false};
int matrice_lungime[14][14] = {0};
int matrice_viteza[14][14] = {0};
int distanta[30];
int distanta_totala;
// Client client;
#define PORT 2909
int valoareRandom = 0;

bool verificare_numar(char *cuvant)
{
  int numar;
  for (int i = 0; i < strlen(cuvant); i++)
  {
    if (!std::isdigit(cuvant[i]))
    {
      return false;
    }
  }
  numar = atoi(cuvant);
  if (numar > 10)
  {
    return false;
  }
  return true;
}
int main(int argc, char *argv[])
{
  auto start = std::chrono::steady_clock::now();
  std::random_device random;
  std::mt19937 gen(random());
  std::uniform_int_distribution<> distributie(30, 60);
  valoareRandom = distributie(gen);
  int ruta_lungime[50];
  bool logat = false;
  int sd;
  int lungime = 0;
  struct sockaddr_in server;
  int nr = 0;
  std::string comanda1;
  std::string comanda2;
  std::string comanda3;
  std::string comanda4;
  std::string buf;
  std::string mesaj_pt_server;
  char mesaj_de_la[MAX_LEN];
  std::string comanda;
  if (argc != 2)
  {
    printf("Sintaxa: %s <adresa_server> \n", argv[0]);
    return -1;
  }
  port = PORT;
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(port);

  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Eroare la connect().\n");
    return errno;
  }
  bool continua_in_continuare = true;

  while (continua_in_continuare)
  {
    std::string cartier1;
    std::string cartier2;
    if (logat == false)
    {
      printf("Comenzile sunt: \n");
      printf("[1]: Login \n");
      printf("[8]: Quit \n");
      printf("[10]: Inregistrare \n");

    }
    else
    {
      printf("Comenzile sunt: \n");
      printf("[2]: Traseu\n");
      printf("[3]: Vreme \n");
      printf("[4]: Evenimente sportive\n");
      printf("[5]: Preturi combustibil statiile PECO \n");
      printf("[6]: Raportarea unui accident \n");
      printf("[7]: Logout \n");
      printf("[8]: Quit \n");
      printf("[9]: Update informatii \n");
    }
    mesaj_pt_server = '\0';
    printf("[client] Introduceti numarul comenzii: ");
    fflush(stdout);
    std::cin >> buf;
    char comanda_verificare[20];
    std::string copie_buf;
    copie_buf = buf;
    strcpy(comanda_verificare, copie_buf.c_str());
    // memset(mesaj_pt_server,0,MAX_LEN);
    // memset(mesaj_de_la,0,MAX_LEN);
    // strcpy(mesaj_pt_server, buf.c_str());
    if (verificare_numar(comanda_verificare) == true)
    {
      mesaj_pt_server = buf;
      // std::cout<<mesaj_pt_server<<"DHHDHDH";
      nr = stoi(buf);
      // scanf("%d",&nr);
      if (logat == false)
      {
        switch (nr)
        {
        case 1:
          std::cout << "Numele de utilizator: ";
          std::cin >> comanda1;
          std::cout << "Parola: ";
          std::cin >> comanda2;
          mesaj_pt_server += ' ' + comanda1 + ' ' + comanda2;
          break;
        case 8:
          continua_in_continuare = 0;
          break;
        case 10:
          std::cout << "Numele de utilizator: ";
          std::cin >> comanda1;
          std::cout << "Parola: ";
          std::cin >> comanda2;
          std::cout << "Confirmare parola: ";
          std::cin >> comanda4;
          mesaj_pt_server += ' ' + comanda1 + ' ' + comanda2 + ' ' + comanda4;
          break;
        default:
          break;
        }
      }
      else
      {
        switch (nr)
        {
        case 2:
          std::cout << "Cartierul in care va aflati: ";
          std::cin >> comanda1;
          std::cout << "Cartierul in care doriti sa ajungeti: ";
          std::cin >> comanda2;
          mesaj_pt_server += ' ' + comanda1 + ' ' + comanda2;
          cartier1 = comanda1;
          cartier2 = comanda2;
          break;
        case 3:
          std::cout << "Doriti sa primiti informatii depre vreme [y/n]? ";
          std::cin >> comanda1;
          mesaj_pt_server += ' ' + comanda1;
          break;
        case 4:
          std::cout << "Doriti sa primiti stiri depre evenimente sportive [y/n]? ";
          std::cin >> comanda1;
          mesaj_pt_server += ' ' + comanda1;
          break;
        case 5:
          std::cout << "Doriti sa primiti preturile de la cel mai apropiat PECO [y/n]? ";
          std::cin >> comanda1;
          mesaj_pt_server += ' ' + comanda1;
          break;
        case 6:
          std::cout << "Doriti sa raportati un accident pe strada dumneavoastra [y/n]? ";
          std::cin >> comanda1;
          mesaj_pt_server += ' ' + comanda1;
          break;
        case 7:

          break;
        case 8:
          continua_in_continuare = 0;

          break;
        default:
          break;
        }
      }
      auto current = std::chrono::steady_clock::now();
      auto diff = std::chrono::duration_cast<std::chrono::seconds>(current - start).count();

      // std::thread fir(actualizeazaValoare);
      if (diff > 60)
      {
        valoareRandom = distributie(gen);
        start = current;
      }
      if (logat == true)
      {
        comanda3 = std::to_string(valoareRandom);
        mesaj_pt_server += " " + comanda3;
      }
      mesaj_pt_server += "\0 ";
      std::cout << mesaj_pt_server << "\n";
      if (mesaj_pt_server.size() > MAX_LEN)
      {
        printf("MESAJ PREA LUNG");
        fflush(stdout);
      }
      lungime = mesaj_pt_server.size();
      printf("[client] Am citit %d\n", nr);
      char msg[MAX_LEN];
      // strcpy(msg,buf.c_str());
      // lungime=strlen(msg);
      lungime += 1;

      strcpy(msg, mesaj_pt_server.c_str());
      if (logat == false && nr != 1 && nr != 8 && nr != 10)
      {
        std::cout << "Trebuie sa fiti logat pentru alte comenzi. \n";
      }
      else if (logat == true && nr == 1)
      {
        std::cout << "Sunteti deja logat.\n";
      }
      else
      {
        if ((write(sd, &lungime, sizeof(int)) <= 0) || (write(sd, &msg, lungime) <= 0))
        {
          perror("[client]Eroare la write() spre server.\n");
          return errno;
        }
        memset(mesaj_de_la, 0, MAX_LEN);
        if ((read(sd, &nr, sizeof(int)) < 0) || (read(sd, &mesaj_de_la, nr) < 0))
        {
          perror("[client]Eroare la read() de la server.\n");
          return errno;
        }
        if (strncmp(mesaj_de_la, "V-ati conectat cu succes", 24) == 0)
        {
          logat = true;
        }
        else if ((strncmp(mesaj_de_la, "Nu sunteti logat", 16) == 0) || (strncmp(mesaj_de_la, "V-ati delogat cu succes", 16) == 0))
        {
          logat = false;
        }
        printf("[client]Mesajul primit este: %s\n", mesaj_de_la);
      }
    }
    else
    {
      std::cout << "Comanda incorecta, comenzile sunt: <1> <2> <3> <4> <5> <6> <7> <8> \n";
    }
  }
  close(sd);
  return 1;
}
