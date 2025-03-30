#include <iostream>
#include <cstring>
#include <graphics.h>
#include <winbgim.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
using namespace std;

//----------------------------------------------------STRUCTURI--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct punct  /// coordonate
{
    int x, y;
};

struct dreptunghi ///pentru a crea butoane, folosing colturile Stanga Sus si Dreapta Jos
{
    punct SS, DJ;
};

struct buton ///caracteristici buton (n-am folosit culoare)
{
    dreptunghi D;
    int culoare;
    char text[20];
} B[35];

struct piese  ///painea jocului, unde initializam tipurile de piese, numele fiecarui jucator, culoarea fiecarei piese, numarul de piese (nu am folosit timp). Timp trebuia folosit pentru timer pe care nu l-am facut
{
    char gobblet[4][3], nume[101][101];
    int nr_mic=2, nr_mediu=2, nr_mare=2, clr = -1, nrNume = 0;
} jucator1,jucator2;

struct marcaj ///struct pentru clasament
{
    char nume[100];
    int punctaj=0;
} scor[100];

//----------------------------------------------------STRUCTURI--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------VARIABILE GLOBALE--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int width = 1920, height = 1080, caz;
int tip_piesa;
char matrice_mica[4][4], matrice_medie[4][4], matrice_mare[4][4],matrice_principala[4][4],simbol;
///-------IMPORTANT----- FOLOSIM 4 MATRICI DEOARECE FIECARE MATRICE REPREZINTA UN NIVEL SPECIFIC PE CARE SE POATE PLASA UN ANUME TIP DE PIESA. CELE MICI SE PUN DOAR PE MATRICE_MICA ETC,
///IAR MATRICEA_PRINCIPALA ESTE CEEA CE SE AFISEAZA GRAFIC
char copie_mica[4][4]= {"---","---","---"},copie_medie[4][4]= {"---","---","---"},copie_mare[4][4]= {"---","---","---"},copie_principala[4][4]= {"---","---","---"}; ///copii pentru undo
int tabla[4][4];
bool ok = true;
int culoare;
int turn = 0; ///randul
int mutare = 0;
int mutare_mica = 2,mutare_medie = 2, mutare_mare = 2, odata = 0;
int lmb = 1; ///lmb setata pe romana by default, 0 pt engleza
bool mutare_undo[2] = {0};
int pozitie_robot = 0, robot = 0, vector_pozitii_robot[10], jocRobot = 0, tipJoc = 0;
int numar_clasament = 0;
int volum = 100, melodie = 0, iarna = 0; ///volum setat pe 100 by default, melodia este 0, adica nu porneste nici o melodie odata cu programul, iarna schimba tema

//----------------------------------------------------VARIABILE GLOBALE--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------FUNCTII DEFINITE SUB MAIN----------------------------------------------------------------------------------------------------------------------------

void OpenGraph(); ///functie foarte simpla care deschide fereastra grafica in functie de width si height

void animatie_iarna(int &iarna); ///functie pentru tema de craciun

int click(int &xclick, int &yclick); ///functie care pastreaza coord click-ului si scoate un sunet cand se apasa click  (click stanga)

bool apartine(punct p, dreptunghi D); ///verificare daca coordonatele trimite de click se incadreaza intr-un buton

void iesire1(); ///functie pt buton de exit

void inapoi1();   ///functie pt buton de back

void iesire(punct pozC);

void inapoi(punct pozC); ///aceleasi butoane ca cele de mai sus,apar doar in jocul efectiv

void clasament(); ///functie care creeaza pagina de clasament

void alegere_dificultate(); ///functie de alegere a dificultatii

void DA_Robot();///variante de raspuns pentru functia de mai sus,dar pt robot

void DA();///variante de raspuns pentru functia de mai sus

void NU(); ///variante de raspuns pentru functia de mai sus

void jucati_din_nou(int jocRobot, int tipJoc); ///functie de repetare a jocului

void rezolutie(int &width, int &height); ///functie care creeaza pagina pentru schimbarea rezolutiei

void FHD(int &width, int &height);

void HD(int &width, int &height); ///functiile care chiar schimba rezolutia

void meniu(int iarna); ///functia care deschide meniul principal, in functie de tema aleasa, normala sau de iarna

void alegere_culoare(); ///functie care deschide prima fereastra inainte de joc, in care jucatorii isi aleg fiecare cate o culoare

void afisare_table(piese jucator,bool ok, int turn, int &odata);  ///functia care afiseaza tabla de joc dupa fiecare mutare noua

void paginaRegulament(); ///functie care deschide pagina regulamentului

void volum_zero(int &volum); ///functie care seteaza volumul muzicii la 0/ inchide muzica de fapt

void volum_jumate(int &volum); ///seteaza la 50%

void volum_full(int &volum); ///seteaza la 100%

///---------------MELODIILE-----------------------------------------------------------------------------------------------------------

void tzanca(int volum,int &melodie);

void opt_bit(int volum,int &melodie);

void creepin(int volum,int &melodie);

void rock(int volum,int &melodie);

void craciun(int volum,int &melodie);

///---------------MELODIILE---------------------------------------------------------------------------------------------------------------

void sunet(); ///functia care deschide pagina de pentru sunet

void setari(); ///functia care deschide pagina de setari

void rom(int &lmb); ///setare limba afisata in romana

void eng(int &lmb); ///setare limba afisata in engleza

void limba(); ///functie care deschide pagina pentru selectarea limbii

void verificare_buton(); ///cea mai importanta functie, cea care verifica daca a fost apasat un buton si face un switch case in functie de butonul apasat

void creare_scor(int &numar_clasament, piese jucator, int turn)
{
    cout<<scor[numar_clasament].nume<<" "<<scor[numar_clasament].punctaj<<endl;
    char aux2[100];
    int aux1;
    int gasitInClasament = 0;
    for(int i=0; i<=100; i++)
        if(strcmp(scor[i].nume, jucator.nume[jucator.nrNume]) == 0)///vede daca exista deja numele in tabela de scor
        {
            gasitInClasament = i;
            break;
        }
    if(gasitInClasament == 0)///daca nu l-o gasit pluseaza numele in clasament si introduce si numele ala
    {
        numar_clasament++;
        strcpy(scor[numar_clasament].nume,jucator.nume[jucator.nrNume]);
        if(turn % 2 == 1) scor[numar_clasament].punctaj = turn/2+1;
        else scor[numar_clasament].punctaj = turn/2;
    }
    else///daca l o gasit schimba valoarea scorului pt acel nume
    {
        numar_clasament = gasitInClasament;
        strcpy(scor[gasitInClasament].nume,jucator.nume[jucator.nrNume]);
        if(turn % 2 == 1)
        {
            if(scor[gasitInClasament].punctaj > turn/2+1)
                scor[gasitInClasament].punctaj = turn/2+1;
            else scor[numar_clasament].punctaj = scor[gasitInClasament].punctaj;
        }
        else
        {
            if(scor[gasitInClasament].punctaj > turn/2)
                scor[gasitInClasament].punctaj=turn/2;
            else scor[numar_clasament].punctaj = scor[gasitInClasament].punctaj;
        }
    }

    for(int i=0; i<numar_clasament-1; i++)///sortarea
        for(int j=i; j<numar_clasament; j++)
        {
            if(scor[i].punctaj > scor[j].punctaj)
            {
                aux1 = scor[i].punctaj;
                scor[i].punctaj = scor[j].punctaj;
                scor[j].punctaj = aux1;

                strcpy(aux2, scor[i].nume);
                strcpy(scor[i].nume, scor[j].nume);
                strcpy(scor[j].nume, aux2);
            }
        }
}

void creare_table() ///creeaza asa-zisa tabla de joc, cu pozitiile de la 1-9
{
    int i=0, j=0, pozitie_numerica = 0;
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            pozitie_numerica++;
            tabla[i][j] = pozitie_numerica;
        }
    }
}

bool verificare_pozitie(int tabla[4][4], int &i, int &j, int pozitie) ///verifica daca s-a introdus o pozitie valida si returneaza coordonatele sale (i,j) din matrice
{
    if(pozitie > 9 || pozitie < 1)
    {
        cout << "Pozitie incorecta,incercati din nou";
        cout << endl;
        return false;
    }
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
        {
            if(tabla[i][j] == pozitie)
                return true;
        }
}

