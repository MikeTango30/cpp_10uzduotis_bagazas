#include <iostream>
#include <fstream>

using namespace std;

int bagazoDydis(int aukstis, int plotis, int ilgis);
void dydzioPervirsisIrMokestis(bool ekonomineKlase, int bagazoDydis, int &pervirsis, float &mokestis);
void svorioPervirsisIrMokestis(bool ekonomineKlase, float rankinioBagazoSvoris, float paprastoBagazoSvoris, float &pervirsis, float &mokestis);
bool virsijoMatmenis(int bagazoDydis, float rankinioBagazoSvoris, float paprastoBagazoSvoris);
void rezultatai(float bagazoMokesciuSuma, float bagazoSvorioSuma, int virsijusiuBagazoMatmenisKiekis, ofstream &write);

//biznio logika
int const RANKINIO_BAGAZO_MAX_DYDIS = 120,
          RANKINIO_BAGAZO_MAX_SVORIS = 8,
          RANKINIO_BAGAZO_MOKESTIS_VIRSIJUS_VERSLO = 5,
          RANKINIO_BAGAZO_MOKESTIS_VIRSIJUS_EKONOMINE = 10,
          PAPRASTO_BAGAZO_MOKESTIS_VIRSIJUS_VERSLO = 5,
          PAPRASTO_BAGAZO_MOKESTIS_EKONOMINE = 5,
          PAPRASTO_BAGAZO_MAX_SVORIS = 23,
          KELEIVIO_DUOMENU_KIEKIS = 6,
//failo indeksai
          KLASE = 0,
          AUKSTIS = 1,
          PLOTIS = 2,
          ILGIS = 3,
          RANKINIO_BAGAZO_SVORIS = 4,
          PAPRASTO_BAGAZO_SVORIS = 5;


int main()
{

    bool ekonomineKlase;

    int   keleiviuSkaicius,
          rankinioBagazoDydis,
          bagazoDydzioPervirsis,
          virsijusiuBagazoMatmenisKiekis = 0;

    float bagazoSvorioPervirsis,
          bagazoSvorioMokestis,
          bagazoDydzioMokestis,
          rankinioBagazoSvoris,
          paprastoBagazoSvoris,
          bagazoMokesciuSuma =0,
          bagazoSvorioSuma = 0;

    ifstream read("duomenys4.txt");
    ofstream write("rezultatai4.txt");
    if(!read) { return 1;}
    read >> keleiviuSkaicius;
    float keleiviai[keleiviuSkaicius][KELEIVIO_DUOMENU_KIEKIS];

    //nuskaitom duomenis
    for (int i = 0; i < keleiviuSkaicius; i++){
        for (int j = 0; j < KELEIVIO_DUOMENU_KIEKIS; j++){
            read >> keleiviai[i][j];
        }
    }

    //apdorojame duomenis
    for (int i = 0; i < keleiviuSkaicius; i++) {
        ekonomineKlase = bool(keleiviai[i][KLASE]);
        rankinioBagazoDydis = bagazoDydis(int(keleiviai[i][AUKSTIS]), int(keleiviai[i][PLOTIS]), int(keleiviai[i][ILGIS]));
        rankinioBagazoSvoris = keleiviai[i][RANKINIO_BAGAZO_SVORIS];
        paprastoBagazoSvoris = keleiviai[i][PAPRASTO_BAGAZO_SVORIS];

        dydzioPervirsisIrMokestis(ekonomineKlase, rankinioBagazoDydis, bagazoDydzioPervirsis, bagazoDydzioMokestis);
        svorioPervirsisIrMokestis(ekonomineKlase, rankinioBagazoSvoris, paprastoBagazoSvoris, bagazoSvorioPervirsis, bagazoSvorioMokestis);

        virsijoMatmenis(rankinioBagazoDydis, rankinioBagazoSvoris, paprastoBagazoSvoris) ? virsijusiuBagazoMatmenisKiekis++ : false;
        bagazoMokesciuSuma += bagazoDydzioMokestis;
        bagazoMokesciuSuma += bagazoSvorioMokestis;
        bagazoSvorioSuma += bagazoSvorioPervirsis;
    }

    rezultatai(bagazoMokesciuSuma, bagazoSvorioSuma, virsijusiuBagazoMatmenisKiekis, write);
    cout << "10 Uzduotis. Atsakymas faile rezultatai4.txt" << endl;

    read.close();
    write.close();

    return 0;
}

int bagazoDydis(int aukstis, int plotis, int ilgis)
{
    int bagazoDydis = 0;
    bagazoDydis = aukstis + plotis + ilgis;

    return bagazoDydis;
}

void dydzioPervirsisIrMokestis(bool ekonomineKlase, int bagazoDydis, int &pervirsis, float &mokestis)
{
    pervirsis = 0;
    mokestis = 0;

    if ((bagazoDydis - RANKINIO_BAGAZO_MAX_DYDIS) > 0) {
        pervirsis += bagazoDydis - RANKINIO_BAGAZO_MAX_DYDIS;
        if (ekonomineKlase) {
            mokestis = float(pervirsis) * RANKINIO_BAGAZO_MOKESTIS_VIRSIJUS_EKONOMINE;
        } else {
            mokestis = float(pervirsis) * RANKINIO_BAGAZO_MOKESTIS_VIRSIJUS_VERSLO;
        }
    }

}

void svorioPervirsisIrMokestis(bool ekonomineKlase, float rankinioBagazoSvoris, float paprastoBagazoSvoris, float &pervirsis, float &mokestis)
{
    pervirsis = 0;
    mokestis = 0;

    if (rankinioBagazoSvoris > RANKINIO_BAGAZO_MAX_SVORIS) {
        pervirsis += (rankinioBagazoSvoris - RANKINIO_BAGAZO_MAX_SVORIS);
    }

    if (!ekonomineKlase) {
        if (paprastoBagazoSvoris > PAPRASTO_BAGAZO_MAX_SVORIS) {
            pervirsis += (paprastoBagazoSvoris - PAPRASTO_BAGAZO_MAX_SVORIS);
        }
        mokestis = pervirsis * PAPRASTO_BAGAZO_MOKESTIS_VIRSIJUS_VERSLO;
    } else {
        /*salygoje nenurodyta kiek apmokestinamas ekonomines klases virsytas rankinis bagazas.
        * Priimu, kad taip kaip paprastas bagazas.
        */
        pervirsis += paprastoBagazoSvoris;
        mokestis = pervirsis * PAPRASTO_BAGAZO_MOKESTIS_EKONOMINE;
    }
}

bool virsijoMatmenis(int bagazoDydis, float rankinioBagazoSvoris, float paprastoBagazoSvoris)
{
    return (bagazoDydis > RANKINIO_BAGAZO_MAX_DYDIS) ||
    (rankinioBagazoSvoris > RANKINIO_BAGAZO_MAX_SVORIS) ||
    (paprastoBagazoSvoris > PAPRASTO_BAGAZO_MAX_SVORIS);

}

void rezultatai(float bagazoMokesciuSuma, float bagazoSvorioSuma, int virsijusiuRankinioBagazoSvoriKiekis, ofstream &write)
{
    write << bagazoMokesciuSuma << " " << bagazoSvorioSuma << " " << virsijusiuRankinioBagazoSvoriKiekis;
}
