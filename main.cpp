#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

/////////////QUESTE SONO LE VARIABILI CHE MI PERMETTONO DI RILEVARE SE QUESTI TASTI SONO PREMUTI/////
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPACEBAR 32
#define TASTO_V 118
#define TASTO_X 120
/////////////////////////////////////////////////////////////////////////////////////////////////////

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
struct PlayerTable{
    char name[20];
    int color=0;
    int ships=10;
    int colpi=0;
    int colpivuoto=0;
    int colpisegno=0;
};
struct first{
    int x;
    int y;
};
struct casella{
    bool hit=false;
    first first;
    bool horizontal;
    int lenght;
    int last;
    int which_ship=0;
};
struct ColorNames {
    char name[20];
    int contrast;
};
struct faction{
    int color;
    int player=0;
};
struct coordAttacchi{
    int x;
    int y;
}strikes[11];
PlayerTable stats[3];//Variabile che contiene tutte le statistiche dei player 1 e 2
casella tabell[3][11][11];//matrice tridimensionale che contiene tutte le informazioni del gioco. La prima dimensione serve a specificare il giocatore.
ColorNames color[16]; //Struct fatto da me che consiste essenzialemte in un registro dei colori
faction faction[4]; //Struct riferita alla scelta delle bandiere ad inizio gioco. Il colore di ogni nazione è cambiabile dal codice sorgente.
void PreparazioneColori();
void PreparazioneCustomizzazioniDefault();//Questo void e' prima del main() perchè contiene le impostazioni cambiabili dal codice sorgente. Provare a cambiare una variabile e poi ricompilare il tutto

void drawTitle();//Disegna il titolo "Goose Game"
void drawRules();//Disegna le regole

void gotoXY(int x, int y);//Funzione che sposta il cursore nella finestra
void setColor(int bg, int fg);//Funzione che setta il colore
void resetColor();//Riporta il colore a testo bianco su sfondo nero
char numberToLetter(int n);//Converte un numero in lettera (es. 1 -> A). Utile per outputtare le coordinate visto che solo il front-end presenta le lettere
int enemy();//Riporta il numero del giocatore nemico a quello che sta attualmente giocando (es. turno del giocatore 1 -> enemy(1)=2)
bool checkShipIntegrity(int xs,int ys);//Prende in input le coordinate di una casella della barca e, risalendo alla prima casella della nave, controlla casella per casella se è integra almeno di una casella

void drawSettingPlayerName();//Disegna il settaggio del nome
void drawSettingPlayerFactions(int select);//Disegna le bandiere + i bordi intorno a loro
void drawSettingDifficulty(int select);//Disegna la sceltra della difficolta'
void drawSettingFlagBorder(int x,int y,int select,int flag);//Disegna il bordo delle bandiera specificata
void drawTabell();//Disegna il tabellone di gioco. Modestamente e' l'algoritmo più complesso che abbia mai fatto. Un capolavoro xD
void drawSelection(int xs,int ys,int lenght,bool horizontal);//Disegna la nave grigia in fase di posizionamento
void deleteSelection(int xs,int ys,int lenght,bool horizontal);//Cancella la selezione della nave grigia
void drawCoord(int xs,int ys,int n);//Disegna il puntatore giallo (della scelta delle coordinate) che si sta muovendo
void drawPlaceCoord(int xs,int ys,int n);//Disegna le coordinate delle coordinate piazzate
void deleteCoord(int xs,int ys);//Cancella una coordinata. Essenzialmente riempie una casella come farebbe stampando la tabella intera
void drawTurn();//Disegna tutte le info/testi del turno (es colpi rimanenti, schema comandi ecc.)
void drawChooseAction(int select);//Disegna la scelta sul da farsi
void drawRemainingShips(int n);//Disegna in fase di posizionamento navi le navi rimanenti sulla destra
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
void showCursor(bool visible, DWORD size);

bool positioning=false;//Segna l'inizio della fase di posizionamento navi
bool settingCoord=false;//Segna l'inizio della fase di posizionamento coordinate attacchi
bool attacking=false;//Segna l'inizio della fase di attacco

int ngioc;//Indica il numero di giocatori
int turno;//Indica il giocatore che ha il turno
int BattleShipColor;//Variabile che indica il colore del tema del gioco. Ad esempio nelmenu principale
int scelta;//Variabile generale per le scelte
bool menurepeat=false;//Indica se ripetere il programma da caso in caso finisca o che venga scelta una cosa non possibile nel menu principale
bool repeat=false;
int maxcolpi;//Numero massimo di colpi per giocatore. Cambia a seconda della difficoltà


//////////////////CARATTERI ASCII SPECIALI/////////////////////
char a=219;
char b=177;
char c=220;
char d=223;

char aa=218; char ba=196; char ca=191; char da=194; char dd=195;
char ab=179;                           char db=180; char de=193;
char ac=192;              char cc=217; char dc=197;

char e=186;

char f=201; char g=187; char j=205;
char h=200; char i=188;
///////////////////////////////////////////////////////////////



using namespace std;
void PreparazioneCustomizzazioniDefault(){
    /* NUMERI ASSOCIATI AI COLORI
    0   BLACK
    1   BLUE
    2   GREEN
    3   CYAN
    4   RED
    5   MAGENTA
    6   BROWN
    7   LIGHTGRAY

    8   DARKGRAY
    9   LIGHTBLUE
    10  LIGHTGREEN
    11  LIGHTCYAN
    12  LIGHTRED
    13  LIGHTMAGENTA
    14  YELLOW
    15  WHITE


    REGOLE ASSEGNAZIONE:
    COLORI: -DA 0 A 15

    */
    BattleShipColor=12; //DETERMINA IL COLORE DELLA SCHERMATA "Battleship Game" iniziale

    faction[0].color=12;     //Colore della Deutsche Kaiserliche Marine
    faction[1].color=9;      //Colore della Marine Nationale
    faction[2].color=10;     //Colore della Regia Marina Italiana
    faction[3].color=3;      //Colore della British Royal Navy

}

