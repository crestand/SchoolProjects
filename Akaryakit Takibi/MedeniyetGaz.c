//list of header files
#include <windows.h>
#include <stdio.h>                   //contains printf,scanf etc
#include <conio.h>                   //contains getch(),gotoxy(),etc.
#include <stdlib.h>
#include <string.h>                  //contains strcmp(),strcpy(),strlen(),etc
#include <ctype.h>                   //contains toupper(), tolower(),etc
#include <direct.h>
#include <time.h>
#include <locale.h>

//list of functions
void mainMenu();
void delay();
void showPricePerUnit();
void updatePricePerUnit();
void sellGasoline();
void showTankStatus();
void showTotalSales();
//int t(void);
void readFile();
void writeToFile();
int fileExists();

//Constants
#define DIESEL_PRICE 0
#define DIESEL_TANK 1
#define DIESEL_TOTAL_LITER 2
#define DIESEL_PROFIT 3
#define GASOLINE_PRICE 4
#define GASOLINE_TANK 5
#define GASOLINE_TOTAL_LITER 6
#define GASOLINE_PROFIT 7
#define LPG_PRICE 8
#define LPG_TANK 9
#define LPG_TOTAL_LITER 10
#define LPG_PROFIT 11
#define MAX 1000


//List of global files
FILE *fp;
float gas[12];

COORD coord = {0, 0}; // sets coordinates to 0,0
COORD max_res,cursor_size;
void gotoxy (int x, int y)
{
        coord.X = x; coord.Y = y; // X and Y coordinates
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}





int main()
{
	//setlocale(LC_ALL, "Turkish"); // With this, I can't use shaded blocks so therefore I'm not going to use Turkish characters.
	if(!(fileExists("db.dat"))) // It checks if the db file exists and if it isn't, it creates it with the initial values.
	{

	    gas[DIESEL_PRICE] = 6.05;
	    gas[DIESEL_PROFIT] = 0.0;
	    gas[DIESEL_TANK] = MAX;
	    gas[DIESEL_TOTAL_LITER] = 0.0;
	    gas[GASOLINE_PRICE] = 6.63;
	    gas[GASOLINE_PROFIT] = 0.0;
	    gas[GASOLINE_TANK] = MAX;
	    gas[GASOLINE_TOTAL_LITER] = 0.0;
	    gas[LPG_PRICE] = 4.06;
	    gas[LPG_PROFIT] = 0.0;
	    gas[LPG_TANK] = MAX;
	    gas[LPG_TOTAL_LITER] = 0.0;
        writeToFile();
        }

        puts(
         "                                                                                                              \n"
         "                                                                                                              \n"
         "                                                                                                              \n"
         "                                                                        .`                                    \n"
         "                                                                    .+dmmho:`                                 \n"
         "                                                                 ../hNNMMMMMNNds/.                            \n"
         "                                                      ./sysssoo+.NNNMMMMMMMMMMMMMd                            \n"
         "                                                  `:sdNNdyyhhhhd/MMMMMMMMMMMMMMMMy                            \n"
         "                                                  -ddy/.`    ```.hMMMMMMMMMMNmmNMNh/`                         \n"
         "                                                   .`            `/dMMMMMMNs+oo:/hmddo-                       \n"
         "                                                   h`              `omMMMMossoyhy/:ydNNy:                     \n"
         "                                                  +Mo                .smNMy`   `:syo+mMMNh/`                  \n"
         "                                                 /NMN+                 `.oNh.     `:oyMMMMNd:                 \n"
         "                                                +NMMMNo                   -dm+`    `+Nd+sNdsh/`               \n"
         "                                               :NMMMMMM/                   `yNd/-:odm+` `-`/dNh.              \n"
         "                                               +MMMNMMMo                     :dNNNd+`       `yNm.             \n"
         "                                               `hNMMMNd.                      `--.`          `dMh             \n"
         "                                                `:+o+:`                                       /MM-            \n"
         "                                                                                              .MM/            \n"
         "    __  __   ___   ___    ___   _  _   ___  __   __  ___   _____      ___     _     ____       MM:            \n"
         "   |  \\/  | | __| |   \\  | __| | \\| | |_ _| \\ \\ / / | __| |_   _|    / __|   /_\\   |_  /      oMN`      \n"
         "   | |\\/| | | _|  | |) | | _|  | .` |  | |   \\ V /  | _|    | |     | (_ |  / _ \\   / /       dMh          \n"
         "   |_|  |_| |___| |___/  |___| |_|\\_| |___|   |_|   |___|   |_|      \\___| /_/ \\_\\ /___|     `MM/         \n"
         "                                                                                             .MM/             \n"
         "                                                                                              mMy             \n"
         "                                                                                              :mh.            \n"
         "                                                                                               `              \n"
         "                                      Gecmek icin bir tusa basiniz...                                           "
         );

    getch();
	mainMenu();
}

