#include "functii.h"
#include <string.h>
std::vector<Drum> drumuri;
std::vector<Cartier> cartiere;
// std::vector<std::pair<int, int>> ruta;
bool localizare(std::string Cartier1, std::string Cartier2, int &start, int &finish)
{
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
            std::cout << start;
            // break;
        }
        else if (cartiere[i].nume == Cartier2)
        {
            finish = cartiere[i].id;
            std::cout << finish;
            // break;
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
void afisare_drum(int drum_minim[], int destinatie, int matrice_lungime[M_MAX][M_MAX], std::vector<std::pair<int, int>> &ruta)
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
    std::cout << "\n";
    std::cout << "Drumul este: ";
    for (const auto &pair : ruta)
    {
        // distanta[i]=matrice_lungime[pair.first][pair.second];
        // i++;
        std::cout << pair.first << " -> " << pair.second << " ";
    }
    std::cout << "\n";
}
int traseu(bool (&matrice_cartiere)[M_MAX][M_MAX], int (&matrice_lungime)[M_MAX][M_MAX], int start, int finish, std::vector<std::pair<int, int>> &ruta)
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
    afisare_drum(drum_minim, finish, matrice_lungime, ruta);
    return 1;
}
int calculeaza_distanta(int matrice_lungime[M_MAX][M_MAX], int (&vector)[50], const std::vector<std::pair<int, int>> &ruta)
{
    int i = 0;
    int distanta = 0;
    for (const auto &pair : ruta)
    {
        vector[i] = matrice_lungime[pair.first][pair.second];
        std::cout << vector[i] << '\n';
        // std::cout << pair.first << " -> " << pair.second << " ";

        distanta += vector[i];
        i++;
    }
    std::cout << distanta << "TOTALA \n";
    return distanta;
}
// bool User::accident(std::string locatie, std::string numarul, std::vector<User> &Clienti)
// {
//     std::string catreTotiUtilizatorii;
//     catreTotiUtilizatorii += "!ACCIDENT! Strada " + locatie + numarul;
//     char mesaj[100];
//     strcpy(mesaj, catreTotiUtilizatorii.c_str());
//     int lungime = strlen(mesaj) + 1;
//     for (int i = 1; i <= useri_conectati_comun; ++i)
//     {
//         if (i != socketul_fiecarui_client)
//         {
//         if((write(Clienti[i].socketul_fiecarui_client,&lungime, sizeof(int))<=0)||(write(Clienti[i].socketul_fiecarui_client, &mesaj, lungime) <= 0))
//         perror("eroare la write catre toti clientii");
//         return 0;
//         }
//     }
//     // static char mesaj[100];
//     // strcpy(mesaj, "lala");
//     // return mesaj;
//     return 1;
// }