int main(int argc, char** argv){
    showCursor(false, 10);//Utilizza questo sottoprogramma preso da Internet che dsattiva il flash fastidioso del cursore
    srand((unsigned)time(NULL));//Seme del random

    system("color 0f");
    PreparazioneColori();//Assegna ad ogni numero il suo nome testuale del colore ed il colore che fa contrasto
    PreparazioneCustomizzazioniDefault();//Definisce i parametri variabili sopra

    do{
        system("cls");
        menurepeat=false;
        drawTitle();//Disegna il titolo "Goose Game"

        ////////SEQUENZA DISEGNO MENU' INIZIALE////
        setColor(0,BattleShipColor);
        gotoXY(78,38);
        cout<<a;
        resetColor();
        cout<<" BENVENUTO! ";
        setColor(0,BattleShipColor);
        cout<<a;
        gotoXY(74,40);
        setColor(0,BattleShipColor);
        cout<<"1. Nuova Partita";
        gotoXY(74,42);
        setColor(0,7);
        cout<<"2. Carica partita";
        gotoXY(74,44);
        setColor(0,BattleShipColor);
        cout<<"3. Exit";
        setColor(0,BattleShipColor);
        gotoXY(74,46);
        resetColor();
        cout<<"> ";
        setColor(0,BattleShipColor);
        ///////////////////////////////////////////
        cin>>scelta;
        switch(scelta){//SCELTA OPZIONE MENU'
        case 1://1. Nuova Partita

        //////////SEQUENZA SCELTA NOMI GIOCATORI///////////////

        ngioc=2;

        for(turno=1;turno<=ngioc;turno++){//Chiede il nome ai giocatori
            system("cls");
            drawTitle();
            drawSettingPlayerName();
            gotoXY(56,41);
            cout<<"> ";
            setColor(0,BattleShipColor);
            cin>>(stats[turno].name);
        }
        system("cls");
        drawTitle();
        ///////////////////////////////////////////////////////
        //////////SEQUENZA SCELTA FAZIONI GIOCATORI/////////////
        for(turno=1;turno<=ngioc;turno++){//Fa scegliere le fazioni ai giocatori
            int select=0;//La variabile indica quale bandiera e' selezionata (NOTA: NON QUELLA SCELTA!) //Quando cambia il turno la selezione della bandiera si azzera
            for(select=0;faction[select].player!=0;select++){//Questo for cerca la prima bandiera a partire da sinistra che non è selezionata e imposta la selezione di conseguenza
            }


            for(int key=0;stats[turno].color==0;){//Questo for continua fino a che il giocatore ha impostato il colore AKA ha scelto una fazione
                drawSettingPlayerFactions(select);//Disegna tutte le bandiere e i bordi di conseguenza. Il bordo varia a secondo di "select". Il bordo della bandiera sara' spesso se select coincide con il suo numero
                key=0;
                switch((key=getch())){
                case KEY_LEFT:
                    select--;//La selezione va a sinistra
                    if(select<0){//Nel caso vada fuori dalle 4 opzioni, ritorna nell'estremo oppsto
                        select=3;
                    }
                    for(;faction[select].player!=0;select--){
                        if(select-1<0){
                            select=4;

                        }
                    }
                    break;
                case KEY_RIGHT:
                    select++;
                    if(select>3){
                        select=0;
                    }
                    for(;faction[select].player!=0;select++){
                        if(select+1>3){
                            select=-1;
                        }
                    }
                    break;
                case SPACEBAR://Seleziona la fazione
                    if(faction[select].player==0){
                        stats[turno].color=faction[select].color;
                        faction[select].player=turno;
                    }
                    break;
                default:

                    break;
                }
                drawSettingPlayerFactions(select);

            }
        }

        gotoXY(80,60);
        setColor(BattleShipColor,color[BattleShipColor].contrast); system("pause"); resetColor();
        ////////////////////////////////////////////
        //////////SEQUENZA SCELTA DIFFICOLTA'///////
        for(int n=0;n==0;n++){
            system("cls");
            drawTitle();

            setColor(0,BattleShipColor);
            gotoXY(78,41);
            cout<<a;
            resetColor();
            cout<<" Scegliete la difficolta'! ";
            setColor(0,BattleShipColor);
            cout<<a;
            resetColor();


            int select;
            bool placed;
            select=1;
            do{
                placed=false;

                int key;
                drawSettingDifficulty(select);
                switch((key=getch())){
                case KEY_UP://Fa salire la selezione
                    if(select-1>0){
                        select--;
                    }
                    else{
                        select=3;
                    }
                    break;
                case KEY_DOWN://Fa scendere la selezione
                    if(select+1>3){
                        select=1;
                    }
                    else{
                        select++;
                    }
                    break;
                case SPACEBAR://Sceglie la difficoltà i base a cosa è selezionato.
                    switch(select){
                    case 1: maxcolpi=150; break;
                    case 2: maxcolpi=80; break;
                    case 3: maxcolpi=30; break;
                    }
                    placed=true;
                    break;
                }

            }while(placed==false);
        }
        /////////////////////////////////////////////////////
        /////////DISEGNO RULES////////////////////
        system("cls");
        drawRules();
        system("pause");
        //////////////////////////////////////////
        //////SEQUENZA POSIZIONAENTO NAVI/////////////difficile
        system("cls");
        positioning=true;//Segna l'inizio della sequenza di posizionamento navi
        for(turno=1;turno<=ngioc;turno++){//Ogni giocatore posiziona le navi
            drawTurn();

            int lenght=0;//Lunghezza della nave corrente che si sta piazzando
            for(int n=1;n<=10;n++){//n e' il numero di nave che si sta piazzando
                bool horizontal=true;//Indica se la nave che si sta muovendo e' in orizzontale o verticale
                bool placed=false;//Quando la nave e' piazzata placed=true e allora passa alla prossima nave
                int xs=1;int ys=1;//Sono le coordinate della nave che si sta piazzando. Ogni volta che si passa ad una nuova nave le coordinate ritornano su 1,1
                bool out_of_limits=false;//Se la nave non puo' essere piazzata la variabile va su true
                bool exit=false;

                if(n<=4){//Se n<=4 vuol dire che si sta piazzando gli incrociatori e così via
                    lenght=2;//Imposta la lunghezza degli incrociatori
                }
                else if(n<=7){
                    lenght=3;
                }
                else if(n<=9){
                    lenght=4;
                }
                else{
                    lenght=5;
                }

                drawTabell();
                drawRemainingShips(n);//Disegna le navi rimanenti da piazzare sulla destra
                do{

                    out_of_limits=false;
                    drawSelection(xs,ys,lenght,horizontal);//Disegna la nave che si sta spostando.

                    int key=0;


                    switch((key=getch())){


                    case KEY_UP:
                        if(ys-1>0){//Se spostando la nave in su non si esce dal tabellone si esegue il decremento
                            deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                            ys--;
                        }
                        break;


                    case KEY_DOWN:
                        for(int n1=0,exit=false;n1<lenght&&exit==false;n1++){//Controlla casella per casella della nave se rientra nei limiti del gioco. n1 viene incrementata mano mano per quanto è lunga la nave

                            if(horizontal==true){//Se è orizzontale n1 viene sommata a xs
                                if(ys+1<11){
                                    deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                                    ys++;
                                    exit=true;
                                }
                            }
                            else{//Se è orizzontale n1 viene sommata a ys
                                if(ys+lenght<11){
                                    deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                                    ys++;
                                    exit=true;
                                }
                            }
                        }
                        break;


                    case KEY_LEFT:
                        if(xs-1>0){
                            deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                            xs--;
                        }
                        break;


                    case KEY_RIGHT:
                        if(horizontal==true){//Se e' orizzontale
                            if(xs+lenght<11){//Controlla la coda della nave per vedere se è entro i limiti limiti
                                deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                                xs++;
                            }
                        }
                        else{
                            if(xs+1<11){
                                deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                                xs++;
                            }
                        }
                        break;


                    case SPACEBAR://PIAZZA NAVE
                        switch(horizontal){//Questi due procedimenti sono esattamente uguali, solo che quando è orizzontale si incrementa la xs, mentre in verticale la ys
                        case true:
                            for(int n1=0;n1<lenght&&out_of_limits==false;n1++){//Questo for cerca una possible collisione della nave in selezione con una nave già piazzata
                                if (tabell[turno][xs+n1][ys].which_ship!=0){
                                    out_of_limits=true;//Se la nave non può essere piazzata out_of_limits=true;
                                }
                            }
                            break;
                        case false:
                            for(int n1=0;n1<lenght&&out_of_limits==false;n1++){
                                if (tabell[turno][xs][ys+n1].which_ship!=0){
                                    out_of_limits=true;
                                }
                            }
                            break;
                        }
                        if(out_of_limits==false){//Se la nave puo' essere piazzata
                            int ship;
                            switch (lenght){ //In base alla lunghezza della nave viene settata la variabile che indica il tipo di nave che sta per essere piazzata
                            case 2: ship=1; break;
                            case 3: ship=2; break;
                            case 4: ship=3; break;
                            case 5: ship=4; break;
                            }

                            for(int n1=0;n1<lenght;n1++){//Piazza su ogni casella su cui si trova la barca delle informazioni fondamentali
                                if(horizontal==true){
                                    tabell[turno][xs+n1][ys].first.x=xs;//Coordinata X della prima casella della nave
                                    tabell[turno][xs+n1][ys].first.y=ys;//Coordinata Y della prima casella della nave
                                    tabell[turno][xs+n1][ys].horizontal=horizontal;//Indica se la nave che e' in quella casella è orizzontale o verticale
                                    tabell[turno][xs+n1][ys].lenght=lenght;//Indica la lunghezza della nave che e' in quella casella
                                    tabell[turno][xs+n1][ys].which_ship=ship;//Indica il tipo di nave che e' in quella casella
                                    tabell[turno][xs+n1][ys].last=xs+lenght-1;//Indica la coordinata dell'ultima casella della nave. Siccome e' orizzontale viene settato come valore quello della X, perchè la Y e' costante
                                }
                                else{
                                    tabell[turno][xs][ys+n1].first.x=xs;
                                    tabell[turno][xs][ys+n1].first.y=ys;
                                    tabell[turno][xs][ys+n1].horizontal=horizontal;
                                    tabell[turno][xs][ys+n1].lenght=lenght;
                                    tabell[turno][xs][ys+n1].which_ship=ship;
                                    tabell[turno][xs][ys+n1].last=ys+lenght-1;//Indica la coordinata dell'ultima casella della nave. Siccome e' verticale viene settato come valore quello della Y, perchè la X e' costante
                                }
                            }
                            placed=true;
                        }
                        break;


                    case 86:
                    case TASTO_V://CAPOVOLGI NAVE
                        deleteSelection(xs,ys,lenght,horizontal);//Prima di cambiare la posizione della nave che si sta spostando, si cancella la nave nella posizione vecchia
                        horizontal=!horizontal;//Viene cvapovolta la nave
                        int recesso=0;//Indica di quante caselle deve tornare indietro la nave se,in caso di capovolgimento,esca dal tabellone
                        switch(horizontal){
                        case true:
                            for(int n1=0;n1<lenght;n1++){//Conta quante sono le caselle che sforano e poi le sottrae alla xs nel caso sia orizzontale
                                if (xs+n1>10){
                                    recesso++;
                                }
                                xs-=recesso;
                            }
                            break;
                        case false:
                            for(int n1=0;n1<lenght;n1++){//Conta quante sono le caselle che sforano e poi le sottrae alla xs nel caso sia verticale
                                if (ys+n1>10){
                                    recesso++;
                                }
                                ys-=recesso;
                            }
                            break;
                        }
                        break;
                    }

                }while(placed==false);
            }
        }
        positioning=false;//Fine sequenza posizionamento


        system("cls");
        /////////////////////////////////////////////////////
        ///////////////SEQUENZA COUNTDOWN////////////////////
        gotoXY(130,20); cout<<"E' TUTTO PRONTO!";
        gotoXY(126,22); cout<<"LA PARTITA INIZIERA' FRA";
        for(int n=4;n<=4&&n>=0;n--){
            Sleep(1000);
            gotoXY(137,24); setColor(BattleShipColor,0); cout<<n;

        }
        resetColor();
        system("cls");
        //////////////////////////////////////////////////////


        //////////////////INIZIO SEQUENZA PRINCIPALE GIOCO/////////////////////////////////////////////
        for(turno=1;turno<=ngioc;turno++){ //Questo for scorre fra tutti i giocatori creando il fattore dei turni, infondo al for c'e il controllo che permette di ritornare a turno=1, nel caso sia turno==ngioc
            settingCoord=false;//Indica che non e' iniziata la fase di settaggio delle coordinate attacchi
            system("cls");
            drawTabell();
            drawTurn();
            ////scelta azione attaccare-saltare turno///
            //////////SOLITO ALGORITMO//////
            int select;
            for(int n=0;n==0;n++){
                gotoXY(130,18); setColor(stats[turno].color,0); cout<<stats[turno].name;
                setColor(stats[turno].color,0); cout<<", ci dia un ordine!";
                resetColor();



                select=1;
                bool placed;
                do{

                    placed=false;

                    int key;
                    drawChooseAction(select);
                    switch((key=getch())){
                        case KEY_UP:
                            if(select-1>0){
                                select--;
                            }
                            else{
                                select=2;
                            }
                        break;
                        case KEY_DOWN:
                            if(select+1>2){
                                select=1;
                            }
                            else{
                                select++;
                            }
                        break;
                        case SPACEBAR:
                            placed=true;
                        break;
                    }
                }while(placed==false);
            /////////////////////////////////////////
            }
            if(select==1){//Nel caso la scelta sia 1 (cioe' attaccare) inziai la fase di designazione delle coordinate attacchi
                system("cls");
                drawTabell();
                bool start_attack=false;//Quando si preme TASTO_X questa variabile va su True ed inizia la fase di attacco
                int xs; int ys;//Coordinate del segnalino che si sta spostando
                bool placed=false;//Quando si piazza un segnalino placed=true;
                int key;
                settingCoord=true;//Indica che non e' iniziata la fase di settaggio delle coordinate attacchi
                for(int n=1;n<=10;n++){//Azzera tutto il registro delle coordinate
                	strikes[n].x=0;
                	strikes[n].y=0;
				}
                int n=1;
                for(n=1,placed=false;start_attack==false;){//APPENA SI E' PIAZZATA LA NAVE
                    xs=1; ys=1;

                    placed=false;
                    do{//MENTRE SI SPOSTA IL SEGNALINO
                        if(n<=stats[turno].ships&&maxcolpi-stats[turno].colpi>0){//Disegna il segnalino corrente solo se non si sta sforando di numeri di attacchi. (es 1uando ho designato 10 attcchi, non mostra l'11 segnalino)
                    	 drawCoord(xs,ys,n);
					    }
                        drawTurn();
                        key=0;
                        switch(key=getch()){
                            case KEY_UP:
                                if(n<=stats[turno].ships && maxcolpi-stats[turno].colpi>0 && ys-1>0){//Sposta il segnalino solo se non si sta sforando di attacchi e non si esce dai limiti del tabellone
                                    deleteCoord(xs,ys);
                                    ys--;
                                }
                            break;
                            case KEY_DOWN:
                                if(n<=stats[turno].ships && maxcolpi-stats[turno].colpi>0 && ys+1<=10){//Sposta il segnalino solo se non si sta sforando di attacchi e non si esce dai limiti del tabellone
                                    deleteCoord(xs,ys);
                                    ys++;
                                }
                            break;
                            case KEY_LEFT:
                                if(n<=stats[turno].ships && maxcolpi-stats[turno].colpi>0 && xs-1>0){//Sposta il segnalino solo se non si sta sforando di attacchi e non si esce dai limiti del tabellone
                                    deleteCoord(xs,ys);
                                    xs--;
                                }
                            break;
                            case KEY_RIGHT:
                                if(n<=stats[turno].ships && maxcolpi-stats[turno].colpi>0 && xs+1<=10){//Sposta il segnalino solo se non si sta sforando di attacchi e non si esce dai limiti del tabellone
                                    deleteCoord(xs,ys);
                                    xs++;
                                }
                            break;
                        case SPACEBAR://PIAZZA IL SEGNALINO
                                if(n<=stats[turno].ships && maxcolpi-stats[turno].colpi>0){//Piazza il segnalino solo se non si sta sforando di attacchi e non si esce dai limiti del tabellone
                                    bool not_possible=false;//Se e' =true non sara' possibile confermare la coordinata
                                    int n1=1;
                                    for(n1=1; n1<n&&not_possible==false; n1++){//Questo for controlla le coordinate di ogni attacco selezionato. Se le coordinate che stiamo cercando di confermare coincidono con delle altre precedenti, non fa nulla
                                        if(strikes[n1].x==xs && strikes[n1].y==ys){
                                            not_possible=true;
                                        }
                                    }
                                    if(not_possible==false){//Se e' possibile piazzare la coordinata, procede
                                        strikes[n1].x=xs;//Piazza nel registro degli attacco la x della coordinata
                                        strikes[n1].y=ys;//Piazza nel registro degli attacco la y della coordinata
                                        drawPlaceCoord(xs,ys,n);
                                        placed=true;
                                        n++;//La coordinata n è stata piazzata, quindi incrementa n e passa alla nave successiva
                                        stats[turno].colpi++;
                                    }
                                }
                            break;
                        case TASTO_V://ANNULLA L'ULTIMO ATTACCO
                                if(n>1){//Se e' stata piazzata almeno una nave
                                    deleteCoord(xs,ys);//Cancella il segnalino sulla tabella
                                    strikes[n].x=0; strikes[n].y=0;//Azzera le coordinate dell'attacco corrente
                                    n--;//Retrocede all'attacco precedente
                                    xs=strikes[n].x; ys=strikes[n].y;//Fa ripartire la selezione dall'attacco precedente appena annullato
                                    strikes[n].x=0; strikes[n].y=0;
                                    stats[turno].colpi--;//Annulla un colpo
                                    deleteCoord(xs,ys);//Cancella la coordinata appena rimossa
                                }
                            break;
                            case TASTO_X:
                            if(n>1){
                                    start_attack=true;//Variabile va su True, quindi esce dal ciclo e inizia l'attacco
                                    placed=true;
                                    deleteCoord(xs,ys);//Cancella il segnalino dell'attacco che si sta spostando
                                }
                            break;
                        }
                    }while(placed==false);
                }

                if(start_attack==true){//Se è stato ordinato di attaccare
                    settingCoord=false;//Segna la fine della fase di decisione coordinate
                    attacking=true;//Segna l'inizio della fase di attacco
                    for(int n1=1;n1<n;n1++){
                        ////////////ANIMAZIONE ATTACCO////////////////////
                        drawCoord(strikes[n1].x,strikes[n1].y,n1);
                        resetColor(); gotoXY(130,10); cout<<"                             ";
                        gotoXY(130,14); cout<<"LANCIO ATTACCO "<<n1<<" IN "; setColor(stats[turno].color,0); cout<<numberToLetter(strikes[n1].y)<<strikes[n1].x;
                        resetColor(); cout<<".";
                        Sleep(400);
                        gotoXY(130,14); cout<<"LANCIO ATTACCO "<<n1<<" IN "; setColor(stats[turno].color,0); cout<<numberToLetter(strikes[n1].y)<<strikes[n1].x;
                        resetColor(); cout<<"..";
                        Sleep(400);
                        gotoXY(130,14); cout<<"LANCIO ATTACCO "<<n1<<" IN "; setColor(stats[turno].color,0); cout<<numberToLetter(strikes[n1].y)<<strikes[n1].x;
                        resetColor(); cout<<"...";
                        Sleep(500);
                        //////////////////////////////////////////////////


                        if(tabell[enemy()][strikes[n1].x][strikes[n1].y].hit==true){//Se quella casella e' gia' stata colpita
                            deleteCoord(strikes[n1].x,strikes[n1].y);//Cancella il segnalino dell'attacco appena sferrato
                            gotoXY(130,15); cout<<"Hai gia' colpito "; setColor(stats[turno].color,0); cout<<numberToLetter(strikes[n1].y)<<strikes[n1].x; resetColor(); cout<<"!";
                            stats[turno].colpivuoto++;//Incrementa il contatore dei colpi andati a vuoto
                            drawTurn();//Stampa tutte le info del turno sulla destra, come i colpi rimanenti, andati a segna, a vuoto ecc.
                        }
                        else{//Se invece ha colpito una casella non colpita precedentemente....
                            if(tabell[enemy()][strikes[n1].x][strikes[n1].y].which_ship!=0){//.....ed e' stata colpita una nave
                                tabell[enemy()][strikes[n1].x][strikes[n1].y].hit=true;//Segna la casella come colpita
                                deleteCoord(strikes[n1].x,strikes[n1].y);//Cancella il segnalino dalla casella appena colpita ed automaticamente la ridisegna in modo aggiornato
                                stats[turno].colpisegno++;//Incrementa i colpi andati a segno
                                drawTurn();//Stampa tutte le info del turno sulla destra, come i colpi rimanenti, andati a segna, a vuoto ecc.
                                gotoXY(130,15); cout<<"Colpito! Hai colpito ";
                                switch(tabell[enemy()][strikes[n1].x][strikes[n1].y].which_ship){//A seconda di quale nave è contenuta in quella casella outputta il nome della nave
                                    case 1:
                                        cout<<"un INCROCIATORE!";
                                    break;
                                    case 2:
                                        cout<<"un SOTTOMARINO!";
                                    break;
                                    case 3:
                                        cout<<"una CORAZZATA!";
                                    break;
                                    case 4:
                                        cout<<"una PORTAEREI!";
                                    break;
                                }
                                switch(checkShipIntegrity(strikes[n1].x,strikes[n1].y)){//Controlla se la nave ha almeno una casella in vita
                            case false://Se e' stata completamente abbattuta
                                        Sleep(300);
                                        gotoXY(130,16); cout<<"Hai affondato tutta la nave! Ottimo lavoro capo!";
                                        Sleep(600);
                                    break;
                                }
                            }
                            else{//....o se invece non e' stato colpito niente
                            	tabell[enemy()][strikes[n1].x][strikes[n1].y].hit=true;
                                deleteCoord(strikes[n1].x,strikes[n1].y);
                                gotoXY(130,15); cout<<"Acqua! Forza capo!";
                                stats[turno].colpivuoto++;
                                drawTurn();
                            }

                        }
                        //////////cancella tutti i messaggi relativi al singolo attacco////////
                        Sleep(1000);
                        resetColor(); gotoXY(130,14); cout<<"                           ";
                        gotoXY(130,15); cout<<"                                       ";
                        gotoXY(130,16); cout<<"                                                  ";

                    }
                }
            }

            if(turno==ngioc){
                turno=0;
            }
        }

        //////////////////FINE SEQUENZA PRINCIPALE GIOCO/////////////////////////////////////////////

        break;



        case 2://2. Carica Partita
            menurepeat=true;

        break;

        case 3://3. Esci
            system("cls");
            drawTitle();
            setColor(0,BattleShipColor);
            gotoXY(59,38);
            cout<<a;
            resetColor();
            cout<<" TE NE VAI DI GIA'? TORNA PRESTO! ";
            setColor(0,BattleShipColor);
            cout<<a;
            resetColor();
            cout<<"\n\n\n\t\t\t\t\t\t\t";
        break;
        default://Opzione inesistente
            gotoXY(49,46);
            cout<<" ";
            gotoXY(49,46);
            menurepeat=true;//Fa in modo che il menu' si ripeta
        break;
        }
    }while (menurepeat==true);//Se e' su true (e' stato digitato un comando non valido) ripete il menu'

    cout<<"\n\n\n";

    return 0 ;
}