void mainMenu()
{
	system("cls"); // Clears cmd screen
	gotoxy(30,3);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Ana Menu \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(30,5);
	printf("\xDB\xDB\xDB\xDB\xB2 1. Birim Fiyat Goster");
	gotoxy(30,7);
	printf("\xDB\xDB\xDB\xDB\xB2 2. Birim Fiyat Guncelle");
	gotoxy(30,9);
	printf("\xDB\xDB\xDB\xDB\xB2 3. Akaryakit Satisi Yap");
	gotoxy(30,11);
	printf("\xDB\xDB\xDB\xDB\xB2 4. Depo Durumunu Goster");
	gotoxy(30,13);
	printf("\xDB\xDB\xDB\xDB\xB2 5. Toplam Satislari Goster");
	gotoxy(30,15);
	printf("\xDB\xDB\xDB\xDB\xB2 6. Uygulamayi Kapat");
	//gotoxy(20,17);
	//printf("\xDB\xDB\xDB\xDB\xB2 6. Menuyu Goster"); // I can't see why we should add "Show Menu" option while we already in the menu, so I disabled it.
	gotoxy(30,19);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(30,20);
	//int t();

	gotoxy(30,21);
	printf("Lutfen gitmek istediginiz menuyu seciniz:");
	switch(getch())
	{
		case '1':
		showPricePerUnit();

		break;
		case '2':
		updatePricePerUnit();

		break;
		case '3':
		sellGasoline();

	    break;
		case '4':
		showTankStatus();

	    break;
		case '5':
		showTotalSales();

		break;
		case '6':
		{
		system("cls");

        for(int i=3;i>0;i--)
        {
            gotoxy(20,3);
            printf("\xB2\xB2 %d saniye icinde program kapatilacaktir \xB2\xB2>",i);
            delay(1000);
        }

		exit(0);
		}
	    default:
		{
		gotoxy(10,23);
		printf("\aYanlis bir secim yaptiniz!!Lutfen tekrar deneyiniz!!");
		if(getch())
		mainMenu();
		}
	}
}

void showPricePerUnit()
{
    system("cls");

    readFile();
    gotoxy(20,5);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Birim Fiyat Listesi \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,7);
	printf("\xDB\xDB\xDB\xDB\xB2 1. Dizel: %.2f Tl/Lt",gas[DIESEL_PRICE]);
	gotoxy(20,9);
	printf("\xDB\xDB\xDB\xDB\xB2 2. Benzin: %.2f Tl/Lt",gas[GASOLINE_PRICE]);
	gotoxy(20,11);
	printf("\xDB\xDB\xDB\xDB\xB2 3. LPG: %.2f Tl/Lt",gas[LPG_PRICE]);
	gotoxy(20,13);
	printf("Menuye donmek icin bir tusa basiniz.");

	if(getch())
		mainMenu();
}

