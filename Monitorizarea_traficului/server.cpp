#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <sqlite3.h>
#include <chrono>
#include <thread>
#include "functii.h"
#define PORT 2909
#define MAX_LEN 1500
#define M_MAX 14
using namespace std;

bool matrice_cartiere[M_MAX][M_MAX] = {false};
int matrice_lungime[M_MAX][M_MAX] = {0};
int matrice_viteza[M_MAX][M_MAX] = {0};
bool distribuirea_accidentului = false;
int numar_conectati = 0;
int matrice_aglomeratie[M_MAX][M_MAX] = {0};
bool accident = false;
// bool transmitere = false;
std::string stringAccident = "";
static void *treat(void *);
// void raspunde(void *);
typedef struct
{
  pthread_t idThread; // id-ul thread-ului
  int thCount;        // nr de conexiuni servite
} Thread;

Thread *threadsPool; // un array de structuri Thread

int sd;
int nthreads;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
vector<User> Clienti_conectati(20);
// int User::useri_conectati_comun=0;
void raspunde(int cl, int idThread);
double secondsPassed = 0.0; // Variabilă globală pentru a stoca secundele trecute
std::chrono::steady_clock::time_point startTime;
bool matrice_catre_server(bool (&matrice_cartiere)[M_MAX][M_MAX], int (&matrice_lungime)[M_MAX][M_MAX], int socket)
{
  int n = M_MAX;
  std::cout << "MAtrice";
  if ((write(socket, &n, sizeof(bool)) < 0) || (write(socket, matrice_cartiere, sizeof(bool) * n * n) < 0))
  {
    std::cout << "nu s-a realizat transmiterea matricei";
    return false;
  }
  if ((write(socket, &n, sizeof(int)) < 0) || (write(socket, matrice_lungime, sizeof(int) * n * n) < 0))
  {
    std::cout << "nu s-a realizat transmiterea matricei";
    return false;
  }

  return true;
}
bool User::accident(std::string locatie, std::string numarul, std::vector<User> &Clienti, int n, int clientul_raportat)
{
  std::cout << "DAR AICI" << n << '\n';
  std::cout << clientul_raportat << '\n';
  std::string catreTotiUtilizatorii;
  catreTotiUtilizatorii += "!ACCIDENT! Strada " + locatie + " " + numarul;
  char mesaj[MAX_LEN];
  strcpy(mesaj, catreTotiUtilizatorii.c_str());
  int lungime = strlen(mesaj) + 1;
  for (int i = 1; i <= n; ++i)
  {
    std::cout << Clienti[i].socketul_fiecarui_client << '\n';
    if (Clienti[i].socketul_fiecarui_client != clientul_raportat)
    {

      std::cout << "AM INTRAT AICI \n";
      if ((write(Clienti[i].socketul_fiecarui_client, &lungime, sizeof(int)) <= 0) || (write(Clienti[i].socketul_fiecarui_client, &mesaj, lungime) <= 0))
      {
        perror("eroare la write catre toti clientii");
        return 0;
      }
    }
  }
  // static char mesaj[100];
  // strcpy(mesaj, "lala");
  // return mesaj;
  return 1;
}

int main(int argc, char *argv[])
{
  struct sockaddr_in server;
  void threadCreate(int);

  if (argc < 2)
  {
    fprintf(stderr, "Eroare: Primul argument este numarul de fire de executie...");
    exit(1);
  }
  nthreads = atoi(argv[1]);
  if (nthreads <= 0)
  {
    fprintf(stderr, "Eroare: Numar de fire invalid...");
    exit(1);
  }
  threadsPool = (Thread *)calloc(sizeof(Thread), nthreads);

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[server]Eroare la socket().\n");
    return errno;
  }

  int on = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  /* acceptam orice adresa */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT);

  /* atasam socketul */
  if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[server]Eroare la bind().\n");
    return errno;
  }

  if (listen(sd, 2) == -1)
  {
    perror("[server]Eroare la listen().\n");
    return errno;
  }

  printf("Nr threaduri %d \n", nthreads);
  fflush(stdout);
  int i;
  for (i = 0; i < nthreads; i++)
    threadCreate(i);

  /* servim in mod concurent clientii...folosind thread-uri */
  for (;;)
  {
    printf("[server]Asteptam la portul %d...\n", PORT);
    pause();
  }
  free(threadsPool);
};