void PreparazioneColori(){/*QUA' ASSEGNA AD OGNI NUMERO DI COLORE UNA STRINGA + IL NUMERO DEL COLORE CHE FA CONTRASTO*/
    strcpy(color[0].name,"Nero");
    strcpy(color[1].name,"Blu");
    strcpy(color[2].name,"Verde");
    strcpy(color[3].name,"Ciano");
    strcpy(color[4].name,"Rosso");
    strcpy(color[5].name,"Magenta");
    strcpy(color[6].name,"Marrone");
    strcpy(color[7].name,"Grigio Chiaro");
    strcpy(color[8].name,"Grigio Scuro");
    strcpy(color[9].name,"Blu Chiaro");
    strcpy(color[10].name,"Verde Chiaro");
    strcpy(color[11].name,"Ciano Chiaro");
    strcpy(color[12].name,"Rosso chiaro");
    strcpy(color[13].name,"Magenta Chiaro");
    strcpy(color[14].name,"Giallo Chiaro");
    strcpy(color[15].name,"Bianco");
    color[0].contrast=15;
    color[1].contrast=15;
    color[2].contrast=0;
    color[3].contrast=15;
    color[4].contrast=0;
    color[5].contrast=0;
    color[6].contrast=15;
    color[7].contrast=0;
    color[8].contrast=15;
    color[9].contrast=15;
    color[10].contrast=0;
    color[11].contrast=0;
    color[12].contrast=0;
    color[13].contrast=0;
    color[14].contrast=0;
    color[15].contrast=0;
}



void drawTitle(){//DISEGNA IL TITOLO "Battleship Game"
    setColor(0 , BattleShipColor);
    cout<<"\n\t\t"<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"                             "<<a<<a<<a<<a<<"               "<<a<<a<<a<<a<<"          "<<a<<a<<a<<a<<a<<a<<a<<"                                     "<<a<<a<<a<<a<<a<<a<<a<<"               "<<a<<a<<a<<a;
    cout<<"\n\t\t"<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"                         "<<a<<a<<a<<b<<b<<b<<a<<"            "<<a<<a<<a<<b<<b<<b<<a<<"          "<<a<<b<<b<<b<<b<<b<<a<<"                                     "<<a<<b<<b<<b<<b<<b<<a<<"              "<<a<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<"                        "<<a<<b<<b<<b<<b<<b<<a<<"            "<<a<<b<<b<<b<<b<<b<<a<<"          "<<a<<b<<b<<b<<b<<b<<a<<"                                     "<<a<<b<<b<<b<<b<<b<<a<<"               "<<a<<a<<a<<a;
    cout<<"\n\t\t"<<a<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<"                       "<<a<<b<<b<<b<<b<<b<<a<<"            "<<a<<b<<b<<b<<b<<b<<a<<"          "<<a<<b<<b<<b<<b<<b<<a<<"                                     "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<"  "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"  "<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<"     "<<a<<b<<b<<b<<b<<a<<"     "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"        "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"   "<<a<<b<<b<<b<<b<<a<<" "<<a<<a<<a<<a<<a<<"       "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"   "<<a<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<"  "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<a<<"   "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<"    "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"  "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<"    "<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<"   "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<a<<"  "<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<"   "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<"             "<<a<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<"     "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<"  "<<a<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<"      "<<a<<b<<b<<b<<b<<b<<a<<"            "<<a<<b<<b<<b<<b<<b<<a<<"           "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"  "<<a<<a<<a<<a<<a<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<"  "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"      "<<a<<b<<b<<b<<b<<b<<a<<"            "<<a<<b<<b<<b<<b<<b<<a<<"           "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<b<<a<<"      "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<"      "<<a<<b<<b<<b<<b<<b<<a<<"            "<<a<<b<<b<<b<<b<<b<<a<<"           "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"        "<<a<<b<<b<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"  "<<a<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<a<<"      "<<a<<b<<b<<b<<b<<b<<a<<"    "<<a<<a<<a<<a<<a<<a<<"  "<<a<<b<<b<<b<<b<<b<<a<<"    "<<a<<a<<a<<a<<a<<a<<" "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<a<<"           "<<a<<a<<a<<a<<a<<a<<"   "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<a<<"      "<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<"  "<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"          "<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<"      " <<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"  "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<"  "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<a<<"       "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<"    "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<"  "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<"  "<<a<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a;
    cout<<"\n\t\t"<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"     "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"  "<<a<<a<<a<<a<<"         "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"        "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"  "<<a<<a<<a<<a<<a<<a<<a<<a<<"    "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"    "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"    "<<a<<a<<a<<a<<a<<a<<a<<"     "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t"<<"                                                                                                                                                       "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                                                                                                                                       "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                                                                                                                                      "<<a<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                                                                                                                                      "<<a<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                           "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"                                                                                              "<<a<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                        "<<a<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"                                                                                              "<<a<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t"<<"                                      "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                     "<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                    "<<a<<b<<b<<b<<b<<b<<a<<"       "<<a<<a<<a<<a<<a<<a<<"  "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"      "<<a<<a<<a<<a<<a<<a<<a<<"    "<<a<<a<<a<<a<<a<<a<<a<<"       "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t"<<"                                   "<<a<<b<<b<<b<<b<<b<<a<<"                "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"   "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a<<"  "<<a<<b<<b<<b<<b<<b<<b<<b<<a<<a<<"   "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a;
    cout<<"\n\t\t"<<"                                   "<<a<<b<<b<<b<<b<<b<<a<<"                "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a <<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a;
    cout<<"\n\t\t"<<"                                   "<<a<<b<<b<<b<<b<<b<<a<<"    "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"           "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<"     "<<a<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                   "<<a<<b<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<a<<"    "<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                   "<<a<<b<<b<<b<<b<<b<<a<<"    "<<a<<a<<a<<a<<a<<b<<b<<b<<b<<a<<"  "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                   "<<a<<b<<b<<b<<b<<b<<a<<"        "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t"<<"                                    "<<a<<b<<b<<b<<b<<b<<a<<"       "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                     "<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<a<<"    "<<a<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                      "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<b<<b<<a<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t"<<"                                        "<<a<<a<<a<<b<<b<<b<<b<<b<<b<<a<<a<<a<<b<<b<<b<<a<<" "<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a<<a<<b<<b<<b<<a<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"   "<<a<<b<<b<<b<<b<<a<<"  "<<a<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<a;
    cout<<"\n\t\t"<<"                                           "<<a<<a<<a<<a<<a<<a<<"   "<<a<<a<<a<<a<<" " <<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"   "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<"   "<<a<<a<<a<<a<<a<<a<<"   "<<a<<a<<a<<a<<a<<a<<"   "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a;
    resetColor();
}