void updatePricePerUnit()
{
    readFile();
	system("cls");
	gotoxy(20,5);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Birim Fiyat Guncelleme \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,7);
	printf("\xDB\xDB\xDB\xDB\xDB\xB2 Guncellemek istediginiz akaryakit tipini seciniz \xB2\xDB\xDB\xDB\xDB\xDB");
	gotoxy(20,9);
	printf("\xDB\xDB\xDB\xDB\xB2 1. Dizel");
	gotoxy(20,11);
	printf("\xDB\xDB\xDB\xDB\xB2 2. Benzin");
	gotoxy(20,13);
	printf("\xDB\xDB\xDB\xDB\xB2 3. LPG");
	gotoxy(20,15);
	printf("\xDB\xDB\xDB\xDB\xB2 4. Ana menuye geri donun");
	gotoxy(20,17);
	printf("Lutfen seciminizi yapiniz: ");


	switch(getch())
	{
		case '1'://Dizel
		    system("cls");
            gotoxy(20,5);
            printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Dizel Fiyat Guncelleme \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		    gotoxy(20,7);
            printf("\xDB\xDB\xDB\xDB\xB2 Yeni Dizel fiyatini giriniz:  ");
		    scanf("%f",&gas[DIESEL_PRICE]);
            writeToFile();
            updatePricePerUnit();
			break;

		case '2'://Benzin
		    system("cls");
		    gotoxy(20,5);
            printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Benzin Fiyat Guncelleme \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		    gotoxy(20,7);
            printf("\xDB\xDB\xDB\xDB\xB2 Yeni Benzin fiyatini giriniz:  ");
		    scanf("%f",&gas[GASOLINE_PRICE]);
            writeToFile();
            updatePricePerUnit();
			break;

		case '3'://LPG
		    system("cls");
		    gotoxy(20,5);
            printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 LPG Fiyat Guncelleme \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		    gotoxy(20,7);
            printf("\xDB\xDB\xDB\xDB\xB2 Yeni LPG fiyatini giriniz:  ");
		    scanf("%f",&gas[LPG_PRICE]);
            writeToFile();
            updatePricePerUnit();
			break;

		case '4'://Mainmenu
			mainMenu();
			break;

		default:

			gotoxy(20,23);
			printf("\aYanlis bir secim yaptiniz!!Lutfen tekrar deneyiniz!!");
			if(getch())
			updatePricePerUnit();
	}
}