std::string login(std::string nume, std::string cheie, User &client)
{

    if (client.conectat == true)
    {
        std::string mesaj = "/" + client.nume_utilizator + "/ suntei conectat deja ";
        return mesaj;
    }
    XMLDocument fisierul;
    std::cout << "AICI";
    if (fisierul.LoadFile("Omuleti.xml") != XML_SUCCESS)
    {
        perror("Eroare la deschiderea xml (in functii.h)");
        return "0";
    }
    XMLElement *primul = fisierul.FirstChildElement("omuleti");
    std::string chestie1;
    std::string chestie2;
    // memset(chestie1,0,50);
    // memset(chestie2,0,50);
    XMLElement *omulet = primul->FirstChildElement("omulet");
    while (omulet)
    {
        if ((omulet->Attribute("utilizator")) && (omulet->Attribute("parola")))
        {
            chestie1 = omulet->Attribute("utilizator");
            chestie2 = omulet->Attribute("parola");
            if ((nume == chestie1 && cheie == chestie2))
            {
                std::string esteConectat = omulet->Attribute("conectat");
                if (esteConectat == "da")
                {
                    std::string mesaj = "Utilizatorul /" + nume + "/ conectat deja ";
                    return mesaj;
                }

                client.nume_utilizator = nume;
                client.info_vreme = false;
                client.inceput_traseu = false;
                client.info_sport = false;
                client.info_PECO = false;
                client.conectat = true;
                omulet->SetAttribute("conectat", "da");
                if (fisierul.SaveFile("Omuleti.xml") != XML_SUCCESS)
                {
                    perror("error la salvare xml");
                    return "Nu s-a salvat in fisier";
                }
                std::cout << client.conectat << "CONECTAT \n";
                return "V-ati conectat cu succes";
            }
            omulet = omulet->NextSiblingElement("omulet");
        }
    }
    return "Nume sau parola gresita";
}

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
bool User::logout()
{
    std::string chestie1;
    std::string chestie2;
    if (this->conectat == true)
    {
        this->conectat = false;
        XMLDocument fisierul;
        // std::cout << "AICI";
        if (fisierul.LoadFile("Omuleti.xml") != XML_SUCCESS)
        {
            perror("Eroare la deschiderea xml (in functii.h)");
            return false;
        }
        XMLElement *primul = fisierul.FirstChildElement("omuleti");
        XMLElement *omulet = primul->FirstChildElement("omulet");
        while (omulet)
        {
            chestie1 = omulet->Attribute("utilizator");
            if (chestie1 == this->nume_utilizator)
            {
                omulet->SetAttribute("conectat", "nu");
                if (fisierul.SaveFile("Omuleti.xml") != XML_SUCCESS)
                {
                    perror("error la salvare xml");
                    return 0;
                }
                return true;
            }
            omulet = omulet->NextSiblingElement("omulet");
        }
    }
    return false;
}

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
        tinyxml2::XMLElement *pecoElement = copil->FirstChildElement("peco");
        if (pecoElement)
        {
            Peco pecoInfo;
            pecoInfo.nume = pecoElement->FirstChildElement("nume")->GetText();
            pecoInfo.benzina = std::stod(pecoElement->FirstChildElement("pret_benzina")->GetText());
            pecoInfo.motorina = std::stod(pecoElement->FirstChildElement("pret_motorina")->GetText());
            drum.peco = pecoInfo;
        }
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
                if ((drum.id_cartier1 == cartiere[i - 1].id) && (drum.id_cartier2 == cartiere[j - 1].id))
                {
                    matrice_cartiere[i][j] = true;
                    matrice_cartiere[j][i] = true;
                    matrice_lungime[i][j] = drum.lungime;
                    matrice_viteza[i][j] = drum.limita;
                    matrice_lungime[j][i] = drum.lungime;
                    matrice_viteza[j][i] = drum.limita;
                    break;
                }
            }
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
// int calculeaza_timp(int (&vector)[50])
// {
//     return 1;
// }
double parcursa(int viteza, double durata)
{
    double viteza2 = (double)viteza;
    double distanta = viteza2 * (durata / 10.0);
    std::cout << "(parcursa):" << distanta << ' ' << viteza << ' ' << durata / 10.0 << '\n';
    return distanta;
}
std::string cartierul_meu(int distanta, int (&vector)[50], bool pret, const std::vector<std::pair<int, int>> &ruta, int &id_first, int &id_second)
{
    int distantaTotala = 0;
    int i = 0;
    id_second = 0;
    id_first = 0;
    for (const auto &pair : ruta)
    {
        distantaTotala += vector[i];
        std::cout << pair.first << "CARTIER\n";
        if (distanta < distantaTotala)
        {

            id_first = pair.first;
            id_second = pair.second;
            break;
        }
        std::cout << pair.first << " eu-> " << pair.second << " ";
        i++;
    }
    std::cout << id_first << "    CU   " << id_second << "\n";
    std::string mesaj = "";
    // std::reverse(cartiere.begin(), cartiere.end());
    for (const auto &drum : drumuri)
    {
        // std::cout << cartier.id << " " << cartier.nume << "\n";
        if (((id_first == drum.id_cartier1) && (id_second == drum.id_cartier2)) || ((id_second == drum.id_cartier1) && (id_first == drum.id_cartier2)))
        {
            if (pret == true)
            {
                std::string benzina = std::to_string(drum.peco.benzina);
                std::string motorina = std::to_string(drum.peco.motorina);
                std::string benzinaZecimale = benzina.substr(0, benzina.find('.') + 3);
                std::string motorinaZecimale = motorina.substr(0, motorina.find('.') + 3);
                mesaj += "[PECO]Sunteti pe: " + drum.nume + " si aveti in drum: \n" + drum.peco.nume + ":\n" + "benzina: " + benzinaZecimale + "\n" + "motorina: " + motorinaZecimale + "\n";
            }
            std::string limita = std::to_string(drum.limita);
            mesaj += "ATENTIE! Limita viteza: " + limita + " pe " + drum.nume + "\n";
            break;
        }
    }
    bool gasit1 = false;
    bool gasit2 = false;
    for (const auto &cartier : cartiere)
    {
        std::cout << cartier.id << " " << cartier.nume << "\n";
        if (id_first == cartier.id)
        {
            mesaj += "Sunteti in cartierul " + cartier.nume;
            gasit1 = true;
        }
        if (id_second == cartier.id)
        {
            gasit2 = true;
            mesaj += " si va indreptati catre cartierul " + cartier.nume + "\n";
        }
        if (gasit1 == true && gasit2 == true)
        {
            return mesaj;
            break;
        }
    }
    return "NU";
}
std::string get_vreme(int oraReala)
{
    tinyxml2::XMLDocument vreme;
    std::cout << "aici\n";
    // vreme.LoadFile("Vreme.xml");
    if (vreme.LoadFile("Vreme.xml") != 0)
    {
        std::cout << "aici2\n";
        perror("Eroare la deschiderea Drumuri.xml (in functii_client.h)");
        return "NU S-A PUTUT DECSHIDE VREMEA";
    }
    std::cout << "aici3\n";
    tinyxml2::XMLElement *root = vreme.FirstChildElement("vreme");
    std::string mesaj = "";
    for (tinyxml2::XMLElement *copil = root->FirstChildElement("ora"); copil != nullptr; copil = copil->NextSiblingElement("ora"))
    {
        int ora = std::stoi(copil->FirstChildElement("ora")->GetText());
        int grade = std::stoi(copil->FirstChildElement("grade")->GetText());
        std::string tip = copil->FirstChildElement("tip")->GetText();
        std::string oramea = std::to_string(ora);
        std::string gradeEu = std::to_string(grade);
        std::cout << "VREME\n";
        if (ora == oraReala)
        {
            std::cout << "VREME\n";
            mesaj = "[Vreme]:La ora " + oramea + ":00 s-au anuntat " + gradeEu + " grade, iar tipul de vreme este: " + tip + "\n";
            return mesaj;
        }
        // std::cout << "drumuri ";
    }
    return "NU";
}
std::string get_sport(int oraReala)
{
    tinyxml2::XMLDocument sport;
    std::cout << "aici_sport\n";
    // vreme.LoadFile("Vreme.xml");
    if (sport.LoadFile("Sport.xml") != 0)
    {
        std::cout << "aici2\n";
        perror("Eroare la deschiderea Drumuri.xml (in functii_client.h)");
        return "NU S-A PUTUT DECSHIDE SPORT";
    }
    std::cout << "aici3\n";
    tinyxml2::XMLElement *root = sport.FirstChildElement("sport");
    std::string mesaj = "";
    for (tinyxml2::XMLElement *copil = root->FirstChildElement("ora"); copil != nullptr; copil = copil->NextSiblingElement("ora"))
    {
        int ora = std::stoi(copil->FirstChildElement("ora")->GetText());
        std::string stire = copil->FirstChildElement("stire")->GetText();
        std::string oramea = std::to_string(ora);
        // std::string gradeEu = std::to_string(grade);
        std::cout << "Sport\n";
        if (ora == oraReala)
        {
            std::cout << "sport\n";
            mesaj = "[Sport]:La ora " + oramea + ":00 are loc " + stire + "\n";
            return mesaj;
        }
    }
    return "NU";
}
// std::string get_PECO(int distanta, int (&vector)[50])
// {
// }
std::string vezi_aglomeratie(int (&matrice)[M_MAX][M_MAX], int precedenta1, int precedenta2, int curenta1, int curenta2)
{
    std::string mesaj = "";
    if (precedenta1 != 0 && precedenta1 != 0)
    {
        matrice[precedenta1][precedenta2]--;
        matrice[precedenta2][precedenta1]--;
    }

    if ((matrice[curenta1][curenta2] > 0) || (matrice[curenta2][curenta1] > 0))
    {
        std::string numar=std::to_string  (matrice[curenta1][curenta2]);
        mesaj += "Strada pe care va aflati este aglomerata: mai sunt "+ numar+ " masini.\n";
    }
    else
    {
        mesaj += "Strada pe care va aflati este libera.\n";
    }
    matrice[curenta1][curenta2]++;
    matrice[curenta2][curenta1]++;
    return mesaj;
}
std::string transmitere_accident(int id_first, int id_second)
{
    std::string mesaj = "";
    for (const auto &drum : drumuri)
    {
        // std::cout << cartier.id << " " << cartier.nume << "\n";
        if (((id_first == drum.id_cartier1) && (id_second == drum.id_cartier2)) || ((id_second == drum.id_cartier1) && (id_first == drum.id_cartier2)))
        {
             std::string drumul = drum.nume;
            for (char &c : drumul)
            {
                c = std::toupper(c);
            }
            mesaj += "ATENTIE! ACCIDENT "+ drumul+ "!\n";
            break;
        }
    }
    return mesaj;
}
std::string inregistrare(std::string nume, std::string parola){
     tinyxml2::XMLDocument fisierul;
    if (fisierul.LoadFile("Omuleti.xml") != tinyxml2::XML_SUCCESS) {
        std::cout<< "Eroare la deschiderea xml (in functii.h)\n";
        return "Eroare la deschiderea xml (in functii.h inregistrare)";
    }

    tinyxml2::XMLElement* radacina = fisierul.FirstChildElement("omuleti");

    for (tinyxml2::XMLElement* omulet = radacina->FirstChildElement("omulet"); omulet; omulet = omulet->NextSiblingElement("omulet")) {
        const char* copil = omulet->Attribute("utilizator");
        if (copil && std::string(copil) == nume) {
            std::cout << "Exista deja acest nume de utilizator.\n";
            return "Exista deja acest nume de utilizator.";
        }
    }

    // Cream un nou element omulet si il adaugam
    tinyxml2::XMLElement* omuletNou = fisierul.NewElement("omulet");
    omuletNou->SetAttribute("utilizator", nume.c_str());
    omuletNou->SetAttribute("parola", parola.c_str());
    omuletNou->SetAttribute("conectat", "nu");
    omuletNou->SetAttribute("vreme", "nu");
    omuletNou->SetAttribute("PECO", "nu");
    omuletNou->SetAttribute("sport", "nu");

    radacina->InsertEndChild(omuletNou);

    // Salvăm schimbările în fișier
    if (fisierul.SaveFile("Omuleti.xml") != tinyxml2::XML_SUCCESS) {
        std::cout << "Eroare la salvarea XML-ului.\n";
        return "Eroare la salvarea XML-ului.";
    }

    return "V-ati inregistrat cu succes, acum va puteti loga.";
}