void drawRules(){//DISEGNA LE REGOLE
    setColor(0 , BattleShipColor);
    cout<<"\n\n\n\t\t\t\t"<<" "<<" "<<" "<<c<<a<<a<<a<<a<<a<<a<<a<<a<<" "<<" "<<" "<<" "<<c<<a<<a<<a<<a<<a<<a<<a<<a<<" "<<" "<<" "<<" "<<c<<a<<a<<a<<a<<a<<a<<c<<" "<<" "<<" "<<c<<a<<a<<a<<a<<a<<a<<c<<" "<<" "<<" "<<c<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<c<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t\t\t"<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a;
    cout<<"\n\t\t\t\t"<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<d<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<d<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<d;
    cout<<"\n\t\t\t\t"<<" "<<c<<a<<a<<a<<c<<c<<c<<c<<a<<a<<d<<" "<<" "<<c<<a<<a<<a<<c<<c<<c<<" "<<" "<<" "<<" "<<" "<<" "<<c<<a<<a<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<c<<a<<a<<a<<c<<c<<c;
    cout<<"\n\t\t\t\t"<<d<<d<<a<<a<<a<<d<<d<<d<<d<<d<<" "<<" "<<" "<<d<<d<<a<<a<<a<<d<<d<<d<<" "<<" "<<" "<<" "<<" "<<d<<d<<a<<a<<a<<" "<<a<<a<<a<<a<<c<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<d<<d<<a<<a<<a<<d<<d<<d;
    cout<<"\n\t\t\t\t"<<d<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<c<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<c;
    cout<<"\n\t\t\t\t"<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<a<<a<<a<<a<<" "<<" "<<" "<<" "<<c<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a;
    cout<<"\n\t\t\t\t"<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a<<" "<<" "<<" "<<a<<a<<a<<a<<a<<a<<a<<a<<d<<" "<<" "<<" "<<d<<a<<a<<a<<a<<a<<a<<d<<" "<<" "<<a<<a<<a<<a<<a<<c<<c<<a<<a<<" "<<" "<<" "<<a<<a<<a<<a<<a<<a<<a<<a<<a<<a;
    cout<<"\n\t\t\t\t"<<" "<<" "<<a<<a<<a<<" "<<" "<<" "<<" "<<a<<a<<a<<"                                      "<<d;

    gotoXY(26,15);
    setColor(0,BattleShipColor);
    cout<<"- ";
    resetColor();
    cout<<"Affonda le navi del nemico e mantieni in vita la tua flotta per vincere! ";
    gotoXY(26,18);
    setColor(0,BattleShipColor);
    cout<<"- ";
    resetColor();
    cout<<"Ogni turno potete attaccare quante volte quante sono le vostre navi in vita";
    gotoXY(26,20);
    setColor(0,BattleShipColor);
    cout<<"- ";
    resetColor();
    cout<<"Vince chi affonda le navi avversarie con meno attacchi";
    gotoXY(26,22);
    setColor(0,BattleShipColor);
    cout<<"- ";
    resetColor();
    cout<<"Avete a disposizione MAX "<<maxcolpi<<" attacchi a testa";
    gotoXY(35,23);
    resetColor();
    cout<<"..Finiteli e finisce la partita";
    gotoXY(35,27);

    resetColor();
}



void gotoXY(int x, int y) {//FUNZIONE PER SPOSTARE IL CURSORE NELLA FINESTRA.
    COORD CursorPos = {x, y};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, CursorPos);
}



void setColor(int bg, int fg) {//IMPOSTA IL COLORE DEL TESTO (SFONDO E TESTO)
    int val;
    if (bg<0) bg=0;
    if (fg>15) fg=15;
    val=bg*16+fg;
    SetConsoleTextAttribute(
    GetStdHandle(STD_OUTPUT_HANDLE),val);
}



void resetColor(){//RESETTA IL TESTO RIPORTANDOLO A BIANCO SU SFONDO NERO
    setColor(0,15);
}



int enemy(){//Se e' il turno del giocatore 1, l'output sara' 2 e viceversa.
    switch(turno){
    case 1:
        return 2;
        break;
    case 2:
        return 1;
        break;
    }
}



char numberToLetter(int n){//Restituisce in char la lettera (in ASCII) corrispondente al numero (es. 1=A,2=B). Serve solo per il front-end
    return 64+n;
}



void drawSettingPlayerName(){//DISEGNA LA DOMANDA SUL NOME DEI GIOCATORI
    setColor(0,BattleShipColor);
    gotoXY(78,38);
    cout<<"                                             ";
    gotoXY(78,38);
    cout<<a;
    resetColor();
    cout<<" GIOCATORE "<<turno;
    cout<<", come ti chiami? ";
    setColor(0,BattleShipColor);
    cout<<a;
    resetColor();
    gotoXY(95,39);
    cout<<"(max 5 lettere)";
}



void drawSettingDifficulty(int select){
    char ea=158;//Carattere ascii della crocetta

    for(int n=1;n<=3;n++){//Disegna tutte e tre le opzioni della difficolta'
        if(select==n){//Se sta disegnando l'opzione che e' selezionata al momento stampa la crocetta prima della difficolta'

            gotoXY(78,42+n*2);
            setColor(0,BattleShipColor);
            cout<<ea;
            setColor(0,15);//Imposta il colore bianco cosi' che il nome che stampera' dopo sara' bianco e spicchera' di piu' rispetto agli altri grigi
        }
        else{//In casoc ontrario stampa un carattere vuoto
            gotoXY(78,42+n*2);
            cout<<" ";
            setColor(0,8);//Imposta il colore grigio cosi' che il nome che stampera' dopo sara' grigio
        }
        gotoXY(80,42+n*2);
        switch(n){//A seconda di cosa sta stampando stampa il nome diverso
        case 1: cout<<"PASSEGGIATA"; break;
        case 2: cout<<"GUERRIGLIA";break;
        case 3: cout<<"LANCIATE L'ATOMICA, PRESTOOO!";break;
        }
    }
    resetColor();

}



void drawSettingPlayerFactions(int select){//DISEGNA LA DOMANDA SUL COLORE DEI GIOCATORI
    setColor(0,BattleShipColor);
    gotoXY(78,41);
    cout<<a;
    resetColor();
    cout<<" "<<stats[turno].name<<", scegli per quale nazione schierarti!";
    setColor(0,BattleShipColor);
    cout<<a;
    resetColor();
    gotoXY(95,42);cout<<"Utilizza le freccette!";

    int x=34; int y=46;
    int yp=0; int xp=0;
    int flag=0;
    ////SEQUENZA DISEGNO BANDIERA TEDESCA////Molto facile da fare visto e considerato che basta 1 for per le 3 striscie
    for(yp=0;yp<9;yp++){
        gotoXY(x+xp,y+yp);
        for(xp=0;xp<30;xp++){
            gotoXY(x+xp,y+yp);
            if(yp<=2){
                setColor(0,0);
            }
            if(yp>2&&yp<=5){
                setColor(15,15);
            }
            if(yp>5){
                setColor(12,12);
            }
            cout<<a;
        }
    }
    resetColor();
        gotoXY(x+3,y+11); cout<<"Deutsche Kaiserliche Marine";
        drawSettingFlagBorder(x,y,select,flag);
        flag++;

        x+=36;
    //////////////////////////////////////////
    ///SEQUENZA DISEGNO BANDIERA FRANCESE///Molto facile da fare visto e considerato che basta 1 for per le 3 striscie
    for(xp=0;xp<30;xp++){
        gotoXY(x+xp,y+yp);
        for(yp=0;yp<9;yp++){
            gotoXY(x+xp,y+yp);
            if(xp<=9){
                setColor(9,9);
            }
            if(xp>9&&yp<=19){
                setColor(15,15);
            }
            if(xp>19){
                setColor(12,12);
            }
            cout<<a;
        }
    }
    resetColor();
    gotoXY(x+7,y+11); cout<<"Marine Nationale";
    drawSettingFlagBorder(x,y,select,flag);
    flag++;

    x+=36;
    //////////////////////////////////////////
    ///SEQUENZA DISEGNO BANDIERA ITALIANA///Molto facile da fare visto e considerato che basta 1 for per le 3 striscie
    for(xp=0;xp<30;xp++){
        gotoXY(x+xp,y+yp);
        for(yp=0;yp<9;yp++){
            gotoXY(x+xp,y+yp);
            if(xp<=9){
                setColor(10,10);
            }
            if(xp>9&&yp<=19){
                setColor(15,15);
            }
            if(xp>19){
                setColor(12,12);
            }
            cout<<a;
        }
    }
    resetColor();
    gotoXY(x+5,y+11); cout<<"Regia Marina Italiana";
    drawSettingFlagBorder(x,y,select,flag);
    flag++;

    x+=36;
    //////////////////////////////////////////
    ///SEQUENZA DISEGNO BANDIERA INGLESE///Un casino.
    gotoXY(x,y);   setColor(12,12);cout<<"   "; setColor(15,15); cout<<"  "; setColor(1,1); cout<<"      "; setColor(15,15); cout<<"  ";   setColor(12,12); cout<<"    ";                           /*secondo quadrante*/setColor(15,15); cout<<"  ";setColor(1,1); cout<<"      ";setColor(15,15); cout<<"  ";setColor(12,12); cout<<"   ";
    gotoXY(x,y+1); setColor(1,1);  cout<<"  "; setColor(12,12); cout<<"   "; setColor(15,15); cout<<"  "; setColor(1,1); cout<<"    "; setColor(15,15); cout<<"  ";   setColor(12,12); cout<<"    ";  /*secondo quadrante*/setColor(15,15); cout<<"  ";setColor(1,1); cout<<"    ";setColor(15,15); cout<<"  ";setColor(12,12); cout<<"   ";setColor(1,1); cout<<"  ";
    gotoXY(x,y+2); setColor(1,1);  cout<<"    "; setColor(12,12); cout<<"   "; setColor(15,15); cout<<"  "; setColor(1,1); cout<<"  "; setColor(15,15); cout<<"  ";   setColor(12,12); cout<<"    ";  /*secondo quadrante*/setColor(15,15); cout<<"  ";setColor(1,1); cout<<"  ";setColor(15,15); cout<<"  ";setColor(12,12); cout<<"   ";setColor(1,1); cout<<"    ";
    gotoXY(x,y+3); setColor(15,15);cout<<"             "; setColor(12,12); cout<<"    "; setColor(15,15);cout<<"             ";
    gotoXY(x,y+4); setColor(12,12);cout<<"                              ";
    gotoXY(x,y+5); setColor(12,12);cout<<"                              ";
    gotoXY(x,y+6); setColor(15,15);cout<<"             "; setColor(12,12); cout<<"    "; setColor(15,15);cout<<"             ";
    gotoXY(x,y+7); setColor(1,1);  cout<<"   "; setColor(12,12);cout<<"    "; setColor(15,15); cout<<"  "; setColor(1,1); cout<<"  "; setColor(15,15); cout<<"  "; setColor(12,12); cout<<"    "; /*secondo quadrante*/  setColor(15,15); cout<<"  "; setColor(1,1); cout<<"  "; setColor(15,15); cout<<"  ";  setColor(12,12);cout<<"    "; setColor(1,1); cout<<"   ";
    gotoXY(x,y+8); setColor(1,1);  cout<<" "; setColor(12,12);cout<<"    "; setColor(15,15); cout<<"  "; setColor(1,1); cout<<"    "; setColor(15,15); cout<<"  "; setColor(12,12); cout<<"    "; /*secondo quadrante*/  setColor(15,15); cout<<"  "; setColor(1,1); cout<<"    "; setColor(15,15); cout<<"  ";  setColor(12,12);cout<<"    "; setColor(1,1); cout<<" ";


    resetColor();
    gotoXY(x+6,y+11); cout<<"British Royal Navy";
    drawSettingFlagBorder(x,y,select,flag);
    flag++;


    //////////////////////////////////////////
    resetColor();
}