void sellGasoline()
{
    int gasLiter = 0;
    int gasPrice = 0;
    readFile();
    system("cls");
	gotoxy(20,5);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Satis Ekrani \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,7);
	printf("\xDB\xDB\xDB\xDB\xDB\xB2 Akaryakit tipini seciniz \xB2\xDB\xDB\xDB\xDB\xDB");
	gotoxy(20,9);
	printf("\xDB\xDB\xDB\xDB\xB2 1. Dizel");
	gotoxy(20,11);
	printf("\xDB\xDB\xDB\xDB\xB2 2. Benzin");
	gotoxy(20,13);
	printf("\xDB\xDB\xDB\xDB\xB2 3. LPG");
	gotoxy(20,15);
	printf("\xDB\xDB\xDB\xDB\xB2 4. Ana menuye geri donun");
	gotoxy(20,17);
	printf("Lutfen seciminizi yapiniz: ");

    char c;

	switch(getch()) // It's the same thing for all the three types of fuel so I'm just going to write a comment for one of them.
	{
	    /*
            First, we ask how much gasoline they wan to buy in Tl. After that we calculate the equivalent of that in liters because we are going to
            use liters while we make required calculations. Actually, we don't need the make it into separate variable and just write gasPrice / gas[*_PRICE]
            everywhere instead but it makes it easier to read and manage in the future.

	    */
		case '1'://Diesel
		    system("cls");

		    gotoxy(20,5);
            printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Satis Ekrani \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		    gotoxy(20,7);
            printf("\xDB\xDB\xDB\xDB\xDB\xB2 Ne kadarlik Dizel alacaksiniz? (Tl) \xB2\xDB\xDB\xDB\xDB\xDB");
            gotoxy(20,9);
            printf("\xDB\xDB\xDB\xDB\xB2 ");
            scanf("%d", &gasPrice);
            gasLiter = gasPrice / gas[DIESEL_PRICE];
            gotoxy(20,11);

            // We check if there is any fuel left in the tank first and if there isn't we just go back to the sellGasoline menu.
            if(gas[DIESEL_TANK] <= 0)
            {
                printf("\xDB\xDB\xDB\xDB\xB2  Depomuzda Dizel kalmamistir.");
                getch();
                sellGasoline();
            }

            /* If the amount of liters that the customer wants to buy is larger than what we have in the tank, we offer the option of buying what remains in the tank.
               If they press 'e' or 'E' (it doesn't matter because we change the letter 'e' to 'E' anyway) we sell them the fuel and do other required processes like
               calculating the profit, amount of liter that we just sell and assign them to gas[] array in correct indexes. After that we call writeToFile function and
               save these changes to database (which is "db.dat" file).

            */
            else if (gasLiter>gas[DIESEL_TANK])
            {

                printf("\xDB\xDB\xDB\xDB\xB2  Depomuzda %.2f Tl'lik Dizel kalmistir. Yuklemeyi onayliyor musunuz? (E/H).", gas[DIESEL_TANK] * gas[DIESEL_PRICE] );
                c = getch();
                if(toupper(c) == 'E' )
                    {
                        gas[DIESEL_PROFIT] += gas[DIESEL_TANK] * gas[DIESEL_PRICE];
                        gas[DIESEL_TOTAL_LITER] += gas[DIESEL_TANK];
                        gas[DIESEL_TANK] = 0;
                        writeToFile();
                        printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz basariyla gerceklesmistir.");
                        getch();
                        sellGasoline();
                    }
                else
                    {
                        printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz iptal edilmistir.");
                        getch();
                        sellGasoline();
                    }
            }

            // If there is enough fuel in the tank, we do the same thing that we did above but with the gasPrice and gasLiter variables.
            else
            {
                gas[DIESEL_PROFIT] += gasPrice;
                gas[DIESEL_TOTAL_LITER] += gasLiter;
                gas[DIESEL_TANK] -= gasLiter;
                writeToFile();
                printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz basariyla gerceklesmistir.");
                getch();
                sellGasoline();
            }


			break;

		case '2'://Gasoline
            system("cls");
		    gotoxy(20,5);
            printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Satis Ekrani \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		    gotoxy(20,7);
            printf("\xDB\xDB\xDB\xDB\xDB\xB2 Ne kadarlik Benzin alacaksiniz? (Tl) \xB2\xDB\xDB\xDB\xDB\xDB");
            gotoxy(20,9);
            printf("\xDB\xDB\xDB\xDB\xB2 ");
            scanf("%d", &gasPrice);
            gasLiter = gasPrice / gas[GASOLINE_PRICE];
            gotoxy(20,11);

            if(gas[GASOLINE_TANK] <= 0)
            {
                printf("\xDB\xDB\xDB\xDB\xB2  Depomuzda Benzin kalmamistir.");
                getch();
                sellGasoline();
            }

            else if (gasLiter>gas[GASOLINE_TANK])
            {

                printf("\xDB\xDB\xDB\xDB\xB2  Depomuzda %.2f Tl'lik Benzin kalmistir. Yuklemeyi onayliyor musunuz? (E/H).", gas[GASOLINE_TANK] * gas[GASOLINE_PRICE] );
                c = getch();
                if(toupper(c) == 'E' )
                    {
                        gas[GASOLINE_PROFIT] += gas[GASOLINE_TANK] * gas[GASOLINE_PRICE];
                        gas[GASOLINE_TOTAL_LITER] += gas[GASOLINE_TANK];
                        gas[GASOLINE_TANK] = 0;
                        writeToFile();
                        printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz basariyla gerceklesmistir.");
                        getch();
                        sellGasoline();
                    }
                else
                    {
                        printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz iptal edilmistir.");
                        getch();
                        sellGasoline();
                    }
            }

            else
            {
                gas[GASOLINE_PROFIT] += gasPrice;
                gas[GASOLINE_TOTAL_LITER] += gasLiter;
                gas[GASOLINE_TANK] -= gasLiter;
                writeToFile();
                printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz basariyla gerceklesmistir.");
                getch();
                sellGasoline();
            }


			break;

		case '3'://LPG
		    system("cls");
		    gotoxy(20,5);
            printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Satis Ekrani \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
		    gotoxy(20,7);
            printf("\xDB\xDB\xDB\xDB\xDB\xB2 Ne kadarlik LPG alacaksiniz? (Tl) \xB2\xDB\xDB\xDB\xDB\xDB");
            gotoxy(20,9);
            printf("\xDB\xDB\xDB\xDB\xB2 ");
            scanf("%d", &gasPrice);
            gasLiter = gasPrice / gas[LPG_PRICE];
            gotoxy(20,11);

            if(gas[LPG_TANK] <= 0)
            {
                printf("\xDB\xDB\xDB\xDB\xB2  Depomuzda Dizel kalmamistir.");
                getch();
                sellGasoline();
            }

            else if (gasLiter>gas[LPG_TANK])
            {

                printf("\xDB\xDB\xDB\xDB\xB2  Depomuzda %.2f Tl'lik LPG kalmistir. Yuklemeyi onayliyor musunuz? (E/H).", gas[LPG_TANK] * gas[LPG_PRICE] );
                c = getch();
                if(toupper(c) == 'E' )
                    {
                        gas[LPG_PROFIT] += gas[LPG_TANK] * gas[LPG_PRICE];
                        gas[LPG_TOTAL_LITER] += gas[LPG_TANK];
                        gas[LPG_TANK] = 0;
                        writeToFile();
                        printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz basariyla gerceklesmistir.");
                        getch();
                        sellGasoline();
                    }
                else
                    {
                        printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz iptal edilmistir.");
                        getch();
                        sellGasoline();
                    }
            }

            else
            {
                gas[LPG_PROFIT] += gasPrice;
                gas[LPG_TOTAL_LITER] += gasLiter;
                gas[LPG_TANK] -= gasLiter;
                writeToFile();
                printf("\xDB\xDB\xDB\xDB\xB2  Isleminiz basariyla gerceklesmistir.");
                getch();
                sellGasoline();
            }

			break;


		case '4'://Mainmenu
			mainMenu();
			break;

		default:

			gotoxy(20,23);
			printf("\aYanlis bir secim yaptiniz!!Lutfen tekrar deneyiniz!!");
			if(getch())
			updatePricePerUnit();
	}


}



