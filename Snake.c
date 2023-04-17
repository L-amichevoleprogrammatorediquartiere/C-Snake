/*Snake*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

enum keys{left=37,up,right,down};

struct coordsnake{
    int x;
    int y;
};

struct coordapple{
    int x;
    int y;
    int mangiata;//0 no 1 si
};

typedef struct coordsnake snake;

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawlab();
void menu();
int key();
void clrscreen();
void play();
void direction(int* const,int* const,int);
void assign_apple(struct coordapple*,snake [],int);
void highscore();
int checkfile();
void credit();
void backbutton();
void changecolor(int); //10 verde,12 rosso,14 giallo,15 bianco

int main(){
    menu();
}

void menu(){
    clrscreen();
    drawlab();
    void (*function[3])()={play,highscore,credit};
    int x=10,y=3;
    gotoxy(x,y);
    char *write[]={" sss ",
                   "s   s              k",  
                   " ss    n     aaaa  k  k   eeee",
                   "  ss   nnnn    aa  kkk    e  e",
                   "s   s  n  n  a  a  k kk   ee",
                   " sss   n  n  aaaa  k  kk  eeee",
                   "Play","Highscore","Credits","Exit"};
    for(int i=0;i<6;i++){
        gotoxy(x,y+i);
        for(int q=0;q<=strlen(write[i]);q++){
            isalnum(write[i][(int)q])?printf("%c",220):printf(" ");
        }
    }
    x=10;
    y=13;
    for(int i=6;i<=9;i++){
        gotoxy(x+3,y+=2);
        printf("%s",write[i]);
    }
    int poso=17,posn=15,keys;
    do{
        keys=key();
        if(poso!=posn){
            gotoxy(x,poso);
            printf(" ");
            gotoxy(x,posn);
            changecolor(12);
            printf("%c",16);
            changecolor(15);
            poso=posn;
        }
        if(keys==up){//up
            Sleep(100);
            if(posn==15)
                posn=21;
            else
                posn-=2;
        }
        if(keys==down){//down
            Sleep(100);
            if(posn==21)
                posn=15;
            else
                posn+=2;
        }
        Sleep(100);
    }while(keys!=right);
    if(posn!=21)
        function[(posn%15)/2]();
}

void play(){
    clrscreen();
    drawlab();
    srand(time(NULL));
    int snakeal=0;//0 vivo 1 morto
    int size=3;
    int dir=rand()%4 + 37;
    int xvalue,yvalue; //valori da sommare alle cordinate in base alla direzione
    direction(&xvalue,&yvalue,dir);

    snake *coordsn = (snake*)malloc(size * sizeof(snake));
    changecolor(10);//verde
    for(int i=0;i<size;i++){
        if(i==0){
            coordsn[i].x=rand()%43 +4;
            coordsn[i].y=rand()%16 +4;
        }
        else{
            coordsn[i].x=coordsn[i-1].x+xvalue;
            coordsn[i].y=coordsn[i-1].y+yvalue;
        }
        gotoxy(coordsn[i].x,coordsn[i].y);
        printf("%c",220);
    }
    if(dir>38)
        dir-=2;
    else
        dir+=2;
    direction(&xvalue,&yvalue,dir);
    struct coordapple coordap;
    assign_apple(&coordap,coordsn,size);
    snake oldhead,temp;
    int keys,score;

    while(snakeal==0){
        //Salvo la vecchia posizione della tesa
        oldhead.x=coordsn[0].x;
        oldhead.y=coordsn[0].y;
        //Ogni ciclo si muove di un tot
        keys=key();
        if(keys!=0 && (keys+2!=dir && keys-2!=dir) && keys!=dir){
            dir=keys;
            direction(&xvalue,&yvalue,dir);
        }

        if(coordsn[0].x+xvalue==0 || coordsn[0].x+xvalue==50){
            if(coordsn[0].x+xvalue==0)
                coordsn[0].x=49;
            else
                coordsn[0].x=1;
            coordsn[0].y+=yvalue;    
        }
        else if(coordsn[0].y+yvalue==0 || coordsn[0].y+yvalue==23){
            if(coordsn[0].y+yvalue==0)
                coordsn[0].y=22;
            else
                coordsn[0].y=1;
        }
        else{
            coordsn[0].x+=xvalue;
            coordsn[0].y+=yvalue;
        }
        //prima di muoverlo bisogna vedere se si è mangiato
        for(int i=1;i<size;i++){
            if(coordsn[0].x==coordsn[i].x && coordsn[0].y==coordsn[i].y)
                snakeal=1;
        }
        //se non si è mangiato allora bisogna vedere se ha mangiato la mela e nel caso reallocarlo
        if(snakeal==0){
            if(coordsn[0].x==coordap.x && coordsn[0].y==coordap.y){
                coordap.mangiata=1;
                size++;
                //Se mela mangiata spostiamo e reallochiamo
                coordsn = realloc(coordsn, size * sizeof(snake));
            }
            for(int i=1;i<size;i++){
                if(i==size-1){
                    if(coordap.mangiata==0){
                        temp.x=coordsn[i].x;
                        temp.y=coordsn[i].y;
                    }
                    coordsn[i].x=oldhead.x;
                    coordsn[i].y=oldhead.y;
                }
                else{
                    temp.x=coordsn[i].x;
                    temp.y=coordsn[i].y;
                    coordsn[i].x=oldhead.x;
                    coordsn[i].y=oldhead.y;
                    oldhead.x=temp.x;
                    oldhead.y=temp.y;
                }
            }
            gotoxy(coordsn[0].x,coordsn[0].y);
            changecolor(10);
            printf("%c",220);
            if(coordap.mangiata==0){
                gotoxy(temp.x,temp.y);
                printf(" ");
            }
            else{
                assign_apple(&coordap,coordsn,size);
                score+=200;
            }
            //Altrimenti spostiamo solo quindi cancellare l'ultimo pallino
        }
        
        Sleep(80);
    }
    changecolor(15);
    FILE *ptr;
    if(checkfile()==0)//esiste
        ptr=fopen("Snake/Score.dat","r+");
    else{             //Non esiste
        ptr=fopen("Snake/Score.dat","w+");
        fprintf(ptr,"0 0");
        rewind(ptr);
    }
    int scorestored;
    char name[30];
    fscanf(ptr,"%s %d ",name,&scorestored);
    clrscreen();
    drawlab();
    char *write[]={"Your score is ",
                   "Congratulations, new record",
                   "Please insert your name"};
    int x=6,y=6;
    gotoxy(x,y);
    printf("%s %d",write[0],score);
    if(score>scorestored){
        for(int i=1;i<=3;i++){
            gotoxy(x,++y);
            if(i!=3){
                printf("%s",write[i]);
            }
            else{
                gets(name);
            }
        }
        rewind(ptr);
        fprintf(ptr,"%s %d ",name,score);
    }
    fclose(ptr);
    backbutton();
}

void assign_apple(struct coordapple* coordap,snake coordsn[],int size){
    int appfree;
    do{
        appfree=0;
        coordap->x=rand()%49 +1;
        coordap->y=rand()%22 +1;
        for(int i=0;i<size;i++){
            if(coordap->x==coordsn[i].x && coordap->y==coordsn[i].y){
                appfree=1;
            }
        }
    }while(appfree==1);
    coordap->mangiata=0;
    gotoxy(coordap->x,coordap->y);
    changecolor(12);
    printf("%c",31);
}

void direction(int *const xvalue,int *const yvalue,int dir){
    switch(dir){
        case left:
        *xvalue=-1;
        *yvalue=0;
        break;
        case up:
        *xvalue=0;
        *yvalue=-1;
        break;
        case right:
        *xvalue=+1;
        *yvalue=0;
        break;
        case down:
        *xvalue=0;
        *yvalue=+1;
        break;
    }
}

void highscore(){
    clrscreen();
    drawlab();
    FILE *ptr;
    int x=6,y=8;
    gotoxy(x,y);
    if(checkfile()==0)//esiste
        ptr=fopen("Snake/Score.dat","r+");
    else{             //Non esiste
        ptr=fopen("Snake/Score.dat","w+");
        fprintf(ptr,"0 0");
        rewind(ptr);
    }
    printf("Max score:");
    gotoxy(x,y+2);
    char name[9];
    int score;
    fscanf(ptr,"%s %d",name,&score);
    score==0?printf("No games played"):printf("%8s %d",name,score);
    fclose(ptr);
    backbutton();
}

void credit(){
    clrscreen();
    drawlab();
    char *write[]={"Snake scritto da Crescenzi Giovanni",
                   "il 25/01/2023.",
                   "Corso di laurea Informatica e tecnologia",
                   "per la produzione del software.",
                   "Esame di Programmazione."};
    int x=6,y=6;
    for(int i=0;i<5;i++){
        gotoxy(x,y++);
        printf("%s",write[i]);
    }
    backbutton();
}

void drawlab(){
    for(int y=0;y<=23;y++){
        for(int x=0;x<=50;x++){
            if(x==0 || x==50 || y==0 || y==23){
                if(x==0 && y==0)
                    printf("%c",201);
                else if(x==50 && y==0)
                    printf("%c",187);
                else if(x==0 && y==23)
                    printf("%c",200);
                else if(x==50 && y==23)
                    printf("%c",188);
                else{
                    x==0 || x==50? printf("%c",186):printf("%c",205);
                }
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

void backbutton(){
    gotoxy(9,19);
    printf("Back");
    gotoxy(6,19);
    changecolor(12);
    printf("%c",16);
    changecolor(15);
    while(key()!=39){
        Sleep(100);
    }
    menu();
}

int key(){
    for(int i=left;i<=down;i++){
        if(GetAsyncKeyState(i))
            return i;
    }
    return 0;
}

void clrscreen(){
    gotoxy(0,0);
    for(int y=0;y<=23;y++){
        for(int x=0;x<=50;x++){
            printf(" ");
        }
        printf("\n");
    }
}

int checkfile(){
    FILE *file;
    if (file = fopen("Snake/Score.dat", "r")) {
        fclose(file);
        return 0;
    } 
    else {
        return 1;
    }
}

void changecolor(int color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}