void drawSettingFlagBorder(int x,int y,int select,int flag){//Disegna il bordo della bandiera a seconda di cosa e' selezionato al momento
    int yp,xp;
    /*
    La variabile flag mi indica di quale bandiera va stampato il bordino
    La variabile select mi indica quale bandiera e' selezionata al momento
    */

    if(faction[flag].player!=0){//Se la bandiera di cui si sta stampando il bordo e' stata scelta da un giocatore, la stampa col nome del giocatore e del suo colore
        setColor(stats[faction[flag].player].color,0);//Imposta il colore del giocatore
        for(yp=-1,xp=-1;yp<=9;yp++){//Stampa i bordi laterali
            gotoXY(x+xp,y+yp);
            cout<<ab;
            gotoXY(x+xp+31,y+yp);
            cout<<ab;
        }
        for(yp=-1,xp=-1;xp<=30;xp++){//Stampa i bordi verticali
            gotoXY(x+xp,y+yp);
            cout<<ba;
            gotoXY(x+xp,y+yp+10);
            cout<<ba;
        }
        ////stampa gli angoli////
        gotoXY(x-1,y-1);
        cout<<aa;
        gotoXY(x-1,y+9);
        cout<<ac;
        gotoXY(x+30,y-1);
        cout<<ca;
        gotoXY(x+30,y+9);
        cout<<cc;

        //Stampa il nome del giocatore che ha scelto quella bandiera
        gotoXY(x,y-1); setColor(stats[faction[flag].player].color, color[stats[faction[flag].player].color].contrast); cout<<stats[faction[flag].player].name;
    }


    else if(flag==select){//Se invece la bandiera e' quella selezionata al momento
        setColor(0,BattleShipColor);//Imposta il colore del tema del gioco
        for(yp=-1,xp=-1;yp<=9;yp++){//Stampa dei caratteri speciali come bordo orizzontale
            gotoXY(x+xp,y+yp);
            cout<<e;
            gotoXY(x+xp+31,y+yp);
            cout<<e;
        }
        for(yp=-1,xp=-1;xp<=30;xp++){//Stampa dei caratteri speciali come bordo verticale
            gotoXY(x+xp,y+yp);
            cout<<j;
            gotoXY(x+xp,y+yp+10);
            cout<<j;
        }

        ////Disegna gli angoli////
        gotoXY(x-1,y-1);
        cout<<f;
        gotoXY(x-1,y+9);
        cout<<h;
        gotoXY(x+30,y-1);
        cout<<g;
        gotoXY(x+30,y+9);
        cout<<i;
    }


    else{//Se invece e' un bordino bianco normalissimo
        setColor(0,15);
        for(yp=-1,xp=-1;yp<=9;yp++){
            gotoXY(x+xp,y+yp);
            cout<<ab;
            gotoXY(x+xp+31,y+yp);
            cout<<ab;
        }
        for(yp=-1,xp=-1;xp<=30;xp++){
            gotoXY(x+xp,y+yp);
            cout<<ba;
            gotoXY(x+xp,y+yp+10);
            cout<<ba;
        }
        gotoXY(x-1,y-1);
        cout<<aa;
        gotoXY(x-1,y+9);
        cout<<ac;
        gotoXY(x+30,y-1);
        cout<<ca;
        gotoXY(x+30,y+9);
        cout<<cc;
    }
}



void drawRemainingShips(int n){//Disegna le navi rimanenti sulla destra mentre vengono piazzate dal giocatore



    int y=4;
    int lenght=2;
    int ship;
    ////a seconda di quante navi sono state piazzate, deduce quale tipo di nave sta venendo piazzata in questo momento////
    if(n<=4){ship=1;}
    else if(n<=7){ship=2;}
    else if(n<=9){ship=3;}
    else if(n<=10){ship=4;}
    //////

    for(int sh=1;sh<=4;sh++,y+=3,lenght++){//La variabile sh indica la nave che si sta disegnando in questo momento. Il for arriva fino ad sh<=4 infatti. Vengono disegnate i 4 tipi di navi


        for(int x=1;x<=lenght;x++){//Disegna ogni casella della nave

            if(ship==sh){//Se il tipo di nave che sta disegnando è dello stesso tipo della nave che sta venendo piazzata

                setColor(stats[turno].color,0);

                gotoXY((x+12)*11,y*4);   cout<<"          ";
                gotoXY((x+12)*11,y*4+1); cout<<"          ";
                gotoXY((x+12)*11,y*4+2); cout<<"          ";
                gotoXY((x+12)*11,y*4+3); cout<<"          ";
                gotoXY((x+12)*11,y*4+4); cout<<"          ";


                if(x==1){//Se sta disegnando la prima casella della nave outputta certi bordi
                    setColor(0,stats[turno].color);
                    gotoXY((x+12)*11-1,y*4);   cout<<ab;
                    gotoXY((x+12)*11-1,y*4+1); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+2); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+3); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+4); cout<<ab;
                }
                else if(x==lenght){//Se sta disegnando l'ultima' casella della nave outputta certi bordi
                    setColor(0,stats[turno].color);
                    gotoXY((x+12)*11+10,y*4);    cout<<ab;
                    gotoXY((x+12)*11+10,y*4+1);  cout<<ab;
                    gotoXY((x+12)*11+10,y*4+2);  cout<<ab;
                    gotoXY((x+12)*11+10,y*4+3);  cout<<ab;
                    gotoXY((x+12)*11+10,y*4+4);  cout<<ab;
                    setColor(stats[turno].color,0);
                    gotoXY((x+12)*11-1,y*4);   cout<<ab;
                    gotoXY((x+12)*11-1,y*4+1); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+2); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+3); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+4); cout<<ab;
                }
                else{//Se sta disegnando tutto il resto delle caselle della nave outputta certi bordi
                    setColor(stats[turno].color,0);
                    gotoXY((x+12)*11-1,y*4);   cout<<ab;
                    gotoXY((x+12)*11-1,y*4+1); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+2); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+3); cout<<ab;
                    gotoXY((x+12)*11-1,y*4+4); cout<<ab;
                }
            }


            else{//Se invece il tipo di nave che sta disegnando NON è dello stesso tipo della nave che sta venendo piazzata
                setColor(0,stats[turno].color);
                gotoXY((x+12)*11-1,y*4);   cout<<ab; gotoXY((x+12)*11+10,y*4);  cout<<ab;
                gotoXY((x+12)*11-1,y*4+1); cout<<ab; gotoXY((x+12)*11+10,y*4+1);  cout<<ab;
                gotoXY((x+12)*11-1,y*4+2); cout<<ab; gotoXY((x+12)*11+10,y*4+2);  cout<<ab;
                gotoXY((x+12)*11-1,y*4+3); cout<<ab; gotoXY((x+12)*11+10,y*4+3);  cout<<ab;
                gotoXY((x+12)*11-1,y*4+4); cout<<ab; gotoXY((x+12)*11+10,y*4+4);  cout<<ab;
                resetColor();
                gotoXY((x+12)*11,y*4);   cout<<"          ";
                gotoXY((x+12)*11,y*4+1); cout<<"          ";
                gotoXY((x+12)*11,y*4+2); cout<<"          ";
                gotoXY((x+12)*11,y*4+3); cout<<"          ";
                gotoXY((x+12)*11,y*4+4); cout<<"          ";

            }
            if(x==1){
                setColor(0,stats[turno].color);
                gotoXY((x+12)*11-1,y*4-1); cout<<aa;
                gotoXY((x+12)*11-1,y*4+5); cout<<ac;
            }

            else if(x==lenght){
                setColor(0,stats[turno].color);
                gotoXY((x+12)*11+10,y*4-1); cout<<ca;
                gotoXY((x+12)*11+10,y*4+5); cout<<cc;
                gotoXY((x+12)*11-1,y*4-1); cout<<da;
                gotoXY((x+12)*11-1,y*4+5); cout<<de;
            }
            else{
                setColor(0,stats[turno].color);
                gotoXY((x+12)*11-1,y*4-1); cout<<da;
                gotoXY((x+12)*11-1,y*4+5); cout<<de;

            }
            setColor(0,stats[turno].color);
            gotoXY((x+12)*11,y*4-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
            gotoXY((x+12)*11,y*4+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;

        }

        //////DISEGNA IL NOME DEL TIPO DELLA BARAC E IL CONTATORE DI QUANTE NE RIMANGONO///////////////////
        if(ship==sh){setColor(0,15);}
        else{setColor(0,8);}
        switch(sh){
            case 1: gotoXY((12)*11-3,y*4+1); cout<<"INCROCIATORE";  break;
            case 2: gotoXY((12)*11-3,y*4+1); cout<<"SOTTOMARINO";  break;
            case 3: gotoXY((12)*11-2,y*4+1); cout<<"CORAZZATA";  break;
            case 4: gotoXY((12)*11-2,y*4+1); cout<<"PORTAEREI";  break;
        }
        gotoXY((12)*11+2,y*4+2); cout<<"X";
        switch(sh){

        case 1:
            if(n<=4){
                cout<<4-n+1;
            }
            else{
                cout<<"0";
            }
            break;

        case 2:
            if(n>4&&n<=7){
                cout<<3-(n-4)+1;
            }
            else if(n<=4){
                cout<<"3";
            }
            else{
                cout<<"0";
            }
            break;
        case 3:
            if(n>7&&n<=9){
                cout<<2-(n-7)+1;
            }
            else if(n<=7){
                cout<<"2";
            }
            else{
                cout<<"0";
            }
            break;
        case 4:
            if(n<=10){
                cout<<"1";
            }
            else{
                cout<<"0";
            }
            break;
        }
        /////////////////////////////////
    }
}