void showTankStatus()
{
    system("cls");
    readFile(); // We call readFile function to make sure that we have the latest state of database.
    int dieselRow = gas[DIESEL_TANK] / 100;
    int gasolineRow = gas[GASOLINE_TANK] / 100;
    int lpgRow = gas[LPG_TANK] / 100;
    char a = 177, b = 219; // char 'a' = dotted block char 'b' = full block


    gotoxy(20,5);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Depo Durumu \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");

    gotoxy(20,8);
    printf("\xB2  ");


    //////////////////////////// Normally I would write a function for this but there is only 3 types so I just copy pasted it.
    for (int i = 0; i < 20; i++)
        printf("%c", a); //First we create the whole bar with dotted blocks.

    printf(" Dizel %%%.0f ", gas[DIESEL_TANK]/10); // We write the actual percentage of the tank
    gotoxy(23,8); // We go back to the beginning of bar

    for (int i = 0; i < dieselRow*2; i++) // We change the required amount of dotted blocks with full blocks
    {
        printf("%c", b);
        delay(25); //We don't need this but it look nicer with animations.
    }

    /////////////////////////////////////////////

    gotoxy(20,10);
    printf("\xB2  ");
    for (int i = 0; i < 20; i++)
        printf("%c", a);

    printf(" Benzin %%%.0f ", gas[GASOLINE_TANK]/10);
    gotoxy(23,10);

    for (int i = 0; i < gasolineRow*2; i++)
    {
        printf("%c", b);
        delay(25);
    }
    //////////////////////////////////////////////////

    gotoxy(20,12);
    printf("\xB2  ");
    for (int i = 0; i < 20; i++)
        printf("%c", a);

    printf(" LPG %%%.0f ", gas[LPG_TANK]/10);
    gotoxy(23,12);

    for (int i = 0; i < lpgRow*2; i++)
    {
        printf("%c", b);
        delay(25);
    }

    ///////////////////////////////////////////////////
    gotoxy(20,15);
    printf("\xDB\xDB\xDB\xDB\xB2 Depolari doldurmak icin 1'e basiniz");
    gotoxy(20,17);
    printf("\xDB\xDB\xDB\xDB\xB2 Ana menuye donmek icin bir tusa basiniz.");

    switch (getch())
    {
        case '1': //I wrote this so we don't need to open and change database file manually everytime.

            gas[DIESEL_TANK] = MAX;
            gas[GASOLINE_TANK] = MAX;
            gas[LPG_TANK] = MAX;
            writeToFile();
            showTankStatus();

            break;

        default:
            mainMenu();
            break;
    }
}