bool verificare_catigator(char matrice_principala[4][4]) ///verifica daca a castigat cineva, intr-un mod foarte muncitoresc
{
    afisare_table(jucator1,ok,turn, odata);
    int contor1 = 0, contor2 = 0;
    char vec_jucator1[4];
    vec_jucator1[0] = jucator1.gobblet[0][1];  ///aici fiecare pozitie din vec_jucator primeste fiecare tip de piesa, 0 pentru mica, 1 pt medie, 2 pt mare
    vec_jucator1[1] = jucator1.gobblet[1][1];  /// noi am predefinit, pentru a usura codul, piesele in ordine mica, medie mare, X, Y, Z pt jucator 1 si A, B, C pt jucator 2
    vec_jucator1[2] = jucator1.gobblet[2][1];
    vec_jucator1[3] = '\0';
    char vec_jucator2[4];
    vec_jucator2[0] = jucator2.gobblet[0][1];
    vec_jucator2[1] = jucator2.gobblet[1][1];
    vec_jucator2[2] = jucator2.gobblet[2][1];
    vec_jucator2[3] = '\0';
    char aux[101];
    if(strchr(vec_jucator1, matrice_principala[0][0]) && strchr(vec_jucator1, matrice_principala[0][1]) && strchr(vec_jucator1, matrice_principala[0][2]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 1;
    }

    if(strchr(vec_jucator2, matrice_principala[0][0]) && strchr(vec_jucator2, matrice_principala[0][1]) && strchr(vec_jucator2, matrice_principala[0][2]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 1;
    }
    if(strchr(vec_jucator1, matrice_principala[1][0]) && strchr(vec_jucator1, matrice_principala[1][1]) && strchr(vec_jucator1, matrice_principala[1][2]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 2;
    }
    if (strchr(vec_jucator2, matrice_principala[1][0]) && strchr(vec_jucator2, matrice_principala[1][1]) && strchr(vec_jucator2, matrice_principala[1][2]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 2;
    }
    if(strchr(vec_jucator1, matrice_principala[2][0]) && strchr(vec_jucator1, matrice_principala[2][1]) && strchr(vec_jucator1, matrice_principala[2][2]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 3;
    }
    if (strchr(vec_jucator2, matrice_principala[2][0]) && strchr(vec_jucator2, matrice_principala[2][1]) && strchr(vec_jucator2, matrice_principala[2][2]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 3;
    }
    if(strchr(vec_jucator1, matrice_principala[0][0]) && strchr(vec_jucator1, matrice_principala[1][0]) && strchr(vec_jucator1, matrice_principala[2][0]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 4;
    }
    if (strchr(vec_jucator2, matrice_principala[0][0]) && strchr(vec_jucator2, matrice_principala[1][0]) && strchr(vec_jucator2, matrice_principala[2][0]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 4;
    }
    if(strchr(vec_jucator1, matrice_principala[0][1]) && strchr(vec_jucator1, matrice_principala[1][1]) && strchr(vec_jucator1, matrice_principala[2][1]))
    {
        cout << "Jucator 1 a castigat" <<endl;
        cout << endl;

        contor1 =  5;
    }
    if (strchr(vec_jucator2, matrice_principala[0][1]) && strchr(vec_jucator2, matrice_principala[1][1]) && strchr(vec_jucator2, matrice_principala[2][1]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 5;
    }
    if(strchr(vec_jucator1, matrice_principala[0][2]) && strchr(vec_jucator1, matrice_principala[1][2]) && strchr(vec_jucator1, matrice_principala[2][2]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 6;
    }
    if (strchr(vec_jucator2, matrice_principala[0][2]) && strchr(vec_jucator2, matrice_principala[1][2]) && strchr(vec_jucator2, matrice_principala[2][2]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 6;
    }
    if(strchr(vec_jucator1, matrice_principala[0][0]) && strchr(vec_jucator1, matrice_principala[1][1]) && strchr(vec_jucator1, matrice_principala[2][2]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 7;
    }
    if (strchr(vec_jucator2, matrice_principala[0][0]) && strchr(vec_jucator2, matrice_principala[1][1]) && strchr(vec_jucator2, matrice_principala[2][2]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 7;
    }
    if(strchr(vec_jucator1, matrice_principala[2][0]) && strchr(vec_jucator1, matrice_principala[1][1]) && strchr(vec_jucator1, matrice_principala[0][2]))
    {
        cout << "Jucator 1 a castigat" << endl;
        cout << endl;

        contor1 = 8;
    }
    if (strchr(vec_jucator2, matrice_principala[2][0]) && strchr(vec_jucator2, matrice_principala[1][1]) && strchr(vec_jucator2, matrice_principala[0][2]))
    {
        cout << "Jucator 2 a castigat" << endl;
        cout << endl;

        contor2 = 8;
    }

    if(contor1 != 0 && contor2 == 0) /// de aici sunt 5000 de linii de cod pentru animatia de castig a unui jucator
    {
        creare_scor(numar_clasament,jucator1,turn); ///pune in clasament numele castigatorului si numarul de mutari in care a terminat
        switch(contor1)
        {
        case 1:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,340,i);
                        }

                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,340,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                            circle(1160,340,i);

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                            circle(1160,340,i);

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,340,i);
                        }
                    }


                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,340,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,340,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,340,i);
                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,340,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,340,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,340,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,220,i);
                        }

                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,220,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                            circle(780,220,i);

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                            circle(780,220,i);

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,220,i);
                        }
                    }


                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,220,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,220,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,220,i);
                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,220,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,220,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,220,i);
                    }
                }
            }
            break;
        }
        case 2:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,540,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }

                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,540,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,540,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,540,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,540,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,540,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,360,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }

                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,360,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,360,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,360,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,360,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,360,i);
                    }
                }

            }
            break;
        }
        case 3:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,740,i);
                        }
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,740,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,740,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,740,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,740,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,740,i);
                        }
                    }


                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,740,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,500,i);
                        }
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,500,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,500,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,500,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,500,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,500,i);
                        }
                    }


                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,500,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,500,i);
                    }
                }
            }
            break;
        }
        case 4:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(760,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(760,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(760,740,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(760,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(760,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(760,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(760,540,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(760,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(760,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(500,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(500,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(500,500,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(500,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(500,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(500,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,360,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(500,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,500,i);
                    }
                }
            }
            break;
        }
        case 5:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(960,740,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(960,740,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(960,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(960,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(640,500,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(640,500,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(640,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(640,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,500,i);
                    }
                }
            }
            break;
        }
        case 6:
        {

            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(1160,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(1160,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(1160,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,740,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,540,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,740,i);

                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(780,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(780,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(780,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(780,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,500,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(780,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,360,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,500,i);
                    }
                }
            }
            break;
        }
        case 7:
        {

            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,740,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,500,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,500,i);
                    }
                }
            }
            break;
        }
        case 8:
        {

            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(1160,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(760,740,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(760,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=63; i++)
                            circle(760,740,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=83; i++)
                            circle(760,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(780,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {

                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='Y')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(500,500,i);
                        }

                    }
                    if (aux[0]=='Z')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(500,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(780,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='X')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='Y')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=47; i++)
                            circle(500,500,i);
                    }
                    if (aux[0]=='Z')
                    {
                        setcolor(jucator1.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,500,i);
                    }
                }
            }

            break;
        }
        }
        delay(3000);
        cleardevice();
        setcolor(jucator1.clr);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        for(int i=1; i<=6; i++)
        {
            delay(150);
            settextstyle(DEFAULT_FONT,HORIZ_DIR,i);
            if(lmb)
            {
                strcpy(aux, jucator1.nume[jucator1.nrNume]);
                strcat(aux, " A CASTIGAT");
                aux[strlen(aux)]='\0';
                //settextjustify(RIGHT_TEXT, CENTER_TEXT);
                //outtextxy(width/2, height/4,jucator1.nume[jucator1.nrNume]);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(width/2, height/4, aux);
            }

            else
            {
                strcpy(aux, jucator1.nume[jucator1.nrNume]);
                strcat(aux, " WON");
                aux[strlen(aux)]='\0';
                //settextjustify(RIGHT_TEXT, CENTER_TEXT);
                //outtextxy(width/2, height/4,jucator1.nume[jucator1.nrNume]);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(width/2, height/4,aux);
            }

        }
        jucati_din_nou(jocRobot, tipJoc);
        return true;
    }
    else if(contor2 != 0 && contor1 == 0)
    {
        creare_scor(numar_clasament,jucator2,turn);

        switch(contor2)
        {
        case 1:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,340,i);
                        }

                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,340,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,340,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,340,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,340,i);
                        }
                    }


                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,340,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,340,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,340,i);
                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,340,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,340,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,340,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,220,i);
                        }

                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,220,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,220,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,220,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,220,i);
                        }
                    }


                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,220,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,220,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,220,i);
                    }

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,220,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,220,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,220,i);
                    }
                }

            }
            break;
        }
        case 2:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,540,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }

                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,540,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,540,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,540,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,540,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,540,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,360,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }

                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,360,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,360,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,360,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,360,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,360,i);
                    }
                }
            }
            break;
        }
        case 3:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,740,i);
                        }
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,740,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,740,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,740,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,740,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,740,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,740,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,740,i);
                        }
                    }


                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,740,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,500,i);
                        }
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,500,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,500,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,500,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,500,i);
                        }

                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {
                            circle(780,500,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,500,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,500,i);
                        }
                    }


                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,500,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,500,i);
                    }
                }
            }
            break;
        }
        case 4:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(760,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(760,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,540,i);
                        }
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(760,740,i);
                        }
                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(760,740,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(760,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(760,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(760,540,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(760,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(760,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(500,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(500,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(500,500,i);
                        }

                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,500,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(500,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(500,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,360,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,500,i);
                    }
                }
            }
            break;
        }
        case 5:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(960,740,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(960,740,i);
                        }

                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {
                            circle(960,740,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(960,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(960,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(640,500,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(640,500,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(640,500,i);
                        }

                    }

                    delay(600);

                    aux[0] = matrice_principala[0][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(640,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(640,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[2][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,500,i);
                    }
                }
            }
            break;
        }
        case 6:
        {

            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(1160,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(1160,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(1160,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,740,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,740,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,540,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(780,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(780,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(780,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(780,360,i);
                        }

                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='B')
                    {


                        for(int i=1; i<=47; i++)
                        {

                            circle(780,500,i);
                        }

                    }
                    if (aux[0]=='C')
                    {

                        for(int i=1; i<=57; i++)
                        {
                            circle(780,500,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(780,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,360,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,500,i);
                    }
                }
            }
            break;
        }
        case 7:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(1160,740,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,740,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(760,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(1160,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(1160,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(1160,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';


                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(500,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(780,500,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {
                            circle(780,500,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(500,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,220,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[2][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(780,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(780,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(780,500,i);
                    }
                }
            }
            break;
        }
        case 8:
        {
            if(width == 1920)
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(1160,340,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(960,540,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {

                            circle(960,540,i);
                        }

                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=63; i++)
                        {

                            circle(760,740,i);
                        }

                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=83; i++)
                        {
                            circle(760,740,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                        {
                            circle(1160,340,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                        {
                            circle(1160,340,i);
                        }
                    }


                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(960,540,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(960,540,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(960,540,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=43; i++)
                            circle(760,740,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=63; i++)
                            circle(760,740,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=83; i++)
                            circle(760,740,i);
                    }
                }
            }
            else
            {
                for(int j=1; j<=5; j++)
                {
                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    setcolor(BLACK);

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(780,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        for(int i=1; i<=37; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(640,360,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {

                            circle(640,360,i);
                        }
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {

                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        for(int i=1; i<=47; i++)
                        {

                            circle(500,500,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        for(int i=1; i<=57; i++)
                        {
                            circle(500,500,i);
                        }
                    }

                    delay(600);

                    aux[0] = matrice_principala[0][2];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                        {
                            circle(780,220,i);
                        }
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                        {
                            circle(780,220,i);
                        }
                    }

                    aux[0] = matrice_principala[1][1];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(640,360,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(640,360,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(640,360,i);
                    }

                    aux[0] = matrice_principala[2][0];
                    aux[1] = '\0';

                    if(aux[0]=='A')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=37; i++)
                            circle(500,500,i);
                    }
                    if(aux[0]=='B')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=47; i++)
                            circle(500,500,i);
                    }
                    if (aux[0]=='C')
                    {
                        setcolor(jucator2.clr);
                        for(int i=1; i<=57; i++)
                            circle(500,500,i);
                    }
                }
            }
            break;
        }
        }



        delay(3000);
        cleardevice();
        setcolor(jucator2.clr);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        for(int i=1; i<=6; i++)
        {
            delay(150);
            settextstyle(DEFAULT_FONT,HORIZ_DIR,i);
            if(lmb)
            {
                strcpy(aux, jucator2.nume[jucator2.nrNume]);
                strcat(aux, " A CASTIGAT");
                aux[strlen(aux)]='\0';
                //settextjustify(RIGHT_TEXT, CENTER_TEXT);
                //outtextxy(width/2, height/4,jucator2.nume[jucator2.nrNume]);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(width/2, height/4, aux);
            }

            else
            {
                strcpy(aux, jucator2.nume[jucator2.nrNume]);
                strcat(aux, " WON");
                aux[strlen(aux)]='\0';
                //settextjustify(RIGHT_TEXT, CENTER_TEXT);
                //outtextxy(width/2, height/4,jucator2.nume[jucator2.nrNume]);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                outtextxy(width/2, height/4,aux);
            }

        }
        jucati_din_nou(jocRobot, tipJoc);
        return true;
    }
    else if(contor2 != 0 && contor1 != 0) ///remiza
    {
        setlinestyle(DOTTED_LINE, 1, 8);
        for(int i=1; i<=6; i++)
        {
            if(width == 1920)
            {

                setcolor(BLACK);

                setlinestyle(DOTTED_LINE, 1, 8);

                if(i%2 == 0)
                    setcolor(jucator1.clr);
                else setcolor(jucator2.clr);

                setlinestyle(DOTTED_LINE, 1, 8);

                setlinestyle(DOTTED_LINE, 1, 8);
            }
            else if(width == 1280)
            {
                //delay(600);

                setcolor(BLACK);
                setlinestyle(SOLID_LINE, 5, 8);

                if(i%2 == 0)
                    setcolor(jucator1.clr);
                else setcolor(jucator2.clr);


            }
        }

        delay(2000);
        cleardevice();
        setcolor(WHITE);
        for(int i=1; i<=6; i++)
        {
            delay(150);
            settextstyle(DEFAULT_FONT,HORIZ_DIR,i);
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            outtextxy(width/2, height/4, "REMIZA");
        }
        jucati_din_nou(jocRobot, tipJoc);
        return true;
    }
    else
        return false;
}
void alegeti_culoare(int &culoare) ///functie cam degeaba
{
    alegere_culoare();
}

void alegeti_simboluri(piese &jucator1, piese &jucator2) ///aici predefinim fiecare piesa a jucatorilor si tot aici se ape
{
    int i = 0;
    char simbol = 'X';
    for(; i<3; i++)
    {
        jucator1.gobblet[i][0] = simbol;
        jucator1.gobblet[i][1] = simbol;
        simbol++;
    }
    simbol = 'A';
    for(i=0; i<3; i++)
    {

        jucator2.gobblet[i][0] = simbol;
        jucator2.gobblet[i][1] = simbol;
        simbol++;
    }
}

void creare_matrici() ///crearea fiecarei matrice, folosita si ca recreere a matricilor
{
    for(int i=0; i<3; i++)
    {
        strcpy(matrice_mare[i], "---");
        strcpy(matrice_medie[i], "---");
        strcpy(matrice_mica[i], "---");
        strcpy(matrice_principala[i], "---");
    }
}

void creare_matrice_principala() ///functia care verifica ce piesa sa afiseze grafic, in functie de priotate
{

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            if(matrice_mare[i][j] != '-')
                matrice_principala[i][j] = matrice_mare[i][j];
            else if(matrice_medie[i][j] != '-')
                matrice_principala[i][j] = matrice_medie[i][j];
            else if(matrice_mica[i][j] != '-')
                matrice_principala[i][j] = matrice_mica[i][j];
        }
}

bool plasare_mica(piese &jucator, int &turn, int &pozitie) ///functie pentru plasarea unei piese mici pe tabla (se apasa click pe piesa, apoi click pe patratul din tabla unde se plaseaza)
{
    ///de asemenea functia verifica si daca se plaseaza corect piesa;
    int i = 0, j = 0;                                      ///idem pentru plasare_medie si plasare_mare
    if(verificare_pozitie(tabla, i, j, pozitie))
    {
        if(((matrice_mare[i][j] == '-' || matrice_medie[i][j] == '-' )&& matrice_principala[i][j] == '-' )&& jucator.nr_mic>0)  ///o matrice goala are de fapt, by default pus de noi, semnul '-'
        {
            if(jucator.nr_mic == 2)  ///aici verifica numarul de piese ramase
            {
                matrice_mica[i][j] = jucator.gobblet[0][1];
                jucator.nr_mic--;
                cout << "Mai aveti o piesa mica"<<endl;
                turn++;
                creare_matrice_principala();
                return true;
            }
            else if(jucator.nr_mic == 1)
            {
                matrice_mica[i][j] = jucator.gobblet[0][0];
                jucator.nr_mic--;
                cout << "Nu mai aveti piese mici"<<endl;
                turn++;
                creare_matrice_principala();
                return true;
            }
            else if(jucator.nr_mic < 1)
            {
                cout<<"Nu mai aveti piese deci nu aveti ce introduce"<<endl;
            }
        }
        else if(jucator.nr_mic > 0)
        {
            cout << "Pozitie deja ocupata"<< endl;
            return false;
        }
        else
        {
            cout<<"Nu mai aveti piese mici pe care sa le introduceti"<<endl;
            return false;
        }

    }
    return false;
}

bool plasare_medie(piese &jucator, int &turn, int &pozitie)
{
    int i = 0, j = 0;
    if(verificare_pozitie(tabla, i, j, pozitie))
    {
        if(matrice_principala[i][j] == '-' && jucator.nr_mediu > 0)
        {
            if(jucator.nr_mediu == 2)
            {
                matrice_medie[i][j] = jucator.gobblet[1][1];
                jucator.nr_mediu--;
                cout << "Mai aveti o piesa medie"<<endl;
                creare_matrice_principala();
                turn++;
                return true;
            }
            else if(jucator.nr_mediu == 1)
            {
                matrice_medie[i][j] = jucator.gobblet[1][0];
                jucator.nr_mediu--;
                cout << "Nu mai aveti piese medii"<<endl;
                creare_matrice_principala();
                turn++;
                return true;
            }
            else
                cout<<"Nu mai aveti piese deci nu aveti ce introduce"<<endl;
        }
        else if(matrice_mica[i][j] != '-' && jucator.nr_mediu > 0 && matrice_medie[i][j] == '-'&& matrice_mare[i][j] == '-')
        {
            if(jucator.nr_mediu == 2)
            {
                matrice_medie[i][j] = jucator.gobblet[1][1];
                jucator.nr_mediu--;
                cout << "Mai aveti o piesa medie"<<endl;
                creare_matrice_principala();
                turn++;
                return true;
            }
            else if(jucator.nr_mediu == 1)
            {
                matrice_medie[i][j] = jucator.gobblet[1][0];
                jucator.nr_mediu--;
                cout << "Nu mai aveti piese medii"<<endl;
                creare_matrice_principala();
                turn++;
                return true;
            }
            else
            {
                cout<<"Nu mai aveti piese deci nu aveti ce introduce"<<endl;
                return false;
            }

        }
        else if(jucator.nr_mediu > 0)
        {
            cout << "Pozitie deja ocupata"<< endl;
            return false;
        }

        else
        {
            cout<<"Nu mai aveti piese medii pe care sa le introduceti"<<endl;
            return false;
        }
    }
    return false;
}

bool plasare_mare(piese &jucator, int &turn, int &pozitie)
{
    int i = 0, j = 0;
    if(verificare_pozitie(tabla, i, j, pozitie))
    {
        if(matrice_principala[i][j] == '-' && jucator.nr_mare > 0)
        {
            switch(jucator.nr_mare)
            {
            case 2:
            {
                matrice_mare[i][j] = jucator.gobblet[2][1];
                jucator.nr_mare--;
                cout << "Mai aveti o piesa mare"<<endl;
                turn++;
                creare_matrice_principala();
                return true;
                break;
            }
            case 1:
            {
                matrice_mare[i][j] = jucator.gobblet[2][0];
                jucator.nr_mare--;
                cout << "Nu mai aveti piese mari"<<endl;
                turn++;
                creare_matrice_principala();
                return true;
                break;
            }
            default:
            {
                cout<<"Nu mai aveti piese deci nu aveti ce introduce"<<endl;
                return false;
            }
            }
        }
        else if(matrice_mare[i][j] == '-' && jucator.nr_mare > 0)
        {
            switch(jucator.nr_mare)
            {
            case 2:
            {
                matrice_mare[i][j] = jucator.gobblet[2][1];
                jucator.nr_mare--;
                cout << "Mai aveti o piesa mare"<<endl;
                turn++;
                creare_matrice_principala();
                return true;
                break;
            }
            case 1:
            {
                matrice_mare[i][j] = jucator.gobblet[2][0];
                jucator.nr_mare--;
                cout << "Nu mai aveti piese mari"<<endl;
                turn++;
                creare_matrice_principala();
                return true;
                break;
            }
            default:
            {
                cout << "Nu mai aveti piese, deci nu aveti ce introduce" << endl;
                return false;
            }
            }
        }
        else
        {
            cout << "Pozitie deja ocupata"<< endl;
            return false;
        }

    }
    return false;
}

bool verificare_pozitie_mutare(int &i, int &j, char &simbol,int &pozitie, piese jucator1) ///functie ce piesa se afla pe pozitia pe care se doreste a se face mutarea si daca mutarea se face de pe o pozitie corecta
{
    i = 0, j = 0;
    if(pozitie > 9 || pozitie < 1)
    {
        return false;
    }
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
        {
            if(tabla[i][j] == pozitie)
            {
                if(matrice_principala[i][j] == '-')
                {
                    cout << "Nu aveti element pe aceasta pozitie" << endl;
                    return false;
                }
                else
                {
                    char vec_jucator1[4]; ///aceeasi vectori ca mai sus, care primesc fie X, Y, Z fie A, B, C
                    vec_jucator1[0] = jucator1.gobblet[0][1];
                    vec_jucator1[1] = jucator1.gobblet[1][1];
                    vec_jucator1[2] = jucator1.gobblet[2][1];
                    vec_jucator1[3]='\0';
                    if(strchr(vec_jucator1, matrice_principala[i][j]))
                    {
                        if(matrice_principala[i][j] == vec_jucator1[0])
                        {
                            matrice_mica[i][j] = '-';
                            matrice_principala[i][j] = '-';
                            simbol = vec_jucator1[0];
                            return true;
                        }
                        else if(matrice_principala[i][j] == vec_jucator1[1])
                        {
                            matrice_medie[i][j] = '-';
                            matrice_principala[i][j] = '-';
                            simbol = vec_jucator1[1];
                            return true;
                        }
                        else if(matrice_principala[i][j] == vec_jucator1[2])
                        {
                            matrice_mare[i][j] = '-';
                            matrice_principala[i][j] = '-';
                            simbol = vec_jucator1[2];
                            return true;
                        }
                    }
                    else
                    {
                        cout << "Piesa nu este a dumneavoastra" << endl;
                        return false;
                    }
                }
            }
        }
}

bool verificare_caractere(piese jucator) /// daca avem piese pe tabla
{
    char vec_jucator[4];
    vec_jucator[0] = jucator.gobblet[0][1];
    vec_jucator[1] = jucator.gobblet[1][1];
    vec_jucator[2] = jucator.gobblet[2][1];
    vec_jucator[3] = '\0';
    int i=0,j=0;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
        {
            if(strchr(vec_jucator, matrice_principala[i][j]))
                return true;
        }
    return false;
}

void creare_copie() ///folosim copiile pentru undo
{
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            copie_mare[i][j]=matrice_mare[i][j];
            copie_medie[i][j]=matrice_medie[i][j];
            copie_mica[i][j]=matrice_mica[i][j];
        }
}

bool mutaremica(piese &jucator, int &turn, int &pozitie,int &pozitie_robot) /// functia facuta foarte muncitoresc care determina pe ce pozitie se face mutarea; idem pt mutaremedie si mutaremare
{
    int  i = 0, j = 0;
    if(pozitie_robot!=0)
        pozitie=pozitie_robot;
    else
        pozitie=-1;
    punct pozC;
    if(width == 1920)
    {
        while(1 && pozitie == -1)
        {
            click(pozC.x, pozC.y);
            if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
            {
                pozitie=1;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
            {
                pozitie=2;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
            {
                pozitie=3;
            }
            if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
            {
                pozitie=4;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
            {
                pozitie=5;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
            {
                pozitie=6;
            }
            if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
            {
                pozitie=7;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
            {
                pozitie=8;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
            {
                pozitie=9;
            }
        }
    }
    else if(width == 1280)
    {
        while(1 && pozitie == -1)
        {
            click(pozC.x,pozC.y);
            if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 1;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 2;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 3;
            }

            if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 4;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 5;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 6;
            }
            if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 7;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 8;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 9;
            }
        }
    }
    if(verificare_pozitie(tabla, i, j, pozitie))
    {
        if(matrice_principala[i][j] == '-')
        {
            matrice_mica[i][j] = jucator.gobblet[0][1];
            turn++;
            creare_matrice_principala();
            return true;
        }
        else
            cout << "Pozitie deja ocupata"<< endl;
    }
    return false;
}

bool mutaremedie( piese &jucator,int &turn,int &pozitie,int &pozitie_robot)
{
    int  i = 0, j = 0;
    if(pozitie_robot!=0)
        pozitie=pozitie_robot;
    else
        pozitie=-1;
    punct pozC;
    if(width == 1920)
    {
        while(1 && pozitie == -1)
        {
            click(pozC.x, pozC.y);
            if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
            {
                pozitie=1;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
            {
                pozitie=2;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
            {
                pozitie=3;
            }
            if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
            {
                pozitie=4;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
            {
                pozitie=5;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
            {
                pozitie=6;
            }
            if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
            {
                pozitie=7;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
            {
                pozitie=8;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
            {
                pozitie=9;
            }
        }

    }
    else if(width == 1280)
    {
        while(1 && pozitie == -1)
        {
            click(pozC.x,pozC.y);
            if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 1;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 2;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 3;
            }

            if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 4;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 5;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 6;
            }
            if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 7;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 8;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 9;

            }
        }
    }
    if(verificare_pozitie(tabla, i, j, pozitie))
    {
        if(matrice_principala[i][j] == '-')
        {
            matrice_medie[i][j] = jucator.gobblet[1][1];
            jucator.nr_mediu--;
            creare_matrice_principala();
            turn++;
            return true;
        }
        else if(matrice_mica[i][j]!='-'&& matrice_medie[i][j]=='-'&& matrice_mare[i][j]=='-')
        {
            matrice_medie[i][j] = jucator.gobblet[1][1];
            jucator.nr_mediu--;
            creare_matrice_principala();
            turn++;
            return true;
        }
        else
            cout << "Pozitie deja ocupata"<< endl;
    }
    return false;
}

bool mutaremare(piese &jucator,int &turn,int &pozitie,int &pozitie_robot)
{
    int i = 0, j = 0;
    if(pozitie_robot!=0)
        pozitie=pozitie_robot;
    else
        pozitie=-1;
    punct pozC;
    if(width == 1920)
    {
        while(1 && pozitie == -1)
        {
            click(pozC.x, pozC.y);
            if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
            {
                pozitie=1;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
            {
                pozitie=2;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
            {
                pozitie=3;
            }
            if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
            {
                pozitie=4;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
            {
                pozitie=5;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
            {
                pozitie=6;
            }
            if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
            {
                pozitie=7;
            }
            if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
            {
                pozitie=8;
            }
            if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
            {
                pozitie=9;
            }
        }

    }
    else if(width == 1280)
    {
        while(1 && pozitie == -1)
        {
            click(pozC.x,pozC.y);
            if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 1;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 2;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
            {
                pozitie = 3;
            }

            if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 4;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 5;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
            {
                pozitie = 6;
            }
            if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 7;
            }
            if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 8;
            }
            if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
            {
                pozitie = 9;
            }
        }
    }
    if(verificare_pozitie(tabla, i, j, pozitie))
    {
        if(matrice_principala[i][j] == '-')
        {
            matrice_mare[i][j] = jucator.gobblet[2][1];
            turn++;
            creare_matrice_principala();
            return true;
        }
        else if(matrice_mare[i][j]=='-')
        {
            matrice_mare[i][j] = jucator.gobblet[2][1];
            turn++;
            creare_matrice_principala();
            return true;
        }
        else
            cout << "Pozitie deja ocupata"<< endl;
    }
    return false;
}

void creare_copie_principala()
{

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            if(copie_mare[i][j] != '-')
                copie_principala[i][j] = copie_mare[i][j];
            else if(copie_medie[i][j] != '-')
                copie_principala[i][j] = copie_medie[i][j];
            else if(copie_mica[i][j] != '-')
                copie_principala[i][j] = copie_mica[i][j];
            else
                copie_principala[i][j]='-';
        }
}

void mutare_pe_tabla(piese jucator,int &turn,char simbol,int &pozitie) ///functia care chiar face mutarea dupa toate verificarile facute anterior
{

    int i = 0, j = 0;
    creare_copie();
    if(verificare_pozitie_mutare(i,j,simbol,pozitie,jucator))
    {
        char vec_jucator[4];
        vec_jucator[0] = jucator.gobblet[0][1];
        vec_jucator[1] = jucator.gobblet[1][1];
        vec_jucator[2] = jucator.gobblet[2][1];
        vec_jucator[3]='\0';
        if(simbol==vec_jucator[0])
        {
            if(mutaremica(jucator,turn,pozitie,pozitie_robot))
            {
                jucator.nr_mic++;
            }
            else
            {
                matrice_mica[i][j]=simbol;
                matrice_principala[i][j]=simbol;
                creare_copie();
            }
        }
        else if(simbol==vec_jucator[1])
        {
            if(mutaremedie(jucator,turn,pozitie,pozitie_robot))
            {
                jucator.nr_mediu++;
            }
            else
            {
                matrice_medie[i][j]=simbol;
                matrice_principala[i][j]=simbol;
                creare_copie();
            }
        }
        else if(simbol==vec_jucator[2])
        {
            if(mutaremare(jucator,turn,pozitie,pozitie_robot))
            {
                jucator.nr_mare++;
            }
            else
            {
                matrice_mare[i][j]=simbol;
                matrice_principala[i][j]=simbol;
                creare_copie();
            }
        }
        else
            cout<<"EROARE"<<endl;
    }
}

void undo(piese &jucator1,piese &jucator2,int &turn) ///functia pt butonul de undo
{
    bool ok=1;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            if(mutare_undo[0]==1)
            {
                if(copie_mare[i][j]!=matrice_mare[i][j] && copie_mare[i][j]=='-')
                {
                    jucator1.nr_mare++;
                    turn++;
                    strcpy(matrice_mare[i],copie_mare[i]);
                    creare_copie_principala();
                }

                else if(copie_medie[i][j]!=matrice_medie[i][j]  && copie_medie[i][j]=='-')
                {
                    jucator1.nr_mediu++;
                    turn++;
                    strcpy(matrice_medie[i],copie_medie[j]);
                    creare_copie_principala();

                }
                else if(copie_mica[i][j]!=matrice_mica[i][j] && copie_mica[i][j]=='-')
                {
                    jucator1.nr_mic++;
                    turn++;
                    strcpy(matrice_mica[i],copie_mica[i]);
                    creare_copie_principala();
                }
            }
            else
            {
                if(copie_mare[i][j]!=matrice_mare[i][j] && copie_mare[i][j]!='-')
                {
                    turn++;
                    for(int k=0; k<3; k++)
                        strcpy(matrice_mare[k],copie_mare[k]);
                    creare_copie_principala();
                }
                else if(copie_medie[i][j]!=matrice_medie[i][j]  && copie_medie[i][j]!='-')
                {
                    turn++;
                    for(int k=0; k<3; k++)
                        strcpy(matrice_medie[k],copie_medie[k]);
                    creare_copie_principala();
                }
                else if(copie_mica[i][j]!=matrice_mica[i][j] && copie_mica[i][j]=='-')
                {
                    turn++;
                    for(int k=0; k<3; k++)
                        strcpy(matrice_mica[k],copie_mica[k]);
                    creare_copie_principala();
                }
            }
        }
}

void alegere_tip_piesa_si_plasare(piese &jucator,int &turn,int &tip_piesa,int &pozitie)
{
    switch(tip_piesa)
    {
    case 0:
    {
        mutare_undo[0]=1;
        creare_copie();
        plasare_mica(jucator,turn,pozitie);
        break;
    }
    case 1:
    {
        mutare_undo[0]=1;
        creare_copie();
        plasare_medie(jucator,turn,pozitie);
        break;
    }
    case 2:
    {
        mutare_undo[0]=1;
        creare_copie();
        plasare_mare(jucator,turn,pozitie);
        break;
    }
    case 3:
    {
        mutare_undo[0]=0;
        if(verificare_caractere(jucator))
        {
            creare_copie();
            mutare_pe_tabla(jucator,turn,simbol,pozitie);
        }
        else
        {
            return;
        }
        break;
    }
    case 4:
    {
        if(turn%2==0)
            undo(jucator2,jucator1,turn);
        else
            undo(jucator1,jucator2,turn);
        for(int i=0; i<3; i++)
            strcpy(matrice_principala[i],copie_principala[i]);
    }
    break;
    }
}

void refacere_table(piese &jucator1,piese &jucator2,bool& ok,int &turn,int &odata) ///la final de joc, in caz ca jucatorii aleg sa joace din nou, se reface tabla cu ajutorul acestei functii si fiecare parametru se reinitializeaza
{
    int i,j;
    turn = 0;
    ok = true;
    odata = 0;
    jucator1.clr = -1;
    jucator2.clr = -1;
    jucator1.nr_mare=2;
    jucator1.nr_mediu=2;
    jucator1.nr_mic=2;
    jucator2.nr_mare=2;
    jucator2.nr_mediu=2;
    jucator2.nr_mic=2;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
        {
            matrice_mica[i][j]='-';
            matrice_mare[i][j]='-';
            matrice_medie[i][j]='-';
            matrice_principala[i][j]='-';
        }
}

void vreti_sa_jucati_din_nou(piese &jucator1,piese &jucator2,bool &ok, int &turn,char varianta_raspuns[]) ///verificare daca jucatorii vor sau nu sa joace din nou
{
    cout <<endl;
    if(ok==false)
    {
        char raspuns[3]="";
        while(strcmp(raspuns,"DA")!=0&&strcmp(raspuns,"da")!=0&&strcmp(raspuns,"Da")!=0&&strcmp(raspuns,"dA")!=0&&strcmp(raspuns,"NU")!=0&&strcmp(raspuns,"nu")!=0&&strcmp(raspuns,"Nu")!=0&&strcmp(raspuns,"nU")!=0)
        {
            cout<<"Vreti sa jucati din nou:"<<endl;
            strncpy(raspuns,varianta_raspuns,2);
            raspuns[2]='\0';
            if(strcmp(raspuns,"DA")==0||strcmp(raspuns,"da")==0||strcmp(raspuns,"Da")==0||strcmp(raspuns,"dA")==0)
            {
                system("CLS");
                alegere_culoare();
                refacere_table(jucator1,jucator2,ok,turn,odata);
                alegeti_simboluri(jucator1,jucator2);
            }
            else if(strcmp(raspuns,"NU")==0||strcmp(raspuns,"nu")==0||strcmp(raspuns,"Nu")==0||strcmp(raspuns,"nU")==0)
            {
                meniu(iarna);
                cout<<"Va multumim, speram ca v-a placut jocu!!<3"<<endl;
            }
            else
            {
                cout<<"Nu ati introdus un raspuns valid,introduceti din nou"<<endl;
            }
        }
    }
}

void plasare_grafica(bool robot) ///functie enorma care face posibila plasarea, mai precis aici se alege patratul in care se plaseaza o noua piesa
{
    int pozitie=-1;
    punct pozC;
    int piesa=-1;
    if(width == 1920)
    {
        while(1 && piesa==-1)
        {
            creare_matrice_principala();
            iesire(pozC);
            inapoi(pozC);
            if(robot == 0)
            {
                settextstyle(DEFAULT_FONT,HORIZ_DIR,5);
                settextjustify(CENTER_TEXT,CENTER_TEXT);
                setcolor(jucator1.clr);
                outtextxy(960,900,"UNDO");
            }
            click(pozC.x,pozC.y);
            if(turn%2==0)
            {
                if(pozC.x>910 && pozC.x<1000 && pozC.y>890 && pozC.y<920 && turn>0 && robot == 0)
                {
                    piesa=4;
                    alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                }
                else if(pozC.x>280 && pozC.x<380 && pozC.y>290 && pozC.y<390)
                {
                    piesa=0;
                    while(1 && pozitie==-1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=1;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=2;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=3;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                        if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=4;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=5;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=6;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=7;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=8;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=9;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>280 && pozC.x<380 && pozC.y>480 && pozC.y<600)
                {
                    piesa=1;
                    while(1 && pozitie==-1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=1;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=2;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=3;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                        if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=4;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=5;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=6;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=7;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=8;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=9;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>280 && pozC.x<380 && pozC.y>670 && pozC.y<810)
                {
                    piesa=2;
                    while(1 && pozitie==-1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=1;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=2;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=3;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                        if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=4;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=5;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=6;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=7;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=8;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=9;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                    }
                }
                else
                {
                    if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                    {
                        piesa=3;
                        pozitie=1;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                    {
                        piesa=3;
                        pozitie=2;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                    {
                        piesa=3;
                        pozitie=3;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }

                    if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                    {
                        piesa=3;
                        pozitie=4;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                    {
                        piesa=3;
                        pozitie=5;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                    {
                        piesa=3;
                        pozitie=6;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                    {
                        piesa=3;
                        pozitie=7;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                    {
                        piesa=3;
                        pozitie=8;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                    {
                        piesa=3;
                        pozitie=9;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                }
            }
            else
            {
                if(pozC.x>910 && pozC.x<1000 && pozC.y>890 && pozC.y<920 && turn>0)
                {
                    piesa=4;
                    alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                }
                else if(pozC.x>1540 && pozC.x<1640 && pozC.y>290 && pozC.y<390)
                {
                    piesa=0;
                    while(1 && pozitie==-1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=1;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=2;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=3;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }

                        if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=4;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=5;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=6;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=7;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=8;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=9;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }

                    }
                }
                else if(pozC.x>1530 && pozC.x<1650 && pozC.y>480 && pozC.y<600)
                {
                    piesa=1;
                    while(1 && pozitie==-1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=1;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=2;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=3;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }

                        if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=4;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=5;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=6;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=7;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=8;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=9;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>1520 && pozC.x<1660 && pozC.y>670 && pozC.y<810)
                {
                    piesa=2;
                    while(1 && pozitie==-1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=1;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=2;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                        {
                            pozitie=3;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }

                        if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=4;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=5;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                        {
                            pozitie=6;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=7;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=8;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                        {
                            pozitie=9;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                    }
                }
                else
                {
                    if(pozC.x>660 && pozC.x<860 && pozC.y>240 && pozC.y<440)
                    {
                        piesa=3;
                        pozitie=1;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>860 && pozC.x<1060 && pozC.y>240 && pozC.y<440)
                    {
                        piesa=3;
                        pozitie=2;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>1060 && pozC.x<1260 && pozC.y>240 && pozC.y<440)
                    {
                        piesa=3;
                        pozitie=3;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }

                    if(pozC.x>660 && pozC.x<860 && pozC.y>440 && pozC.y<640)
                    {
                        piesa=3;
                        pozitie=4;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>860 && pozC.x<1060 && pozC.y>440 && pozC.y<640)
                    {
                        piesa=3;
                        pozitie=5;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>1060 && pozC.x<1260 && pozC.y>440 && pozC.y<640)
                    {
                        piesa=3;
                        pozitie=6;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>660 && pozC.x<860 && pozC.y>640 && pozC.y<840)
                    {
                        piesa=3;
                        pozitie=7;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>860 && pozC.x<1060 && pozC.y>640 && pozC.y<840)
                    {
                        piesa=3;
                        pozitie=8;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>1060 && pozC.x<1260 && pozC.y>640 && pozC.y<840)
                    {
                        piesa=3;
                        pozitie=9;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                }
            }
        }
    }
    else if(width == 1280)
    {
        if(robot == 0)
        {
        settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        setcolor(jucator1.clr);
        outtextxy(640,600,"UNDO");
        }
        iesire(pozC);
        inapoi(pozC);
        while(1 && piesa==-1)
        {
            click(pozC.x,pozC.y);
            iesire(pozC);
            inapoi(pozC);
            if(turn%2==0)
            {
                if(pozC.x>610 && pozC.x<670 && pozC.y>590 && pozC.y<620 && robot == 0)
                {
                    piesa=4;
                    alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                }
                if(pozC.x>185 && pozC.x<255 && pozC.y>185 && pozC.y<255)
                {
                    piesa = 0;
                    while(1 && pozitie == -1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 1;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 2;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 3;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                        if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 4;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 5;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 6;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 7;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 8;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 9;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>170 && pozC.x<260 && pozC.y>315 && pozC.y<405)
                {
                    piesa = 1;
                    while(1 && pozitie == -1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 1;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 2;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 3;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                        if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 4;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 5;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 6;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 7;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 8;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 9;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>160 && pozC.x<270 && pozC.y>445 && pozC.y<555)
                {
                    piesa = 2;
                    while(1 && pozitie == -1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 1;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 2;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 3;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }

                        if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 4;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 5;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 6;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 7;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 8;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 9;
                            alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                        }
                    }
                }
                else
                {
                    piesa = 3;
                    if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                    {
                        pozitie = 1;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                    {
                        pozitie = 2;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                    {
                        pozitie = 3;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }

                    if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                    {
                        pozitie = 4;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                    {
                        pozitie = 5;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                    {
                        pozitie = 6;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                    {
                        pozitie = 7;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                    {
                        pozitie = 8;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                    if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                    {
                        pozitie = 9;
                        alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                    }
                }
            }
            else
            {
                if(pozC.x>610 && pozC.x<670 && pozC.y>590 && pozC.y<620)
                {
                    piesa=4;
                    alegere_tip_piesa_si_plasare(jucator1,turn,piesa,pozitie);
                }
                if(pozC.x>1030 && pozC.x<1100 && pozC.y>185 && pozC.y<255)
                {
                    piesa=0;
                    while(1 && pozitie == -1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 1;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 2;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 3;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 4;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 5;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 6;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 7;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 8;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 9;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>1020 && pozC.x<1110 && pozC.y>315 && pozC.y<405)
                {
                    piesa=1;
                    while(1 && pozitie == -1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 1;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 2;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 3;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }

                        if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 4;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 5;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 6;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 7;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 8;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 9;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                    }
                }
                else if(pozC.x>1010 && pozC.x<1120 && pozC.y>445 && pozC.y<555)
                {
                    piesa=2;
                    while(1 && pozitie == -1)
                    {
                        click(pozC.x,pozC.y);
                        if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 1;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 2;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                        {
                            pozitie = 3;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }

                        if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 4;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 5;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                        {
                            pozitie = 6;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 7;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 8;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                        if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                        {
                            pozitie = 9;
                            alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                        }
                    }
                }
                else
                {
                    piesa=3;
                    if(pozC.x>430 && pozC.x<570 && pozC.y>150 && pozC.y<290)
                    {
                        pozitie = 1;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>570 && pozC.x<710 && pozC.y>150 && pozC.y<290)
                    {
                        pozitie = 2;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>710 && pozC.x<850 && pozC.y>150 && pozC.y<290)
                    {
                        pozitie = 3;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }

                    if(pozC.x>430 && pozC.x<570 && pozC.y>290 && pozC.y<430)
                    {
                        pozitie = 4;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>570 && pozC.x<710 && pozC.y>290 && pozC.y<430)
                    {
                        pozitie = 5;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>710 && pozC.x<850 && pozC.y>290 && pozC.y<430)
                    {
                        pozitie = 6;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>430 && pozC.x<570 && pozC.y>430 && pozC.y<570)
                    {
                        pozitie = 7;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>570 && pozC.x<710 && pozC.y>430 && pozC.y<570)
                    {
                        pozitie = 8;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                    if(pozC.x>710 && pozC.x<850 && pozC.y>430 && pozC.y<570)
                    {
                        pozitie = 9;
                        alegere_tip_piesa_si_plasare(jucator2,turn,piesa,pozitie);
                    }
                }
            }
        }
    }
}
void joc(piese &jucator1,piese &jucator2,bool &ok) ///jocul principal, functia de unde pleaca tot, inclusiv toate problemele noastre
{

    punct pozCursor;
    alegeti_simboluri(jucator1,jucator2);
    creare_table();
    creare_matrici();
    //tabela_marcaj(numar_clasament);
    char varianta_raspuns[10] = "DA";
    alegere_culoare();
    while(ok)
    {
        if(turn == 0)
            afisare_table(jucator1,ok,turn, odata);
        if(turn %2 == 0)
        {
            cout<<"Jucator 1"<<endl;
            cout<<endl;
            plasare_grafica(0);
            //afisare_table(jucator1,ok,turn, odata);
            cout<<endl;
            if(verificare_catigator(matrice_principala))
            {
                ok = false;
            }
        }
        else
        {
            cout<<"Jucator 2"<<endl;
            cout<<endl;
            plasare_grafica(0);
            //afisare_table(jucator2, ok, turn, odata);
            if(verificare_catigator(matrice_principala))
            {
                ok = false;
            }
        }
        vreti_sa_jucati_din_nou(jucator1,jucator2,ok,turn,varianta_raspuns);
    }
}

///------------------------------FUNCTII PENTRU ROBOT CARE MERG TOT TIMPU-----------------------------------------------------------------------------------------------------


void creare_vector_pozitii_robot(piese jucator1, piese jucator2)
{
    int i,j,k=1;
    for(i=1; i<=10; i++)
        vector_pozitii_robot[i]=0;

    char vec_robot[4];
    vec_robot[0]=jucator2.gobblet[0][1];
    vec_robot[1]=jucator2.gobblet[1][1];
    vec_robot[2]=jucator2.gobblet[2][1];
    vec_robot[3]='/0';

    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            if(strchr(vec_robot,matrice_principala[i][j]))
            {
                vector_pozitii_robot[k]=1;
                k++;
            }
            else
                k++;
}

void creare_vector_pozitii_mutare_robot(piese jucator1, piese jucator2,int &matrice)
{
    int i,j,k=1;
    for(i=1; i<=10; i++)
        vector_pozitii_robot[i]=0;

    char vec_jucator1[4],vec_robot[4];
    vec_jucator1[0]=jucator1.gobblet[0][1];
    vec_jucator1[1]=jucator1.gobblet[1][1];
    vec_jucator1[2]=jucator1.gobblet[2][1];
    vec_jucator1[3]='/0';

    vec_robot[0]=jucator2.gobblet[0][1];
    vec_robot[1]=jucator2.gobblet[1][1];
    vec_robot[2]=jucator2.gobblet[2][1];
    vec_robot[3]='/0';

    if(matrice==0)
    {
        for(i=0; i<3; i++)
            for(j=0; j<3; j++)
            {
                if(matrice_principala[i][j]=='-')
                {
                    vector_pozitii_robot[k]=1;
                    k++;
                }
                else
                    k++;
            }
    }
    else if(matrice==1)
    {
        for(i=0; i<3; i++)
            for(j=0; j<3; j++)
            {

                if(matrice_principala[i][j]=='-' || matrice_principala[i][j]==vec_jucator1[0]|| matrice_principala[i][j]==vec_robot[0])
                {
                    vector_pozitii_robot[k]=1;
                    k++;
                }
                else
                    k++;
            }
    }
    else if(matrice==2)
    {
        for(i=0; i<3; i++)
            for(j=0; j<3; j++)
            {
                if(matrice_principala[i][j]=='-' || matrice_principala[i][j]==vec_jucator1[0] || matrice_principala[i][j]==vec_jucator1[1]|| matrice_principala[i][j]==vec_robot[0]|| matrice_principala[i][j]==vec_robot[1])
                {
                    vector_pozitii_robot[k]=1;
                    k++;
                }
                else
                    k++;
            }
    }
    for(i=1; i<=9; i++)
        cout<<vector_pozitii_robot[i]<<" ";
    cout<<endl;
}

void plasare_ai_usor(piese &jucator1,piese &jucator,int &turn)
{
    int i,j;
    int matrice=-1,pozitie=-1,mutare=1;
    int pozitie_mutare=-1;
    bool ok=1;
    char vec_jucator[4];
    char simbol;
    int ai=0;
    srand(time(0));
    while(mutare)
    {
        matrice=rand()%4;
        if(matrice==2)
            if(jucator.nr_mare==0)
                matrice=-1;
            else
                matrice=2;
        if(matrice==1)
            if(jucator.nr_mediu==0)
                matrice=-1;
            else
                matrice=1;
        if(matrice==0)
            if(jucator.nr_mic==0)
                matrice=-1;
            else
                matrice=0;
        if(matrice==0 &&jucator.nr_mic!=0)
        {
            pozitie=rand()%9+1;

                creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);
                while(vector_pozitii_robot[pozitie]==0)
                    pozitie=rand()%9+1;
            if(plasare_mica(jucator,turn,pozitie))
                mutare=0;
        }
        else if(matrice==1&&jucator.nr_mediu!=0)
        {
            pozitie=rand()%9+1;

                creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);
                while(vector_pozitii_robot[pozitie]==0)
                    pozitie=rand()%9+1;
            if(plasare_medie(jucator,turn,pozitie))
                mutare=0;
        }
        else if(matrice==2 && jucator.nr_mare!=0)
        {
           pozitie=rand()%9+1;

                creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);
                while(vector_pozitii_robot[pozitie]==0)
                    pozitie=rand()%9+1;
            if(plasare_mare(jucator,turn,pozitie))
                mutare=0;
        }
        else if(matrice==3 && jucator.nr_mare+jucator.nr_mediu+jucator.nr_mic<6 && verificare_caractere(jucator))
        {
            creare_vector_pozitii_robot(jucator1,jucator2);

            pozitie=rand()%9+1;
            while(vector_pozitii_robot[pozitie]==0)
                pozitie=rand()%9+1;

            verificare_pozitie_mutare(i,j,simbol,pozitie,jucator);
            cout<<pozitie<<" De unde ia piesa"<<endl;

            while(mutare)
            {
                pozitie_mutare=rand()%9+1;

                char vec_jucator[4];
                vec_jucator[0] = jucator.gobblet[0][1];
                vec_jucator[1] = jucator.gobblet[1][1];
                vec_jucator[2] = jucator.gobblet[2][1];
                vec_jucator[3]='\0';

                verificare_pozitie(tabla,i,j,pozitie_mutare);

                if(simbol==vec_jucator[0])
                {
                    matrice=0;
                    pozitie_mutare=rand()%9+1;

                    creare_vector_pozitii_mutare_robot(jucator1,jucator,matrice);

                    while(vector_pozitii_robot[pozitie_mutare]==0)
                        pozitie_mutare=rand()%9+1;
                    cout<<pozitie_mutare<<" UNDE PUNE PIESA"<<endl;

                    jucator.nr_mic++;
                    if(plasare_mica(jucator,turn,pozitie_mutare))
                        mutare=0;
                }
                else if(simbol==vec_jucator[1])
                {
                    matrice=1;
                    pozitie_mutare=rand()%9+1;
                    jucator.nr_mediu++;

                    creare_vector_pozitii_mutare_robot(jucator1,jucator,matrice);

                    while(vector_pozitii_robot[pozitie_mutare]==0)
                        pozitie_mutare=rand()%9+1;
                    cout<<pozitie_mutare<<" UNDE PUNE PIESA"<<endl;
                    if(plasare_medie(jucator,turn,pozitie_mutare))
                        mutare=0;
                }
                else if(simbol==vec_jucator[2])
                {
                    matrice=2;
                    pozitie_mutare=rand()%9+1;
                    jucator.nr_mare++;

                    creare_vector_pozitii_mutare_robot(jucator1,jucator,matrice);

                    while(vector_pozitii_robot[pozitie_mutare]==0)
                        pozitie_mutare=rand()%9+1;

                    cout<<pozitie_mutare<<" UNDE PUNE PIESA"<<endl;
                    if(plasare_mare(jucator,turn,pozitie_mutare))
                        mutare=0;
                }
            }
        }
    }
}

void joc_ai_usor(piese &jucator1,piese &jucator2,bool &ok)
{
    robot=1;
    punct pozCursor;
    alegeti_simboluri(jucator1,jucator2);
    creare_table();
    creare_matrici();
    alegere_culoare();
    char varianta_raspuns[10] = "DA";
    while(ok)
    {
        if(turn == 0)
            afisare_table(jucator1,ok,turn, odata);
        if(turn%2 == 0)
        {
            creare_copie();
            cout<<"Jucator 1"<<endl;
            pozitie_robot=0;
            creare_copie();
            plasare_grafica(robot);
            if(verificare_catigator(matrice_principala))
            {
                ok = false;
            }
            //buton_jucator(jucator1,turn);
        }
        else
        {
            cout<<"COMPUTER"<<endl;
            plasare_ai_usor(jucator1,jucator2,turn);
            creare_copie();
            if(verificare_catigator(matrice_principala))
            {
                ok = false;
            }
        }
    }
}

int verificare_castig_robot(piese &jucator1,piese &jucator2)
{
    int pozitie=-1,matrice=-1;
    char vec_jucator1[4],vec_robot[4];
    vec_jucator1[0]=jucator1.gobblet[0][1];
    vec_jucator1[1]=jucator1.gobblet[1][1];
    vec_jucator1[2]=jucator1.gobblet[2][1];
    vec_jucator1[3]='/0';

    vec_robot[0]=jucator2.gobblet[0][1];
    vec_robot[1]=jucator2.gobblet[1][1];
    vec_robot[2]=jucator2.gobblet[2][1];
    vec_robot[3]='/0';

    if((matrice_principala[0][0]=='A' ||matrice_principala[0][0]=='B' ||matrice_principala[0][0]=='C') && (matrice_principala[0][1]=='A' || matrice_principala[0][1]=='B' || matrice_principala[0][1]=='C' ) && matrice_principala[0][2]=='-')
        pozitie=3;
    if((matrice_principala[0][0]=='A' ||matrice_principala[0][0]=='B' ||matrice_principala[0][0]=='C') && (matrice_principala[0][2]=='A' || matrice_principala[0][2]=='B' || matrice_principala[0][2]=='C' ) && matrice_principala[0][1]=='-')
        pozitie=2;
    if((matrice_principala[0][1]=='A' ||matrice_principala[0][1]=='B' ||matrice_principala[0][1]=='C') && (matrice_principala[0][2]=='A' || matrice_principala[0][2]=='B' || matrice_principala[0][2]=='C' ) && matrice_principala[0][0]=='-')
        pozitie=1;
    if((matrice_principala[1][0]=='A' ||matrice_principala[1][0]=='B' ||matrice_principala[1][0]=='C') && (matrice_principala[1][1]=='A' || matrice_principala[1][1]=='B' || matrice_principala[1][1]=='C' ) && matrice_principala[1][2]=='-')
        pozitie=6;
    if((matrice_principala[1][0]=='A' ||matrice_principala[1][0]=='B' ||matrice_principala[1][0]=='C') && (matrice_principala[1][2]=='A' || matrice_principala[1][2]=='B' || matrice_principala[1][2]=='C' ) && matrice_principala[1][1]=='-')
        pozitie=5;
    if((matrice_principala[1][1]=='A' ||matrice_principala[1][1]=='B' ||matrice_principala[1][1]=='C') && (matrice_principala[1][2]=='A' || matrice_principala[1][2]=='B' || matrice_principala[1][2]=='C' ) && matrice_principala[1][0]=='-')
        pozitie=4;
    if((matrice_principala[2][0]=='A' ||matrice_principala[2][0]=='B' ||matrice_principala[2][0]=='C') && (matrice_principala[2][1]=='A' || matrice_principala[2][1]=='B' || matrice_principala[2][1]=='C' ) && matrice_principala[2][2]=='-')
        pozitie=9;
    if((matrice_principala[2][0]=='A' ||matrice_principala[2][0]=='B' ||matrice_principala[2][0]=='C') && (matrice_principala[2][2]=='A' || matrice_principala[2][2]=='B' || matrice_principala[2][2]=='C' ) && matrice_principala[2][1]=='-')
        pozitie=8;
    if((matrice_principala[2][1]=='A' ||matrice_principala[2][1]=='B' ||matrice_principala[2][1]=='C') && (matrice_principala[2][2]=='A' || matrice_principala[2][2]=='B' || matrice_principala[2][2]=='C' ) && matrice_principala[2][0]=='-')
        pozitie=7;
    if((matrice_principala[0][0]=='A' ||matrice_principala[0][0]=='B' ||matrice_principala[0][0]=='C') && (matrice_principala[1][0]=='A' || matrice_principala[1][0]=='B' || matrice_principala[1][0]=='C' ) && matrice_principala[2][0]=='-')
        pozitie=7;
    if((matrice_principala[0][0]=='A' ||matrice_principala[0][0]=='B' ||matrice_principala[0][0]=='C') && (matrice_principala[2][0]=='A' || matrice_principala[2][0]=='B' || matrice_principala[2][0]=='C' ) && matrice_principala[1][0]=='-')
        pozitie=4;
    if((matrice_principala[1][0]=='A' ||matrice_principala[1][0]=='B' ||matrice_principala[1][0]=='C') && (matrice_principala[2][0]=='A' || matrice_principala[2][0]=='B' || matrice_principala[2][0]=='C' ) && matrice_principala[0][0]=='-')
        pozitie=1;
    if((matrice_principala[0][1]=='A' ||matrice_principala[0][1]=='B' ||matrice_principala[0][1]=='C') && (matrice_principala[1][1]=='A' || matrice_principala[1][1]=='B' || matrice_principala[1][1]=='C' ) && matrice_principala[2][1]=='-')
        pozitie=8;
    if((matrice_principala[0][1]=='A' ||matrice_principala[0][1]=='B' ||matrice_principala[0][1]=='C') && (matrice_principala[2][1]=='A' || matrice_principala[2][1]=='B' || matrice_principala[2][1]=='C' ) && matrice_principala[1][1]=='-')
        pozitie=5;
    if((matrice_principala[1][1]=='A' ||matrice_principala[1][1]=='B' ||matrice_principala[1][1]=='C') && (matrice_principala[2][1]=='A' || matrice_principala[2][1]=='B' || matrice_principala[2][1]=='C' ) && matrice_principala[0][1]=='-')
        pozitie=2;
    if((matrice_principala[0][2]=='A' ||matrice_principala[0][2]=='B' ||matrice_principala[0][2]=='C') && (matrice_principala[1][2]=='A' || matrice_principala[1][2]=='B' || matrice_principala[1][2]=='C' ) && matrice_principala[2][2]=='-')
        pozitie=9;
    if((matrice_principala[0][2]=='A' ||matrice_principala[0][2]=='B' ||matrice_principala[0][2]=='C') && (matrice_principala[2][2]=='A' || matrice_principala[2][2]=='B' || matrice_principala[2][2]=='C' ) && matrice_principala[1][2]=='-')
        pozitie=6;
    if((matrice_principala[1][2]=='A' ||matrice_principala[1][2]=='B' ||matrice_principala[1][2]=='C') && (matrice_principala[2][2]=='A' || matrice_principala[2][2]=='B' || matrice_principala[2][2]=='C' ) && matrice_principala[0][2]=='-')
        pozitie=3;
    if((matrice_principala[0][0]=='A' ||matrice_principala[0][0]=='B' ||matrice_principala[0][0]=='C') && (matrice_principala[1][1]=='A' || matrice_principala[1][1]=='B' || matrice_principala[1][1]=='C' ) && matrice_principala[2][2]=='-')
        pozitie=9;
    if((matrice_principala[0][0]=='A' ||matrice_principala[0][0]=='B' ||matrice_principala[0][0]=='C') && (matrice_principala[2][2]=='A' || matrice_principala[2][2]=='B' || matrice_principala[2][2]=='C' ) && matrice_principala[1][1]=='-')
        pozitie=5;
    if((matrice_principala[1][1]=='A' ||matrice_principala[1][1]=='B' ||matrice_principala[1][1]=='C') && (matrice_principala[2][2]=='A' || matrice_principala[2][2]=='B' || matrice_principala[2][2]=='C' ) && matrice_principala[0][0]=='-')
        pozitie=1;
    if((matrice_principala[0][2]=='A' ||matrice_principala[0][2]=='B' ||matrice_principala[0][2]=='C') && (matrice_principala[1][1]=='A' || matrice_principala[1][1]=='B' || matrice_principala[1][1]=='C' ) && matrice_principala[2][0]=='-')
        pozitie=7;
    if((matrice_principala[0][2]=='A' ||matrice_principala[0][2]=='B' ||matrice_principala[0][2]=='C') && (matrice_principala[2][0]=='A' || matrice_principala[2][0]=='B' || matrice_principala[2][0]=='C' ) && matrice_principala[1][1]=='-')
        pozitie=5;
    if((matrice_principala[1][1]=='A' ||matrice_principala[1][1]=='B' ||matrice_principala[1][1]=='C') && (matrice_principala[2][0]=='A' || matrice_principala[2][0]=='B' || matrice_principala[2][0]=='C' ) && matrice_principala[0][2]=='-')
        pozitie=3;
    cout<<"POZITIA UNDE CASTIGA="<<pozitie<<endl;
    return pozitie;
}

int verificare_jucator(piese &jucator1,piese &jucator2)
{
    int pozitie=-1,matrice=-1;
    char vec_jucator1[4],vec_robot[4];
    vec_jucator1[0]=jucator1.gobblet[0][1];
    vec_jucator1[1]=jucator1.gobblet[1][1];
    vec_jucator1[2]=jucator1.gobblet[2][1];
    vec_jucator1[3]='/0';

    vec_robot[0]=jucator2.gobblet[0][1];
    vec_robot[1]=jucator2.gobblet[1][1];
    vec_robot[2]=jucator2.gobblet[2][1];
    vec_robot[3]='/0';

    if(strchr(vec_jucator1,matrice_principala[0][0]) && strchr(vec_jucator1,matrice_principala[0][1]) && strchr(vec_robot,matrice_principala[0][2])==NULL)
        pozitie=3;
    if(strchr(vec_jucator1,matrice_principala[0][0]) && strchr(vec_jucator1,matrice_principala[0][2]) && strchr(vec_robot,matrice_principala[0][1])==NULL)
        pozitie=2;
    if(strchr(vec_jucator1,matrice_principala[0][1]) && strchr(vec_jucator1,matrice_principala[0][2]) && strchr(vec_robot,matrice_principala[0][0])==NULL)
        pozitie=1;
    if(strchr(vec_jucator1,matrice_principala[1][0]) && strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_robot,matrice_principala[1][2])==NULL)
        pozitie=6;
    if(strchr(vec_jucator1,matrice_principala[1][0]) && strchr(vec_jucator1,matrice_principala[1][2]) && strchr(vec_robot,matrice_principala[1][1])==NULL)
        pozitie=5;
    if(strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_jucator1,matrice_principala[1][2]) && strchr(vec_robot,matrice_principala[1][0])==NULL)
        pozitie=4;
    if(strchr(vec_jucator1,matrice_principala[2][0]) && strchr(vec_jucator1,matrice_principala[2][1]) && strchr(vec_robot,matrice_principala[2][2])==NULL)
        pozitie=9;
    if(strchr(vec_jucator1,matrice_principala[2][0]) && strchr(vec_jucator1,matrice_principala[2][2]) && strchr(vec_robot,matrice_principala[2][1])==NULL)
        pozitie=8;
    if(strchr(vec_jucator1,matrice_principala[2][1]) && strchr(vec_jucator1,matrice_principala[2][2]) && strchr(vec_robot,matrice_principala[2][0])==NULL)
        pozitie=7;
    if(strchr(vec_jucator1,matrice_principala[0][0]) && strchr(vec_jucator1,matrice_principala[1][0]) && strchr(vec_robot,matrice_principala[2][0])==NULL)
        pozitie=7;
    if(strchr(vec_jucator1,matrice_principala[0][0]) && strchr(vec_jucator1,matrice_principala[2][0]) && strchr(vec_robot,matrice_principala[1][0])==NULL)
        pozitie=4;
    if(strchr(vec_jucator1,matrice_principala[1][0]) && strchr(vec_jucator1,matrice_principala[2][0]) && strchr(vec_robot,matrice_principala[0][0])==NULL)
        pozitie=1;
    if(strchr(vec_jucator1,matrice_principala[0][1]) && strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_robot,matrice_principala[2][1])==NULL)
        pozitie=8;
    if(strchr(vec_jucator1,matrice_principala[0][1]) && strchr(vec_jucator1,matrice_principala[2][1]) && strchr(vec_robot,matrice_principala[1][1])==NULL)
        pozitie=5;
    if(strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_jucator1,matrice_principala[2][1]) && strchr(vec_robot,matrice_principala[0][1])==NULL)
        pozitie=2;
    if(strchr(vec_jucator1,matrice_principala[0][2]) && strchr(vec_jucator1,matrice_principala[1][2]) && strchr(vec_robot,matrice_principala[2][2])==NULL)
        pozitie=9;
    if(strchr(vec_jucator1,matrice_principala[0][2]) && strchr(vec_jucator1,matrice_principala[2][2]) && strchr(vec_robot,matrice_principala[1][2])==NULL)
        pozitie=6;
    if(strchr(vec_jucator1,matrice_principala[1][2]) && strchr(vec_jucator1,matrice_principala[2][2]) && strchr(vec_robot,matrice_principala[0][2])==NULL)
        pozitie=3;
    if(strchr(vec_jucator1,matrice_principala[0][0]) && strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_robot,matrice_principala[2][2])==NULL)
        pozitie=9;
    if(strchr(vec_jucator1,matrice_principala[0][0]) && strchr(vec_jucator1,matrice_principala[2][2]) && strchr(vec_robot,matrice_principala[1][1])==NULL)
        pozitie=5;
    if(strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_jucator1,matrice_principala[2][2]) && strchr(vec_robot,matrice_principala[0][0])==NULL)
        pozitie=1;
    if(strchr(vec_jucator1,matrice_principala[0][2]) && strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_robot,matrice_principala[2][0])==NULL)
        pozitie=7;
    if(strchr(vec_jucator1,matrice_principala[0][2]) && strchr(vec_jucator1,matrice_principala[2][0]) && strchr(vec_robot,matrice_principala[1][1])==NULL)
        pozitie=5;
    if(strchr(vec_jucator1,matrice_principala[1][1]) && strchr(vec_jucator1,matrice_principala[2][0]) && strchr(vec_robot,matrice_principala[0][2])==NULL)
        pozitie=3;
    cout<<"POZITIA UNDE BLOCHEAZA="<<pozitie<<endl;
    return pozitie;
}

void plasare_ai_greu(piese &jucator1,piese &jucator2,int &turn)
{
    int i,j;
    int ai=0;
    bool ok=1;
    int matrice=1,pozitie=-1,mutare=1;
    int pozitie_mutare=1;
    srand(time(0));
    while(mutare)
    {
        matrice=rand()%4;
        if(matrice==2)
            if(jucator2.nr_mare==0)
                matrice=-1;
        if(matrice==1)
            if(jucator2.nr_mediu==0)
                matrice=-1;
        if(matrice==0)
            if(jucator2.nr_mic==0)
                matrice=-1;
        if(matrice==-1)
            matrice=3;

        if(matrice==0 && jucator2.nr_mic!=0)
        {
            pozitie=verificare_castig_robot(jucator1,jucator2);
            if(pozitie==-1)
                pozitie=verificare_jucator(jucator1,jucator2);
            if(pozitie==-1)
            {
                pozitie=rand()%9+1;

                creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);
                while(vector_pozitii_robot[pozitie]==0)
                    pozitie=rand()%9+1;
            }
            if(plasare_mica(jucator2,turn,pozitie))
                mutare=0;
        }

        else if(matrice==1&&jucator2.nr_mediu!=0)
        {
            pozitie=verificare_castig_robot(jucator1,jucator2);
            if(pozitie==-1)
                pozitie=verificare_jucator(jucator1,jucator2);
            if(pozitie==-1)
            {
                pozitie=rand()%9+1;

                creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);
                while(vector_pozitii_robot[pozitie]==0)
                    pozitie=rand()%9+1;
            }
            if(plasare_medie(jucator2,turn,pozitie))
                mutare=0;
        }

        else if(matrice==2 && jucator2.nr_mare!=0)
        {
            pozitie=verificare_castig_robot(jucator1,jucator2);
            if(pozitie==-1)
                pozitie=verificare_jucator(jucator1,jucator2);
            if(pozitie==-1)
            {
                pozitie=rand()%9+1;

                creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);
                while(vector_pozitii_robot[pozitie]==0)
                    pozitie=rand()%9+1;
            }
            if(plasare_mare(jucator2,turn,pozitie))
                mutare=0;
        }
        else if(matrice==3 && jucator2.nr_mare+jucator2.nr_mediu+jucator2.nr_mic<6 && verificare_caractere(jucator2))
        {
            creare_vector_pozitii_robot(jucator1,jucator2);

            pozitie=rand()%9+1;
            while(vector_pozitii_robot[pozitie]==0)
                pozitie=rand()%9+1;

            verificare_pozitie_mutare(i,j,simbol,pozitie,jucator2);
            cout<<pozitie<<" De unde ia piesa"<<endl;

            while(mutare)
            {
                pozitie_mutare=rand()%9+1;

                char vec_jucator[4];
                vec_jucator[0] = jucator2.gobblet[0][1];
                vec_jucator[1] = jucator2.gobblet[1][1];
                vec_jucator[2] = jucator2.gobblet[2][1];
                vec_jucator[3]='\0';

                verificare_pozitie(tabla,i,j,pozitie_mutare);

                if(simbol==vec_jucator[0])
                {
                    matrice=0;
                    pozitie_mutare=verificare_castig_robot(jucator1,jucator2);
                    if(pozitie_mutare==-1)
                        pozitie_mutare=verificare_jucator(jucator1,jucator2);
                    if(pozitie_mutare==-1)
                    {
                        pozitie_mutare=rand()%9+1;

                        creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);

                        while(vector_pozitii_robot[pozitie_mutare]==0)
                            pozitie_mutare=rand()%9+1;
                    }
                    cout<<pozitie_mutare<<" UNDE PUNE PIESA"<<endl;
                    jucator2.nr_mic++;
                    if(plasare_mica(jucator2,turn,pozitie_mutare))
                        mutare=0;
                    else
                    {
                        mutare=0;
                        ok=0;
                    }
                }
                else if(simbol==vec_jucator[1])
                {
                    matrice=1;
                    jucator2.nr_mediu++;
                    pozitie_mutare=verificare_castig_robot(jucator1,jucator2);
                    if(pozitie_mutare==-1)
                        pozitie_mutare=verificare_jucator(jucator1,jucator2);
                    if(pozitie_mutare==-1)
                    {
                        pozitie_mutare=rand()%9+1;

                        creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);

                        while(vector_pozitii_robot[pozitie_mutare]==0)
                            pozitie_mutare=rand()%9+1;
                    }

                    cout<<pozitie_mutare<<" UNDE PUNE PIESA"<<endl;
                    if(plasare_medie(jucator2,turn,pozitie_mutare))
                        mutare=0;
                    else
                    {
                        mutare=0;
                        ok=0;
                    }
                }
                else if(simbol==vec_jucator[2])
                {
                    matrice=2;

                    jucator2.nr_mare++;
                    pozitie_mutare=verificare_castig_robot(jucator1,jucator2);
                    if(pozitie_mutare==-1)
                        pozitie_mutare=verificare_jucator(jucator1,jucator2);
                    if(pozitie_mutare==-1)
                    {
                        pozitie_mutare=rand()%9+1;

                        creare_vector_pozitii_mutare_robot(jucator1,jucator2,matrice);

                        while(vector_pozitii_robot[pozitie_mutare]==0)
                            pozitie_mutare=rand()%9+1;
                    }


                    cout<<pozitie_mutare<<" UNDE PUNE PIESA"<<endl;
                    if(plasare_mare(jucator2,turn,pozitie_mutare))
                        mutare=0;
                    else
                    {
                        mutare=0;
                        ok=0;
                    }
                }
            }
            if(ok==0)
                mutare=1;
        }
    }
}

void joc_ai_greu(piese &jucator1,piese &jucator2,bool &ok)
{
    robot=2;
    punct pozCursor;
    alegeti_simboluri(jucator1,jucator2);
    creare_table();
    creare_matrici();
    alegere_culoare();
    char varianta_raspuns[10] = "DA";
    while(ok)
    {
        if(turn==0)
        {
            afisare_table(jucator1,ok,turn, odata);
        }
        if(turn%2 == 0)
        {
            cout<<"Jucator 1"<<endl;
            plasare_grafica(1);
            if(verificare_catigator(matrice_principala))
            {
                ok = false;
            }

        }
        else
        {
            cout<<"Jucator 2"<<endl;
            cout<<endl;
            plasare_ai_greu(jucator1,jucator2,turn);
            if(verificare_catigator(matrice_principala))
            {
                ok = false;
            }
        }
        vreti_sa_jucati_din_nou(jucator1,jucator2,ok,turn,varianta_raspuns);
    }
}


///----------------------------------------------------------AICI SE TERMINA FUNCTIILE PT ROBOT--------------------------------------------------------------------------------------------------------------


void main_adevarat() ///functia main adevarat care ma ajuta sa ma intorc in meniu din alte pagini
{
    B[21].D.SS.x = 0;
    B[21].D.DJ.x = 0;
    B[21].D.SS.y = 0;
    B[21].D.DJ.y = 0;

    B[22].D.SS.x = 0;
    B[22].D.DJ.x = 0;
    B[22].D.SS.y = 0;
    B[22].D.DJ.y = 0;

    B[23].D.SS.x = 0;
    B[23].D.DJ.x = 0;
    B[23].D.SS.y = 0;
    B[23].D.DJ.y = 0;


    B[24].D.SS.x = 0;
    B[24].D.DJ.x = 0;
    B[24].D.SS.y = 0;
    B[24].D.DJ.y = 0;


    B[25].D.SS.x = 0;
    B[25].D.DJ.x = 0;
    B[25].D.SS.y = 0;
    B[25].D.DJ.y = 0;


    B[26].D.SS.x = 0;
    B[26].D.DJ.x = 0;
    B[26].D.SS.y = 0;
    B[26].D.DJ.y = 0;


    B[27].D.SS.x = 0;
    B[27].D.DJ.x = 0;
    B[27].D.SS.y = 0;
    B[27].D.DJ.y = 0;


    B[28].D.SS.x = 0;
    B[28].D.DJ.x = 0;
    B[28].D.SS.y = 0;
    B[28].D.DJ.y = 0;

    B[29].D.SS.x = 0;
    B[29].D.DJ.x = 0;
    B[29].D.SS.y = 0;
    B[29].D.DJ.y = 0;

    meniu(iarna);
    while(1)       ///pentru a verifica ce se intampla la fiecare apasare de buton
        verificare_buton();

}

//----------------------------------------------------BACK-END--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------MAIN--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    OpenGraph();
    main_adevarat();
    return 0;
}
//----------------------------------------------------MAIN--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------GRAFICA--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void OpenGraph()
{
    initwindow(width,height,"",-3,-3);
}


void animatie_iarna(int &iarna)
{
    cleardevice();
    if(iarna == 0)
    {

        iarna = 1;
        setfillstyle(SOLID_FILL, COLOR(191,221,229));
        bar(0, 0, width, height);
        readimagefile("snow.gif", 0, 0, width, height);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        setbkcolor(COLOR(191,221,229));
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
        if(lmb)
        {
            setcolor(RED);
            settextjustify(RIGHT_TEXT, CENTER_TEXT);
            outtextxy(width/2, height/2, "PREGATIM ");
            setcolor(GREEN);
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(width/2, height/2, "CRACIUNUL");
            PlaySound(TEXT("Mosu.wav"), NULL, SND_ASYNC);
        }
        else
        {
            setcolor(RED);
            settextjustify(RIGHT_TEXT, CENTER_TEXT);
            outtextxy(width/2, height/2, "CHRISTMAS IS");
            setcolor(GREEN);
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(width/2, height/2, " ON THE WAY!");
            PlaySound(TEXT("Mosu.wav"), NULL, SND_ASYNC);
        }

        delay(6000);

        PlaySound(TEXT("Clopotei.wav"), NULL, SND_ASYNC);
        setfillstyle(SOLID_FILL, GREEN);
        int ok = 1;

        if(width == 1920)
        {
            for(int i=480; i<1440; i+=64)
            {
                delay(200);
                setfillstyle(SOLID_FILL, GREEN);
                bar(i, height/2 - height/8, i+64, height/2 + height/8);
                setfillstyle(SOLID_FILL, RED);
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 - height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 + height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 - width/4 + 5, height/2 + height/8 - 5);
                bar(width/2 + width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 - 5);

            }
            delay(1000);

            setfillstyle(SOLID_FILL, COLOR(191,221,229));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            setfillstyle(SOLID_FILL, GREEN);
            delay(600);
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            setcolor(RED);
            setbkcolor(GREEN);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if(lmb) outtextxy(width/2, height/2, "CRACIUN FERICIT!");
            else outtextxy(width/2, height/2, "MERRY CHRISTMAS!");

            delay(600);
            setfillstyle(SOLID_FILL, COLOR(191,221,229));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            delay(600);

            setfillstyle(SOLID_FILL, GREEN);
            bar(480, height/2 - height/8, 1440, height/2 + height/8);


            setcolor(RED);
            setbkcolor(GREEN);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if(lmb) outtextxy(width/2, height/2, "CRACIUN FERICIT!");
            else outtextxy(width/2, height/2, "MERRY CHRISTMAS!");


            delay(600);
            setfillstyle(SOLID_FILL, COLOR(191,221,229));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            delay(600);
            setfillstyle(SOLID_FILL, GREEN);
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            setcolor(RED);
            setbkcolor(GREEN);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if(lmb) outtextxy(width/2, height/2, "CRACIUN FERICIT!");
            else outtextxy(width/2, height/2, "MERRY CHRISTMAS!");
        }
        else if(width == 1280)
        {
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            for(int i=320; i<960; i+=64)
            {
                delay(200);
                setfillstyle(SOLID_FILL, GREEN);
                bar(i, height/2 - height/8, i+64, height/2 + height/8);
                setfillstyle(SOLID_FILL, RED);
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 - height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 + height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 - width/4 + 5, height/2 + height/8 - 5);
                bar(width/2 + width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 - 5);

            }
            delay(1000);

            setfillstyle(SOLID_FILL, COLOR(191,221,229));
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setfillstyle(SOLID_FILL, GREEN);

            delay(600);
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setcolor(RED);
            setbkcolor(GREEN);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if(lmb) outtextxy(width/2, height/2, "CRACIUN FERICIT!");
            else outtextxy(width/2, height/2, "MERRY CHRISTMAS!");

            delay(600);
            setfillstyle(SOLID_FILL, COLOR(191,221,229));
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setfillstyle(SOLID_FILL, GREEN);

            delay(600);
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setcolor(RED);
            setbkcolor(GREEN);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if(lmb) outtextxy(width/2, height/2, "CRACIUN FERICIT!");
            else outtextxy(width/2, height/2, "MERRY CHRISTMAS!");


            delay(600);
            setfillstyle(SOLID_FILL, COLOR(191,221,229));
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setfillstyle(SOLID_FILL, GREEN);

            delay(600);
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setcolor(RED);
            setbkcolor(GREEN);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if(lmb) outtextxy(width/2, height/2, "CRACIUN FERICIT!");
            else outtextxy(width/2, height/2, "MERRY CHRISTMAS!");
        }

        delay(3000);
        melodie = 25;


        PlaySound(TEXT("craciun_50"), NULL, SND_ASYNC);

    }
    else
    {
        iarna = 0;
        PlaySound(0, 0, 0);
        cleardevice();
        setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
        bar(0, 0, width, height);
        if(width == 1920)
        {
            for(int i=480; i<1440; i+=64)
            {
                delay(200);
                setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
                bar(i, height/2 - height/8, i+64, height/2 + height/8);
                setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 - height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 + height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 - width/4 + 5, height/2 + height/8 - 5);
                bar(width/2 + width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 - 5);

            }
            delay(1000);

            setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
            delay(600);
            bar(480, height/2 - height/8, 1440, height/2 + height/8);



            delay(600);
            setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            delay(600);

            setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);




            delay(600);
            setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);

            delay(600);
            setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
            bar(480, height/2 - height/8, 1440, height/2 + height/8);


        }
        else if(width == 1280)
        {
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            for(int i=320; i<960; i+=64)
            {
                delay(200);
                setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
                bar(i, height/2 - height/8, i+64, height/2 + height/8);
                setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 - height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 + height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 + 5);
                bar(width/2 - width/4 - 5, height/2 - height/8 - 5, width/2 - width/4 + 5, height/2 + height/8 - 5);
                bar(width/2 + width/4 - 5, height/2 - height/8 - 5, width/2 + width/4 + 5, height/2 + height/8 - 5);

            }
            delay(1000);

            setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setfillstyle(SOLID_FILL, COLOR(0, 210, 255));

            delay(600);
            bar(320, height/2 - height/8, 960, height/2 + height/8);


            delay(600);
            setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setfillstyle(SOLID_FILL, COLOR(0, 210, 255));

            delay(600);
            bar(320, height/2 - height/8, 960, height/2 + height/8);



            delay(600);
            setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
            bar(320, height/2 - height/8, 960, height/2 + height/8);

            setfillstyle(SOLID_FILL, COLOR(0, 210, 255));

            delay(600);
            bar(320, height/2 - height/8, 960, height/2 + height/8);


        }

    }

    setbkcolor(BLACK);
    main_adevarat();
}

void clasament() ///pagina de clasament
{
    cleardevice();
    if(iarna == 0)
    {
        setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
        bar(0, 0, width, height);
        setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
        bar(0, height/12 - 5, width, height/12 + 5);
        bar(width/2 - 5, height/12, width/2 + 5, height);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        setcolor(COLOR(255, 83, 10));
        setbkcolor(COLOR(0, 210, 255));
    }

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    if(lmb)
    {

        outtextxy(width/4, height/24, "NUME");
        outtextxy(width/4 + width/4 + width/4, height/24, "NUMAR MUTARI");
    }
    else
    {
        outtextxy(width/4, height/24, "NAME");
        outtextxy(width/4 + width/4 + width/4, height/24, "NUMBER OF MOVES");
    }
    int pozitie = height/8, aux, inv;
    setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
    for(int i=1; i<=10; i++)
    {
        bar(0, i*height/12 - 2, width, i*height/12 + 2);
    }
    if(scor[1].nume != NULL) ///doar asa mergea, altfel aveam buguri :c
    {
        outtextxy(width/4, pozitie, scor[1].nume);
        switch(scor[1].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[2].nume != NULL)
    {

        outtextxy(width/4, pozitie, scor[2].nume);
        switch(scor[2].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[3].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[3].nume);
        switch(scor[3].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;

        }
        pozitie+=height/12;
    }
    if(scor[4].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[4].nume);
        switch(scor[4].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[5].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[5].nume);
        switch(scor[5].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[6].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[6].nume);
        switch(scor[6].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[7].nume != NULL)
    {
        inv = 0;
        outtextxy(width/4, pozitie, scor[7].nume);
        switch(scor[7].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[8].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[8].nume);
        switch(scor[8].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;

        }
        pozitie+=height/12;
    }
    if(scor[9].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[9].nume);
        switch(scor[9].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    if(scor[10].nume != NULL)
    {
        outtextxy(width/4, pozitie, scor[10].nume);
        switch(scor[10].punctaj-1)
        {
        case 0:
        {
            outtextxy(width/4+width/4+width/4,pozitie,"1");;
            break;
        }

        case 1:
            outtextxy(width/4+width/4+width/4,pozitie,"2");;
            break;

        case 2:
            outtextxy(width/4+width/4+width/4,pozitie,"3");;
            break;

        case 3:
            outtextxy(width/4+width/4+width/4,pozitie,"4");;
            break;

        case 4:
            outtextxy(width/4+width/4+width/4,pozitie,"5");;
            break;

        case 5:
            outtextxy(width/4+width/4+width/4,pozitie,"6");;
            break;

        case 6:
            outtextxy(width/4+width/4+width/4,pozitie,"7");;
            break;

        case 7:
            outtextxy(width/4+width/4+width/4,pozitie,"8");;
            break;

        case 8:
            outtextxy(width/4+width/4+width/4,pozitie,"9");;
            break;

        case 9:
            outtextxy(width/4+width/4+width/4,pozitie,"10");;
            break;

        case 10:
            outtextxy(width/4+width/4+width/4,pozitie,"11");;
            break;

        case 11:
            outtextxy(width/4+width/4+width/4,pozitie,"12");;
            break;

        case 12:
            outtextxy(width/4+width/4+width/4,pozitie,"13");;
            break;

        case 13:
            outtextxy(width/4+width/4+width/4,pozitie,"14");;
            break;

        case 14:
            outtextxy(width/4+width/4+width/4,pozitie,"15");;
            break;

        case 15:
            outtextxy(width/4+width/4+width/4,pozitie,"16");;
            break;

        case 16:
            outtextxy(width/4+width/4+width/4,pozitie,"17");;
            break;

        case 17:
            outtextxy(width/4+width/4+width/4,pozitie,"18");;
            break;

        case 18:
            outtextxy(width/4+width/4+width/4,pozitie,"19");;
            break;

        case 19:
            outtextxy(width/4+width/4+width/4,pozitie,"20");;
            break;

        case 20:
            outtextxy(width/4+width/4+width/4,pozitie,"21");;
            break;

        case 21:
            outtextxy(width/4+width/4+width/4,pozitie,"22");;
            break;

        case 22:
            outtextxy(width/4+width/4+width/4,pozitie,"23");;
            break;

        case 23:
            outtextxy(width/4+width/4+width/4,pozitie,"24");;
            break;

        case 24:
            outtextxy(width/4+width/4+width/4,pozitie,"25");;
            break;

        case 25:
            outtextxy(width/4+width/4+width/4,pozitie,"26");;
            break;

        case 26:
            outtextxy(width/4+width/4+width/4,pozitie,"27");;
            break;

        case 27:
            outtextxy(width/4+width/4+width/4,pozitie,"28");;
            break;

        case 28:
            outtextxy(width/4+width/4+width/4,pozitie,"29");;
            break;

        case 29:
            outtextxy(width/4+width/4+width/4,pozitie,"30");;
            break;

        case 30:
            outtextxy(width/4+width/4+width/4,pozitie,"31");;
            break;

        case 31:
            outtextxy(width/4+width/4+width/4,pozitie,"32");;
            break;

        case 32:
            outtextxy(width/4+width/4+width/4,pozitie,"33");;
            break;

        case 33:
            outtextxy(width/4+width/4+width/4,pozitie,"34");;
            break;

        case 34:
            outtextxy(width/4+width/4+width/4,pozitie,"35");;
            break;

        case 35:
            outtextxy(width/4+width/4+width/4,pozitie,"36");;
            break;

        case 36:
            outtextxy(width/4+width/4+width/4,pozitie,"37");;
            break;

        case 37:
            outtextxy(width/4+width/4+width/4,pozitie,"38");;
            break;

        case 38:
            outtextxy(width/4+width/4+width/4,pozitie,"39");;
            break;

        case 39:
            outtextxy(width/4+width/4+width/4,pozitie,"40");;
            break;

        case 40:
            outtextxy(width/4+width/4+width/4,pozitie,"41");;
            break;

        case 41:
            outtextxy(width/4+width/4+width/4,pozitie,"42");;
            break;

        case 42:
            outtextxy(width/4+width/4+width/4,pozitie,"43");;
            break;

        case 43:
            outtextxy(width/4+width/4+width/4,pozitie,"44");;
            break;

        case 44:
            outtextxy(width/4+width/4+width/4,pozitie,"45");;
            break;

        case 45:
            outtextxy(width/4+width/4+width/4,pozitie,"46");;
            break;

        case 46:
            outtextxy(width/4+width/4+width/4,pozitie,"47");;
            break;

        case 47:
            outtextxy(width/4+width/4+width/4,pozitie,"48");;
            break;

        case 48:
            outtextxy(width/4+width/4+width/4,pozitie,"49");;
            break;

        case 49:
            outtextxy(width/4+width/4+width/4,pozitie,"50");;
            break;

        case 50:
            outtextxy(width/4+width/4+width/4,pozitie,"51");;
            break;
        }
        pozitie+=height/12;
    }
    inapoi1();
    iesire1();
}
void meniu(int iarna) ///meniul
{
    refacere_table(jucator1,jucator2,ok,turn,odata);
    cleardevice();

    settextjustify(CENTER_TEXT, CENTER_TEXT);

    if(iarna)
    {

        if(width == 1920)
        {
            readimagefile("iarna_1920.gif", 0, 0, width, height);
            readimagefile("titlu_zapada.gif", width/2 - 300, 0, width/2 + 300, 300);
            readimagefile("dracii_craciun_1920.gif", 0, 0, width, height + 130);
        }
        else if(width == 1280)
        {
            readimagefile("iarna_1280.gif", 0, 0, width, height);
            readimagefile("titlu_zapada.gif", width/2 - 200, 0, width/2 + 200, 200);
            readimagefile("dracii_craciun_1280.gif", 0, 0, width, height + 70);
        }
    }
    else
    {
        setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
        bar(0, 0, width/2, height);
        setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
        bar(width/2, 0, width, height);
        if(width == 1920)
        {
            readimagefile("titlu.gif", width/2 - 300, 0, width/2 + 300, 300);
            readimagefile("dracii_1920.gif", 0, 0, width, height + 130);
        }
        else if(width == 1280)
        {
            readimagefile("titlu.gif", width/2 - 200, 0, width/2 + 200, 200);
            readimagefile("dracii_1280.gif", 0, 0, width, height + 70);
        }
    }

    setcolor(WHITE);
    for(int i=0; i<=6; i++)
    {
        switch(i)
        {
        case 0:
        {
            if(!iarna)
            {
                setbkcolor(COLOR(0, 210, 255));
                setcolor(COLOR(255, 83, 10));
            }
            else
            {
                setbkcolor(COLOR(191,221,229));
                setcolor(COLOR(101,149,153));
            }
            if(width == 1920)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[0].text, "CLASAMENT");
                    B[0].D.SS.x = width/3 + width/3 - textwidth("CLASAMENT")/2;
                    B[0].D.DJ.x = width/3 + width/3 + textwidth("CLASAMENT")/2;
                    B[0].D.SS.y = height/6 + height/6 + height/6 - textheight("CLASAMENT")/2;
                    B[0].D.DJ.y = height/6 + height/6 + height/6 + textheight("CLASAMENT")/2 - 7;
                    bar(B[0].D.SS.x, B[0].D.SS.y, B[0].D.DJ.x, B[0].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6 + height/6, B[0].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[0].text, "SCOREBOARD");
                    B[0].D.SS.x = width/3 + width/3 - textwidth("SCOREBOARD")/2;
                    B[0].D.DJ.x = width/3 + width/3 + textwidth("SCOREBOARD")/2;
                    B[0].D.SS.y = height/6 + height/6 + height/6 - textheight("SCOREBOARD")/2;
                    B[0].D.DJ.y = height/6 + height/6 + height/6 + textheight("SCOREBOARD")/2 - 7;
                    bar(B[0].D.SS.x, B[0].D.SS.y, B[0].D.DJ.x, B[0].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6 + height/6, B[0].text);
                }
            }
            else if(width == 1280)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[0].text, "CLASAMENT");
                    B[0].D.SS.x = width/3 + width/3 - textwidth("CLASAMENT")/2;
                    B[0].D.DJ.x = width/3 + width/3 + textwidth("CLASAMENT")/2;
                    B[0].D.SS.y = height/6 + height/6 + height/6 - textheight("CLASAMENT")/2;
                    B[0].D.DJ.y = height/6 + height/6 + height/6 + textheight("CLASAMENT")/2 - 5;
                    bar(B[0].D.SS.x, B[0].D.SS.y, B[0].D.DJ.x, B[0].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6 + height/6, B[0].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[0].text, "SCOREBOARD");
                    B[0].D.SS.x = width/3 + width/3 - textwidth("SCOREBOARD")/2;
                    B[0].D.DJ.x = width/3 + width/3 + textwidth("SCOREBOARD")/2;
                    B[0].D.SS.y = height/6 + height/6 + height/6 - textheight("SCOREBOARD")/2;
                    B[0].D.DJ.y = height/6 + height/6 + height/6 + textheight("SCOREBOARD")/2 - 5;
                    bar(B[0].D.SS.x, B[0].D.SS.y, B[0].D.DJ.x, B[0].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6 + height/6, B[0].text);
                }
            }
        }
        case 1:
        {
            if(!iarna)
            {
                setbkcolor(COLOR(255, 83, 10));
                setcolor(COLOR(0, 210, 255));
            }
            else
            {
                setbkcolor(COLOR(191,221,229));
                setcolor(COLOR(101,149,153));
            }
            if(width == 1920)
            {
                if(lmb)
                {

                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[1].text, "JUCATOR VS JUCATOR");                      ///asa cream butoanele, folosind structura definita si cate un index pt ficare buton. Indexurile sunt vai de capul lor
                    B[1].D.SS.x = width/3 - textwidth("JUCATOR VS JUCATOR")/2;
                    B[1].D.DJ.x = width/3 + textwidth("JUCATOR VS JUCATOR")/2;
                    B[1].D.SS.y = height/6 + height/6 - textheight("JUCATOR VS JUCATOR")/2;
                    B[1].D.DJ.y = height/6 + height/6 + textheight("JUCATOR VS JUCATOR")/2 - 7;
                    bar(B[1].D.SS.x, B[1].D.SS.y, B[1].D.DJ.x, B[1].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6, B[1].text);
                }
                else ///engleza
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[1].text, "PLAYER VS PLAYER");
                    B[1].D.SS.x = width/3 - textwidth("PLAYER VS PLAYER")/2;
                    B[1].D.DJ.x = width/3 + textwidth("PLAYER VS PLAYER")/2;
                    B[1].D.SS.y = height/6 + height/6 - textheight("PLAYER VS PLAYER")/2;
                    B[1].D.DJ.y = height/6 + height/6 + textheight("PLAYER VS PLAYER")/2 - 7;
                    bar(B[1].D.SS.x, B[1].D.SS.y, B[1].D.DJ.x, B[1].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6, B[1].text);
                }
            }
            else if(width == 1280)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[1].text, "JUCATOR VS JUCATOR");
                    B[1].D.SS.x = width/3 - textwidth("JUCATOR VS JUCATOR")/2;
                    B[1].D.DJ.x = width/3 + textwidth("JUCATOR VS JUCATOR")/2;
                    B[1].D.SS.y = height/6 + height/6 - textheight("JUCATOR VS JUCATOR")/2;
                    B[1].D.DJ.y = height/6 + height/6, + textheight("JUCATOR VS JUCATOR")/2 - 5;
                    bar(B[1].D.SS.x, B[1].D.SS.y, B[1].D.DJ.x, B[1].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6, B[1].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[1].text, "PLAYER VS PLAYER");
                    B[1].D.SS.x = width/3 - textwidth("PLAYER VS PLAYER")/2 + 10;
                    B[1].D.DJ.x = width/3 + textwidth("PLAYER VS PLAYER")/2 - 10;
                    B[1].D.SS.y = height/6 + height/6 - textheight("PLAYER VS PLAYER")/2;
                    B[1].D.DJ.y = height/6 + height/6, + textheight("PLAYER VS PLAYER")/2 - 5;
                    bar(B[1].D.SS.x, B[1].D.SS.y, B[1].D.DJ.x, B[1].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6, B[1].text);
                }
            }
        }
        case 2:
        {
            if(!iarna)
            {
                setbkcolor(COLOR(0, 210, 255));
                setcolor(COLOR(255, 83, 10));
            }
            else
            {
                setbkcolor(COLOR(191,221,229));
                setcolor(COLOR(101,149,153));
            }
            if(width == 1920)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[2].text, "JUCATOR VS CALCULATOR");
                    B[2].D.SS.x = width/3 + width/3 - textwidth("JUCATOR VS CALCULATOR")/2;
                    B[2].D.DJ.x = width/3 + width/3 + textwidth("JUCATOR VS CALCULATOR")/2;
                    B[2].D.SS.y = height/6 + height/6 - textheight("JUCATOR VS CALCULATOR")/2;
                    B[2].D.DJ.y = height/6 + height/6 + textheight("JUCATOR VS CALCULATOR")/2 - 7;
                    bar(B[2].D.SS.x, B[2].D.SS.y, B[2].D.DJ.x, B[2].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6, B[2].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[2].text, "PLAYER VS COMPUTER");
                    B[2].D.SS.x = width/3 + width/3 - textwidth("PLAYER VS COMPUTER")/2 + 10;
                    B[2].D.DJ.x = width/3 + width/3 + textwidth("PLAYER VS COMPUTER")/2 - 10;
                    B[2].D.SS.y = height/6 + height/6 - textheight("PLAYER VS COMPUTER")/2;
                    B[2].D.DJ.y = height/6 + height/6 + textheight("PLAYER VS COMPUTER")/2 - 7;
                    bar(B[2].D.SS.x, B[2].D.SS.y, B[2].D.DJ.x, B[2].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6, B[2].text);
                }
            }
            else if(width == 1280)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[2].text, "JUCATOR VS CALCULATOR");
                    B[2].D.SS.x = width/3 + width/3 - textwidth("JUCATOR VS CALCULATOR")/2;
                    B[2].D.DJ.x = width/3 + width/3 + textwidth("JUCATOR VS CALCULATOR")/2;
                    B[2].D.SS.y = height/6 + height/6 - textheight("JUCATOR VS CALCULATOR")/2;
                    B[2].D.DJ.y = height/6 + height/6 + textheight("JUCATOR VS CALCULATOR")/2 - 5;
                    bar(B[2].D.SS.x, B[2].D.SS.y, B[2].D.DJ.x, B[2].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6, B[2].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[2].text, "PLAYER VS COMPUTER");
                    B[2].D.SS.x = width/3 + width/3 - textwidth("PLAYER VS COMPUTER")/2;
                    B[2].D.DJ.x = width/3 + width/3 + textwidth("PLAYER VS COMPUTER")/2;
                    B[2].D.SS.y = height/6 + height/6 - textheight("PLAYER VS COMPUTER")/2;
                    B[2].D.DJ.y = height/6 + height/6 + textheight("PLAYER VS COMPUTER")/2 - 5;
                    bar(B[2].D.SS.x, B[2].D.SS.y, B[2].D.DJ.x, B[2].D.DJ.y);
                    outtextxy(width/3 + width/3, height/6 + height/6, B[2].text);
                }
            }
        }
        case 3:
        {
            if(!iarna)
            {
                setbkcolor(COLOR(255, 83, 10));
                setcolor(COLOR(0, 210, 255));
            }
            else
            {
                setbkcolor(COLOR(191,221,229));
                setcolor(COLOR(101,149,153));
            }
            if(width == 1920)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[3].text, "REGULAMENT");
                    B[3].D.SS.x = width/3 - textwidth("REGULAMENT")/2;
                    B[3].D.DJ.x = width/3 + textwidth("REGULAMENT")/2;
                    B[3].D.SS.y = height/6 + height/6 + height/6 - textheight("REGULAMENT")/2;
                    B[3].D.DJ.y = height/6 + height/6 + height/6 + textheight("REGULAMENT")/2 - 7;
                    bar(B[3].D.SS.x, B[3].D.SS.y, B[3].D.DJ.x, B[3].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6 + height/6, B[3].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[3].text, "RULES");
                    B[3].D.SS.x = width/3 - textwidth("RULES")/2;
                    B[3].D.DJ.x = width/3 + textwidth("RULES")/2;
                    B[3].D.SS.y = height/6 + height/6 + height/6 - textheight("RULES")/2;
                    B[3].D.DJ.y = height/6 + height/6 + height/6 + textheight("RULES")/2 - 7;
                    bar(B[3].D.SS.x, B[3].D.SS.y, B[3].D.DJ.x, B[3].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6 + height/6, B[3].text);

                }
            }
            else if(width == 1280)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[3].text, "REGULAMENT");
                    B[3].D.SS.x = width/3 - textwidth("REGULAMENT")/2;
                    B[3].D.DJ.x = width/3 + textwidth("REGULAMENT")/2;
                    B[3].D.SS.y = height/6 + height/6 + height/6 - textheight("REGULAMENT")/2;
                    B[3].D.DJ.y = height/6 + height/6 + height/6 + textheight("REGULAMENT")/2 - 5;
                    bar(B[3].D.SS.x, B[3].D.SS.y, B[3].D.DJ.x, B[3].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6 + height/6, B[3].text);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[3].text, "RULES");
                    B[3].D.SS.x = width/3 - textwidth("RULES")/2;
                    B[3].D.DJ.x = width/3 + textwidth("RULES")/2;
                    B[3].D.SS.y = height/6 + height/6 + height/6 - textheight("RULES")/2;
                    B[3].D.DJ.y = height/6 + height/6 + height/6 + textheight("RULES")/2 - 5;
                    bar(B[3].D.SS.x, B[3].D.SS.y, B[3].D.DJ.x, B[3].D.DJ.y);
                    outtextxy(width/3, height/6 + height/6 + height/6, B[3].text);
                }

            }
        }
        case 4:
        {
            if(width == 1920)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[4].text, "SETARI");
                    B[4].D.SS.x = width/2 - textwidth("SETARI")/2;
                    B[4].D.DJ.x = width/2 + textwidth("SETARI")/2;
                    B[4].D.SS.y = height/6 + height/6 + height/6 + height/6 - textheight("SETARI")/2;
                    B[4].D.DJ.y = height/6 + height/6 + height/6 + height/6 + textheight("SETARI")/2 - 7;

                    if(!iarna)
                    {
                        setbkcolor(COLOR(255, 83, 10));
                        setcolor(COLOR(0, 210, 255));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(RIGHT_TEXT, CENTER_TEXT);
                    outtextxy(width/2, height/6 + height/6 + height/6 + height/6,"SET");
                    if(!iarna)
                    {
                        setbkcolor(COLOR(0, 210, 255));
                        setcolor(COLOR(255, 83, 10));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(LEFT_TEXT, CENTER_TEXT);
                    outtextxy(width/2+2, height/6 + height/6 + height/6 + height/6,"ARI");
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                    strcpy(B[4].text, "SETTINGS");
                    B[4].D.SS.x = width/2 - textwidth("SETTINGS")/2;
                    B[4].D.DJ.x = width/2 + textwidth("SETTINGS")/2;
                    B[4].D.SS.y = height/6 + height/6 + height/6 + height/6 - textheight("SETTINGS")/2;
                    B[4].D.DJ.y = height/6 + height/6 + height/6 + height/6 + textheight("SETTINGS")/2 - 7;
                    bar(B[4].D.SS.x, B[4].D.SS.y, B[4].D.DJ.x, B[4].D.DJ.y);
                    if(!iarna)
                    {
                        setbkcolor(COLOR(255, 83, 10));
                        setcolor(COLOR(0, 210, 255));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(RIGHT_TEXT, CENTER_TEXT);
                    outtextxy(width/2, height/6 + height/6 + height/6 + height/6,"SETT");
                    if(!iarna)
                    {
                        setbkcolor(COLOR(0, 210, 255));
                        setcolor(COLOR(255, 83, 10));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(LEFT_TEXT, CENTER_TEXT);
                    outtextxy(width/2+1, height/6 + height/6 + height/6 + height/6,"INGS");
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                }
            }
            else if(width == 1280)
            {
                if(lmb)
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[4].text, "SETARI");
                    B[4].D.SS.x = width/2 - textwidth("SETARI")/2;
                    B[4].D.DJ.x = width/2 + textwidth("SETARI")/2;
                    B[4].D.SS.y = height/6 + height/6 + height/6 + height/6 - textheight("SETARI")/2;
                    B[4].D.DJ.y = height/6 + height/6 + height/6 + height/6 + textheight("SETARI")/2 - 5;
                    bar(B[4].D.SS.x, B[4].D.SS.y, B[4].D.DJ.x, B[4].D.DJ.y);
                    if(!iarna)
                    {
                        setbkcolor(COLOR(255, 83, 10));
                        setcolor(COLOR(0, 210, 255));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(RIGHT_TEXT, CENTER_TEXT);
                    outtextxy(width/2, height/6 + height/6 + height/6 + height/6,"SET");
                    if(!iarna)
                    {
                        setbkcolor(COLOR(0, 210, 255));
                        setcolor(COLOR(255, 83, 10));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(LEFT_TEXT, CENTER_TEXT);
                    outtextxy(width/2+2, height/6 + height/6 + height/6 + height/6,"ARI");
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                }
                else
                {
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    strcpy(B[4].text, "SETTINGS");
                    B[4].D.SS.x = width/2 - textwidth("SETTINGS")/2;
                    B[4].D.DJ.x = width/2 + textwidth("SETTINGS")/2;
                    B[4].D.SS.y = height/6 + height/6 + height/6 + height/6 - textheight("SETTINGS")/2;
                    B[4].D.DJ.y = height/6 + height/6 + height/6 + height/6 + textheight("SETTINGS")/2 - 5;
                    bar(B[4].D.SS.x, B[4].D.SS.y, B[4].D.DJ.x, B[4].D.DJ.y);
                    if(!iarna)
                    {
                        setcolor(COLOR(0, 210, 255));
                        setbkcolor(COLOR(255, 83, 10));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(RIGHT_TEXT, CENTER_TEXT);
                    outtextxy(width/2, height/6 + height/6 + height/6 + height/6,"SETT");
                    if(!iarna)
                    {
                        setbkcolor(COLOR(0, 210, 255));
                        setcolor(COLOR(255, 83, 10));
                    }
                    else
                    {
                        setbkcolor(COLOR(191,221,229));
                        setcolor(COLOR(101,149,153));
                    }
                    settextjustify(LEFT_TEXT, CENTER_TEXT);
                    outtextxy(width/2+1, height/6 + height/6 + height/6 + height/6,"INGS");
                    settextjustify(CENTER_TEXT, CENTER_TEXT);

                }
            }
        }
        }
        iesire1();
    }
}

void afisare_table(piese jucator,bool ok, int turn, int &odata) ///functia care afiseaza tablele dupa fiecare mutare SI care afiseaza orice inseamna grafica in cadrul jocului propriu-zis
{
    int i,j;

    setbkcolor(BLACK);
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            cout<<" ";
            if(matrice_principala[i][j]=='-')
                cout<<tabla[i][j];
            else
                cout<<matrice_principala[i][j];
            if(j!=2)
                cout<<" | ";
        }
        cout<<endl;
    }
    if(width == 1920)
    {
        cleardevice();
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        setlinestyle(SOLID_LINE,5,8);
        setcolor(jucator1.clr);
        settextjustify(RIGHT_TEXT, CENTER_TEXT);
        outtextxy(960, 30, "GOBBLET ");
        setcolor(jucator2.clr);
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        outtextxy(960, 30, "GOBBLERS");

        settextjustify(CENTER_TEXT, CENTER_TEXT);

        if(turn % 2 == 1)
        {
            setlinestyle(SOLID_LINE,5,8); /// nu merge cum trebuie :c
            setcolor(jucator1.clr);
            line(660, 240, 1260, 240);
            line(1260, 240, 1260, 840);
            setcolor(jucator2.clr);
            line(660, 840, 1260, 840);
            line(660, 240, 660, 840);

        }
        else
        {
            if(odata == 1) ///odata este folosit pentru ca crea o animatie atunci cand incepe jocul; animatia se face pentru odata == 0
            {
                setlinestyle(SOLID_LINE,5,8);
                setcolor(jucator2.clr);
                line(660,240,1260,240);
                line(1260, 240, 1260, 840);
                setcolor(jucator1.clr);
                line(660, 840, 1260, 840);
                line(660, 240, 660, 840);

            }
            else if(odata == 0)
            {
                setlinestyle(SOLID_LINE,5,8);
                delay(250);
                setcolor(jucator2.clr);
                line(660,240,1260,240);
                delay(250);
                setcolor(jucator1.clr);
                line(660, 840, 1260, 840);
                delay(250);
                setcolor(jucator2.clr);
                line(1260, 240, 1260, 840);
                delay(250);
                setcolor(jucator1.clr);
                line(660, 240, 660, 840);
            }
        }

        setlinestyle(SOLID_LINE,5,8);
        setcolor(jucator1.clr);
        if(odata == 0) delay(250);
        line(660,440,1260,440);
        setcolor(jucator2.clr);
        if(odata == 0) delay(250);
        line(860,240,860,840);
        setcolor(jucator1.clr);
        if(odata == 0) delay(250);
        line(660,640,1260,640);
        setcolor(jucator2.clr);
        if(odata == 0) delay(250);
        line(1060,240,1060,840);
        char aux[200];
        setcolor(WHITE);

        if(odata == 0) delay(600);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,3);

        setlinestyle(SOLID_LINE,5,2);

        char vec_jucator1[4];
        vec_jucator1[0] = jucator1.gobblet[0][1];
        vec_jucator1[1] = jucator1.gobblet[1][1];
        vec_jucator1[2] = jucator1.gobblet[2][1];
        vec_jucator1[3]='\0';
        char vec_jucator2[4];
        vec_jucator2[0] = jucator2.gobblet[0][1];
        vec_jucator2[1] = jucator2.gobblet[1][1];
        vec_jucator2[2] = jucator2.gobblet[2][1];
        vec_jucator2[3]='\0';

        if(matrice_principala[0][0]=='-') ///de aici mult in jos, se creeaza cercurile (piesele) de fiecare data cand se afiseaza tabla, in cazul in care exista o piesa pe acea pozitie
            outtextxy(760,340,"");        /// luam fiecare pozitie in parte si verificam ce piesa este pe pozitia respectiva si un functie de rezultat, desenam cercuri
        else
        {
            aux[0]=matrice_principala[0][0];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(760,340,40);
                for(int i=0; i<=39; i++)
                {
                    circle(760,340,i);  ///pentru a umple cercul
                }
            }
            if(aux[0]=='Y')
            {
                circle(760,340,60);
                for(int i=0; i<=59; i++)
                {
                    circle(760,340,i);
                }
            }
            if (aux[0]=='Z')
            {
                circle(760,340,80);
                for(int i=0; i<=79; i++)
                {
                    circle(760,340,i);
                }
            }
            if (aux[0]=='A')
            {
                circle(760,340,40);
                for(int i=0; i<=39; i++)
                {
                    circle(760,340,i);
                }
            }
            if (aux[0]=='B')
            {
                circle(760,340,60);
                for(int i=0; i<=59; i++)
                {
                    circle(760,340,i);
                }
            }
            if (aux[0]=='C')
            {
                circle(760,340,80);
                for(int i=0; i<=79; i++)
                {
                    circle(760,340,i);
                }
            }
            setcolor(WHITE);
        }

        if(matrice_principala[1][0]=='-')
            outtextxy(760,540,"");
        else
        {
            aux[0]=matrice_principala[1][0];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(760,540,40);
                for(int i=0; i<=39; i++)
                    circle(760,540,i);
            }
            if(aux[0]=='Y')
            {
                circle(760,540,60);
                for(int i=0; i<=59; i++)
                    circle(760,540,i);
            }
            if (aux[0]=='Z')
            {
                circle(760,540,80);
                for(int i=0; i<=79; i++)
                    circle(760,540,i);
            }
            if (aux[0]=='A')
            {
                circle(760,540,40);
                for(int i=0; i<=39; i++)
                    circle(760,540,i);
            }
            if (aux[0]=='B')
            {
                circle(760,540,60);
                for(int i=0; i<=59; i++)
                    circle(760,540,i);
            }
            if (aux[0]=='C')
            {
                circle(760,540,80);
                for(int i=0; i<=79; i++)
                    circle(760,540,i);
            }
            setcolor(WHITE);
        }

        if(matrice_principala[2][0]=='-')
            outtextxy(760,740,"");
        else
        {
            aux[0]=matrice_principala[2][0];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(760,740,40);
                for(int i=0; i<=39; i++)
                    circle(760,740,i);
            }
            if(aux[0]=='Y')
            {
                circle(760,740,60);
                for(int i=0; i<=59; i++)
                    circle(760,740,i);
            }
            if (aux[0]=='Z')
            {
                circle(760,740,80);
                for(int i=0; i<=79; i++)
                    circle(760,740,i);
            }
            if (aux[0]=='A')
            {
                circle(760,740,40);
                for(int i=0; i<=39; i++)
                    circle(760,740,i);
            }
            if (aux[0]=='B')
            {
                circle(760,740,60);
                for(int i=0; i<=59; i++)
                    circle(760,740,i);
            }
            if (aux[0]=='C')
            {
                circle(760,740,80);
                for(int i=0; i<=79; i++)
                    circle(760,740,i);
            }
            setcolor(WHITE);
        }

        if(matrice_principala[0][1]=='-')
            outtextxy(960,340,"");
        else
        {
            aux[0]=matrice_principala[0][1];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(960,340,40);
                for(int i=0; i<=39; i++)
                    circle(960,340,i);
            }
            if(aux[0]=='Y')
            {
                circle(960,340,60);
                for(int i=0; i<=59; i++)
                    circle(960,340,i);
            }
            if (aux[0]=='Z')
            {
                circle(960,340,80);
                for(int i=0; i<=79; i++)
                    circle(960,340,i);
            }
            if (aux[0]=='A')
            {
                circle(960,340,40);
                for(int i=0; i<=39; i++)
                    circle(960,340,i);
            }
            if (aux[0]=='B')
            {
                circle(960,340,60);
                for(int i=0; i<=59; i++)
                    circle(960,340,i);
            }
            if (aux[0]=='C')
            {
                circle(960,340,80);
                for(int i=0; i<=79; i++)
                    circle(960,340,i);
            }
            setcolor(WHITE);
        }

        if(matrice_principala[1][1]=='-')
            outtextxy(960,540,"");
        else
        {
            aux[0]=matrice_principala[1][1];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(960,540,40);
                for(int i=0; i<=39; i++)
                    circle(960,540,i);
            }
            if(aux[0]=='Y')
            {
                circle(960,540,60);
                for(int i=0; i<=59; i++)
                    circle(960,540,i);
            }
            if (aux[0]=='Z')
            {
                circle(960,540,80);
                for(int i=0; i<=79; i++)
                    circle(960,540,i);
            }
            if (aux[0]=='A')
            {
                circle(960,540,40);
                for(int i=0; i<=39; i++)
                    circle(960,540,i);
            }
            if (aux[0]=='B')
            {
                circle(960,540,60);
                for(int i=0; i<=59; i++)
                    circle(960,540,i);
            }
            if (aux[0]=='C')
            {
                circle(960,540,80);
                for(int i=0; i<=79; i++)
                    circle(960,540,i);
            }
            setcolor(WHITE);
        }
        if(matrice_principala[2][1]=='-')
            outtextxy(960,740,"");
        else
        {
            aux[0]=matrice_principala[2][1];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(960,740,40);
                for(int i=0; i<=39; i++)
                    circle(960,740,i);
            }
            if(aux[0]=='Y')
            {
                circle(960,740,60);
                for(int i=0; i<=59; i++)
                    circle(960,740,i);
            }
            if (aux[0]=='Z')
            {
                circle(960,740,80);
                for(int i=0; i<=79; i++)
                    circle(960,740,i);
            }
            if (aux[0]=='A')
            {
                circle(960,740,40);
                for(int i=0; i<=39; i++)
                    circle(960,740,i);
            }
            if (aux[0]=='B')
            {
                circle(960,740,60);
                for(int i=0; i<=59; i++)
                    circle(960,740,i);
            }
            if (aux[0]=='C')
            {
                circle(960,740,80);
                for(int i=0; i<=79; i++)
                    circle(960,740,i);
            }
            setcolor(WHITE);
        }

        if(matrice_principala[0][2]=='-')
            outtextxy(1160,340,"");
        else
        {
            aux[0]=matrice_principala[0][2];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(1160,340,40);
                for(int i=0; i<=39; i++)
                    circle(1160,340,i);
            }
            if(aux[0]=='Y')
            {
                circle(1160,340,60);
                for(int i=0; i<=59; i++)
                    circle(1160,340,i);
            }
            if (aux[0]=='Z')
            {
                circle(1160,340,80);
                for(int i=0; i<=79; i++)
                    circle(1160,340,i);
            }
            if (aux[0]=='A')
            {
                circle(1160,340,40);
                for(int i=0; i<=39; i++)
                    circle(1160,340,i);
            }
            if (aux[0]=='B')
            {
                circle(1160,340,60);
                for(int i=0; i<=59; i++)
                    circle(1160,340,i);
            }
            if (aux[0]=='C')
            {
                circle(1160,340,80);
                for(int i=0; i<=79; i++)
                    circle(1160,340,i);
            }
            setcolor(WHITE);
        }
        if(matrice_principala[1][2]=='-')
            outtextxy(1160,540,"");
        else
        {
            aux[0]=matrice_principala[1][2];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(1160,540,40);
                for(int i=0; i<=39; i++)
                    circle(1160,540,i);
            }
            if(aux[0]=='Y')
            {
                circle(1160,540,60);
                for(int i=0; i<=59; i++)
                    circle(1160,540,i);
            }
            if (aux[0]=='Z')
            {
                circle(1160,540,80);
                for(int i=0; i<=79; i++)
                    circle(1160,540,i);
            }
            if (aux[0]=='A')
            {
                circle(1160,540,40);
                for(int i=0; i<=39; i++)
                    circle(1160,540,i);
            }
            if (aux[0]=='B')
            {
                circle(1160,540,60);
                for(int i=0; i<=59; i++)
                    circle(1160,540,i);
            }
            if (aux[0]=='C')
            {
                circle(1160,540,80);
                for(int i=0; i<=79; i++)
                    circle(1160,540,i);
            }
            setcolor(WHITE);
        }
        if(matrice_principala[2][2]=='-')
            outtextxy(1160,740,"");
        else
        {
            aux[0]=matrice_principala[2][2];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0]=='X')
            {
                circle(1160,740,40);
                for(int i=0; i<=39; i++)
                    circle(1160,740,i);
            }
            if(aux[0]=='Y')
            {
                circle(1160,740,60);
                for(int i=0; i<=59; i++)
                    circle(1160,740,i);
            }
            if(aux[0]=='Z')
            {
                circle(1160,740,80);
                for(int i=0; i<=79; i++)
                    circle(1160,740,i);
            }
            if(aux[0]=='A')
            {
                circle(1160,740,40);
                for(int i=0; i<=39; i++)
                    circle(1160,740,i);
            }
            if(aux[0]=='B')
            {
                circle(1160,740,60);
                for(int i=0; i<=59; i++)
                    circle(1160,740,i);
            }
            if(aux[0]=='C')
            {
                circle(1160,740,80);
                for(int i=0; i<=79; i++)
                    circle(1160,740,i);
            }

            setcolor(WHITE);
        }