void drawTabell(){//Difficile. DIsegna il tabellone del/dei giocatori. Si adatta in base alle esigenze del momento
    /*
    Per fare un esempio, se l'algoritmo rileva che siamo nella fase di piazzamento delle navi, disegna solo il campo di gioco del giocatore che ha il turno,+
    e non stampa quello del nemico.
    Inoltre le navi le colora di colore grigio.
    In fase di gioco invece, stampa prima il campo nemico e poi quello amico. E sul campo nemico fa apparire solo le navi che sono state colpite ovviamente.
    Il tutto con i colori dei rispettivi GIOCATORI
    */
    int yp=0;
    int player=enemy(); //La variabile player indica il giocatore del quale si sta trattando durante la stampa del tabellone, e di base e' il nemico
    for(int n=1;n<=2;n++,yp+=11){//Questo for stampa 2 tabelloni (in certi casi): quello nemico e quello amico
        //Dopo lo stampaggio della prima tabella viene incrementato yp che viene sommato alla y di ogni carattere cosi' da stampare la 2a tabella sotto la 1a
        if(n==2){//Se sta stampando il secondo tabellone, imposta player su quella del giocatore che ha il turno. Così che viene stampato il tabellone di quel giocatore
            player=turno;
        }
        if(positioning==true){//Se rileva che si stanno posizionando le navi, stampa direttamente il tabellone 2 (quello delg iocatore che ah il turno) perche' il giocatore non ha bisogno di vedere il campo nemico per piazzare le navi
            n++;
            player=turno;
        }



        for(int y=1;y<=10;y++){//Questo for scende di riga in riga
            for(int x=1;x<=10;x++){//Questo for e' per i caratteri sulla riga

                ////////////FASE DI DISEGNO DELLE ETICHETTE A BORDO TABELLONE (1,2,3,4....A,B,C,D,E.)
                setColor(0,stats[player].color);
                //////////etichette orizzontali in cima////////
                if(y==1){
                    if(x==1){
                        gotoXY(x*11-1,(y+yp)*6-3); cout<<aa; gotoXY(x*11+10,(y+yp)*6-3); cout<<da;//Bordi speciali per l'etichetta in alto a sinistra

                        gotoXY(x*11-1,(y+yp)*6-4); resetColor(); cout<<"                            ";gotoXY(x*11-1,(y+yp)*6-4); setColor(stats[player].color,0);
                        switch (n){//In base al tabellone che sta disegnando stampa se sono acque nemiche o amiche ed il nome del proprietario di quel campo
                        case 1:
                            cout<<"ACQUE NEMICHE (";
                            break;
                        case 2:
                            cout<<"ACQUE AMICHE (";
                            break;
                        }
                        cout<<stats[player].name<<")";
                        setColor(0,stats[player].color);
                    }
                    else if(x==10){
                        gotoXY(x*11+10,(y+yp)*6-3); cout<<ca;//Bordi speciali per l'etichetta in alto a destra
                    }
                    else{
                        gotoXY(x*11+10,(y+yp)*6-3); cout<<da;//Bordi speciali per il resto delle etichette
                    }
                    gotoXY(x*11,(y+yp)*6-3); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;//Stampa il sopra delle etichette

                    gotoXY(x*11-1,(y+yp)*6-2);cout<<ab;   gotoXY(x*11+4,(y+yp)*6-2);cout<<x;    gotoXY(x*11+10,(y+yp)*6-2); cout<<ab;
                }
                ///////////////////////////////////////////////
                ////////etichette verticali a sinistra////////
                if(x==1){
                    if(y==1){
                        gotoXY(x*11-5,(y+yp)*6-1); cout<<aa; gotoXY(x*11-5,(y+yp)*6+5); cout<<dd;
                    }
                    else if(y==10){
                        gotoXY(x*11-5,(y+yp)*6+5); cout<<ac;
                    }
                    else{
                        gotoXY(x*11-5,(y+yp)*6+5); cout<<dd;
                    }

                    gotoXY(x*11-4,(y+yp)*6-1); cout<<ba<<ba<<ba;
                    gotoXY(x*11-4,(y+yp)*6+5); cout<<ba<<ba<<ba;

                    for(int n=0;n<=4;n++){
                        gotoXY(x*11-5,(y+yp)*6+n); cout<<ab;
                        if(n==2){
                            gotoXY(x*11-3,(y+yp)*6+n); cout<<numberToLetter(y);
                        }
                    }
                }
                //////////////////////////////////////////////////
                ///////////stesso principio di prima
                if(x==10){
                    if(y==10){
                        gotoXY(x*11+10,(y+yp)*6+5); cout<<cc;
                    }
                    else{
                        gotoXY(x*11+10,(y+yp)*6-1); cout<<db;    gotoXY(x*11+10,(y+yp)*6+5); cout<<db;
                    }
                }
                else if(y==10){
                    if(x==10){
                        gotoXY(x*11+10,(y+yp)*6+5); cout<<cc;
                    }
                    else{
                        gotoXY(x*11-1,(y+yp)*6+5); cout<<de;    gotoXY(x*11+10,(y+yp)*6+5); cout<<de;
                    }
                }
                else{
                    gotoXY(x*11-1,(y+yp)*6-1); cout<<dc;    gotoXY(x*11+10,(y+yp)*6-1); cout<<dc;
                    gotoXY(x*11-1,(y+yp)*6+5); cout<<dc;    gotoXY(x*11+10,(y+yp)*6+5); cout<<dc;
                }
                //////////////////////////////////////////////////////////////////////////////////////


                ////////Sezione importante. Stampaggio della tabella vera e propria////////

                if(tabell[player][x][y].which_ship!=0){//Se la casella che sta disegnando contiene una nave
                    if (positioning==true){//Se sto piazzando le navi setto il colore su grigio
                        setColor(8,0);
                    }
                    else if(n==1&&tabell[player][x][y].hit==true){//Se sto disegnando la tabella nemica e la casella e' stata colpita setta il colore del nemico. Questo perche' del nemico io posso vedere solo le navi colpite
                        setColor(stats[player].color,0);
                    }
                    else if(n==2){//Se sta stampando il tabellone alleato imposta il colore del giocatore che ha il turno
                        setColor(stats[player].color,0);
                    }
                    else{//Senno' non setta niente
                        setColor(0,0);
                    }
                     //////Qua viene riempita la casella////////
                    gotoXY(x*11,(y+yp)*6);   cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+1); cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+2); cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+3); cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+4); cout<<"          ";

                    gotoXY(x*11+3,(y+yp)*6+4);
                    switch(tabell[player][x][y].which_ship){
                    case 1:
                        cout<<"INC";
                    break;
                    case 2:
                        cout<<"SOT";
                    break;
                    case 3:
                        cout<<"COR";
                    break;
                    case 4:
                        cout<<"POR";
                    break;
                    }

                    if(tabell[player][x][y].horizontal==true){//Se la nave nella casella e' messa per orizzontale. Stampa tutti i bordi verticali in base a certe regole
                        if(tabell[player][x][y].first.x==x){//Se e' la prima casella della nave
                        setColor(0,stats[player].color);
                        gotoXY(x*11-1,(y+yp)*6);   cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+1); cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+2); cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+3); cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+4); cout<<ab;
                        }
                        else if(tabell[player][x][y].last==x){//Se e' l'ultima casella della nave
                            setColor(0,stats[player].color);
                            gotoXY(x*11+10,(y+yp)*6);    cout<<ab;
                            gotoXY(x*11+10,(y+yp)*6+1);  cout<<ab;
                            gotoXY(x*11+10,(y+yp)*6+2);  cout<<ab;
                            gotoXY(x*11+10,(y+yp)*6+3);  cout<<ab;
                            gotoXY(x*11+10,(y+yp)*6+4);  cout<<ab;
                            if (positioning==true){
                                setColor(8,0);
                            }
                            else if(n==1&&tabell[player][x-1][y].hit==true&&tabell[player][x][y].hit==true){//Se sta stampando il campo nemico e la casella e' colpita ed ha una casella consecuitive prima di se' colpita a sua volta, disegna il separatorio verticale colorato, cosi' da unire le due caselle
                                setColor(stats[player].color,0);
                            }
                            else if(n==2){//Se sta stampando il giocatore che ha il turno, viene stampato direttamente il bordino di un certo colore cosi' da rendere le caselle comunicanti
                                setColor(stats[player].color,0);
                            }
                            else{//Senno' le stampa su sfondo nero
                                setColor(0,stats[player].color);
                            }
                            gotoXY(x*11-1,(y+yp)*6);   cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+1); cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+2); cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+3); cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+4); cout<<ab;
                        }
                        else{//Se e' il resto delle caselle nel mezzo
                            //////stesso principio di prima////
                            if (positioning==true){
                                setColor(8,0);
                            }
                            else if(n==1&&tabell[player][x-1][y].hit==true&&tabell[player][x][y].hit==true){
                                setColor(stats[player].color,0);
                            }
                            else if(n==2){
                                setColor(stats[player].color,0);
                            }
                            else{
                                setColor(0,stats[player].color);
                            }

                            gotoXY(x*11-1,(y+yp)*6);   cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+1); cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+2); cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+3); cout<<ab;
                            gotoXY(x*11-1,(y+yp)*6+4); cout<<ab;
                        }
                        setColor(0,stats[player].color);
                        gotoXY(x*11,(y+yp)*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                        gotoXY(x*11,(y+yp)*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                    }
                    else{//Se invece la casella fa parte di una nave disposta per verticale
                        //////stesso principio/////
                        if(tabell[player][x][y].first.y==y){//Se e' la prima casella della nave
                            setColor(0,stats[player].color);
                            gotoXY(x*11,(y+yp)*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                        }
                        else if(tabell[player][x][y].last==y){//Se e' l'ultima casella della nave
                            setColor(0,stats[player].color);
                            gotoXY(x*11,(y+yp)*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                            if (positioning==true){
                                setColor(8,0);
                            }
                            else if(n==1&&tabell[player][x][y-1].hit==true&&tabell[player][x][y].hit==true){
                                setColor(stats[player].color,0);
                            }
                            else if(n==2){
                                setColor(stats[player].color,0);
                            }
                            else{
                                setColor(0,stats[player].color);
                            }
                            gotoXY(x*11,(y+yp)*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                        }
                        else{//Se e' il resto delle caselle nel mezzo
                            if (positioning==true){
                                setColor(8,0);
                            }
                            else if(n==1&&tabell[player][x][y-1].hit==true&&tabell[player][x][y].hit==true){
                                setColor(stats[player].color,0);
                            }
                            else if(n==2){
                                setColor(stats[player].color,0);
                            }
                            else{
                                setColor(0,stats[player].color);
                            }
                            gotoXY(x*11,(y+yp)*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                        }
                        setColor(0,stats[player].color);
                        gotoXY(x*11-1,(y+yp)*6);   cout<<ab;  gotoXY(x*11+10,(y+yp)*6);    cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+1); cout<<ab;  gotoXY(x*11+10,(y+yp)*6+1);  cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+2); cout<<ab;  gotoXY(x*11+10,(y+yp)*6+2);  cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+3); cout<<ab;  gotoXY(x*11+10,(y+yp)*6+3);  cout<<ab;
                        gotoXY(x*11-1,(y+yp)*6+4); cout<<ab;  gotoXY(x*11+10,(y+yp)*6+4);  cout<<ab;
                    }
                }
                else{//Se invece la casella e' vuota, la stampa vuota
                    setColor(0,stats[player].color);
                    gotoXY(x*11,(y+yp)*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                    gotoXY(x*11,(y+yp)*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                    gotoXY(x*11-1,(y+yp)*6);   cout<<ab; gotoXY(x*11+10,(y+yp)*6);  cout<<ab;
                    gotoXY(x*11-1,(y+yp)*6+1); cout<<ab; gotoXY(x*11+10,(y+yp)*6+1);  cout<<ab;
                    gotoXY(x*11-1,(y+yp)*6+2); cout<<ab; gotoXY(x*11+10,(y+yp)*6+2);  cout<<ab;
                    gotoXY(x*11-1,(y+yp)*6+3); cout<<ab; gotoXY(x*11+10,(y+yp)*6+3);  cout<<ab;
                    gotoXY(x*11-1,(y+yp)*6+4); cout<<ab; gotoXY(x*11+10,(y+yp)*6+4);  cout<<ab;
                    resetColor();
                    gotoXY(x*11,(y+yp)*6);   cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+1); cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+2); cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+3); cout<<"          ";
                    gotoXY(x*11,(y+yp)*6+4); cout<<"          ";
                }
                //////////////////////////////////////////////////////////////
                ////////SEQUENZA DISEGNO CROCE SULLE CASELLE COLPITE//////////
                if(tabell[player][x][y].hit==true){//Se la casella e' stata colpita
                    if(tabell[player][x][y].which_ship!=0){//Se la casella colpita contiene una nave imposta il colore sul rosso
                        setColor(4,0);
                    }
                    else{//sENNO' SUL GRIGIO
                        setColor(8,0);
                    }
                    gotoXY(x*11,(y+yp)*6);   cout<<"  "; gotoXY(x*11+8,(y+yp)*6);   cout<<"  ";
                    gotoXY(x*11+2,(y+yp)*6+1); cout<<"  "; gotoXY(x*11+6,(y+yp)*6+1);   cout<<"  ";
                    gotoXY(x*11+4,(y+yp)*6+2); cout<<"  ";
                    gotoXY(x*11+2,(y+yp)*6+3); cout<<"  "; gotoXY(x*11+6,(y+yp)*6+3);   cout<<"  ";
                    gotoXY(x*11,(y+yp)*6+4); cout<<"  "; gotoXY(x*11+8,(y+yp)*6+4);   cout<<"  ";
                }
                ////////////////////////////////////////////////////////////////
            }
        }
    }
    resetColor();
}