void showTotalSales()
{
    system("cls");
    readFile();
	gotoxy(20,5);
	printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Toplam Satis Durumu \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
	gotoxy(20,7);
	printf("\xDB\xDB\xDB\xDB\xB2 Satilan toplam Dizel: %.2f Litre", gas[DIESEL_TOTAL_LITER]);
	gotoxy(20,9);
	printf("\xDB\xDB\xDB\xDB\xB2 Kazanilan para: %.2f Tl", gas[DIESEL_PROFIT]);
	gotoxy(20,11);
	printf("\xDB\xDB\xDB\xDB\xB2 Satilan toplam Benzin: %.2f Litre", gas[GASOLINE_TOTAL_LITER]);
	gotoxy(20,13);
	printf("\xDB\xDB\xDB\xDB\xB2 Kazanilan para: %.2f Tl", gas[GASOLINE_PROFIT]);
	gotoxy(20,15);
	printf("\xDB\xDB\xDB\xDB\xB2 Satilan toplam LPG: %.2f Litre", gas[LPG_TOTAL_LITER]);
	gotoxy(20,17);
	printf("\xDB\xDB\xDB\xDB\xB2 Kazanilan para: %.2f Tl", gas[LPG_PROFIT]);
	gotoxy(20,19);
	printf("\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2 Toplam Kar: %.2f Tl \xB2\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB", gas[DIESEL_PROFIT] + gas[GASOLINE_PROFIT] + gas[LPG_PROFIT]);
	gotoxy(20,21);
	printf("Satis gecmisini sifirlamak icin 1'e basiniz.");
	gotoxy(20,23);
	printf("Menuye donmek icin bir tusa basiniz.");

    switch (getch())
    {
        case '1': //I wrote this so we don't need to open and change the database file manually everytime.
            printf("deneme");
            gas[DIESEL_TOTAL_LITER] = 0.0;
            gas[GASOLINE_TOTAL_LITER] = 0.0;
            gas[LPG_TOTAL_LITER] = 0.0;
            gas[DIESEL_PROFIT] = 0.0;
            gas[GASOLINE_PROFIT] = 0.0;
            gas[LPG_PROFIT] = 0.0;
            writeToFile();
            showTotalSales();
            break;

        default:
			mainMenu();
			break;
    }


}

// I kinda forgot about the sleep() function and don't know why the delay() funciton in conio.h isn't working so I wrote it myself.
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void readFile()
{
    fp=fopen("db.dat","rb"); //It opens the file
    int i;
    i=0;
    while(fscanf(fp,"%f",&gas[i++]) !=EOF); // It reads the content of the file and assigns them to gas array till the End Of File.
    fclose(fp); //It closes the file
}

void writeToFile()
{
    fp=fopen("db.dat","w+"); //It opens the file
    int i;
    for(i=0; i<12;i++) // It writes the content of gas[] array to file with spaces between them so we can read it later
    {
        fprintf (fp, "%.2f ", gas[i]);
    }
    fclose(fp); //It closes the file
}

int fileExists(const char * fileName) // It checks if the file given in the parameter exists and return true according to it.
{

    if (fp = fopen(fileName, "r")){
        fclose(fp);
        return 1;
    }
    return 0;
}