///------- AICI SE TERMINA DE AFISAT PIESELE PE TABLA-------------------------------------------------------------------------------------------------------------------

        setbkcolor(BLACK); ///de aici desenam cercurile din afara tablei, pe care se apasa pentru a le folosi
        settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        setcolor(jucator1.clr);
        if(turn % 2 == 0)
            if(lmb)
                outtextxy(960, 845, jucator1.nume[jucator1.nrNume]);
            else
                outtextxy(960, 845, jucator1.nume[jucator1.nrNume]);
        if(jucator1.nr_mare+jucator1.nr_mediu+jucator1.nr_mic)
        {
            if(jucator1.nr_mic == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator1.clr);
                for(int i=0; i<=50; i++)
                    circle(330, 340, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(330, 340, "x2");
            }

            else if(jucator1.nr_mic == 1)
            {
                setcolor(jucator1.clr);
                for(int i=0; i<=50; i++)
                    circle(330, 340, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(330, 340, "x1");
            }
            if(jucator1.nr_mediu == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator1.clr);
                for(int i=0; i<=60; i++)
                    circle(330, 540, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(330, 540, "x2");
            }
            else if(jucator1.nr_mediu == 1)
            {
                setcolor(jucator1.clr);
                for(int i=0; i<=60; i++)
                    circle(330, 540, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(330, 540, "x1");
            }
            if(jucator1.nr_mare == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator1.clr);
                for(int i=0; i<=70; i++)
                    circle(330, 740, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(330, 740, "x2");
            }
            else if(jucator1.nr_mare == 1)
            {
                setcolor(jucator1.clr);
                for(int i=0; i<=70; i++)
                    circle(330, 740, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(330, 740, "x1");
            }
        }
        setcolor(jucator2.clr);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        setbkcolor(BLACK);
        if(turn % 2 != 0)
            if(lmb)
                outtextxy(960, 845, jucator2.nume[jucator2.nrNume]);
            else
                outtextxy(960, 845, jucator2.nume[jucator2.nrNume]);
        if(jucator2.nr_mare+jucator2.nr_mediu+jucator2.nr_mic)
        {
            if(jucator2.nr_mic == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator2.clr);
                for(int i=0; i<=50; i++)
                    circle(1590, 340, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1590, 340, "x2");
            }

            else if(jucator2.nr_mic == 1)
            {
                setcolor(jucator2.clr);
                for(int i=0; i<=50; i++)
                    circle(1590, 340, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1590, 340, "x1");
            }
            if(jucator2.nr_mediu == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator2.clr);
                for(int i=0; i<=60; i++)
                    circle(1590, 540, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1590, 540, "x2");
            }
            else if(jucator2.nr_mediu == 1)
            {
                setcolor(jucator2.clr);
                for(int i=0; i<=60; i++)
                    circle(1590, 540, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1590, 540, "x1");
            }
            if(jucator2.nr_mare == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator2.clr);
                for(int i=0; i<=70; i++)
                    circle(1590, 740, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1590, 740, "x2");
            }
            else if(jucator2.nr_mare == 1)
            {
                setcolor(jucator2.clr);
                for(int i=0; i<=70; i++)
                    circle(1590, 740, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1590, 740, "x1");
            }
            setbkcolor(BLACK);
        }
        odata = 1;
    }
    else if(width == 1280)
    {
        cleardevice();
        settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        setcolor(jucator1.clr);
        outtextxy(540, 30, "GOBBLET");
        setcolor(jucator2.clr);
        outtextxy(740, 30, "GOBBLERS");
        if(turn % 2 == 1)
        {
            setlinestyle(SOLID_LINE,5,8);
            setcolor(jucator1.clr);
            line(430, 150, 850, 150);
            line(850, 150, 850, 570);
            setcolor(jucator2.clr);
            line(430, 570, 850, 570);
            line(430, 150, 430, 570);
        }
        else
        {
            if(odata == 1)
            {
                setlinestyle(SOLID_LINE,5,8);
                setcolor(jucator2.clr);
                line(430, 150, 850, 150);
                line(850, 150, 850, 570);
                setcolor(jucator1.clr);
                line(430, 570, 850, 570);
                line(430, 150, 430, 570);
            }
            else if(odata == 0)
            {
                setlinestyle(SOLID_LINE,5,8);
                delay(250);
                setcolor(jucator2.clr);
                line(430, 150, 850, 150);
                delay(250);
                setcolor(jucator1.clr);
                line(430, 570, 850, 570);
                delay(250);
                setcolor(jucator2.clr);
                line(850, 150, 850, 570);
                delay(250);
                setcolor(jucator1.clr);
                line(430, 150, 430, 570);
            }
        }
        setlinestyle(SOLID_LINE,5,8);
        setcolor(jucator1.clr);
        if(odata == 0) delay(250);
        line(430,290,850,290);
        setcolor(jucator2.clr);
        if(odata == 0) delay(250);
        line(570,150,570,570);
        setcolor(jucator1.clr);
        if(odata == 0) delay(250);
        line(430,430,850,430);
        setcolor(jucator2.clr);
        if(odata == 0) delay(250);
        line(710,150,710,570);
        char aux[200];
        setcolor(WHITE);
        if(odata == 0) delay(600);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,3);

        setlinestyle(SOLID_LINE,5,2);

        char vec_jucator1[4];
        vec_jucator1[0] = jucator1.gobblet[0][1];
        vec_jucator1[1] = jucator1.gobblet[1][1];
        vec_jucator1[2] = jucator1.gobblet[2][1];
        vec_jucator1[3] = '\0';
        char vec_jucator2[4];
        vec_jucator2[0] = jucator2.gobblet[0][1];
        vec_jucator2[1] = jucator2.gobblet[1][1];
        vec_jucator2[2] = jucator2.gobblet[2][1];
        vec_jucator2[3] = '\0';

        if(matrice_principala[0][0] == '-')
            outtextxy(500,220," ");
        else
        {
            aux[0] = matrice_principala[0][0];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1] = '\0';
            if(aux[0] == 'X')
            {
                circle(500,220,35);
                for(int i=0; i<=34; i++)
                {
                    circle(500,220,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(500,220,45);
                for(int i=0; i<=44; i++)
                {
                    circle(500,220,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(500,220,55);
                for(int i=0; i<=54; i++)
                {
                    circle(500,220,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(500,220,35);
                for(int i=0; i<=34; i++)
                {
                    circle(500,220,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(500,220,45);
                for(int i=0; i<=44; i++)
                {
                    circle(500,220,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(500,220,55);
                for(int i=0; i<=54; i++)
                {
                    circle(500,220,i);
                }
            }
            setcolor(WHITE);
        }

        if(matrice_principala[1][0] == '-')
            outtextxy(500,360," ");
        else
        {
            aux[0] = matrice_principala[1][0];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1] = '\0';
            if(aux[0] == 'X')
            {
                circle(500,360,35);
                for(int i=0; i<=34; i++)
                {
                    circle(500,360,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(500,360,45);
                for(int i=0; i<=44; i++)
                {
                    circle(500,360,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(500,360,55);
                for(int i=0; i<=54; i++)
                {
                    circle(500,360,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(500,360,35);
                for(int i=0; i<=34; i++)
                {
                    circle(500,360,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(500,360,45);
                for(int i=0; i<=44; i++)
                {
                    circle(500,360,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(500,360,55);
                for(int i=0; i<=54; i++)
                {
                    circle(500,360,i);
                }
            }
            setcolor(WHITE);
        }

        if(matrice_principala[2][0] == '-')
            outtextxy(500,740," ");
        else
        {
            aux[0] = matrice_principala[2][0];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            if(aux[0] == 'X')
            {
                circle(500,500,35);
                for(int i=0; i<=34; i++)
                {
                    circle(500,500,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(500,500,45);
                for(int i=0; i<=44; i++)
                {
                    circle(500,500,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(500,500,55);
                for(int i=0; i<=54; i++)
                {
                    circle(500,500,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(500,500,35);
                for(int i=0; i<=34; i++)
                {
                    circle(500,500,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(500,500,45);
                for(int i=0; i<=44; i++)
                {
                    circle(500,500,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(500,500,55);
                for(int i=0; i<=54; i++)
                {
                    circle(500,500,i);
                }
            }
            setcolor(WHITE);
        }

        if(matrice_principala[0][1] == '-')
            outtextxy(640,220," ");
        else
        {
            aux[0] = matrice_principala[0][1];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1] = '\0';
            if(aux[0] == 'X')
            {
                circle(640,220,35);
                for(int i=0; i<=34; i++)
                {
                    circle(640,220,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(640,220,45);
                for(int i=0; i<=44; i++)
                {
                    circle(640,220,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(640,220,55);
                for(int i=0; i<=54; i++)
                {
                    circle(640,220,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(640,220,35);
                for(int i=0; i<=34; i++)
                {
                    circle(640,220,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(640,220,45);
                for(int i=0; i<=44; i++)
                {
                    circle(640,220,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(640,220,55);
                for(int i=0; i<=54; i++)
                {
                    circle(640,220,i);
                }
            }
            setcolor(WHITE);
        }

        if(matrice_principala[1][1] == '-')
            outtextxy(640,360," ");
        else
        {
            aux[0] = matrice_principala[1][1];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1] = '\0';
            if(aux[0] == 'X')
            {
                circle(640,360,35);
                for(int i=0; i<=34; i++)
                {
                    circle(640,360,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(640,360,45);
                for(int i=0; i<=44; i++)
                {
                    circle(640,360,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(640,360,55);
                for(int i=0; i<=54; i++)
                {
                    circle(640,360,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(640,360,35);
                for(int i=0; i<=34; i++)
                {
                    circle(640,360,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(640,360,45);
                for(int i=0; i<=44; i++)
                {
                    circle(640,360,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(640,360,55);
                for(int i=0; i<=54; i++)
                {
                    circle(640,360,i);
                }
            }
            setcolor(WHITE);
        }
        if(matrice_principala[2][1] == '-')
            outtextxy(640,500," ");
        else
        {
            aux[0] = matrice_principala[2][1];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1] = '\0';
            if(aux[0] == 'X')
            {
                circle(640,500,35);
                for(int i=0; i<=34; i++)
                {
                    circle(640,500,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(640,500,45);
                for(int i=0; i<=44; i++)
                {
                    circle(640,500,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(640,500,55);
                for(int i=0; i<=54; i++)
                {
                    circle(640,500,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(640,500,35);
                for(int i=0; i<=34; i++)
                {
                    circle(640,500,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(640,500,45);
                for(int i=0; i<=44; i++)
                {
                    circle(640,500,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(640,500,55);
                for(int i=0; i<=54; i++)
                {
                    circle(640,500,i);
                }
            }
            setcolor(WHITE);
        }

        if(matrice_principala[0][2] == '-')
            outtextxy(780,220," ");
        else
        {
            aux[0] = matrice_principala[0][2];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            if(aux[0] == 'X')
            {
                circle(780,220,35);
                for(int i=0; i<=34; i++)
                {
                    circle(780,220,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(780,220,45);
                for(int i=0; i<=44; i++)
                {
                    circle(780,220,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(780,220,55);
                for(int i=0; i<=54; i++)
                {
                    circle(780,220,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(780,220,35);
                for(int i=0; i<=34; i++)
                {
                    circle(780,220,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(780,220,45);
                for(int i=0; i<=44; i++)
                {
                    circle(780,220,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(780,220,55);
                for(int i=0; i<=54; i++)
                {
                    circle(780,220,i);
                }
            }
            setcolor(WHITE);
        }
        if(matrice_principala[1][2] == '-')
            outtextxy(780,360," ");
        else
        {
            aux[0] = matrice_principala[1][2];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1] = '\0';
            if(aux[0] == 'X')
            {
                circle(780,360,35);
                for(int i=0; i<=34; i++)
                {
                    circle(780,360,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(780,360,45);
                for(int i=0; i<=44; i++)
                {
                    circle(780,360,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(780,360,55);
                for(int i=0; i<=54; i++)
                {
                    circle(780,360,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(780,360,35);
                for(int i=0; i<=34; i++)
                {
                    circle(780,360,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(780,360,45);
                for(int i=0; i<=44; i++)
                {
                    circle(780,360,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(780,360,55);
                for(int i=0; i<=54; i++)
                {
                    circle(780,360,i);
                }
            }
            setcolor(WHITE);
        }
        if(matrice_principala[2][2] == '-')
            outtextxy(780,500," ");
        else
        {
            aux[0] = matrice_principala[2][2];
            if(strchr(vec_jucator1, aux[0]))
                setcolor(jucator1.clr);
            else setcolor(jucator2.clr);
            aux[1]='\0';
            if(aux[0] == 'X')
            {
                circle(780,500,35);
                for(int i=0; i<=34; i++)
                {
                    circle(780,500,i);
                }
            }
            if(aux[0] == 'Y')
            {
                circle(780,500,45);
                for(int i=0; i<=44; i++)
                {
                    circle(780,500,i);
                }
            }
            if (aux[0] == 'Z')
            {
                circle(780,500,55);
                for(int i=0; i<=54; i++)
                {
                    circle(780,500,i);
                }
            }
            if (aux[0] == 'A')
            {
                circle(780,500,35);
                for(int i=0; i<=34; i++)
                {
                    circle(780,500,i);
                }
            }
            if (aux[0] == 'B')
            {
                circle(780,500,45);
                for(int i=0; i<=44; i++)
                {
                    circle(780,500,i);
                }
            }
            if (aux[0] == 'C')
            {
                circle(780,500,55);
                for(int i=0; i<=54; i++)
                {
                    circle(780,500,i);
                }
            }
            setcolor(WHITE);
        }
        setbkcolor(BLACK);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        setcolor(jucator1.clr);
        if(turn % 2 == 0)
            if(lmb)
                outtextxy(640, 575, jucator1.nume[jucator1.nrNume]);
            else
                outtextxy(640, 575, jucator1.nume[jucator1.nrNume]);
        if(jucator1.nr_mare+jucator1.nr_mediu+jucator1.nr_mic)
        {
            if(jucator1.nr_mic == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator1.clr);
                for(int i=0; i<=35; i++)
                    circle(215, 220, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(215, 220, "x2");
            }

            else if(jucator1.nr_mic == 1)
            {
                setcolor(jucator1.clr);
                for(int i=0; i<=35; i++)
                    circle(215, 220, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(215, 220, "x1");
            }
            if(jucator1.nr_mediu == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator1.clr);
                for(int i=0; i<=45; i++)
                    circle(215, 360, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(215, 360, "x2");
            }
            else if(jucator1.nr_mediu == 1)
            {
                setcolor(jucator1.clr);
                for(int i=0; i<=45; i++)
                    circle(215, 360, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(215, 360, "x1");
            }
            if(jucator1.nr_mare == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator1.clr);
                for(int i=0; i<=55; i++)
                    circle(215, 500, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(215, 500, "x2");
            }
            else if(jucator1.nr_mare == 1)
            {
                setcolor(jucator1.clr);
                for(int i=0; i<=55; i++)
                    circle(215, 500, i);
                setcolor(BLACK);
                setbkcolor(jucator1.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(215, 500, "x1");
            }
        }
        setcolor(jucator2.clr);
        setbkcolor(BLACK);
        if(turn % 2 != 0)
            if(lmb)
                outtextxy(640, 580, jucator2.nume[jucator2.nrNume]);
            else
                outtextxy(640, 580, jucator2.nume[jucator2.nrNume]);

        if(jucator2.nr_mare+jucator2.nr_mediu+jucator2.nr_mic)
        {
            if(jucator2.nr_mic == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator2.clr);
                for(int i=0; i<=35; i++)
                    circle(1065, 220, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1065, 220, "x2");
            }

            else if(jucator2.nr_mic == 1)
            {
                setcolor(jucator2.clr);
                for(int i=0; i<=35; i++)
                    circle(1065, 220, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1065, 220, "x1");
            }
            if(jucator2.nr_mediu == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator2.clr);
                for(int i=0; i<=45; i++)
                    circle(1065, 360, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1065, 360, "x2");
            }
            else if(jucator2.nr_mediu == 1)
            {
                setcolor(jucator2.clr);
                for(int i=0; i<=45; i++)
                    circle(1065, 360, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1065, 360, "x1");
            }
            if(jucator2.nr_mare == 2)
            {
                if(odata == 0)
                    delay(250);
                setcolor(jucator2.clr);
                for(int i=0; i<=55; i++)
                    circle(1065, 500, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1065, 500, "x2");
            }
            else if(jucator2.nr_mare == 1)
            {
                setcolor(jucator2.clr);
                for(int i=0; i<=55; i++)
                    circle(1065, 500, i);
                setcolor(BLACK);
                setbkcolor(jucator2.clr);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
                outtextxy(1065, 500, "x1");
            }
            setbkcolor(BLACK);
        }
        odata = 1;
    }
}
void CitesteSir(piese &jucator) ///functie care ne lasa sa scriem un nickname pe care il folosim in timpul jocului
{
    cleardevice();
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, width, height);
    setbkcolor(BLACK);
    char mesaj[256]="",S[256]="";
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    int x=width/3 + width/7,y=height/2;
    if(lmb)
    {
        setcolor(WHITE);
        outtextxy(width/2, height/3, "Alegeti-va un nume: ");
        outtextxy(width/3, height/2, "Nume :");
    }
    else
    {
        setcolor(WHITE);
        outtextxy(width/2, height/3, "Choose a nickname: ");
        outtextxy(width/3, height/2, "Name :");
    }
    char Enter = 13;
    char BackSpace = 8;
    char Escape = 27;
    char s2[2]="";
    s2[1]='\0';
    char MultimeDeCaractereAcceptabile[]="0123456789abcdefghijklmnopqrstuvwxyzQWERTYUIOPLKJHGFDSAZXCVBNM~!`@#$%+-^&*/\\()_=[],;.?<>:;{} ";
    char S_initial[256]="";
    char tasta;
    char S_[256]="";
    char mesaj_[256]="";
    strcpy(mesaj_,mesaj);
    strcpy(S_initial,S);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,4);
    //settextstyle(10,0,4);
    setcolor(jucator.clr);
    settextjustify(0,0);
    outtextxy(x,y,mesaj_);
    x=x+textwidth(mesaj);
    strcpy(S,"");
    strcpy(S_,S);
    strcat(S_,"_");
    setcolor(BLACK);
    outtextxy(x,y,S_);
    s2[0]=tasta;
    strcat(S,s2);
    strcpy(S_,S);
    strcat(S_,"_");
    setcolor(jucator.clr);
    outtextxy(x,y,S_);

    do
    {
        tasta=getch();
        if (tasta==0)
        {
            tasta=getch();
            Beep(1000,500);
        }
        else if (strchr(MultimeDeCaractereAcceptabile, tasta))
        {
            // stergem fostul sir
            strcpy(S_,S);
            strcat(S_,"_");
            setcolor(BLACK);
            outtextxy(x,y,S_);
            s2[0]=tasta;
            strcat(S,s2);
            strcpy(S_,S);
            strcat(S_,"_");
            setcolor(jucator.clr);
            outtextxy(x,y,S_);
        }
        if (tasta==BackSpace)
            if (!(strcmp(S,""))) Beep(500,100);
            else
            {
                setcolor(BLACK);
                outtextxy(x,y,S_);
                setcolor(jucator.clr);
                S[strlen(S)-1]='\0';
                strcpy(S_,S);
                strcat(S_,"_") ;
                outtextxy(x,y,S_);
                Beep(200,20);
            }
        else if (tasta!=Enter && tasta!=Escape)
        {
            Beep(200,20);
        }
    }
    while(tasta!=Enter && tasta!=Escape);
    if (tasta == Escape)
        strcpy(S,S_initial);
    Beep(300, 50);
    setcolor(BLACK);
    outtextxy(x,y,S_);
    setcolor(jucator.clr);
    outtextxy(x,y,S);
    cout<<S<<endl;
    strcpy(jucator.nume[jucator.nrNume],S);
}

void alegere_culoare() ///functia care face sa apare patratele colorate pentru a alege culoarea
{
    cleardevice();

    char afisare[101];

    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, width, height);
    setbkcolor(BLACK);
    punct poz, pozCursor;
    buton C[16];
    int color = 1;
    if(width == 1920)
    {
        setcolor(BLACK);
        rectangle(460, 240, 1460, 840);
        line(660,240,1260,240);
        line(1260, 240, 1260, 840);
        line(660, 840, 1260, 840);
        line(660, 240, 660, 840);
        line(660,240,1260,240);
        line(1260, 240, 1260, 840);
        line(660, 840, 1260, 840);
        line(660, 240, 660, 840);
        line(460,440,1460,440);
        line(860,240,860,840);
        line(460,640,1460,640);
        line(1060,240,1060,840);

        poz.x = 460;
        poz.y = 240;

        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
        setcolor(WHITE);
        if(jucator1.clr == -1)
        {
            strcpy(afisare, jucator1.nume[jucator1.nrNume]);
            if(lmb)
                strcat(afisare, " ALEGETI CULOAREA");

            else
                strcat(afisare, " CHOOSE YOUR COLOUR");
            afisare[strlen(afisare)]='\0';
            outtextxy(960, 120, afisare);
        }
        else
        {
            strcpy(afisare, jucator2.nume[jucator2.nrNume]);
            if(lmb)
                strcat(afisare, " ALEGETI CULOAREA");

            else
                strcat(afisare, " CHOOSE YOUR COLOUR");
            afisare[strlen(afisare)]='\0';
            outtextxy(960, 120, afisare);
        }
        for(int i=1; i<=3; ++i)
        {
            poz.x = 460;
            for(int j=1; j<=5; ++j)
            {
                if(jucator1.clr != color)
                {
                    setfillstyle(SOLID_FILL, color);
                    bar(poz.x+5, poz.y+5, poz.x+195, poz.y+195);
                    C[color].D.SS.x = poz.x+5;
                    C[color].D.SS.y = poz.y+5;
                    C[color].D.DJ.x = poz.x+195;
                    C[color].D.DJ.y = poz.y+195;
                }
                else
                {
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(poz.x+5, poz.y+5, poz.x+195, poz.y+195);
                }
                poz.x+=200;
                color++;
            }
            poz.y+=200;
        }
    }
    else if(width == 1280)
    {
        setcolor(BLACK);
        rectangle(280, 150, 1000, 570);
        line(430,150,850,150);
        line(850, 150, 850, 570);
        line(430, 570, 850, 570);
        line(430, 150, 430, 570);
        line(430,150,850,150);
        line(850, 150, 850, 570);
        line(430, 570, 850, 570);
        line(430, 150, 430, 570);
        line(280,290,1000,290);
        line(570,150,570,570);
        line(280,430,1000,430);
        line(710,150,710,570);

        poz.x = 280;
        poz.y = 150;


        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
        setcolor(WHITE);
        if(lmb)
        {
            if(jucator1.clr == -1)
                outtextxy(640, 80, "JUCATOR 1 ALEGETI CULOAREA");
            else
                outtextxy(640, 80, "JUCATOR 2 ALEGETI CULOAREA");
        }
        else
        {
            if(jucator1.clr== -1)
                outtextxy(640, 80, "PLAYER 1 CHOOSE COLOUR");
            else
                outtextxy(640, 80, "PLAYER 2 CHOOSE COLOUR");
        }
        for(int i=1; i<=3; ++i)
        {
            poz.x = 280;
            for(int j=1; j<=5; ++j)
            {
                if(jucator1.clr != color)
                {
                    setfillstyle(SOLID_FILL, color);
                    bar(poz.x+5, poz.y+5, poz.x+135, poz.y+135);
                    C[color].D.SS.x = poz.x+5;
                    C[color].D.SS.y = poz.y+5;
                    C[color].D.DJ.x = poz.x+135;
                    C[color].D.DJ.y = poz.y+135;
                }
                else
                {
                    setfillstyle(SOLID_FILL, BLACK);
                    bar(poz.x+5, poz.y+5, poz.x+135, poz.y+135);
                }
                poz.x+=140;
                color++;
            }
            poz.y+=140;
        }
    }
    while(jucator2.clr == -1)
    {
        click(pozCursor.x,pozCursor.y);
        for(int i=1; i<=15; i++)
        {
            if(apartine(pozCursor,C[i].D) && jucator1.clr == -1)
            {
                jucator1.clr = i;
                alegere_culoare();
            }
            else if(apartine(pozCursor,C[i].D))
            {
                jucator2.clr = i;
                alegere_culoare();
            }
        }
    }
    cleardevice();
}

void FHD(int &width, int &height)
{
    if(width != 1920)
    {
        closegraph();
        width = 1920;
        height = 1080;
        caz = 12;
        OpenGraph();
        rezolutie(width, height);
    }


}

void HD(int &width, int &height)
{
    if(width != 1280)
    {
        closegraph();
        width = 1280;
        height = 720;
        caz = 12;
        OpenGraph();
        rezolutie(width, height);
    }

}

void alegere_dificultate()
{
    cleardevice();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if(iarna == 0)
    {
        setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
        setbkcolor(COLOR(255, 83, 10));
        setcolor(COLOR(0, 210, 255));
        bar(0, 0, width, height);
    }
    else
    {
        if(width == 1920) readimagefile("iarna_1920.gif", 0, 0, width, height);
        else readimagefile("iarna_1280.gif", 0, 0, width, height);
        setbkcolor(COLOR(191,221,229));
        setcolor(COLOR(101,149,153));
    }
    if(width == 1920)
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    else settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    if(lmb)
    {
        outtextxy(width/2, height/4, "Alegeti dificultatea: ");
        outtextxy(width/2 - width/4, height/2, "USOR");
        outtextxy(width/2 + width/4, height/2, "GREU");

        B[18].D.SS.x = width/2 - width/4 - textwidth("USOR")/2;
        B[18].D.DJ.x = width/2 - width/4 + textwidth("USOR")/2;
        B[18].D.SS.y = height/2 - textheight("USOR")/2;
        B[18].D.DJ.y = height/2 + textheight("USOR")/2;

        B[19].D.SS.x = width/2 + width/4 - textwidth("GREU")/2;
        B[19].D.DJ.x = width/2 + width/4 + textwidth("GREU")/2;
        B[19].D.SS.y = height/2 - textheight("GREU")/2;
        B[19].D.DJ.y = height/2 + textheight("GREU")/2;
    }

    else
    {
        outtextxy(width/2, height/4, "CHOOSE DIFFICULTY: ");
        outtextxy(width/2 - width/4, height/2, "EASY");
        outtextxy(width/2 + width/4, height/2, "HARD");

        B[18].D.SS.x = width/2 - width/4 - textwidth("EASY")/2;
        B[18].D.DJ.x = width/2 - width/4 + textwidth("EASY")/2;
        B[18].D.SS.y = height/2 - textheight("EASY")/2;
        B[18].D.DJ.y = height/2 + textheight("EASY")/2;

        B[19].D.SS.x = width/2 + width/4 - textwidth("HARD")/2;
        B[19].D.DJ.x = width/2 + width/4 + textwidth("HARD")/2;
        B[19].D.SS.y = height/2 - textheight("HARD")/2;
        B[19].D.DJ.y = height/2 + textheight("HARD")/2;
    }

    iesire1();
    inapoi1();
}


void verificare_buton()    ///cea mai importanta functie, cea care verifica ce buton s-a apasat si ruleaza comanda aferenta butonului
{
    punct pozCursor;
    int caz = 0;
    while(1)
    {
        click(pozCursor.x, pozCursor.y);
        for(int i=0; i<=35; i++)
        {
            if(apartine(pozCursor, B[i].D))
            {
                switch(i)
                {
                case 0:
                {
                    caz = 31;
                    clasament();

                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;


                    break;
                }
                case 1:                      /// meniu, buton 1 - pvp, buton 2 - pvc, buton 3 - regulament, buton 4 - setari
                {
                    cleardevice();
                    jocRobot = 0;
                    caz = 1;                  ///caz e folosit pentru a vedea de pe ce pagina dam back si pentru a sti unde sa ne intoarcem
                    // alegere_culoare();
                    CitesteSir(jucator1);
                    CitesteSir(jucator2);
                    joc(jucator1,jucator2,ok);

                    B[1].D.SS.x = 0;          ///butoanele se fac 0 pentru a nu se apasa din alte pagini
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;
                    break;
                }
                case 2:    /// intra in player vs computer
                {
                    caz = 2;
                    jocRobot = 1;
                    CitesteSir(jucator1);
                    strcpy(jucator2.nume[jucator2.nrNume], "ROBOT");
                    alegere_dificultate();
                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;
                    break;
                }
                case 3:   ///intra in regulament
                {
                    caz = 3;
                    paginaRegulament();
                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;
                    break;
                }
                case 4:   ///intra in setari
                {
                    caz = 4;
                    setari();
                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;

                    pozCursor.x = -1;
                    pozCursor.y = -1;

                    break;
                }
                case 6:   ///buton de back
                {

                    switch(caz)
                    {
                    case 1:
                    {
                        caz = 0;
                        meniu(iarna);

                        B[18].D.SS.x = 0;
                        B[18].D.DJ.x = 0;
                        B[18].D.SS.y = 0;
                        B[18].D.DJ.y = 0;

                        B[19].D.SS.x = 0;
                        B[19].D.DJ.x = 0;
                        B[19].D.SS.y = 0;
                        B[19].D.DJ.y = 0;

                        break;
                    }
                    case 2:
                    {
                        caz = 0;
                        meniu(iarna);
                        B[18].D.SS.x = 0;
                        B[18].D.DJ.x = 0;
                        B[18].D.SS.y = 0;
                        B[18].D.DJ.y = 0;

                        B[19].D.SS.x = 0;
                        B[19].D.DJ.x = 0;
                        B[19].D.SS.y = 0;
                        B[19].D.DJ.y = 0;

                        break;
                    }
                    case 3:
                    {
                        caz = 0;
                        meniu(iarna);
                        break;
                    }
                    case 4:
                    {
                        caz = 0;
                        meniu(iarna);


                        B[13].D.SS.x = 0;
                        B[13].D.DJ.x = 0;
                        B[13].D.SS.y = 0;
                        B[13].D.DJ.y = 0;

                        B[12].D.SS.x = 0;
                        B[12].D.DJ.x = 0;
                        B[12].D.SS.y = 0;
                        B[12].D.DJ.y = 0;

                        B[16].D.SS.x = 0;
                        B[16].D.DJ.x = 0;
                        B[16].D.SS.y = 0;
                        B[16].D.DJ.y = 0;

                        B[17].D.SS.x = 0;
                        B[17].D.DJ.x = 0;
                        B[17].D.SS.y = 0;
                        B[17].D.DJ.y = 0;

                        B[30].D.SS.x = 0;
                        B[30].D.DJ.x = 0;
                        B[30].D.SS.y = 0;
                        B[30].D.DJ.y = 0;

                        break;
                    }
                    case 8:
                    {
                        caz = 4;
                        setari();
                        break;
                    }
                    case 12:
                    {
                        caz = 4;
                        setari();
                        B[8].D.SS.x = 0;
                        B[8].D.DJ.x = 0;
                        B[8].D.SS.y = 0;
                        B[8].D.DJ.y = 0;

                        B[9].D.SS.x = 0;
                        B[9].D.DJ.x = 0;
                        B[9].D.SS.y = 0;
                        B[9].D.DJ.y = 0;

                        B[16].D.SS.x = 0;
                        B[16].D.DJ.x = 0;
                        B[16].D.SS.y = 0;
                        B[16].D.DJ.y = 0;


                        break;
                    }
                    case 14:
                    {
                        caz = 14;
                        setari();
                        break;
                    }
                    case 16:
                    {
                        caz = 4;
                        setari();
                        break;
                    }
                    case 17:
                    {
                        caz = 4;
                        setari();

                        B[14].D.SS.x = 0;
                        B[14].D.DJ.x = 0;
                        B[14].D.SS.y = 0;
                        B[14].D.DJ.y = 0;

                        B[16].D.SS.x = 0;
                        B[16].D.DJ.x = 0;
                        B[16].D.SS.y = 0;
                        B[16].D.DJ.y = 0;



                        break;
                    }
                    case 30:
                    {
                        caz = 4;
                        setari();

                        B[21].D.SS.x = 0;
                        B[21].D.DJ.x = 0;
                        B[21].D.SS.y = 0;
                        B[21].D.DJ.y = 0;

                        B[22].D.SS.x = 0;
                        B[22].D.DJ.x = 0;
                        B[22].D.SS.y = 0;
                        B[22].D.DJ.y = 0;

                        B[23].D.SS.x = 0;
                        B[23].D.DJ.x = 0;
                        B[23].D.SS.y = 0;
                        B[23].D.DJ.y = 0;


                        B[24].D.SS.x = 0;
                        B[24].D.DJ.x = 0;
                        B[24].D.SS.y = 0;
                        B[24].D.DJ.y = 0;


                        B[25].D.SS.x = 0;
                        B[25].D.DJ.x = 0;
                        B[25].D.SS.y = 0;
                        B[25].D.DJ.y = 0;


                        B[26].D.SS.x = 0;
                        B[26].D.DJ.x = 0;
                        B[26].D.SS.y = 0;
                        B[26].D.DJ.y = 0;


                        B[27].D.SS.x = 0;
                        B[27].D.DJ.x = 0;
                        B[27].D.SS.y = 0;
                        B[27].D.DJ.y = 0;


                        B[28].D.SS.x = 0;
                        B[28].D.DJ.x = 0;
                        B[28].D.SS.y = 0;
                        B[28].D.DJ.y = 0;

                        B[29].D.SS.x = 0;
                        B[29].D.DJ.x = 0;
                        B[29].D.SS.y = 0;
                        B[29].D.DJ.y = 0;



                        break;
                    }
                    case 31:
                    {
                        caz = 0;
                        meniu(iarna);

                        break;
                    }
                    }
                    break;
                }
                case 8: ///rezolutie mare 1920x1080
                {
                    caz = 12;
                    FHD(width, height);
                    break;
                }
                case 9: ///rezolutie medie 1280x720
                {
                    caz = 12;
                    HD(width, height);
                    break;
                }
                case 12:  ///pagina de rezolutii
                {
                    caz = 12;
                    rezolutie(width, height);
                    B[17].D.SS.x = 0;
                    B[17].D.DJ.x = 0;
                    B[17].D.SS.y = 0;
                    B[17].D.DJ.y = 0;

                    B[13].D.SS.x = 0;
                    B[13].D.DJ.x = 0;
                    B[13].D.SS.y = 0;
                    B[13].D.DJ.y = 0;

                    B[30].D.SS.x = 0;
                    B[30].D.DJ.x = 0;
                    B[30].D.SS.y = 0;
                    B[30].D.DJ.y = 0;

                    B[12].D.SS.x = 0;
                    B[12].D.DJ.x = 0;
                    B[12].D.SS.y = 0;
                    B[12].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;

                    pozCursor.x = -1;
                    pozCursor.y = -1;

                    break;
                }
                case 14: ///schimbare text in limba romana
                {
                    caz = 17;

                    rom(lmb);

                    B[18].D.SS.x = 0;
                    B[18].D.DJ.x = 0;
                    B[18].D.SS.y = 0;
                    B[18].D.DJ.y = 0;

                    B[19].D.SS.x = 0;
                    B[19].D.DJ.x = 0;
                    B[19].D.SS.y = 0;
                    B[19].D.DJ.y = 0;

                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;

                    B[8].D.SS.x = 0;
                    B[8].D.DJ.x = 0;
                    B[8].D.SS.y = 0;
                    B[8].D.DJ.y = 0;

                    B[9].D.SS.x = 0;
                    B[9].D.DJ.x = 0;
                    B[9].D.SS.y = 0;
                    B[9].D.DJ.y = 0;

                    B[13].D.SS.x = 0;
                    B[13].D.DJ.x = 0;
                    B[13].D.SS.y = 0;
                    B[13].D.DJ.y = 0;

                    B[12].D.SS.x = 0;
                    B[12].D.DJ.x = 0;
                    B[12].D.SS.y = 0;
                    B[12].D.DJ.y = 0;

                    B[30].D.SS.x = 0;
                    B[30].D.DJ.x = 0;
                    B[30].D.SS.y = 0;
                    B[30].D.DJ.y = 0;

                    B[17].D.SS.x = 0;
                    B[17].D.DJ.x = 0;
                    B[17].D.SS.y = 0;
                    B[17].D.DJ.y = 0;


                    pozCursor.x = -1;
                    pozCursor.y = -1;

                    break;
                }
                case 15: ///buton de exit
                {

                    system("CLS");
                    exit(0);
                    closegraph();
                    break;
                }
                case 16: /// schimbare text in limba engleza
                {
                    caz = 17;

                    eng(lmb);

                    B[18].D.SS.x = 0;
                    B[18].D.DJ.x = 0;
                    B[18].D.SS.y = 0;
                    B[18].D.DJ.y = 0;

                    B[19].D.SS.x = 0;
                    B[19].D.DJ.x = 0;
                    B[19].D.SS.y = 0;
                    B[19].D.DJ.y = 0;

                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;

                    B[8].D.SS.x = 0;
                    B[8].D.DJ.x = 0;
                    B[8].D.SS.y = 0;
                    B[8].D.DJ.y = 0;

                    B[9].D.SS.x = 0;
                    B[9].D.DJ.x = 0;
                    B[9].D.SS.y = 0;
                    B[9].D.DJ.y = 0;

                    B[13].D.SS.x = 0;
                    B[13].D.DJ.x = 0;
                    B[13].D.SS.y = 0;
                    B[13].D.DJ.y = 0;

                    B[12].D.SS.x = 0;
                    B[12].D.DJ.x = 0;
                    B[12].D.SS.y = 0;
                    B[12].D.DJ.y = 0;


                    break;
                }
                case 17: ///deschide pagina pt limbi
                {
                    caz = 17;

                    limba();

                    B[18].D.SS.x = 0;
                    B[18].D.DJ.x = 0;
                    B[18].D.SS.y = 0;
                    B[18].D.DJ.y = 0;

                    B[19].D.SS.x = 0;
                    B[19].D.DJ.x = 0;
                    B[19].D.SS.y = 0;
                    B[19].D.DJ.y = 0;

                    B[1].D.SS.x = 0;
                    B[1].D.DJ.x = 0;
                    B[1].D.SS.y = 0;
                    B[1].D.DJ.y = 0;

                    B[2].D.SS.x = 0;
                    B[2].D.DJ.x = 0;
                    B[2].D.SS.y = 0;
                    B[2].D.DJ.y = 0;

                    B[3].D.SS.x = 0;
                    B[3].D.DJ.x = 0;
                    B[3].D.SS.y = 0;
                    B[3].D.DJ.y = 0;

                    B[4].D.SS.x = 0;
                    B[4].D.DJ.x = 0;
                    B[4].D.SS.y = 0;
                    B[4].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;

                    B[8].D.SS.x = 0;
                    B[8].D.DJ.x = 0;
                    B[8].D.SS.y = 0;
                    B[8].D.DJ.y = 0;

                    B[9].D.SS.x = 0;
                    B[9].D.DJ.x = 0;
                    B[9].D.SS.y = 0;
                    B[9].D.DJ.y = 0;

                    B[13].D.SS.x = 0;
                    B[13].D.DJ.x = 0;
                    B[13].D.SS.y = 0;
                    B[13].D.DJ.y = 0;

                    B[12].D.SS.x = 0;
                    B[12].D.DJ.x = 0;
                    B[12].D.SS.y = 0;
                    B[12].D.DJ.y = 0;

                    break;
                }
                case 18: ///deschide joc impotriva ai usor
                {
                    caz = 2;
                    //  alegere_culoare();
                    joc_ai_usor(jucator1, jucator2, ok);
                    break;
                }
                case 19: ///deschide joc impotriva ai greu
                {
                    caz = 2;

                    // alegere_culoare();
                    joc_ai_greu(jucator1, jucator2, ok);
                    break;
                }
                case 21: ///melodii
                {
                    caz = 30;
                    tzanca(volum,melodie);
                    break;
                }
                case 22:
                {
                    caz = 30;
                    opt_bit(volum,melodie);
                    break;
                }
                case 23:
                {
                    caz = 30;
                    creepin(volum,melodie);
                    break;
                }
                case 24:
                {
                    caz = 30;
                    rock(volum,melodie);
                    break;
                }
                case 25:
                {
                    caz = 30;
                    animatie_iarna(iarna);

                    B[21].D.SS.x = 0;
                    B[21].D.DJ.x = 0;
                    B[21].D.SS.y = 0;
                    B[21].D.DJ.y = 0;

                    B[22].D.SS.x = 0;
                    B[22].D.DJ.x = 0;
                    B[22].D.SS.y = 0;
                    B[22].D.DJ.y = 0;

                    B[23].D.SS.x = 0;
                    B[23].D.DJ.x = 0;
                    B[23].D.SS.y = 0;
                    B[23].D.DJ.y = 0;


                    B[24].D.SS.x = 0;
                    B[24].D.DJ.x = 0;
                    B[24].D.SS.y = 0;
                    B[24].D.DJ.y = 0;


                    B[25].D.SS.x = 0;
                    B[25].D.DJ.x = 0;
                    B[25].D.SS.y = 0;
                    B[25].D.DJ.y = 0;


                    B[26].D.SS.x = 0;
                    B[26].D.DJ.x = 0;
                    B[26].D.SS.y = 0;
                    B[26].D.DJ.y = 0;


                    B[27].D.SS.x = 0;
                    B[27].D.DJ.x = 0;
                    B[27].D.SS.y = 0;
                    B[27].D.DJ.y = 0;


                    B[28].D.SS.x = 0;
                    B[28].D.DJ.x = 0;
                    B[28].D.SS.y = 0;
                    B[28].D.DJ.y = 0;

                    B[29].D.SS.x = 0;
                    B[29].D.DJ.x = 0;
                    B[29].D.SS.y = 0;
                    B[29].D.DJ.y = 0;

                    B[13].D.SS.x = 0;
                    B[13].D.DJ.x = 0;
                    B[13].D.SS.y = 0;
                    B[13].D.DJ.y = 0;

                    B[12].D.SS.x = 0;
                    B[12].D.DJ.x = 0;
                    B[12].D.SS.y = 0;
                    B[12].D.DJ.y = 0;

                    B[16].D.SS.x = 0;
                    B[16].D.DJ.x = 0;
                    B[16].D.SS.y = 0;
                    B[16].D.DJ.y = 0;

                    B[17].D.SS.x = 0;
                    B[17].D.DJ.x = 0;
                    B[17].D.SS.y = 0;
                    B[17].D.DJ.y = 0;

                    B[30].D.SS.x = 0;
                    B[30].D.DJ.x = 0;
                    B[30].D.SS.y = 0;
                    B[30].D.DJ.y = 0;

                    B[0].D.SS.x = 0;
                    B[0].D.DJ.x = 0;
                    B[0].D.SS.y = 0;
                    B[0].D.DJ.y = 0;

                    break;
                }
                case 26:   ///volum
                {
                    volum_zero(volum);
                    switch(melodie)
                    {
                    case 1:
                    {
                        tzanca(volum,melodie);
                        break;
                    }
                    case 3:
                    {
                        creepin(volum,melodie);
                        break;
                    }
                    case 8:
                    {
                        opt_bit(volum,melodie);
                        break;
                    }
                    case 25:
                    {
                        craciun(volum,melodie);
                        break;
                    }
                    case 69:
                    {
                        rock(volum,melodie);
                        break;
                    }
                    }
                    break;
                }
                case 27:
                {
                    volum_jumate(volum);
                    switch(melodie)
                    {
                    case 1:
                    {
                        tzanca(volum,melodie);
                        break;
                    }
                    case 3:
                    {
                        creepin(volum,melodie);
                        break;
                    }
                    case 8:
                    {
                        opt_bit(volum,melodie);
                        break;
                    }
                    case 25:
                    {
                        craciun(volum,melodie);
                        break;
                    }
                    case 69:
                    {
                        rock(volum,melodie);
                        break;
                    }
                    }
                    break;
                }
                case 28:
                {
                    volum_full(volum);
                    switch(melodie)
                    {
                    case 1:
                    {
                        tzanca(volum,melodie);
                        break;
                    }
                    case 3:
                    {
                        creepin(volum,melodie);
                        break;
                    }
                    case 8:
                    {
                        opt_bit(volum,melodie);
                        break;
                    }
                    case 25:
                    {
                        craciun(volum,melodie);
                        break;
                    }
                    case 69:
                    {
                        rock(volum,melodie);
                        break;
                    }
                    }
                    break;
                }
                case 30: ///pagina de muzica
                {
                    caz = 30;
                    sunet();

                    B[17].D.SS.x = 0;
                    B[17].D.DJ.x = 0;
                    B[17].D.SS.y = 0;
                    B[17].D.DJ.y = 0;

                    B[30].D.SS.x = 0;
                    B[30].D.DJ.x = 0;
                    B[30].D.SS.y = 0;
                    B[30].D.DJ.y = 0;

                    B[12].D.SS.x = 0;
                    B[12].D.DJ.x = 0;
                    B[12].D.SS.y = 0;
                    B[12].D.DJ.y = 0;


                    break;



                }
                }
            }
        }
    }
}
int click(int &xclick, int &yclick)  ///functie care retine coord clickului si scoate un sunet cand apesi click
{
    int x, y;
    while(!ismouseclick(WM_LBUTTONDOWN))
        delay(50);

    getmouseclick(WM_LBUTTONDOWN, x, y);
    xclick = x;
    yclick = y;
    Beep(300,30);

}
void DA_Robot()
{
    char varianta_raspuns[10] = "DA";
    bool ok = false;
    refacere_table(jucator1,jucator2,ok,turn,odata);
    if(robot == 1)
        joc_ai_usor(jucator1, jucator2, ok);
    else if(robot == 2)
        joc_ai_greu(jucator1, jucator2, ok);
}
void DA()
{
    char varianta_raspuns[10] = "DA";
    bool ok = false;
    refacere_table(jucator1,jucator2,ok,turn,odata);
    joc(jucator1,jucator2,ok);
}

void NU()
{
    char varianta_raspuns[10] = "NU";
    main_adevarat();
}

void iesire(punct pozC)
{
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(jucator2.clr);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    if(pozC.x>1750&& pozC.x<1920 &&pozC.y>0 &&pozC.y<30)
    {
        system("CLS");
        exit(0);
        closegraph();
    }
    outtextxy(1900,30,"X");
}

void inapoi(punct pozC)
{
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(jucator1.clr);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    if(pozC.x>0&& pozC.x<30 &&pozC.y>0 &&pozC.y<20)
        main_adevarat();
    outtextxy(30,20,"<-");
}

bool apartine(punct p, dreptunghi D)
{
    if(D.SS.x <= p.x && p.x <= D.DJ.x && D.SS.y <= p.y && p.y <= D.DJ.y)
        return true;
    return false;
}

void iesire1()
{
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(RED);
    if(width == 1920)
    {
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
        B[15].D.SS.x = width - width/42 - textwidth("X")/2;
        B[15].D.DJ.x = width - width/42 + textwidth("X")/2;
        B[15].D.SS.y = height/36 - textheight("X")/2;
        B[15].D.DJ.y = height/36 + textheight("X")/2 - 14;
        strcpy(B[15].text, "X");
        bar(B[15].D.SS.x, B[15].D.SS.y, B[15].D.DJ.x, B[15].D.DJ.y);
        outtextxy(width - width/42,height/36, B[15].text);
    }
    else if(width == 1280)
    {
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        B[15].D.SS.x = width - width/42 - textwidth("X")/2;
        B[15].D.DJ.x = width - width/42 + textwidth("X")/2;
        B[15].D.SS.y = height/36 - textheight("X")/2;
        B[15].D.DJ.y = height/36 + textheight("X")/2 - 7;
        strcpy(B[15].text, "X");
        bar(B[15].D.SS.x, B[15].D.SS.y, B[15].D.DJ.x, B[15].D.DJ.y);
        outtextxy(width - width/42,height/36, B[15].text);
    }
}


void inapoi1()
{
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if(width == 1920)
    {
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
        B[6].D.SS.x = width/42 - textwidth("<~")/2;
        B[6].D.DJ.x = width/42 + textwidth("<~")/2;
        B[6].D.SS.y = height/36 - textheight("<~")/2;
        B[6].D.DJ.y = height/36 + textheight("<~")/2 - 7;
        strcpy(B[6].text, "<-");
        outtextxy(width/42, height/36, B[6].text);
    }
    else if(width == 1280)
    {
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        B[6].D.SS.x = width/42 - textwidth("<~")/2;
        B[6].D.DJ.x = width/42 + textwidth("<~")/2;
        B[6].D.SS.y = height/36 - textheight("<~")/2;
        B[6].D.DJ.y = height/36 + textheight("<~")/2 - 7;
        strcpy(B[6].text, "<-");
        outtextxy(width/42, height/36, B[6].text);
    }
}
void jucati_din_nou(int jocRobot, int tipJoc) ///functia care deschide fereastra pentru a se decide daca jucatorii incep o noua runda sau se intorc in meniu
{

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    delay(150);
    if(width == 1920)
    {
        if(lmb)
            outtextxy(960, 540, "VRETI SA JUCATI DIN NOU?");
        else
            outtextxy(960, 540, "PLAY AGAIN?");

        if(lmb)
        {
            B[5].D.SS.x = 860 - textwidth("DA")/2;
            B[5].D.DJ.x = 860 + textwidth("DA")/2;
            B[5].D.SS.y = 700 - textheight("DA")/2;
            B[5].D.DJ.y = 700 + textheight ("DA")/2;
            delay(100);
            outtextxy(860, 700, "DA");
        }
        else
        {
            B[5].D.SS.x = 860 - textwidth("YES")/2;
            B[5].D.DJ.x = 860 + textwidth("YES")/2;
            B[5].D.SS.y = 700 - textheight("YES")/2;
            B[5].D.DJ.y = 700 + textheight ("YES")/2;
            delay(100);
            outtextxy(860, 700, "YES");
        }
        if(lmb)
        {
            B[7].D.SS.x = 1060 - textwidth("NU")/2;
            B[7].D.DJ.x = 1060 + textwidth("NU")/2;
            B[7].D.SS.y = 700 - textheight("NU")/2;
            B[7].D.DJ.y = 700 + textheight ("NU")/2;
            delay(100);
            outtextxy(1060, 700, "NU");
        }
        else
        {
            B[7].D.SS.x = 1060 - textwidth("NO")/2;
            B[7].D.DJ.x = 1060 + textwidth("NO")/2;
            B[7].D.SS.y = 700 - textheight("NO")/2;
            B[7].D.DJ.y = 700 + textheight ("NO")/2;
            delay(100);
            outtextxy(1060, 700, "NO");
        }
    }
    else
    {
        if(lmb)
            outtextxy(640, 360, "VRETI SA JUCATI DIN NOU?");
        else
            outtextxy(640, 360, "PLAY AGAIN?");

        if(lmb)
        {
            B[5].D.SS.x = 440 - textwidth("DA")/2;
            B[5].D.DJ.x = 440 + textwidth("DA")/2;
            B[5].D.SS.y = 520 - textheight("DA")/2;
            B[5].D.DJ.y = 520 + textheight ("DA")/2;
            delay(100);
            outtextxy(440, 520, "DA");
        }
        else
        {
            B[5].D.SS.x = 440 - textwidth("YES")/2;
            B[5].D.DJ.x = 440 + textwidth("YES")/2;
            B[5].D.SS.y = 520 - textheight("YES")/2;
            B[5].D.DJ.y = 520 + textheight ("YES")/2;
            delay(100);
            outtextxy(440, 520, "YES");
        }
        if(lmb)
        {
            B[7].D.SS.x = 840 - textwidth("NU")/2;
            B[7].D.DJ.x = 840 + textwidth("NU")/2;
            B[7].D.SS.y = 520 - textheight("NU")/2;
            B[7].D.DJ.y = 520 + textheight ("NU")/2;
            delay(80);
            outtextxy(840, 520, "NU");
        }
        else
        {
            B[7].D.SS.x = 840 - textwidth("NO")/2;
            B[7].D.DJ.x = 840 + textwidth("NO")/2;
            B[7].D.SS.y = 520 - textheight("NO")/2;
            B[7].D.DJ.y = 520 + textheight ("NO")/2;
            delay(80);
            outtextxy(840, 520, "NO");
        }
    }
    punct pozCursor;
    while(1)
    {
        click(pozCursor.x, pozCursor.y);
        if(apartine(pozCursor, B[5].D))
        {
            if(robot == 0) DA();
            else DA_Robot();
        }
        else if(apartine(pozCursor, B[7].D))
        {
            NU();
        }
    }
}
void rom(int &lmb) ///schimba pe romana daca nu e deja
{
    if(lmb != 1)
    {
        cleardevice();
        lmb = 1;
        limba();
    }

}
void eng(int &lmb) ///schimba pe eng daca nu e deja
{
    if(lmb != 0)
    {
        cleardevice();
        lmb = 0;
        limba();
    }

}
void limba() ///functia care creeaza pagina pentru limba si butoanele aferente paginii
{
    cleardevice();

    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, width, height);
    setbkcolor(BLACK);

    readimagefile("romania.gif", 0, 0, width/2-15, height);
    readimagefile("anglia.gif", width/2+15, 0, width, height);
    if(width == 1920)
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    else if(width == 1280)
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    setcolor(COLOR(206,17,39));
    setbkcolor(COLOR(253,209,22));
    if(lmb) outtextxy(width/4, height/2, "ROMANA");
    else outtextxy(width/4, height/2, "ROMANIAN");



    setcolor(COLOR(1,34,105));
    setbkcolor(COLOR(200,15,46));
    if(lmb) outtextxy(width/4 + width/4 + width/4, height/2, "ENGLEZA");
    else outtextxy(width/4 + width/4 + width/4, height/2, "ENGLISH");



    B[14].D.SS.x = width/4 - textwidth("ROMANIAN")/2;
    B[14].D.DJ.x = width/4 + textwidth("ROMANIAN")/2;
    B[14].D.SS.y = height/2 - textheight("ROMANIAN")/2;
    B[14].D.DJ.y = height/2 + textheight ("ROMANIAN")/2;


    B[16].D.SS.x = width/4 + width/4 + width/4 - textwidth("ENGLEZA")/2;
    B[16].D.DJ.x = width/4 + width/4 + width/4 + textwidth("ENGLEZA")/2;
    B[16].D.SS.y = height/2 - textheight("ENGLEZA")/2;
    B[16].D.DJ.y = height/2 + textheight ("ENGLEZA")/2;
    setbkcolor(BLACK);

    setbkcolor(COLOR(200,15,46));
    iesire1();
    setbkcolor(COLOR(1,43,127));
    inapoi1();
    setbkcolor(BLACK);

}

void volum_zero(int &volum)
{
    if(volum != 0)
    {
        volum = 0;
        sunet();
    }
}

void volum_full(int &volum)
{
    if(volum != 100)
    {
        volum = 100;
        sunet();
    }
}

void volum_jumate(int &volum)
{
    if(volum != 50)
    {
        volum = 50;
        sunet();
    }
}

void tzanca(int volum,int &melodie)
{
    melodie=1;
    if(volum == 100)
        PlaySound(TEXT("Tzanca_100.wav"),NULL,SND_ASYNC);
    else if (volum == 50)
        PlaySound(TEXT("Tzanca_50.wav"),NULL,SND_ASYNC);
    else
        PlaySound(0,0,0);
}

void opt_bit(int volum,int &melodie)
{
    melodie=8;
    if(volum == 100)
        PlaySound(TEXT("game_100.wav"),NULL,SND_ASYNC);
    else if (volum == 50)
        PlaySound(TEXT("game_50.wav"),NULL,SND_ASYNC);
    else
        PlaySound(0,0,0);
}

void creepin(int volum,int &melodie)
{
    melodie=3;
    if(volum == 100)
        PlaySound(TEXT("creepin_100.wav"),NULL,SND_ASYNC);
    else if (volum == 50)
        PlaySound(TEXT("creepin_50.wav"),NULL,SND_ASYNC);
    else
        PlaySound(0,0,0);
}

void rock(int volum,int &melodie)
{
    melodie = 69;
    if(volum == 100)
        PlaySound(TEXT("rock_100.wav"),NULL,SND_ASYNC);
    else if (volum == 50)
        PlaySound(TEXT("rock_50.wav"),NULL,SND_ASYNC);
    else
        PlaySound(0,0,0);
}

void craciun(int volum,int &melodie)
{
    melodie = 25;
    if(volum == 100)
        PlaySound(TEXT("craciun_100.wav"),NULL,SND_ASYNC);
    else if (volum == 50)
        PlaySound(TEXT("craciun_50.wav"),NULL,SND_ASYNC);
    else
        PlaySound(0,0,0);
}

void sunet() ///functia care creeaza pagina pt sunet, butoanele si pseudo slider-ul
{
    cleardevice();
    if(iarna == 0)
    {
        setfillstyle(SOLID_FILL, COLOR(255, 83, 10));
        bar(0, 0, width, height);
        setcolor(COLOR(0, 210, 255));
        setbkcolor(COLOR(255, 83, 10));
    }
    else
    {
        if(width == 1920) readimagefile("iarna_1920.gif", 0, 0, width, height);
        else readimagefile("iarna_1280.gif", 0, 0, width, height);
        setbkcolor(COLOR(191,221,229));
        setcolor(COLOR(101,149,153));
    }

    settextstyle(HORIZ_DIR, DEFAULT_FONT, 4);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    B[21].D.SS.x = width/4 - textwidth("TZANCA")/2;
    B[21].D.DJ.x = width/4 + textwidth("TZANCA")/2;
    B[21].D.SS.y = height/4 - textheight("TZANCA")/2;
    B[21].D.DJ.y = height/4 + textheight("TZANCA")/2 - 7;
    outtextxy(width/4, height/4, "TZANCA");

    B[22].D.SS.x = width/4 + width/4 - textwidth("8-BIT")/2;
    B[22].D.DJ.x = width/4 + width/4 + textwidth("8-BIT")/2;
    B[22].D.SS.y = height/4 - textheight("8-BIT")/2;
    B[22].D.DJ.y = height/4 + textheight("8-BIT")/2 - 7;
    outtextxy(width/4 + width/4, height/4, "8-BIT");

    B[23].D.SS.x = width/4 + width/4 + width/4 - textwidth("CREEPIN")/2;
    B[23].D.DJ.x = width/4 + width/4 + width/4 + textwidth("CREEPIN")/2;
    B[23].D.SS.y = height/4 - textheight("CREEPIN")/2;
    B[23].D.DJ.y = height/4 + textheight("CREEPIN")/2 - 7;
    outtextxy(width/4 + width/4 + width/4, height/4, "CREEPIN");

    B[24].D.SS.x = width/4 + width/8 - textwidth("ROCK")/2;
    B[24].D.DJ.x = width/4 + width/8 + textwidth("ROCK")/2;
    B[24].D.SS.y = height/4 + height/4 +  - textheight("ROCK")/2;
    B[24].D.DJ.y = height/4 + height/4 + textheight("ROCK")/2 - 7;
    outtextxy(width/4 + width/8, height/4 + height/4, "ROCK");

    if(lmb)
    {
        B[25].D.SS.x = width/4 + width/4 + width/8 - textwidth("CRACIUN")/2;
        B[25].D.DJ.x = width/4 + width/4 + width/8 + textwidth("CRACIUN")/2;
        B[25].D.SS.y = height/4 + height/4 - textheight("CRACIUN")/2;
        B[25].D.DJ.y = height/4 + height/4 + textheight("CRACIUN")/2 - 7;
        outtextxy(width/4 + width/4 + width/8, height/4 + height/4, "CRACIUN");
    }
    else
    {
        B[25].D.SS.x = width/4 + width/4 + width/8 - textwidth("CHRISTMAS")/2;
        B[25].D.DJ.x = width/4 + width/4 + width/8 + textwidth("CHRISTMAS")/2;
        B[25].D.SS.y = height/4 + height/4 - textheight("CHRISTMAS")/2;
        B[25].D.DJ.y = height/4 + height/4 + textheight("CHRISTMAS")/2 - 7;
        outtextxy(width/4 + width/4 + width/8, height/4 + height/4, "CHRISTMAS");
    }

    settextstyle(HORIZ_DIR, DEFAULT_FONT, 3);
    setlinestyle(SOLID_LINE,5,8);
    if(volum == 100)
    {
        setcolor(COLOR(0, 210, 255));
        line(width/4, height/4 + height/4 + height/4, width/4 + width/4, height/4 + height/4 + height/4);

        line(width/4 + width/4, height/4 + height/4 + height/4, width/4 + width/4 + width/4, height/4 + height/4 + height/4);

        setcolor(COLOR(0, 210, 255));;
        line(width/4 + width/4, height/4 + height/4 + height/4 - height/32, width/4 + width/4, height/4 + height/4 + height/4 + height/32); ///mijloc

        line(width/4, height/4 + height/4 + height/4 - height/32, width/4, height/4 + height/4 + height/4 + height/32); ///prima

        setcolor(COLOR(0, 210, 255));
        line(width/4 + width/4 + width/4, height/4 + height/4 + height/4 - height/32, width/4 + width/4 + width/4, height/4 + height/4 + height/4 + height/32); ///ultima

        setcolor(BLACK);
        outtextxy(width/4, height/4 + height/4 + height/4 + height/24, "0");

        outtextxy(width/4 + width/4, height/4 + height/4 + height/4 + height/24, "50");

        setcolor(COLOR(0, 210, 255));;
        outtextxy(width/4 + width/4 + width/4, height/4 + height/4 + height/4 + height/24, "100");
    }
    else if(volum == 50)
    {
        setcolor(COLOR(0, 210, 255));
        line(width/4, height/4 + height/4 + height/4, width/4 + width/4, height/4 + height/4 + height/4);

        setcolor(BLACK);
        line(width/4 + width/4, height/4 + height/4 + height/4, width/4 + width/4 + width/4, height/4 + height/4 + height/4);

        setcolor(COLOR(0, 210, 255));
        line(width/4 + width/4, height/4 + height/4 + height/4 - height/32, width/4 + width/4, height/4 + height/4 + height/4 + height/32); ///mijloc

        setcolor(COLOR(0, 210, 255));
        line(width/4, height/4 + height/4 + height/4 - height/32, width/4, height/4 + height/4 + height/4 + height/32); ///prima

        setcolor(BLACK);
        line(width/4 + width/4 + width/4, height/4 + height/4 + height/4 - height/32, width/4 + width/4 + width/4, height/4 + height/4 + height/4 + height/32); ///ultima

        setcolor(BLACK);
        outtextxy(width/4, height/4 + height/4 + height/4 + height/24, "0");

        setcolor(COLOR(0, 210, 255));;
        outtextxy(width/4 + width/4, height/4 + height/4 + height/4 + height/24, "50");

        setcolor(BLACK);
        outtextxy(width/4 + width/4 + width/4, height/4 + height/4 + height/4 + height/24, "100");
    }
    else
    {
        setcolor(BLACK);
        line(width/4, height/4 + height/4 + height/4, width/4 + width/4, height/4 + height/4 + height/4);

        line(width/4 + width/4, height/4 + height/4 + height/4, width/4 + width/4 + width/4, height/4 + height/4 + height/4);

        line(width/4 + width/4, height/4 + height/4 + height/4 - height/32, width/4 + width/4, height/4 + height/4 + height/4 + height/32); ///mijloc

        setcolor(COLOR(0, 210, 255));
        line(width/4, height/4 + height/4 + height/4 - height/32, width/4, height/4 + height/4 + height/4 + height/32); ///prima

        setcolor(BLACK);
        line(width/4 + width/4 + width/4, height/4 + height/4 + height/4 - height/32, width/4 + width/4 + width/4, height/4 + height/4 + height/4 + height/32); ///ultima

        setcolor(COLOR(0, 210, 255));
        outtextxy(width/4, height/4 + height/4 + height/4 + height/24, "0");

        setcolor(BLACK);
        outtextxy(width/4 + width/4, height/4 + height/4 + height/4 + height/24, "50");

        outtextxy(width/4 + width/4 + width/4, height/4 + height/4 + height/4 + height/24, "100");
    }

    B[26].D.SS.x = width/4 - textwidth("0");
    B[26].D.DJ.x = width/4 + textwidth("0");
    B[26].D.SS.y = height/4 + height/4 + height/4 - height/32 - textheight("0");
    B[26].D.DJ.y = height/4 + height/4 + height/4 + height/24 + textheight("0");

    B[27].D.SS.x = width/4 + width/4 - textwidth("50");
    B[27].D.DJ.x = width/4 + width/4 + textwidth("50");
    B[27].D.SS.y = height/4 + height/4 + height/4 - height/32 - textheight("50");
    B[27].D.DJ.y = height/4 + height/4 + height/4 + height/24 + textheight("50");

    B[28].D.SS.x = width/4 + width/4 + width/4 - textwidth("100");
    B[28].D.DJ.x = width/4 + width/4 + width/4 + textwidth("100");
    B[28].D.SS.y = height/4 + height/4 + height/4 - height/32 - textheight("100");
    B[28].D.DJ.y = height/4 + height/4 + height/4 + height/24 + textheight("100");



    iesire1();
    inapoi1();
}
void setari() ///idem ca la celelalte functii cu nume sugestive
{

    cleardevice();
    if(iarna == 0)
    {
        setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
        bar(0, 0, width, height);
        setbkcolor(COLOR(0, 210, 255));
        setcolor(COLOR(255, 83, 10));
    }
    else
    {
        if(width == 1920) readimagefile("iarna_1920.gif", 0, 0, width, height);
        else readimagefile("iarna_1280.gif", 0, 0, width, height);
        setbkcolor(COLOR(191,221,229));
        setcolor(COLOR(101,149,153));
    }
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if(width == 1920)
    {
        if(lmb)
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            B[17].D.SS.x = width/4 - textwidth("LIMBA")/2;
            B[17].D.DJ.x = width/4 + textwidth("LIMBA")/2;
            B[17].D.SS.y = height/2 - textheight("LIMBA")/2;
            B[17].D.DJ.y = height/2 + textheight("LIMBA")/2 - 7;
            outtextxy(width/4, height/2, "LIMBA");
            B[30].D.SS.x = width/4 + width/4 - textwidth("SUNET")/2;
            B[30].D.DJ.x = width/4 + width/4 + textwidth("SUNET")/2;
            B[30].D.SS.y = height/2 - textheight("SUNET")/2;
            B[30].D.DJ.y = height/2 + textheight("SUNET")/2 - 7;
            outtextxy(width/4 + width/4, height/2, "SUNET");
            B[12].D.SS.x = width/4 + width/4 + width/4 - textwidth("REZOLUTIE")/2;
            B[12].D.DJ.x = width/4 + width/4 + width/4 + textwidth("REZOLUTIE")/2;
            B[12].D.SS.y = height/2 - textheight("REZOLUTIE")/2;
            B[12].D.DJ.y = height/2 + textheight("REZOLUTIE")/2 - 7;
            outtextxy(width/4 + width/4 + width/4, height/2, "REZOLUTIE");
        }
        else
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            B[17].D.SS.x = width/4 - textwidth("LANGUAGE")/2;
            B[17].D.DJ.x = width/4 + textwidth("LANGUAGE")/2;
            B[17].D.SS.y = height/2 - textheight("LANGUAGE")/2;
            B[17].D.DJ.y = height/2 + textheight("LANGUGAE")/2 - 7;
            outtextxy(width/4, height/2, "LANGUAGE");
            B[30].D.SS.x = width/4 + width/4 - textwidth("SOUND")/2;
            B[30].D.DJ.x = width/4 + width/4 + textwidth("SOUND")/2;
            B[30].D.SS.y = height/2 - textheight("SOUND")/2;
            B[30].D.DJ.y = height/2 + textheight("SOUND")/2 - 7;
            outtextxy(width/4 + width/4, height/2, "SOUND");
            B[12].D.SS.x = width/4 + width/4 + width/4 - textwidth("RESOLUTION")/2;
            B[12].D.DJ.x = width/4 + width/4 + width/4 + textwidth("RESOLUTION")/2;
            B[12].D.SS.y = height/2 - textheight("RESOLUTION")/2;
            B[12].D.DJ.y = height/2 + textheight("RESOLUTION")/2 - 7;
            outtextxy(width/4 + width/4 + width/4, height/2, "RESOLUTION");
        }
    }
    else if(width == 1280)
    {
        if(lmb)
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            B[17].D.SS.x = width/4 - textwidth("LIMBA")/2;
            B[17].D.DJ.x = width/4 + textwidth("LIMBA")/2;
            B[17].D.SS.y = height/2 - textheight("LIMBA")/2;
            B[17].D.DJ.y = height/2 + textheight("LIMBA")/2 - 7;
            outtextxy(width/4, height/2, "LIMBA");
            B[30].D.SS.x = width/4 + width/4 - textwidth("SUNET")/2;
            B[30].D.DJ.x = width/4 + width/4 + textwidth("SUNET")/2;
            B[30].D.SS.y = height/2 - textheight("SUNET")/2;
            B[30].D.DJ.y = height/2 + textheight("SUNET")/2 - 7;
            outtextxy(width/4 + width/4, height/2, "SUNET");
            B[12].D.SS.x = width/4 + width/4 + width/4 - textwidth("REZOLUTIE")/2;
            B[12].D.DJ.x = width/4 + width/4 + width/4 + textwidth("REZOLUTIE")/2;
            B[12].D.SS.y = height/2 - textheight("REZOLUTIE")/2;
            B[12].D.DJ.y = height/2 + textheight("REZOLUTIE")/2 - 7;
            outtextxy(width/4 + width/4 + width/4, height/2, "REZOLUTIE");
        }
        else
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            B[17].D.SS.x = width/4 - textwidth("LANGUAGE")/2;
            B[17].D.DJ.x = width/4 + textwidth("LANGUAGE")/2;
            B[17].D.SS.y = height/2 - textheight("LANGUAGE")/2;
            B[17].D.DJ.y = height/2 + textheight("LANGUAGE")/2 - 7;
            outtextxy(width/4, height/2, "LANGUAGE");
            B[30].D.SS.x = width/4 + width/4 - textwidth("SOUND")/2;
            B[30].D.DJ.x = width/4 + width/4 + textwidth("SOUND")/2;
            B[30].D.SS.y = height/2 - textheight("SOUND")/2;
            B[30].D.DJ.y = height/2 + textheight("SOUND")/2 - 7;
            outtextxy(width/4 + width/4, height/2, "SOUND");
            B[12].D.SS.x = width/4 + width/4 + width/4 - textwidth("RESOLUTION")/2;
            B[12].D.DJ.x = width/4 + width/4 + width/4 + textwidth("RESOLUTION")/2;
            B[12].D.SS.y = height/2 - textheight("RESOLUTION")/2;
            B[12].D.DJ.y = height/2 + textheight("RESOLUTION")/2 - 7;
            outtextxy(width/4 + width/4 + width/4, height/2, "RESOLUTION");
        }
    }

    iesire1();
    inapoi1();

}

void paginaRegulament()
{

    cleardevice();
    if (iarna == 0)
    {
        setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
        bar(0, 0, width, height);
        setbkcolor(COLOR(0, 210, 255));
        setcolor(COLOR(255, 83, 10));
    }
    else
    {
        if (width == 1920) readimagefile("iarna_1920.gif", 0, 0, width, height);
        else readimagefile("iarna_1280.gif", 0, 0, width, height);
        setbkcolor(COLOR(191, 221, 229));
        setcolor(COLOR(101, 149, 153));
    }
    if (width == 1280)
    {
        if (lmb)
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(width / 2, height / (height / 10) * 10, "REGULAMENT");
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(10, 200, "1.La inceputul jocului, fiecare jucator are cate 2 piese mici,");
            outtextxy(30, 230, "2 piese medii, 2 piese mari;");
            outtextxy(10, 280, "2.Fiecare jucator are 2 posibilitati:");
            outtextxy(30, 310, "* poate alege o piesa noua pe care sa o puna pe tabla de joc");
            outtextxy(30, 340, "* poate muta o piesa de-a sa in cadrul tablei de joc");
            outtextxy(10, 390, "3.Piesa de pe tabla de joc poate fi mutata intr-un spatiu liber,");
            outtextxy(30, 420, "dar si peste un alt gobblet mai mic, fie al sau, fie al adversarului;");
            outtextxy(10, 480, "4.Castiga jucatorul care completeaza o linie, fie orizontala, verticala ");
            outtextxy(30, 510, "sau diagonala, cu trei piese de aceeasi culoare;");
            outtextxy(10, 810, "acea piesa.");

            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(width / 2, 670, "!DISTRACTIE PLACUTA!");
        }
        else
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(width / 2, height / (height / 10) * 10, "RULES");
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(10, 200, "1.At the beggining of the game, each player has 2 small pieces,");
            outtextxy(30, 230, "2 medium pieces, 2 large pieces;");
            outtextxy(10, 280, "2.Each player has 2 choices:");
            outtextxy(30, 310, "* can choose a piece to be placed on the table");
            outtextxy(30, 340, "* can move a piece that is already placed");
            outtextxy(10, 390, "3.The piece on the table can be placed on a free position, also above");
            outtextxy(30, 420, "another smaller gobblet, either his or the opponent's;");
            outtextxy(10, 480, "4.The player that completes a line, either horizontally, vertically ");
            outtextxy(30, 510, "or diagonally, with 3 pieces of the same color,wins.;");
            outtextxy(10, 560, "5.Once a player has selected a piece, he has to use it.");
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(width / 2, 670, "!HAVE FUN!");


        }
    }
    else if (width == 1920)
    {
        if (lmb)
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            settextjustify(CENTER_TEXT, CENTER_TEXT);

            outtextxy(width / 2, height / (height / 10) * 10, "REGULAMENT");
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            settextjustify(LEFT_TEXT, CENTER_TEXT);

            outtextxy(10, 400, "1.La inceputul jocului, fiecare jucator are cate 2 piese mici, ");
            outtextxy(30, 430, "2 piese medii, 2 piese mari;");
            outtextxy(10, 480, "2.Fiecare jucator are 2 posibilitati:");
            outtextxy(30, 510, "* poate alege o piesa noua pe care sa o puna pe tabla de joc");
            outtextxy(30, 540, "* poate muta o piesa de-a sa in cadrul tablei de joc");
            outtextxy(10, 590, "3.Piesa de pe tabla de joc poate fi mutata intr-un spatiu liber, ");
            outtextxy(30, 620, "dar si peste un alt gobblet mai mic, fie al sau, fie al adversarului;");
            outtextxy(10, 680, "4.Castiga jucatorul care completeaza o linie, fie orizontala, verticala ");
            outtextxy(30, 710, "sau diagonala, cu trei piese de aceeasi culoare;");
            outtextxy(10, 760, "5.Odata ce un jucator a 'atins' o piesa, acesta trebuie sa foloseasca");
            outtextxy(10, 810, "acea piesa.");

            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(width / 2, 980, "!DISTRACTIE PLACUTA!");

        }
        else
        {
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            settextjustify(CENTER_TEXT, CENTER_TEXT);

            outtextxy(width / 2, height / (height / 10) * 10, "RULES");
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            settextjustify(LEFT_TEXT, CENTER_TEXT);

            outtextxy(10, 400, "1.At the beggining of the game, each player has 2 small pieces, ");
            outtextxy(30, 430, "2 medium pieces, 2 large pieces;");
            outtextxy(10, 480, "2.Each player has 2 choices:");
            outtextxy(30, 510, "* can choose a piece to be placed on the table");
            outtextxy(30, 540, "* can move a piece that is already placed");
            outtextxy(10, 590, "3.The piece on the table can be placed on a free position, also above");
            outtextxy(30, 620, "another smaller gobblet, either his or the opponent's;");
            outtextxy(10, 680, "4.The player that completes a line, either horizontally, vertically ");
            outtextxy(30, 710, "or diagonally, with 3 pieces of the same color,wins.;");
            outtextxy(10, 760, "5.Once a player has selected a piece, he has to use it.");
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy(width / 2, 980, "!HAVE FUN!");

        }
    }
    iesire1();
    inapoi1();
}

void rezolutie(int& width, int& height)
{

    cleardevice();

    if (iarna == 0)
    {
        setfillstyle(SOLID_FILL, COLOR(0, 210, 255));
        bar(0, 0, width, height);
        setbkcolor(COLOR(0, 210, 255));
        setcolor(COLOR(255, 83, 10));
    }
    else
    {
        if (width == 1920) readimagefile("iarna_1920.gif", 0, 0, width, height);
        else readimagefile("iarna_1280.gif", 0, 0, width, height);
        setbkcolor(COLOR(191, 221, 229));
        setcolor(COLOR(101, 149, 153));
    }

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if (width == 1920)
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    else if (width == 1280)
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    B[8].D.SS.x = width / 4 - textwidth("1920x1080") / 2;
    B[8].D.DJ.x = width / 4 + textwidth("1920x1080") / 2;
    B[8].D.SS.y = height / 2 - textheight("1920x1080") / 2;
    B[8].D.DJ.y = height / 2 + textheight("1920x1080") / 2 - 7;
    strcpy(B[8].text, "1920x1080");
    bar(B[8].D.SS.x, B[8].D.SS.y, B[8].D.DJ.x, B[8].D.DJ.y);
    if (width == 1920)
    {
        if (iarna) setcolor(COLOR(101, 149, 153));
        else setcolor(DARKGRAY);
    }
    else setcolor(COLOR(255, 83, 10));
    outtextxy(width / 4, height / 2, B[8].text);


    B[9].D.SS.x = width / 4 + width / 4 + width / 4 - textwidth("1280x720") / 2;
    B[9].D.DJ.x = width / 4 + width / 4 + width / 4 + textwidth("1280x720") / 2;
    B[9].D.SS.y = height / 2 - textheight("1280x720") / 2;
    B[9].D.DJ.y = height / 2 + textheight("1280x720") / 2 - 7;
    strcpy(B[9].text, "1280x720");
    bar(B[9].D.SS.x, B[9].D.SS.y, B[9].D.DJ.x, B[9].D.DJ.y);
    if (width == 1280)
    {
        if (iarna) setcolor(COLOR(101, 149, 153));
        else setcolor(DARKGRAY);
    }
    else setcolor(COLOR(255, 83, 10));
    outtextxy(width / 4 + width / 4 + width / 4, height / 2, B[9].text);


    iesire1();
    inapoi1();
}