void drawSelection(int xs,int ys,int lenght,bool horizontal){//Disegna la nave che si sta spostando
    bool no_space=false;//Se questa variabile e' true, la nave assume un colore rosso scuro indicando che non si può piazzare

    for(int n1=0;n1<lenght;n1++){//Controlla tutte le caselle della nave che si sta spostando se non sono in conflitto con navi gia' piazzate in precedenza
        if(horizontal==true){
            if(tabell[turno][xs+n1][ys].which_ship!=0){
                no_space=true;
            }
        }
        else{
            if(tabell[turno][xs][ys+n1].which_ship!=0){
                no_space=true;
            }
        }
    }


    for(int n1=0;n1<lenght;n1++){//Questo for stampa tutte le caselle
        if(no_space==true){setColor(4,0);}//Se non c'e' spazio per la nave imposta il colore sul rosso
        else{setColor(stats[turno].color,0);}//Senno' del colore del giocatore


        if(horizontal==true){//Se la nave e' orizzontale
            if(n1!=0){//Se non e' la prima casella
                gotoXY((xs+n1)*11-1,ys*6);   cout<<ab;
                gotoXY((xs+n1)*11-1,ys*6+1); cout<<ab;
                gotoXY((xs+n1)*11-1,ys*6+2); cout<<ab;
                gotoXY((xs+n1)*11-1,ys*6+3); cout<<ab;
                gotoXY((xs+n1)*11-1,ys*6+4); cout<<ab;
            }
            if(tabell[turno][xs+n1][ys].which_ship!=0){//Se in questa casella e' gia' presente yìuna di un'altra nave, stampa la texture che fa capire che questa casella e' in conflitto
                setColor(8,4);
                gotoXY((xs+n1)*11,ys*6);   cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY((xs+n1)*11,ys*6+1); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY((xs+n1)*11,ys*6+2); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY((xs+n1)*11,ys*6+3); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY((xs+n1)*11,ys*6+4); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
            }
            else{//Se invece la casella e' libera
                if(no_space==true){//Se non c'è spazio la riempie di rosso
                    setColor(4,0);
                }
                else{//Senno' del colore del giocatore
                    setColor(stats[turno].color,0);
                }
                gotoXY((xs+n1)*11,ys*6);   cout<<"          ";
                gotoXY((xs+n1)*11,ys*6+1); cout<<"          ";
                gotoXY((xs+n1)*11,ys*6+2); cout<<"          ";
                gotoXY((xs+n1)*11,ys*6+3); cout<<"          ";
                gotoXY((xs+n1)*11,ys*6+4); cout<<"          ";
            }
        }
        else{//Se invece e' verticale
            ////////stesso principio//////
            if(n1!=0){
                gotoXY(xs*11,(ys+n1)*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
            }
            if(tabell[turno][xs][ys+n1].which_ship!=0){
                setColor(8,4);
                gotoXY(xs*11,(ys+n1)*6);   cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY(xs*11,(ys+n1)*6+1); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY(xs*11,(ys+n1)*6+2); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY(xs*11,(ys+n1)*6+3); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
                gotoXY(xs*11,(ys+n1)*6+4); cout<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b;
            }
            else{
                if(no_space==true){
                    setColor(4,0);
                }
                else{
                    setColor(stats[turno].color,0);
                }
                gotoXY(xs*11,(ys+n1)*6);   cout<<"          ";
                gotoXY(xs*11,(ys+n1)*6+1); cout<<"          ";
                gotoXY(xs*11,(ys+n1)*6+2); cout<<"          ";
                gotoXY(xs*11,(ys+n1)*6+3); cout<<"          ";
                gotoXY(xs*11,(ys+n1)*6+4); cout<<"          ";
            }
        }
        resetColor();
    }

}



void deleteSelection(int xs,int ys,int lenght,bool horizontal){////Cancella le caselle della nave che si sta piazzando con le caselle del tabellone
    for(int n=0;n<lenght;n++){



        if(tabell[turno][xs][ys].which_ship!=0){
            setColor(8,0);



            gotoXY(xs*11,ys*6);   cout<<"          ";
            gotoXY(xs*11,ys*6+1); cout<<"          ";
            gotoXY(xs*11,ys*6+2); cout<<"          ";
            gotoXY(xs*11,ys*6+3); cout<<"          ";
            gotoXY(xs*11,ys*6+4); cout<<"          ";

            gotoXY(xs*11+3,ys*6+4);
            switch(tabell[turno][xs][ys].which_ship){
            case 1:
                cout<<"INC";
            break;
            case 2:
                cout<<"SOT";
            break;
            case 3:
                cout<<"COR";
            break;
            case 4:
                cout<<"POR";
            break;
            }

            if(tabell[turno][xs][ys].horizontal==true){
                if(tabell[turno][xs][ys].first.x==xs){//Se e' la prima casella della nave
                setColor(0,stats[turno].color);
                gotoXY(xs*11-1,ys*6);   cout<<ab;
                gotoXY(xs*11-1,ys*6+1); cout<<ab;
                gotoXY(xs*11-1,ys*6+2); cout<<ab;
                gotoXY(xs*11-1,ys*6+3); cout<<ab;
                gotoXY(xs*11-1,ys*6+4); cout<<ab;
                }
                else if(tabell[turno][xs][ys].last==xs){//Se e' l'ultima casella della nave
                    setColor(0,stats[turno].color);
                    gotoXY(xs*11+10,ys*6);    cout<<ab;
                    gotoXY(xs*11+10,ys*6+1);  cout<<ab;
                    gotoXY(xs*11+10,ys*6+2);  cout<<ab;
                    gotoXY(xs*11+10,ys*6+3);  cout<<ab;
                    gotoXY(xs*11+10,ys*6+4);  cout<<ab;

                    setColor(8,0);
                    gotoXY(xs*11-1,ys*6);   cout<<ab;
                    gotoXY(xs*11-1,ys*6+1); cout<<ab;
                    gotoXY(xs*11-1,ys*6+2); cout<<ab;
                    gotoXY(xs*11-1,ys*6+3); cout<<ab;
                    gotoXY(xs*11-1,ys*6+4); cout<<ab;
                }
                else{//Se e' il resto delle caselle nel mezzo

                    setColor(8,0);
                    gotoXY(xs*11-1,ys*6);   cout<<ab;
                    gotoXY(xs*11-1,ys*6+1); cout<<ab;
                    gotoXY(xs*11-1,ys*6+2); cout<<ab;
                    gotoXY(xs*11-1,ys*6+3); cout<<ab;
                    gotoXY(xs*11-1,ys*6+4); cout<<ab;
                }
                setColor(0,stats[turno].color);
                gotoXY(xs*11,ys*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
            }
            else{//Se invece e' verticale
                if(tabell[turno][xs][ys].first.y==ys){//Se e' la prima casella della nave
                    setColor(0,stats[turno].color);
                    gotoXY(xs*11,ys*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                }
                else if(tabell[turno][xs][ys].last==ys){//Se e' l'ultima casella della nave
                    setColor(0,stats[turno].color);
                    gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;

                    setColor(8,0);
                    gotoXY(xs*11,ys*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                }
                else{//Se e' il resto delle caselle nel mezzo
                    setColor(8,0);
                    gotoXY(xs*11,ys*6-1); cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                }
                setColor(0,stats[turno].color);
                gotoXY(xs*11-1,ys*6);   cout<<ab;  gotoXY(xs*11+10,ys*6);    cout<<ab;
                gotoXY(xs*11-1,ys*6+1); cout<<ab;  gotoXY(xs*11+10,ys*6+1);  cout<<ab;
                gotoXY(xs*11-1,ys*6+2); cout<<ab;  gotoXY(xs*11+10,ys*6+2);  cout<<ab;
                gotoXY(xs*11-1,ys*6+3); cout<<ab;  gotoXY(xs*11+10,ys*6+3);  cout<<ab;
                gotoXY(xs*11-1,ys*6+4); cout<<ab;  gotoXY(xs*11+10,ys*6+4);  cout<<ab;
            }
        }
        else{//Se invece la casella e' vuota la stampa vuota
            setColor(0,stats[turno].color);
            gotoXY(xs*11,ys*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
            gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
            gotoXY(xs*11-1,ys*6);   cout<<ab; gotoXY(xs*11+10,ys*6);  cout<<ab;
            gotoXY(xs*11-1,ys*6+1); cout<<ab; gotoXY(xs*11+10,ys*6+1);  cout<<ab;
            gotoXY(xs*11-1,ys*6+2); cout<<ab; gotoXY(xs*11+10,ys*6+2);  cout<<ab;
            gotoXY(xs*11-1,ys*6+3); cout<<ab; gotoXY(xs*11+10,ys*6+3);  cout<<ab;
            gotoXY(xs*11-1,ys*6+4); cout<<ab; gotoXY(xs*11+10,ys*6+4);  cout<<ab;
            resetColor();
            gotoXY(xs*11,ys*6);   cout<<"          ";
            gotoXY(xs*11,ys*6+1); cout<<"          ";
            gotoXY(xs*11,ys*6+2); cout<<"          ";
            gotoXY(xs*11,ys*6+3); cout<<"          ";
            gotoXY(xs*11,ys*6+4); cout<<"          ";
        }

        if(tabell[turno][xs][ys].hit==true){//Se la casella e' stata colpita
            if(tabell[turno][xs][ys].which_ship!=0){//Se la casella colpita contiene una nave imposta il colore sul rosso
                setColor(4,0);
            }
            else{//sENNO' SUL GRIGIO
                setColor(8,0);
            }
            gotoXY(xs*11,ys*6);   cout<<"  "; gotoXY(xs*11+8,ys*6);   cout<<"  ";
            gotoXY(xs*11+2,ys*6+1); cout<<"  "; gotoXY(xs*11+6,ys*6+1);   cout<<"  ";
            gotoXY(xs*11+4,ys*6+2); cout<<"  ";
            gotoXY(xs*11+2,ys*6+3); cout<<"  "; gotoXY(xs*11+6,ys*6+3);   cout<<"  ";
            gotoXY(xs*11,ys*6+4); cout<<"  "; gotoXY(xs*11+8,ys*6+4);   cout<<"  ";
        }

        if(horizontal==true){
            xs++;
        }
        else{
            ys++;
        }
    }
}



void drawTurn(){//Disegan tutte le informazioni sul turno corrente a seconda della faee del gioco. Deducibile dai vari bool
    if (positioning==false){
        gotoXY(173,19); setColor(0,stats[turno].color); cout<<"-"; resetColor(); cout<<" Colpi rimanenti: "; setColor(0,stats[turno].color); cout<<maxcolpi-stats[turno].colpi;
        gotoXY(173,20); setColor(0,stats[turno].color); cout<<"-"; resetColor(); cout<<" Colpi massimi questo turno: "; setColor(0,stats[turno].color); cout<<stats[turno].ships;
        gotoXY(173,21); setColor(0,stats[turno].color); cout<<"-"; resetColor(); cout<<" Colpi a segno: "; setColor(0,stats[turno].color); cout<<stats[turno].colpisegno;
        gotoXY(173,22); setColor(0,stats[turno].color); cout<<"-"; resetColor(); cout<<" Colpi a vuoto: "; setColor(0,stats[turno].color); cout<<stats[turno].colpivuoto;
        if(settingCoord==true){
            int x=130; int y=3;
            gotoXY(x,y);resetColor(); cout<<"                                                           ";gotoXY(x,y); setColor(stats[turno].color,color[stats[turno].color].contrast); cout<<stats[turno].name<<", PIAZZA LE NAVI!";
            gotoXY(x+10,y+2); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"Frecce";resetColor(); cout<<" per MUOVERE il SELETTORE!";
            gotoXY(x+10,y+4); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"'V'";resetColor(); cout<<" per MODIFICARE le ultime coordinate inserite!";
            gotoXY(x+10,y+6); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"Spazio";resetColor(); cout<<" per AGGIUNGERE la COORDINATA!";
            gotoXY(x+10,y+8); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"'X'";resetColor(); cout<<" per LANCIARE l' ATTACCO!";
        }
    }
    else{
        int x=130; int y=3;
        gotoXY(x,y);resetColor(); cout<<"                                                           ";gotoXY(x,y); setColor(stats[turno].color,color[stats[turno].color].contrast); cout<<stats[turno].name<<", PIAZZA LE NAVI!";
        gotoXY(x+10,y+2); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"Frecce";resetColor(); cout<<" per MUOVERE la nave!";
        gotoXY(x+10,y+4); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"'V'";resetColor(); cout<<" per RUOTARE la nave!";
        gotoXY(x+10,y+6); resetColor(); cout<<"- "; setColor(0,stats[turno].color); cout<<"Spazio";resetColor(); cout<<" per PIAZZARE la nave!";

    }
    resetColor();
}



void drawChooseAction(int select){//Stampa le scelte sul da farsi
    char ea=158;

    for(int n=1;n<=2;n++){
        if(select==n){
            gotoXY(129,19+n*2);
            setColor(0,stats[turno].color);
            cout<<ea;
            setColor(0,15);
        }
        else{
            gotoXY(129,19+n*2);
            cout<<" ";
            setColor(0,20);
        }
        gotoXY(131,19+n*2);
        switch(n){
            case 1: cout<<"Definisci coordinate attacchi"; break;
            case 2: cout<<"Cedi il turno";break;
        }
    }
    resetColor();
}



void drawCoord(int xs,int ys,int n){//Stampa il segnalino della coordinata che si sta spostando
    setColor(14,0);
    gotoXY(xs*11,ys*6); cout<<"   "; gotoXY(xs*11+7,ys*6); cout<<"   ";gotoXY(xs*11+7,ys*6); cout<<numberToLetter(ys)<<xs;
    gotoXY(xs*11,ys*6+1); cout<<" "; gotoXY(xs*11+9,ys*6+1); cout<<" ";
    gotoXY(xs*11,ys*6+3); cout<<" "; gotoXY(xs*11+9,ys*6+3); cout<<" ";
    gotoXY(xs*11,ys*6+4); cout<<"   "; gotoXY(xs*11+7,ys*6+4); cout<<"   ";
    gotoXY(xs*11+2,ys*6); cout<<n;
    resetColor();

}

void drawPlaceCoord(int xs,int ys,int n){//Stampa il segnalino della coordinata piazzata
    setColor(stats[turno].color,0);
    gotoXY(xs*11,ys*6); cout<<"   "; gotoXY(xs*11+7,ys*6); cout<<"   ";
    gotoXY(xs*11,ys*6+1); cout<<" "; gotoXY(xs*11+9,ys*6+1); cout<<" ";
    gotoXY(xs*11,ys*6+3); cout<<" "; gotoXY(xs*11+9,ys*6+3); cout<<" ";
    gotoXY(xs*11,ys*6+4); cout<<"   "; gotoXY(xs*11+7,ys*6+4); cout<<"   ";
    gotoXY(xs*11+2,ys*6); cout<<n;
    resetColor();
}


void deleteCoord(int xs,int ys){//Cancella il seganlino della coordinata e lo sostituisce con il tabellone. Il codice e' pressochè identico a drawTabell (solo che e' ottimizzato per dover stampare solo una casella.)
    if(tabell[enemy()][xs][ys].which_ship!=0){
        if(tabell[enemy()][xs][ys].hit==true){
            setColor(stats[enemy()].color,0);
        }
        else{
            setColor(0,0);
        }

        gotoXY(xs*11,ys*6);   cout<<"          ";
        gotoXY(xs*11,ys*6+1); cout<<"          ";
        gotoXY(xs*11,ys*6+2); cout<<"          ";
        gotoXY(xs*11,ys*6+3); cout<<"          ";
        gotoXY(xs*11,ys*6+4); cout<<"          ";

        gotoXY(xs*11+3,ys*6+4);
        switch(tabell[turno][xs][ys].which_ship){
        case 1:
            cout<<"INC";
        break;
        case 2:
            cout<<"SOT";
        break;
        case 3:
            cout<<"COR";
        break;
        case 4:
            cout<<"POR";
        break;
        }

        if(tabell[enemy()][xs][ys].horizontal==true){
            if(tabell[enemy()][xs][ys].first.x==xs){//la prima
                setColor(0,stats[enemy()].color);
                gotoXY(xs*11-1,ys*6);   cout<<ab;
                gotoXY(xs*11-1,ys*6+1); cout<<ab;
                gotoXY(xs*11-1,ys*6+2); cout<<ab;
                gotoXY(xs*11-1,ys*6+3); cout<<ab;
                gotoXY(xs*11-1,ys*6+4); cout<<ab;

                if(tabell[enemy()][xs+1][ys].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs+1][ys].first.x==tabell[enemy()][xs][ys].first.x){
                    setColor(stats[enemy()].color,0);
                    gotoXY(xs*11+10,ys*6);    cout<<ab;
                    gotoXY(xs*11+10,ys*6+1);  cout<<ab;
                    gotoXY(xs*11+10,ys*6+2);  cout<<ab;
                    gotoXY(xs*11+10,ys*6+3);  cout<<ab;
                    gotoXY(xs*11+10,ys*6+4);  cout<<ab;
                }
            }

            else if(tabell[enemy()][xs][ys].last==xs){//l'ultima
                setColor(0,stats[enemy()].color);
                gotoXY(xs*11+10,ys*6);    cout<<ab;
                gotoXY(xs*11+10,ys*6+1);  cout<<ab;
                gotoXY(xs*11+10,ys*6+2);  cout<<ab;
                gotoXY(xs*11+10,ys*6+3);  cout<<ab;
                gotoXY(xs*11+10,ys*6+4);  cout<<ab;
                if(tabell[enemy()][xs-1][ys].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs-1][ys].first.x==tabell[enemy()][xs][ys].first.x){
                    setColor(stats[enemy()].color,0);
                }
                else{
                    setColor(0,stats[enemy()].color);
                }
                gotoXY(xs*11-1,ys*6);   cout<<ab;
                gotoXY(xs*11-1,ys*6+1); cout<<ab;
                gotoXY(xs*11-1,ys*6+2); cout<<ab;
                gotoXY(xs*11-1,ys*6+3); cout<<ab;
                gotoXY(xs*11-1,ys*6+4); cout<<ab;
            }
            else{//quelle nel mezzo
                if(tabell[enemy()][xs-1][ys].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs-1][ys].first.x==tabell[enemy()][xs][ys].first.x){
                    setColor(stats[enemy()].color,0);
                }
                else{
                    setColor(0,stats[enemy()].color);
                }
                gotoXY(xs*11-1,ys*6);   cout<<ab;
                gotoXY(xs*11-1,ys*6+1); cout<<ab;
                gotoXY(xs*11-1,ys*6+2); cout<<ab;
                gotoXY(xs*11-1,ys*6+3); cout<<ab;
                gotoXY(xs*11-1,ys*6+4); cout<<ab;

                if(tabell[enemy()][xs+1][ys].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs+1][ys].first.x==tabell[enemy()][xs][ys].first.x){
                    setColor(stats[enemy()].color,0);
                }
                else{
                    setColor(0,stats[enemy()].color);
                }
                gotoXY(xs*11+10,ys*6);    cout<<ab;
                gotoXY(xs*11+10,ys*6+1);  cout<<ab;
                gotoXY(xs*11+10,ys*6+2);  cout<<ab;
                gotoXY(xs*11+10,ys*6+3);  cout<<ab;
                gotoXY(xs*11+10,ys*6+4);  cout<<ab;

            }

        }
        else{
            if(tabell[enemy()][xs][ys].first.y==ys){//la prima
                setColor(0,stats[enemy()].color);
                gotoXY(xs*11,ys*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;

                if(tabell[enemy()][xs][ys+1].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs][ys+1].first.y==tabell[enemy()][xs][ys].first.y){
                    setColor(stats[enemy()].color,0);
                    gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                }
            }

            else if(tabell[enemy()][xs][ys].last==ys){//l'ultima
                setColor(0,stats[enemy()].color);
                gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
                if(tabell[enemy()][xs][ys-1].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs][ys-1].first.y==tabell[enemy()][xs][ys].first.y){
                    setColor(stats[enemy()].color,0);
                }
                else{
                    setColor(0,stats[enemy()].color);
                }
                gotoXY(xs*11,ys*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
            }
            else{//quelle nel mezzo
                if(tabell[enemy()][xs][ys-1].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs][ys-1].first.y==tabell[enemy()][xs][ys].first.y){
                    setColor(stats[enemy()].color,0);
                }
                else{
                    setColor(0,stats[enemy()].color);
                }
                gotoXY(xs*11,ys*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;

                if(tabell[enemy()][xs][ys+1].hit==true&&tabell[enemy()][xs][ys].hit==true&&tabell[enemy()][xs][ys+1].first.y==tabell[enemy()][xs][ys].first.y){
                    setColor(stats[enemy()].color,0);
                }
                else{
                    setColor(0,stats[enemy()].color);
                }
                gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;

            }
        }
    }
    else{
        setColor(0,stats[enemy()].color);
        gotoXY(xs*11,ys*6-1);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
        gotoXY(xs*11,ys*6+5);   cout<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba<<ba;
        gotoXY(xs*11-1,ys*6);   cout<<ab; gotoXY(xs*11+10,ys*6);  cout<<ab;
        gotoXY(xs*11-1,ys*6+1); cout<<ab; gotoXY(xs*11+10,ys*6+1);  cout<<ab;
        gotoXY(xs*11-1,ys*6+2); cout<<ab; gotoXY(xs*11+10,ys*6+2);  cout<<ab;
        gotoXY(xs*11-1,ys*6+3); cout<<ab; gotoXY(xs*11+10,ys*6+3);  cout<<ab;
        gotoXY(xs*11-1,ys*6+4); cout<<ab; gotoXY(xs*11+10,ys*6+4);  cout<<ab;
        resetColor();
        gotoXY(xs*11,ys*6);   cout<<"          ";
        gotoXY(xs*11,ys*6+1); cout<<"          ";
        gotoXY(xs*11,ys*6+2); cout<<"          ";
        gotoXY(xs*11,ys*6+3); cout<<"          ";
        gotoXY(xs*11,ys*6+4); cout<<"          ";
    }

    if(tabell[enemy()][xs][ys].hit==true){
        if(tabell[enemy()][xs][ys].which_ship!=0){
            setColor(4,0);
        }
        else{
            setColor(8,0);
        }
        gotoXY(xs*11,ys*6);   cout<<"  "; gotoXY(xs*11+8,ys*6);   cout<<"  ";
        gotoXY(xs*11+2,ys*6+1); cout<<"  "; gotoXY(xs*11+6,ys*6+1);   cout<<"  ";
        gotoXY(xs*11+4,ys*6+2); cout<<"  ";
        gotoXY(xs*11+2,ys*6+3); cout<<"  "; gotoXY(xs*11+6,ys*6+3);   cout<<"  ";
        gotoXY(xs*11,ys*6+4); cout<<"  "; gotoXY(xs*11+8,ys*6+4);   cout<<"  ";
    }
    bool exit=false;
    ////////Nel caso un segnalino sia stato piazzato qui, lo stampa///////
    if(settingCoord==true){
        for(int n1=1;n1<stats[turno].ships && exit==false;n1++){
            if(strikes[n1].x==xs&&strikes[n1].y==ys){
                setColor(stats[turno].color,0);
                gotoXY(xs*11,ys*6); cout<<"   "; gotoXY(xs*11+7,ys*6); cout<<"   ";
                gotoXY(xs*11,ys*6+1); cout<<" "; gotoXY(xs*11+9,ys*6+1); cout<<" ";
                gotoXY(xs*11,ys*6+3); cout<<" "; gotoXY(xs*11+9,ys*6+3); cout<<" ";
                gotoXY(xs*11,ys*6+4); cout<<"   "; gotoXY(xs*11+7,ys*6+4); cout<<"   ";
                gotoXY(xs*11+2,ys*6); cout<<n1;
                exit=true;
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////
    resetColor();
}



bool checkShipIntegrity(int xs,int ys){//Outputta un bool che indica se una nave (di cui fa parte una casella specificata in ingresso) ha almeno una casella in vita o meno
    xs=tabell[enemy()][xs][ys].first.x;
    ys=tabell[enemy()][xs][ys].first.y;


    for(int n=0;n<tabell[enemy()][xs][ys].lenght;n++){
        switch(tabell[enemy()][xs][ys].horizontal){
        case true:
            if(tabell[enemy()][xs+n][ys].hit==false){ return true;}
        break;
        case false:
            if(tabell[enemy()][xs][ys+n].hit==false){ return true;}
        break;
        }
    }
    stats[enemy()].ships--;
    return false;
}


void showCursor(bool visible, DWORD size){//SOTTOPROGRAMMA PRESO SU INTERNET CHE ATTIVA/DISATTIVA IL FLASH FASTIDIOSO DEL CURSORE
    // set bool visible = 0 - invisible, bool visible = 1 - visible
    if(size == 0){
        size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
    }
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console,&lpCursor);
}
