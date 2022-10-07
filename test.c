#include<stdio.h>
#include<string.h>
#include"main.c"

void menu() {
    printf("Zadajte cislo testu (0 ukonci program):\n\n");

    printf("1. Preddefinovana mapa.\n");
    printf("2. Mapa, kde sa neda dostat k drakovi.\n");
    printf("3. Mapa, kde sa neda dostat k princeznej.\n");
}

int main()
{
    char** mapa;
    int i, test, dlzka_cesty, cas, * cesta;
    int n = 0, m = 0, t = 0;
    FILE* f;
    while (1) {
        menu();
        scanf("%d", &test);
        dlzka_cesty = 0;
        n = m = t = 0;
        switch (test) {
        case 0://ukonci program
            return 0;
        case 1://nacitanie preddefinovanej mapy
            n = 10;
            m = 10;
            t = 12;
            mapa = (char**)malloc(n * sizeof(char*));
            mapa[0] = strdup("CCHCNHCCHN");
            mapa[1] = strdup("NNCCCHHCCC");
            mapa[2] = strdup("DNCCNNHHHC");
            mapa[3] = strdup("CHHHCCCCCC");
            mapa[4] = strdup("CCCCCNHHHH");
            mapa[5] = strdup("PCHCCCNNNN");
            mapa[6] = strdup("NNNNNHCCCC");
            mapa[7] = strdup("CCCCCPCCCC");
            mapa[8] = strdup("CCCNNHHHHH");
            mapa[9] = strdup("HHHPCCCCCC");
            cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
            break;
        case 2://nacitanie mapy zo suboru
            f = fopen("null drake.txt", "r");
            if (f)
                fscanf(f, "%d %d %d", &n, &m, &t);
            else
                continue;
            mapa = (char**)malloc(n * sizeof(char*));
            for (i = 0; i < n; i++) {
                mapa[i] = (char*)malloc(m * sizeof(char));
                for (int j = 0; j < m; j++) {
                    char policko = fgetc(f);
                    if (policko == '\n') policko = fgetc(f);
                    mapa[i][j] = policko;
                }
            }
            fclose(f);
            cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
            break;
        case 3: //pridajte vlastne testovacie vzorky
            f = fopen("null princess.txt", "r");
            if (f)
                fscanf(f, "%d %d %d", &n, &m, &t);
            else
                continue;
            mapa = (char**)malloc(n * sizeof(char*));
            for (i = 0; i < n; i++) {
                mapa[i] = (char*)malloc(m * sizeof(char));
                for (int j = 0; j < m; j++) {
                    char policko = fgetc(f);
                    if (policko == '\n') policko = fgetc(f);
                    mapa[i][j] = policko;
                }
            }
            fclose(f);
            cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
        default:
            continue;
        }
        cas = 0;
        for (i = 0; i < dlzka_cesty; i++) {
            printf("%d %d\n", cesta[i * 2], cesta[i * 2 + 1]);
            if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'H')
                cas += 2;
            else
                cas += 1;
            if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'D' && cas > t)
                printf("Nestihol si zabit draka!\n");
            if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'N')
                printf("Prechod cez nepriechodnu prekazku!\n");
            if (i > 0 && abs(cesta[i * 2 + 1] - cesta[(i - 1) * 2 + 1]) + abs(cesta[i * 2] - cesta[(i - 1) * 2]) > 1)
                printf("Neplatny posun Popolvara!\n");
        }
        printf("%d\n", cas);
    }
    return 0;
}