void threadCreate(int i)
{
  void *treat(void *);
  int *t = (int *)malloc(sizeof(int));
  *t = i;
  pthread_create(&threadsPool[i].idThread, NULL, &treat, (void *)t);
  // pthread_create(&threadsPool[i].idThread, NULL, &treat, (void *)(intptr_t)i);
  return;
}

void *treat(void *arg)
{
  int client;
  int *t = (int *)arg;
  int idT = *t;

  struct sockaddr_in from;
  bzero(&from, sizeof(from));
  printf("[thread]- %d - pornit...\n", idT);
  fflush(stdout);
  free(t);

  for (;;)
  {
    socklen_t length = sizeof(from);
    pthread_mutex_lock(&mlock);
    // printf("Thread %d trezit\n",(int)arg);
    if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
    {
      perror("[thread]Eroare la accept().\n");
    }
    pthread_mutex_unlock(&mlock);
    threadsPool[idT].thCount++;

    raspunde(client, idT);
    close(client);
  }
}

void raspunde(int cl, int idThread)
{
  // char comanda2[50];
  // char comanda3[50];
  // char comanda4[50];
  int nr;
  int ruta_lungime[50] = {0};
  // char mesaj[100];
  // memset(mesaj, 0, MAX_LEN);
  int lungime = 0;
  char mesaj_de_la_client[MAX_LEN];
  // memset(mesaj_de_la_client, 100, 0);
  // clock_t t_start;
  bool transmitere = true;
  User client;
  numar_conectati++;
  bool clientulRuleaza = true;
  int indexClient = numar_conectati;
  Clienti_conectati[indexClient].socketul_fiecarui_client = cl;
  Clienti_conectati[indexClient].idThread = idThread;
  Clienti_conectati[indexClient].index = indexClient;
  if (realizare_matrice(matrice_cartiere, matrice_lungime, matrice_viteza) == false)
  {
    std::cout << "Nu s-a putut realiza matricea";
  }
  int locatie1 = 0;
  int locatie2 = 0;
  std::vector<std::pair<int, int>> ruta;
  // const std::vector<std::pair<int, int>>& ruta
  while (clientulRuleaza)
  {
    char mesaj[MAX_LEN];
    memset(mesaj, 0, MAX_LEN);
    // sem_wait(&cod_rosu);
    char copie_mesaj[MAX_LEN];
    int viteza = 0;
    memset(mesaj_de_la_client, 0, MAX_LEN);
    if ((read(cl, &nr, sizeof(int)) <= 0) || (read(cl, &mesaj_de_la_client, nr) <= 0))
    {
      printf("[Thread %d]\n", idThread);
      perror("Eroare la read() de la client.\n");
      return;
    }
    printf("[Thread %d]Mesajul a fost receptionat...%s\n", idThread, mesaj_de_la_client);
    std::string inapoi;
    memset(copie_mesaj, 0, MAX_LEN);
    strcpy(copie_mesaj, mesaj_de_la_client);
    strcat(copie_mesaj, "\0");
    char *pointer = strtok(copie_mesaj, " ");
    char strada[50];
    char prima_comanda[10];
    strcpy(prima_comanda, pointer);
    int numar = atoi(prima_comanda);
    int numar2 = numar;
    memset(mesaj, 0, MAX_LEN);
    if (numar == 1)
    {
      char comanda2[50];
      char comanda3[50];
      pointer = strtok(NULL, " ");
      strcpy(comanda2, pointer);
      pointer = strtok(NULL, " ");
      strcpy(comanda3, pointer);
      std::string altceva2(comanda2);
      std::string altceva3(comanda3);
      std::cout << altceva2 << '\n'
                << altceva3 << '\n';
      std::cout << "LALA";
      std::string inapoi;
      // if (client.login(comanda2, comanda3) == "V-ati conectat cu succes")
      //   strcpy(mesaj, "V-ati inregistrat cu succes");
      // else
      //   strcpy(mesaj, "Utilizator sau parola gresita");
      inapoi = login(comanda2, comanda3, Clienti_conectati[indexClient]);
      strcpy(mesaj, inapoi.c_str());
      lungime = strlen(mesaj);
      // if(inapoi=="V-ati logat cu succes"){
      //   if(matrice_catre_server(matrice_cartiere, matrice_lungime, cl)==false){
      //     std::cout<<"Nu s-a realizat trimiterea matricelor";
    }
    // client.login(strada);
    else if (numar == 2)
    {
      if (Clienti_conectati[indexClient].conectat == true)
      {
        // if ((read(sd, &viteza, sizeof(int)) <= 0))
        // {
        //   perror("eroare la read viteza");
        //   return;
        // }
        // Clienti_conectati[indexClient].viteza = viteza;
        // std::cout << " VITEZA " << Clienti_conectati[indexClient].viteza << "\n";
        char comanda2[50];
        char comanda3[50];
        char comanda4[50];
        pointer = strtok(NULL, " ");
        strcpy(comanda2, pointer);
        pointer = strtok(NULL, " ");
        strcpy(comanda3, pointer);
        pointer = strtok(NULL, " ");
        strcpy(comanda4, pointer);
        Clienti_conectati[indexClient].viteza = std::stoi(comanda4);
        std::cout << Clienti_conectati[indexClient].viteza << '\n';
        std::string altceva2(comanda2);
        std::string altceva3(comanda3);
        std::cout << altceva2 << '\n'
                  << altceva3 << '\n';
        int viteza;

        if (localizare(altceva2, altceva3, Clienti_conectati[indexClient].id_start, Clienti_conectati[indexClient].id_finish) == true)
        {
          if (traseu(matrice_cartiere, matrice_lungime, Clienti_conectati[indexClient].id_start, Clienti_conectati[indexClient].id_finish, ruta) > 0)
          {
            Clienti_conectati[indexClient].distanta_totala = calculeaza_distanta(matrice_lungime, ruta_lungime, ruta);
            // calculeaza_timp(ruta_lungime);
            //  client.inceput_traseu = true;
            //  Clienti_conectati[indexClient].distantaParcursa+=
            std::cout << Clienti_conectati[indexClient].distanta_totala;
            Clienti_conectati[indexClient].distantaParcursa = 0;
            strcpy(mesaj, "S-a realizat traseul\n");
            Clienti_conectati[indexClient].inceput_traseu = true;
            startTime = std::chrono::steady_clock::now();
            // Clienti_conectati[indexClient].t_start = clock();
          }
        }
        else
        {
          strcpy(mesaj, "Incercati alte cartiere\n");
        }
        // strcpy(mesaj, "introduceti viteza dumneavoastra");
        // printf("lal");
        // lungime = strlen(mesaj);
        // Clienti_conectati[indexClient].alege_viteza(viteza);
      }
      else
      {
        strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      }
    }
    else if (numar == 3)
    {
      if (Clienti_conectati[indexClient].conectat == true)
      {
        char comanda2[50];
        pointer = strtok(NULL, " ");
        strcpy(comanda2, pointer);
        std::string altceva2(comanda2);
        if (altceva2 == "y" || altceva2 == "Y")
        {
          Clienti_conectati[indexClient].info_vreme = true;
          strcpy(mesaj, "De acum o sa primiti si informatii despre vreme\n");
        }
        else if (altceva2 == "n" || altceva2 == "N")
        {
          Clienti_conectati[indexClient].info_vreme = false;
          strcpy(mesaj, "Nu o sa primiti informatii despre vreme\n");
        }
        else
        {
          strcpy(mesaj, "Nu am inteles raspunsul\n");
        }
      }
      else
      {
        strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      }
    }
    else if (numar == 4)
    {
      if (Clienti_conectati[indexClient].conectat == true)
      {
        char comanda2[50];
        pointer = strtok(NULL, " ");
        strcpy(comanda2, pointer);
        std::string altceva2(comanda2);
        if (altceva2 == "y" || altceva2 == "Y")
        {
          Clienti_conectati[indexClient].info_sport = true;
          strcpy(mesaj, "De acum o sa primiti si stiri despre sport\n");
        }
        else if (altceva2 == "n" || altceva2 == "N")
        {
          Clienti_conectati[indexClient].info_sport = false;
          strcpy(mesaj, "Nu o sa primiti stiri despre sport\n");
        }
        else
        {
          strcpy(mesaj, "Nu am inteles raspunsul\n");
        }
      }
      else
      {
        strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      }
    }
    else if (numar == 5)
    {
      if (Clienti_conectati[indexClient].conectat == true)
      {
        char comanda2[50];
        pointer = strtok(NULL, " ");
        strcpy(comanda2, pointer);
        std::string altceva2(comanda2);
        if (Clienti_conectati[indexClient].inceput_traseu == true)
        {
          if (altceva2 == "y" || altceva2 == "Y")
          {
            Clienti_conectati[indexClient].info_PECO = true;
            strcpy(mesaj, "De acum o sa primiti preturi de la cel mai apropiat PECO \n");
          }
          else if (altceva2 == "n" || altceva2 == "N")
          {
            Clienti_conectati[indexClient].info_PECO = false;
            strcpy(mesaj, "Nu o sa primiti primiti preturi de la PECO\n");
          }
          else
          {
            strcpy(mesaj, "Nu am inteles raspunsul\n");
          }
        }
        else
        {
          strcpy(mesaj, "Avem nevoie de locatia dumneavoastra mai intai\n");
        }
      }
      else
      {
        strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      }
    }
    else if (numar == 6)
    {
      if (Clienti_conectati[indexClient].conectat == true)
      {
        char comanda2[50];
        pointer = strtok(NULL, " ");
        strcpy(comanda2, pointer);
        std::string altceva2(comanda2);
        if (Clienti_conectati[indexClient].inceput_traseu == true)
        {
          if (altceva2 == "y" || altceva2 == "Y")
          {
            // Clienti_conectati[indexClient].info_PECO = true;
            accident = true;
            transmitere = false;
            strcpy(mesaj, "Se trimite accidentul \n");
          }
          else if (altceva2 == "n" || altceva2 == "N")
          {
            // Clienti_conectati[indexClient].info_PECO = false;
            strcpy(mesaj, "In regula\n");
          }
          else
          {
            strcpy(mesaj, "Nu am inteles raspunsul\n");
          }
        }
        else
        {
          strcpy(mesaj, "Avem nevoie de locatia dumneavoastra mai intai\n");
        }
      }
      else
      {
        strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      }
      // if (Clienti_conectati[indexClient].conectat == true)
      // {
      //   char comanda2[50];
      //   char comanda3[50];
      //   pointer = strtok(NULL, " ");
      //   strcpy(comanda2, pointer);
      //   pointer = strtok(NULL, " ");
      //   strcpy(comanda3, pointer);
      //   std::string altceva2(comanda2);
      //   std::string altceva3(comanda3);
      //   std::cout << altceva2 << '\n'
      //             << altceva3 << '\n';
      //   int max = 0;
      //   // int ceva=0;
      //   for (int i = 0; i < Clienti_conectati.size(); i++)
      //   {
      //     // if (Clienti_conectati[indexClient].socketul_fiecarui_client > max)
      //     // {
      //     //   max = Clienti_conectati[indexClient].socketul_fiecarui_client;
      //     // }
      //   }
      //   if (Clienti_conectati[indexClient].accident(altceva2, altceva3, Clienti_conectati, numar_conectati, Clienti_conectati[indexClient].socketul_fiecarui_client))
      //   {
      //     if (numar_conectati == 1)
      //     {
      //       strcpy(mesaj, "Nu exista alti utilizatori conectati catre care sa se raporteze accidentul");
      //     }
      //     else
      //       strcpy(mesaj, "S-a raportat accidentul cu succes");
      //   }
      //   else
      //   {
      //     strcpy(mesaj, "Nu s-a raportat accidentul");
      //   }
      // }
      // else
      // {
      //   strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      // }
      // std::cout << "LALA";
      // std::string inapoi;
      // // if (client.login(comanda2, comanda3) == "V-ati conectat cu succes")
      // //   strcpy(mesaj, "V-ati inregistrat cu succes");
      // // else
      // //   strcpy(mesaj, "Utilizator sau parola gresita");
      // inapoi = Clienti_conectati[indexClient].accident(altceva2, altceva3);
      // strcpy(mesaj, inapoi.c_str());
      // lungime = strlen(mesaj);

      // char locatie[50];
      //  strcpy(mesaj, "S-a primit raportarea accidentului. ");
      //  lungime = strlen(mesaj);
      // Clienti_conectati[indexClient].accident(locatie);
      // distribuirea_accidentului = true;
    }
    else if (numar == 7)
    {
      if (Clienti_conectati[indexClient].conectat == true)
      {
        if (Clienti_conectati[indexClient].logout() == true)
          strcpy(mesaj, "V-ati delogat cu succes");
        else
          strcpy(mesaj, "Nu sunteti logat");
      }
      else
      {
        strcpy(mesaj, "Nu sunteti logat, incearca comanda [1]");
      }

      lungime = strlen(mesaj);
    }
    if (numar == 8)
    {
      if (Clienti_conectati[indexClient].logout() == true)
        strcpy(mesaj, "Ati iesit din program");
      else
        strcpy(mesaj, "Ati iesit din program,nu erati logat");
      lungime = strlen(mesaj);
      clientulRuleaza = false;
    }
    if (numar == 9)
    {
      if (Clienti_conectati[indexClient].inceput_traseu == false && Clienti_conectati[indexClient].info_sport == false && Clienti_conectati[indexClient].info_vreme == false)
        strcpy(mesaj, "Inca nu sunt informatii de transmis.");
    }
    if (numar == 10)
    {
      
      if (Clienti_conectati[indexClient].conectat == false)
      {char comanda2[50];
      char comanda3[50];
      char comanda5[50];
      pointer = strtok(NULL, " ");
      strcpy(comanda2, pointer);
      pointer = strtok(NULL, " ");
      strcpy(comanda3, pointer);
      pointer = strtok(NULL, " ");
      strcpy(comanda5, pointer);
        std::string altceva2(comanda2);
        std::string altceva3(comanda3);
        std::string altceva5(comanda5);
        std::cout << altceva2 << '\n'
                  << altceva3 << '\n'
                  << altceva5 << '\n';
        std::cout << "LALA";
        std::string inapoi;
        if (altceva3 != altceva5)
        {
          strcpy(mesaj, "Parolele NU sunt la fel.");
        }
        else
        {
          inapoi = inregistrare(comanda2, comanda3);
          strcpy(mesaj, inapoi.c_str());
          lungime = strlen(mesaj);
        }
      }
      else{
        strcpy(mesaj, "Trebuie sa va delogati pentru aceasta comanda.");
      }
    }
    char comanda4[50];
    if (numar < 7 && numar > 1 && numar != 2)
    {
      pointer = strtok(NULL, " ");
      strcpy(comanda4, pointer);
      Clienti_conectati[indexClient].viteza = std::stoi(comanda4);
      std::cout << Clienti_conectati[indexClient].viteza << '\n';
    }
    // Clienti_conectati.push_back(client);
    // strcpy(mesaj, "Comanda incorecta");
    // lungime = strlen(mesaj);
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct std::tm *ptm = std::localtime(&currentTime);
    if (Clienti_conectati[indexClient].info_sport == true && Clienti_conectati[indexClient].conectat == true)
    {
      // auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      // struct std::tm *ptm = std::localtime(&currentTime);
      int ora_actuala = ptm->tm_hour;

      if (ora_actuala > 12)
      {
        ora_actuala -= 12;
      }
      std::string inapoi = get_sport(ora_actuala);
      strcat(mesaj, "\n");
      strcat(mesaj, inapoi.c_str());
    }
    int distantaFacuta = 0;
    if (Clienti_conectati[indexClient].info_vreme == true && Clienti_conectati[indexClient].conectat == true)
    {
      int ora_actuala = ptm->tm_hour;

      if (ora_actuala > 12)
      {
        ora_actuala -= 12;
      }
      std::string inapoi = get_vreme(ora_actuala);
      strcat(mesaj, "\n");
      strcat(mesaj, inapoi.c_str());
    }
    // if (Clienti_conectati[indexClient].conectat == true && Clienti_conectati[indexClient].inceput_traseu == true && Clienti_conectati[indexClient].info_PECO == true)
    // {
    //   std::string inapoi = get_PECO();
    //   strcat(mesaj, "\n");
    //   strcat(mesaj, inapoi.c_str());
    // }
    if (Clienti_conectati[indexClient].inceput_traseu == true && numar != 7 && numar != 8 && Clienti_conectati[indexClient].conectat == true)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1)); // Pauză de 1 secundă
      std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
      // Afișează timpul trecut în secunde
      secondsPassed = elapsedSeconds.count();
      std::cout << "Timp trecut: " << secondsPassed << " secunde\n";
      // std::thread timeThread(timeUpdate);
      //  double durata = (double)Clienti_conectati[indexClient].t_start- Clienti_conectati[indexClient].t_finish;
      int durata = (int)secondsPassed;
      // Clienti_conectati[indexClient].t_finish = clock();
      Clienti_conectati[indexClient].distantaParcursa += parcursa(Clienti_conectati[indexClient].viteza, durata);

      distantaFacuta = (int)Clienti_conectati[indexClient].distantaParcursa;
      int distantaRamasa = Clienti_conectati[indexClient].distanta_totala - distantaFacuta;
      std::cout << ' ' << distantaRamasa << '\n';
      std::cout << distantaFacuta << '\n';
      // if ( Clienti_conectati[indexClient].info_PECO == true)
      // {
      //   std::string inapoi = get_PECO(distantaFacuta,ruta_lungime);
      //   strcat(mesaj, "\n");
      //   strcat(mesaj, inapoi.c_str());
      // }

      if (Clienti_conectati[indexClient].distantaParcursa >= (double)Clienti_conectati[indexClient].distanta_totala)
      {

        strcat(mesaj, "\n");
        strcat(mesaj, "[Traseu]:Ati ajuns la destinatie.\n");
        Clienti_conectati[indexClient].inceput_traseu = false;
      }
      else
      {
        // std::string stringAccident = "";
        locatie1 = Clienti_conectati[indexClient].id_loc1;
        locatie2 = Clienti_conectati[indexClient].id_loc2;
        char charAccident[MAX_LEN];
        char cartier[MAX_LEN];
        memset(cartier, 0, MAX_LEN);
        std::string cartier1 = cartierul_meu(distantaFacuta, ruta_lungime, Clienti_conectati[indexClient].info_PECO, ruta, Clienti_conectati[indexClient].id_loc1, Clienti_conectati[indexClient].id_loc2);
        strcpy(cartier, cartier1.c_str());
        std::string aglomeratie = "";
        aglomeratie = vezi_aglomeratie(matrice_aglomeratie, locatie1, locatie2, Clienti_conectati[indexClient].id_loc1, Clienti_conectati[indexClient].id_loc2);
        char pentruAglomeratie[MAX_LEN];
        strcpy(pentruAglomeratie, aglomeratie.c_str());

        std::string distanta1 = std::to_string(distantaRamasa);
        char distanta2[30];
        strcpy(distanta2, distanta1.c_str());
        strcat(mesaj, "\n");
        strcat(mesaj, "[Traseu]:");
        strcat(mesaj, cartier);
        strcat(mesaj, "\n");
        strcat(mesaj, pentruAglomeratie);
        strcat(mesaj, "[Traseu]:Mai aveti ");
        strcat(mesaj, distanta2);
        strcat(mesaj, " metri pana la destinatie\n");
        if (accident == true)
        {
          if (transmitere == false)
          {
            stringAccident += transmitere_accident(Clienti_conectati[indexClient].id_loc1, Clienti_conectati[indexClient].id_loc2);
            transmitere = true;
          }
          else if (transmitere == true)
          {
            strcpy(charAccident, stringAccident.c_str());
            strcat(mesaj, charAccident);
          }
        }
      }

      // Clienti_conectati[indexClient].t_start = Clienti_conectati[indexClient].t_finish;
    }
    // std::cout << "AM AJUNS";
    lungime = strlen(mesaj);
    printf("[Thread %d]Trimitem mesajul inapoi...%s\n", idThread, mesaj);
    lungime += 1;
    if ((write(cl, &lungime, sizeof(int)) <= 0) || (write(cl, &mesaj, lungime) <= 0))
    {
      printf("[Thread %d] ", idThread);
      perror("[Thread]Eroare la write() catre client.\n");
    }
    else
      printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", idThread);
  }
}
