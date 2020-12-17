#include <stdio.h>
#include <Windows.h>
#define LOSE -1000
#define WIN 1000
#define A 4


char board[8][8];                               //name says, its board xD

//sets colours to all pieces
enum colours{
    niebieskiDeep = 1, //zamiast średników stosuje się przecinki
    zielony = 2,
    niebieskiLight = 3,
    czerwony = 4,
    fioletowy = 5,
    zolty = 6,
    szarawy = 7,
    szary = 8,
    niebieski = 9,
    zielonyLight = 10,
    niebieskiUltraLight = 11,
    rozowy = 12,
    fioletowyLight = 13,
    kremowy = 14,
    bialy = 15
};

//tells if we can do a castle
int castleWhiteLeft=0;                          //if 0 maybe we can do long white castle
int castleWhiteRight=0;                         //if 0 maybe we can do short white castle
int castleWhiteKing=0;                          //if 0 we can do white castle
int castleBlackLeft=0;                          //if 0 maybe we can do long black castle
int castleBlackRight=0;                         //if 0 maybe we can do short black castle
int castleBlackKing=0;                          //if 0 we can do black castle

//tells if we can do a flight kill
int whiteFlight[8]={1,1,1,1,1,1,1,1};           //only if certain whiteFlight=1 we can kill white in flight on that width
char blackFlight[8]="PPPPPPPP";                 //only if certain blackFlight=P we can kill black in flight on that width

//here u choose which type of game u wanna play
int start(int type){
    if(type==0) {
        printf("Hey, nice to see you!\n"
               "Do you wanna play chess against my Pc algorithm?\n"
               "-->If no, and you wanna play against player type: (1)\n"
               "-->If yes type: (2)\n"
               "-->");
        while (1) {
            scanf("%d", &type);
            if (type == 1 || type == 2) {
                return type;
            }
            else{
                printf("BAD TYPE! Choose once again!\n"
                       "-->");
            }
        }
    }
    if (type==2){
        printf("Which colour u wanna play?\n"
               "-->If white type: (1)\n"
               "-->If black type: (2)\n"
               "-->");
        while (1) {
            scanf("%d", &type);
            if (type == 1 || type == 2) {
                return type;
            }
            else{
                printf("BAD TYPE! Choose once again!\n"
                       "-->");
            }
        }
    }
}

//cleaning the board
void clear(char b[8][8]){
    int i, j;
    for (i = 0;  i<8 ; i++) {
        for (j = 0;  j<8 ; j++) {
            b[i][j]=' ';
        }
    }
}

//rating the board
int bs=0, bl=0, ws=0, wl=0;
int countBlack=0, countWhite=0;
int rating(char b[8][8]){
    int heuristics[8][8]={{100,100,100,100,100,100,100,100},
                          {100,120,120,120,120,120,120,100},
                          {100,120,140,140,140,140,120,100},
                          {100,120,140,160,160,140,120,100},
                          {100,120,140,160,160,140,120,100},
                          {100,120,140,140,140,140,120,100},
                          {100,120,120,120,120,120,120,100},
                          {100,100,100,100,100,100,100,100},};
    int BlackKing=0, WhiteKing=0;
    int sum=0;
    int i, j;
    for(i=0; i<8; ++i){
        for(j=0; j<8; ++j){
            if(b[i][j]=='K'){
                BlackKing=1;
                break;
            }
        }
    }
    for(i=0; i<8; ++i){
        for(j=0; j<8; ++j){
            if(b[i][j]=='0'){
                WhiteKing=1;
                break;
            }
        }
    }
    if(BlackKing==0){
        return LOSE*3;
    }
    if(WhiteKing==0){
        return WIN*3;
    }
    for(i=0; i <8; ++i){
        for(j=0; j <8; ++j){
            if(b[i][j]=='1') sum-=1*heuristics[i][j];
            if(b[i][j]=='P') sum+=1*heuristics[i][j];
            if(b[i][j]=='4') sum-=3*heuristics[i][j];
            if(b[i][j]=='S') sum+=3*heuristics[i][j];
            if(b[i][j]=='3') sum-=3*heuristics[i][j];
            if(b[i][j]=='G') sum+=3*heuristics[i][j];
            if(b[i][j]=='5') sum-=5*heuristics[i][j];
            if(b[i][j]=='W') sum+=5*heuristics[i][j];
            if(b[i][j]=='9') sum-=9*heuristics[i][j];
            if(b[i][j]=='H') sum+=9*heuristics[i][j];
        }
    }
    //king with no move + rook with no move
    if (b[0][4]=='K'){
        sum+=20;
    }
    if (b[7][4]=='0'){
        sum-=20;
    }
    if (castleBlackRight==0 && castleBlackKing==0){
        sum+=20;
    }
    if (castleBlackLeft==0 && castleBlackKing==0){
        sum+=20;
    }
    if (castleWhiteRight==0 && castleWhiteKing==0){
        sum-=20;
    }
    if (castleWhiteLeft==0 && castleWhiteKing==0){
        sum-=20;
    }
    //castled
    if (countBlack==0) {
        if (b[0][6] == 'K' && b[0][5] == 'W') {
            bs=+120;
            countBlack++;
        }
        if (b[0][2] == 'K' && b[0][3] == 'W') {
            bl=+100;
            countBlack++;
        }
    }
    if (countWhite==0) {
        if (b[7][6] == '0' && b[7][5] == '5') {
            ws=-120;
            countWhite++;
        }
        if (b[7][2] == '0' && b[7][3] == '5') {
            wl=-100;
            countWhite++;
        }
    }
    return sum+bs+bl+ws+wl;
}

//for minimax algorithm
//rating the board
int ratingV1(char b[8][8]){
    static int heuristics[8][8]={{100,100,100,100,100,100,100,100},
                                {100,120,120,120,120,120,120,100},
                                {100,120,140,140,140,140,120,100},
                                {100,120,140,160,160,140,120,100},
                                {100,120,140,160,160,140,120,100},
                                {100,120,140,140,140,140,120,100},
                                {100,120,120,120,120,120,120,100},
                                {100,100,100,100,100,100,100,100},};
    int BlackKing=0, WhiteKing=0;
    int sum=0;
    int i, j;
    for(i=0; i<8; ++i){
        for(j=0; j<8; ++j){
            if(b[i][j]=='K'){
                BlackKing=1;
                break;
            }
        }
    }
    for(i=0; i<8; ++i){
        for(j=0; j<8; ++j){
            if(b[i][j]=='0'){
                WhiteKing=1;
                break;
            }
        }
    }
    if(BlackKing==0){
        return LOSE*3;
    }
    if(WhiteKing==0){
        return WIN*3;
    }
    for(i=0; i <8; ++i){
        for(j=0; j <8; ++j){
            if(b[i][j]=='1') sum-=1*heuristics[i][j];
            if(b[i][j]=='P') sum+=1*heuristics[i][j];
            if(b[i][j]=='4') sum-=3*heuristics[i][j];
            if(b[i][j]=='S') sum+=3*heuristics[i][j];
            if(b[i][j]=='3') sum-=3*heuristics[i][j];
            if(b[i][j]=='G') sum+=3*heuristics[i][j];
            if(b[i][j]=='5') sum-=5*heuristics[i][j];
            if(b[i][j]=='W') sum+=5*heuristics[i][j];
            if(b[i][j]=='9') sum-=9*heuristics[i][j];
            if(b[i][j]=='H') sum+=9*heuristics[i][j];
        }
    }
    return sum;
}

//printing the board
void writee(char b[8][8]) {
    int i, j;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("\n Board, (black on top):                                            Board, (white on top):\n");
    printf("*****************************************************             *****************************************************\n");
    printf("*\\\\  0  *  1  *  2  *  3  *  4  *  5  *  6  *  7  //*             *\\\\  7  *  6  *  5  *  4  *  3  *  2  *  1  *  0  //*");
    printf("\n* +-----+-----+-----+-----+-----+-----+-----+-----+ *             * +-----+-----+-----+-----+-----+-----+-----+-----+ *\n");
    //printf("*\\\\-----------------------------------------------//*         *\\\\-----------------------------------------------//*\n");
    for (i = 0; i < 8; i++) {
        printf("*%d|", i);
        for (j = 0; j < 8; j++) {
            if (board[i][j] == 'P' || board[i][j] == 'W' || board[i][j] == 'S' || board[i][j] == 'G' ||
                board[i][j] == 'H' || board[i][j] == 'K') {
                SetConsoleTextAttribute(hConsole, niebieski);
            }
            if (board[i][j] == '1' || board[i][j] == '5' || board[i][j] == '4' || board[i][j] == '3' ||
                board[i][j] == '9' || board[i][j] == '0') {
                SetConsoleTextAttribute(hConsole, zolty);
            }
            printf("  %c  ", b[i][j]);
            SetConsoleTextAttribute(hConsole, bialy);
            if (j < 7) {
                printf("|");
            }
        }
        printf("|%d*", i);
        printf("             *%d|", 7 - i);
        for (j = 0; j < 8; j++) {
            if (board[7 - i][7 - j] == 'P' || board[7 - i][7 - j] == 'W' || board[7 - i][7 - j] == 'S' ||
                board[7 - i][7 - j] == 'G' || board[7 - i][7 - j] == 'H' || board[7 - i][7 - j] == 'K') {
                SetConsoleTextAttribute(hConsole, niebieski);
            }
            if (board[7 - i][7 - j] == '1' || board[7 - i][7 - j] == '5' || board[7 - i][7 - j] == '4' ||
                board[7 - i][7 - j] == '3' || board[7 - i][7 - j] == '9' || board[7 - i][7 - j] == '0') {
                SetConsoleTextAttribute(hConsole, zolty);
            }
            printf("  %c  ", b[7 - i][7 - j]);
            SetConsoleTextAttribute(hConsole, bialy);
            if (j < 7) {
                printf("|");
            }
        }
        printf("|%d*", 7 - i);
        printf("\n* +-----+-----+-----+-----+-----+-----+-----+-----+ *             * +-----+-----+-----+-----+-----+-----+-----+-----+ *");
        printf("\n");
    }
    //printf("*//-----------------------------------------------\\\\*             *//-----------------------------------------------\\\\*\n");
    printf("*//  0  *  1  *  2  *  3  *  4  *  5  *  6  *  7  \\\\*             *//  7  *  6  *  5  *  4  *  3  *  2  *  1  *  0  \\\\*\n");
    printf("*****************************************************             *****************************************************");
    if (rating(board) > 0) {
        printf("\n                                                                                                          Rating: %d\n",
               -rating(board));
    }
    else{
        printf("\n                                                                                                          Rating: %d\n",
               -rating(board));
    }
}

//setting pieces on the board
void beforeMoves(char b[8][8]){                 //every x, y, z, c are used in all kind of "for" X IS NOT WIDTH AND Y IS NOT HIGH
    int x;
    for (x = 0; x < 8; x++) {
        b[1][x] = 'P';
        b[6][x]='1';
    }
    b[0][0]='W'; b[0][7]='W'; b[0][1]='S'; b[0][6]='S'; b[0][2]='G'; b[0][5]='G'; b[0][3]='H'; b[0][4]='K';
    b[7][0]='5'; b[7][7]='5'; b[7][1]='4'; b[7][6]='4'; b[7][2]='3'; b[7][5]='3'; b[7][3]='9'; b[7][4]='0';

    //WSGHKGSW
    //PPPPPPPP

    //11111111
    //54390345
}

//switching the white pawn
//if we are at 0 high with white pawn then in whiteMove function we are running this one
void whitePawnSwitch(int k, int l){
    int exchange;
    printf("For what you wanna change your pawn!-->");
    scanf("%d", &exchange);             //k and l are positions after move
    if (exchange==1) {                          //k is telling the high and l is telling the width
        board[k][l]='1';                        //exchange for pawn
    }
    if (exchange==5) {
        board[k][l]='5';                        //exchange for rook
    }
    if (exchange==4) {
        board[k][l]='4';                        //exchange for knight
    }
    if (exchange==3) {
        board[k][l]='3';                        //exchange for bishop
    }
    if (exchange==9) {
        board[k][l]='9';                        //exchange for queen
    }
}

//switching the black pawn
//if we are at 7 high with black pawn then in blackMove function we are running this one
void blackPawnSwitch(int k, int l){
    char exchange;
    printf("For what you wanna change your pawn!-->");
    scanf("%c", &exchange);             //k and l are positions after move
    if (exchange=='P') {                        //k is telling the high and l is telling the width
        board[k][l]='P';                        //exchange for pawn
    }
    if (exchange=='W') {
        board[k][l]='W';                        //exchange for rook
    }
    if (exchange=='S') {
        board[k][l]='S';                        //exchange for knight
    }
    if (exchange=='G') {
        board[k][l]='G';                        //exchange for bishop
    }
    if (exchange=='H') {
        board[k][l]='H';                        //exchange for queen
    }
}

//before every white move we check if certain piece ever made a move so white castle isnt possible
void castleCheckWhite(char b[8][8]){
    if (b[7][0]!='5'){
        castleWhiteLeft++;
    }
    if (b[7][7]!='5'){
        castleWhiteRight++;
    }
    if (b[7][4]!='0'){
        castleWhiteKing++;
    }
}

//if ure not sure how it work google it PLZ
//castle conditions fucntion is runned by whiteCorrectMove function
//if castleWhite returns 1 to whiteCorrectMove function castle is "ok"
int castleWhite(int l){                                          //every x, y, z, c are used in all kind of "for" X IS NOT WIDTH AND Y IS NOT HIGH
    if (castleWhiteKing!=0){                                 //if king ever made a move we cant do a castle
        return 0;
    }
    int x, y, z, c, count=0, count1;                         //count checks free space between king and rook, count1 checks free space between enemy piece and king
    if (l==2){                                               //here we check if we do loong castle (whiteLeft)
        if (castleWhiteLeft==0){                             //here we check if we may do loong castle (whiteLeft)
            for (x=1; x<4 ; x++) {
                if (board[7][x]==' '){
                    count++;
                }
            }
            if (count==3){                                   //here we check if we can do loong castle (whiteLeft)
                for (x=1; x<6; x++) {                                                   //pawn check
                    if (board[6][x] == 'P') {
                        return 0;
                    }
                }
                for (x=0; x<7; x++) {                                                   //rook check
                    for (y=2; y<5; y++) {
                        if (board[x][y] == 'W') {
                            c=y;
                            count1=0;
                            for (z=x+1; z<7; z++) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==7-x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[7][5]=='W' || board[6][2]=='W' || board[6][3]=='W' || board[6][4]=='W'){
                    return 0;
                }
                if (board[7][6]=='W' && board[7][5]==' '){
                    return 0;
                }
                if (board[7][7]=='W' && board[7][6]==' ' && board[7][5]==' '){
                    return 0;
                }
                for (x=1; x<6; x++) {                                                   //knight check
                    if (board[5][x]=='S'){
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[6][x]=='S'){
                        return 0;
                    }
                }
                for (x=4; x<7; x++) {
                    if (board[6][x]=='S'){
                        return 0;
                    }
                }
                for (x=1; x<6; x++) {                                                   //bishop check
                    if (board[6][x] == 'G') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[5][x] == 'G' && board[6][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[4][x] == 'G' && board[5][1+x] == ' ' && board[6][2+x] == ' ') {
                        return 0;
                    }
                }
                if (board[3][0] == 'G' && board[4][1] == ' ' && board[5][2] == ' ' && board[6][3] == ' '){
                    return 0;
                }
                for (x=0; x<3; x++) {
                    if (board[5][4+x]=='G' && board[6][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[4][5+x]=='G' && board[5][4+x]==' ' && board[6][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[3][6+x]=='G' && board[4][5+x]==' ' && board[5][4+x]==' ' && board[6][3+x]==' '){
                        return 0;
                    }
                }
                if (board[2][7]=='G' && board[3][6]==' ' && board[4][5]==' ' && board[5][4]==' ' && board[6][3]==' '){
                    return 0;
                }
                for (x=0; x<7; x++) {                                                   //queen check coppied from rook and bishop
                    for (y=2; y<5; y++) {
                        if (board[x][y] == 'H') {
                            c=y;
                            count1=0;
                            for (z=x+1; z<7; z++) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==7-x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[7][5]=='H' || board[6][2]=='H' || board[6][3]=='H' || board[6][4]=='H'){
                    return 0;
                }
                if (board[7][6]=='H' && board[7][5]==' '){
                    return 0;
                }
                if (board[7][7]=='H' && board[7][6]==' ' && board[7][5]==' '){
                    return 0;
                }
                for (x=1; x<6; x++) {
                    if (board[6][x] == 'H') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[5][x] == 'H' && board[6][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[4][x] == 'H' && board[5][1+x] == ' ' && board[6][2+x] == ' ') {
                        return 0;
                    }
                }
                if (board[3][0] == 'H' && board[4][1] == ' ' && board[5][2] == ' ' && board[6][3] == ' '){
                    return 0;
                }
                for (x=0; x<3; x++) {
                    if (board[5][4+x]=='H' && board[6][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[4][5+x]=='H' && board[5][4+x]==' ' && board[6][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[3][6+x]=='H' && board[4][5+x]==' ' && board[5][4+x]==' ' && board[6][3+x]==' '){
                        return 0;
                    }
                }
                if (board[2][7]=='H' && board[3][6]==' ' && board[4][5]==' ' && board[5][4]==' ' && board[6][3]==' '){
                    return 0;
                }
                for (x=1; x<6; x++) {                                                   //king check
                    if (board[6][x]=='K'){
                        return 0;
                    }
                }
                if (board[7][5]=='K'){
                    return 0;
                }                                                                       //if any of above didnt happen we do castle
                board[7][3]='5';                                                        //moove the rook
                board[7][0]=' ';                                                        //clean the board
                return 1;                                                               //we return 1 to whiteCorrectMove because castle is possible
            }
            return 0;
        }
        return 0;
    }
    if (l==6){                                                                          //here we check if we do short castle (whiteRight)
        if (castleWhiteRight==0){                                                       //here we check if we may do short castle (whiteRight)
            for (x=5; x<7 ; x++) {
                if (board[7][x]==' '){
                    count++;
                }
            }
            if (count==2){                                                              //here we check if we can do short castle (whiteRight)
                for (x=3; x<8; x++) {                                                   //pawn check
                    if (board[6][x] == 'P') {
                        return 0;
                    }
                }
                for (x=0; x<7; x++) {                                                   //rook check
                    for (y=4; y<7; y++) {
                        if (board[x][y] == 'W') {
                            c=y;
                            count1=0;
                            for (z=x+1; z<7; z++) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==7-x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[7][3]=='W' || board[6][4]=='W' || board[6][5]=='W' || board[6][6]=='W'){
                    return 0;
                }
                if (board[7][2]=='W' && board[7][3]==' '){
                    return 0;
                }
                if (board[7][1]=='W' && board[7][2]==' ' && board[7][3]==' '){
                    return 0;
                }
                if (board[7][0]=='W' && board[7][1]==' ' && board[7][2]==' ' && board[7][3]==' '){
                    return 0;
                }
                for (x=3; x<8; x++) {                                                   //knight check
                    if (board[5][x]=='S'){
                        return 0;
                    }
                }
                for (x=2; x<5; x++) {
                    if (board[6][x]=='S'){
                        return 0;
                    }
                }
                for (x=6; x<8; x++) {
                    if (board[6][x]=='S'){
                        return 0;
                    }
                }
                for (x=3; x<8; x++) {                                                   //bishop check
                    if (board[6][x] == 'G') {
                        return 0;
                    }
                }
                for (x=2; x<5; x++) {
                    if (board[5][x] == 'G' && board[6][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=1; x<4; x++) {
                    if (board[4][x] == 'G' && board[5][1+x] == ' ' && board[6][2+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[3][x] == 'G' && board[4][1+x] == ' ' && board[5][2+x] == ' ' && board[6][3+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[2][x] == 'G' && board[3][1+x] == ' ' && board[4][2+x] == ' ' && board[5][3+x] == ' ' && board[6][4+x] == ' ') {
                        return 0;
                    }
                }
                if (board[1][0] == 'G' && board[2][1] == ' ' && board[3][2] == ' ' && board[4][3] == ' ' && board[5][4] == ' ' && board[6][5] == ' '){
                    return 0;
                }
                for (x=0; x<2; x++) {
                    if (board[5][6+x]=='G' && board[6][5+x]==' '){
                        return 0;
                    }
                }
                if (board[4][7]=='G' && board[5][6]==' ' && board[6][5]==' '){
                    return 0;
                }
                for (x=0; x<7; x++) {                                                   //queen check coppied from rook and bishop
                    for (y=4; y<7; y++) {
                        if (board[x][y] == 'H') {
                            c=y;
                            count1=0;
                            for (z=x+1; z<7; z++) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==7-x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[7][3]=='H' || board[6][4]=='H' || board[6][5]=='H' || board[6][6]=='H'){
                    return 0;
                }
                if (board[7][2]=='H' && board[7][3]==' '){
                    return 0;
                }
                if (board[7][1]=='H' && board[7][2]==' ' && board[7][3]==' '){
                    return 0;
                }
                if (board[7][0]=='H' && board[7][1]==' ' && board[7][2]==' ' && board[7][3]==' '){
                    return 0;
                }
                for (x=3; x<8; x++) {
                    if (board[6][x] == 'H') {
                        return 0;
                    }
                }
                for (x=2; x<5; x++) {
                    if (board[5][x] == 'H' && board[6][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=1; x<4; x++) {
                    if (board[4][x] == 'H' && board[5][1+x] == ' ' && board[6][2+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[3][x] == 'H' && board[4][1+x] == ' ' && board[5][2+x] == ' ' && board[6][3+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[2][x] == 'H' && board[3][1+x] == ' ' && board[4][2+x] == ' ' && board[5][3+x] == ' ' && board[6][4+x] == ' ') {
                        return 0;
                    }
                }
                if (board[1][0] == 'H' && board[2][1] == ' ' && board[3][2] == ' ' && board[4][3] == ' ' && board[5][4] == ' ' && board[6][5] == ' '){
                    return 0;
                }
                for (x=0; x<2; x++) {
                    if (board[5][6+x]=='H' && board[6][5+x]==' '){
                        return 0;
                    }
                }
                if (board[4][7]=='H' && board[5][6]==' ' && board[6][5]==' '){
                    return 0;
                }
                for (x=3; x<8; x++) {                                                   //king check
                    if (board[6][x]=='K'){
                        return 0;
                    }
                }
                if (board[7][3]=='K'){
                    return 0;
                }//if any of above didnt happen we do castle
                board[7][5] = '5';                                                        //moove the rook
                board[7][7] = ' ';                                                        //clean the board
                return 1;                                                                 //we return 1 to whiteCorrectMove because castle is possible
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

//before every black move we check if certain piece ever made a move so black castle isnt possible
void castleCheckBlack(char b[8][8]){
    if (b[0][0]!='W'){
        castleBlackLeft++;
    }
    if (b[0][7]!='W'){
        castleBlackRight++;
    }
    if (b[0][4]!='K'){
        castleBlackKing++;
    }
}

//if ure not sure how it work google it PLZ
//castle conditions fucntion is runned by blackCorrectMove function
//if castleBlack returns 1 to blackCorrectMove function castle is "ok"
int castleBlack(int l){                                          //every x, y, z, c are used in all kind of "for" X IS NOT WIDTH AND Y IS NOT HIGH
    if (castleBlackKing!=0){                                 //if king ever made a move we cant do a castle
        return 0;
    }
    int x, y, z, c, count=0, count1;                         //count checks free space between king and rook, count1 checks free space between enemy piece and king
    if (l==2){                                               //here we check if we do loong castle (blackLeft)
        if (castleBlackLeft==0){                             //here we check if we may do loong castle (blackLeft)
            for (x=1; x<4 ; x++) {
                if (board[0][x]==' '){
                    count++;
                }
            }
            if (count==3){                                   //here we check if we can do loong castle (blackLeft)
                for (x=1; x<6; x++) {                                                   //pawn check
                    if (board[1][x] == '1') {
                        return 0;
                    }
                }
                for (x=1; x<8; x++) {                                                   //rook check
                    for (y=2; y<5; y++) {
                        if (board[x][y] == '5') {
                            c=y;
                            count1=0;
                            for (z=x-1; z>0; z--) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[0][5]=='5' || board[1][2]=='5' || board[1][3]=='5' || board[1][4]=='5'){
                    return 0;
                }
                if (board[0][6]=='5' && board[0][5]==' '){
                    return 0;
                }
                if (board[0][7]=='5' && board[0][6]==' ' && board[0][5]==' '){
                    return 0;
                }
                for (x=1; x<6; x++) {                                                   //knight check
                    if (board[2][x]=='4'){
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[1][x]=='4'){
                        return 0;
                    }
                }
                for (x=4; x<7; x++) {
                    if (board[1][x]=='4'){
                        return 0;
                    }
                }
                for (x=1; x<6; x++) {                                                   //bishop check
                    if (board[1][x] == '3') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[2][x] == '3' && board[1][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[3][x] == '3' && board[2][1+x] == ' ' && board[1][2+x] == ' ') {
                        return 0;
                    }
                }
                if (board[4][0] == '3' && board[3][1] == ' ' && board[2][2] == ' ' && board[1][3] == ' '){
                    return 0;
                }
                for (x=0; x<3; x++) {
                    if (board[2][4+x]=='3' && board[1][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[3][5+x]=='3' && board[2][4+x]==' ' && board[1][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[4][6+x]=='3' && board[3][5+x]==' ' && board[2][4+x]==' ' && board[1][3+x]==' '){
                        return 0;
                    }
                }
                if (board[5][7]=='3' && board[4][6]==' ' && board[3][5]==' ' && board[2][4]==' ' && board[1][3]==' '){
                    return 0;
                }
                for (x=1; x<8; x++) {                                                   //queen check coppied from rook and bishop
                    for (y=2; y<5; y++) {
                        if (board[x][y] == '9') {
                            c=y;
                            count1=0;
                            for (z=x-1; z>0; z--) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[0][5]=='9' || board[1][2]=='9' || board[1][3]=='9' || board[1][4]=='9'){
                    return 0;
                }
                if (board[0][6]=='9' && board[0][5]==' '){
                    return 0;
                }
                if (board[0][7]=='9' && board[0][6]==' ' && board[0][5]==' '){
                    return 0;
                }
                for (x=1; x<6; x++) {
                    if (board[1][x] == '9') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[2][x] == '9' && board[1][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[3][x] == '9' && board[2][1+x] == ' ' && board[1][2+x] == ' ') {
                        return 0;
                    }
                }
                if (board[4][0] == '9' && board[3][1] == ' ' && board[2][2] == ' ' && board[1][3] == ' '){
                    return 0;
                }
                for (x=0; x<3; x++) {
                    if (board[2][4+x]=='9' && board[1][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[3][5+x]=='9' && board[2][4+x]==' ' && board[1][3+x]==' '){
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[4][6+x]=='9' && board[3][5+x]==' ' && board[2][4+x]==' ' && board[1][3+x]==' '){
                        return 0;
                    }
                }
                if (board[5][7]=='9' && board[4][6]==' ' && board[3][5]==' ' && board[2][4]==' ' && board[1][3]==' '){
                    return 0;
                }
                for (x=1; x<6; x++) {                                                   //king check
                    if (board[1][x]=='0'){
                        return 0;
                    }
                }
                if (board[0][5]=='0'){
                    return 0;
                }                                                                       //if any of above didnt happen we do castle
                /*board[0][3]='W';                                                        //moove the rook
                board[0][0]=' ';                                                        //clean the board*/
                return 1;                                                               //we return 1 to whiteCorrectMove because castle is possible
            }
            return 0;
        }
        return 0;
    }
    if (l==6){                                                                          //here we check if we do short castle (blackRight)
        if (castleBlackRight==0){                                                       //here we check if we may do short castle (blackRight)
            for (x=5; x<7 ; x++) {
                if (board[0][x]==' '){
                    count++;
                }
            }
            if (count==2){                                                              //here we check if we can do short castle (blackRight)
                for (x=3; x<8; x++) {                                                   //pawn check
                    if (board[1][x] == '1') {
                        return 0;
                    }
                }
                for (x=1; x<8; x++) {                                                   //rook check
                    for (y=4; y<7; y++) {
                        if (board[x][y] == '5') {
                            c=y;
                            count1=0;
                            for (z=x-1; z>0; z--) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[0][3]=='5' || board[1][4]=='5' || board[1][5]=='5' || board[1][6]=='5'){
                    return 0;
                }
                if (board[0][2]=='5' && board[0][3]==' '){
                    return 0;
                }
                if (board[0][1]=='5' && board[0][2]==' ' && board[0][3]==' '){
                    return 0;
                }
                if (board[0][0]=='5' && board[0][1]==' ' && board[0][2]==' ' && board[0][3]==' '){
                    return 0;
                }
                for (x=3; x<8; x++) {                                                   //knight check
                    if (board[2][x]=='4'){
                        return 0;
                    }
                }
                for (x=2; x<5; x++) {
                    if (board[1][x]=='4'){
                        return 0;
                    }
                }
                for (x=6; x<8; x++) {
                    if (board[1][x]=='4'){
                        return 0;
                    }
                }
                for (x=3; x<8; x++) {                                                   //bishop check
                    if (board[1][x] == '3') {
                        return 0;
                    }
                }
                for (x=2; x<5; x++) {
                    if (board[2][x] == '3' && board[1][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=1; x<4; x++) {
                    if (board[3][x] == '3' && board[2][1+x] == ' ' && board[1][2+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[4][x] == '3' && board[3][1+x] == ' ' && board[2][2+x] == ' ' && board[1][3+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[5][x] == '3' && board[4][1+x] == ' ' && board[3][2+x] == ' ' && board[2][3+x] == ' ' && board[1][4+x] == ' ') {
                        return 0;
                    }
                }
                if (board[6][0] == '3' && board[5][1] == ' ' && board[4][2] == ' ' && board[3][3] == ' ' && board[2][4] == ' ' && board[1][5] == ' '){
                    return 0;
                }
                for (x=0; x<2; x++) {
                    if (board[2][6+x]=='3' && board[1][5+x]==' '){
                        return 0;
                    }
                }
                if (board[3][7]=='3' && board[2][6]==' ' && board[1][5]==' '){
                    return 0;
                }
                for (x=1; x<8; x++) {                                                   //queen check coppied from rook and bishop
                    for (y=4; y<7; y++) {
                        if (board[x][y] == '9') {
                            c=y;
                            count1=0;
                            for (z=x-1; z>0; z--) {
                                if (board[z][c]==' '){
                                    count1++;
                                }
                            }
                            if (count1==x-1) {
                                return 0;
                            }
                        }
                    }
                }
                if (board[0][3]=='9' || board[1][4]=='9' || board[1][5]=='9' || board[1][6]=='9'){
                    return 0;
                }
                if (board[0][2]=='9' && board[0][3]==' '){
                    return 0;
                }
                if (board[0][1]=='9' && board[0][2]==' ' && board[0][3]==' '){
                    return 0;
                }
                if (board[0][0]=='9' && board[0][1]==' ' && board[0][2]==' ' && board[0][3]==' '){
                    return 0;
                }
                for (x=3; x<8; x++) {
                    if (board[1][x] == '9') {
                        return 0;
                    }
                }
                for (x=2; x<5; x++) {
                    if (board[2][x] == '9' && board[1][1+x] == ' ') {
                        return 0;
                    }
                }
                for (x=1; x<4; x++) {
                    if (board[3][x] == '9' && board[2][1+x] == ' ' && board[1][2+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<3; x++) {
                    if (board[4][x] == '9' && board[3][1+x] == ' ' && board[2][2+x] == ' ' && board[1][3+x] == ' ') {
                        return 0;
                    }
                }
                for (x=0; x<2; x++) {
                    if (board[5][x] == '9' && board[4][1+x] == ' ' && board[3][2+x] == ' ' && board[2][3+x] == ' ' && board[1][4+x] == ' ') {
                        return 0;
                    }
                }
                if (board[6][0] == '9' && board[5][1] == ' ' && board[4][2] == ' ' && board[3][3] == ' ' && board[2][4] == ' ' && board[1][5] == ' '){
                    return 0;
                }
                for (x=0; x<2; x++) {
                    if (board[2][6+x]=='9' && board[1][5+x]==' '){
                        return 0;
                    }
                }
                if (board[3][7]=='9' && board[2][6]==' ' && board[1][5]==' '){
                    return 0;
                }
                for (x=3; x<8; x++) {                                                   //king check
                    if (board[1][x]=='0'){
                        return 0;
                    }
                }                                                                       //if any of above didnt happen we do castle
                /*board[0][5]='W';                                                        //moove the rook
                board[0][7]=' ';                                                        //clean the board*/
                return 1;                                                               //we return 1 to blackCorrectMove because castle is possible
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

//before every white move
//checks if its a possibility to kill white pawn by black pawn NEXT BLACK MOVE! in flight
void whiteFlightBeatBeforeMove(char b[8][8]){
    int x;
    for (x=0; x<8; x++) {
        if (b[6][x] == '1' && b[4][x] == ' ') {   //if the pawn has not made a move, and can move two fields forward,
            whiteFlight[x] = 1;                   //the possibility of killing the white in flight is setting as true
        }
        else {
            whiteFlight[x] = 0;                   //else its setting as false
        }
    }
}

//after every white move
//checks if black cant kill white in flight
void whiteFlightBeat(char b[8][8]){
    int x;
    for (x=0; x<8; x++) {                                                                                   //if white moved but not 2 squares forward
        if (b[6][x]==' ' && b[4][x]!='1'){
            whiteFlight[x]=0;
        }
        if (b[6][x]=='P' || b[6][x]=='W' || b[6][x]=='S' || b[6][x]=='G' || b[6][x]=='H' || b[6][x]=='K'){  //if white was killed
            whiteFlight[x]=0;
        }
        if (b[6][x]=='1'){                                                                                  //if white didnt make a move
            whiteFlight[x]=0;
        }
    }
}

//before every black move
//checks if its a possibility to kill black pawn by white pawn NEXT WHITE MOVE! in flight
void blackFlightBeatBeforeMove(char b[8][8]){//we are checking if there is a possibility to kill black in flight
    int x;
    for (x=0; x<8; x++) {
        if (b[1][x] == 'P' && b[3][x] == ' ') {   //if the pawn has not made a move, and can move two fields forward,
            blackFlight[x] ='P';                  //the possibility of killing the black in flight is setting as true
        }
        else {
            blackFlight[x] ='X';                  //else its setting as false
        }
    }
}

//after every black move
//checks if white cant kill black in flight
void blackFlightBeat(char b[8][8]){//we are checking if we can kill black in flight
    int x;
    for (x=0; x<8; x++) {                                                                                   //if black moved but not 2 squares forward
        if (b[1][x]==' ' && b[3][x]!='P'){
            blackFlight[x]='X';
        }
        if (b[1][x]=='1' || b[1][x]=='5' || b[1][x]=='4' || b[1][x]=='3' || b[1][x]=='9' || b[1][x]=='0'){  //if black was killed
            blackFlight[x]='X';
        }
        if (b[1][x]=='P'){                                                                                  //if black didnt make a move
            blackFlight[x]='X';
        }
    }
}

//is started by whiteMove function if there is possibility for certain move
//check if player made a gg move, if it returns 1 the move was gg
//i, j are co-ordinates of piece before move (high, width)
//k, l are co-ordinates of piece after move (high, width)
//preMove tells what symbol was at (i,j)
//afterMove tells what symbol was at (k,l)
int xd=0;
int whiteCorrectMoves(int preMove, char afterMove, int i, int j, int k, int l){
    int temp, temp1;
    if (preMove=='1'){//pawn moves, 1 forward, kill slant
        if (i==6){
            if (k==i-2 && l==j && afterMove==' ' && board[5][j]==' ' || k==i-1 && j==l && afterMove==' '){
                return 1;
            }
        }
        if (i==3){
            if (k==i-1 && l==j-1 && board[i][j-1]=='P' && board[k][l]==' ' && blackFlight[l]=='P'){
                board[i][j-1]=' ';
                return 1;
            }
            if (k==i-1 && l==j+1 && board[i][j+1]=='P' && board[k][l]==' ' && blackFlight[l]=='P'){
                board[i][j+1]=' ';
                return 1;
            }
            if (k==i-1 && l==j && afterMove==' '){
                return 1;
            }
            if (k==i-1 && l==j-1 && afterMove!=' ' || k==i-1 && l==j+1 && afterMove!=' '){
                return 1;
            }
        }
        else {
            if (k==i-1 && l==j && afterMove==' '){
                return 1;
            }
            if (k==i-1 && l==j-1 && afterMove!=' ' || k==i-1 && l==j+1 && afterMove!=' '){
                return 1;
            }
        }
    }
    if (preMove=='5'){//rook moves
//[ ][ ][ ][5][ ][ ][ ][ ]
//[ ][ ][ ][5][ ][ ][ ][ ]
//[ ][ ][ ][5][ ][ ][ ][ ]
//[ ][ ][ ][5][ ][ ][ ][ ]
//[5][5][5] 5 [5][5][5][5]
//[ ][ ][ ][5][ ][ ][ ][ ]
//[ ][ ][ ][5][ ][ ][ ][ ]
//[ ][ ][ ][5][ ][ ][ ][ ]
        if (i>k && j==l){
            temp=i-1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp-1;
            }
        }
        if (i<k && j==l){
            temp=i+1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp+1;
            }
        }
        if (i==k){
            if (j>l){
                temp=j-1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp-1;
                }
            }
            if (j<l){
                temp=j+1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp+1;
                }
            }
        }
    }
    if (preMove=='4'){//knight moves
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][4][ ][4][ ][ ][ ]
//[ ][4][ ][ ][ ][4][ ][ ]
//[ ][ ][ ] 4 [ ][ ][ ][ ]
//[ ][4][ ][ ][ ][4][ ][ ]
//[ ][ ][4][ ][4][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
        if (i-1==k && j+2==l){
            return 1;
        }
        if (i+1==k && j+2==l){
            return 1;
        }
        if (i+2==k && j+1==l){
            return 1;
        }
        if (i+2==k && j-1==l){
            return 1;
        }
        if (i+1==k && j-2==l){
            return 1;
        }
        if (i-1==k && j-2==l){
            return 1;
        }
        if (i-2==k && j-1==l){
            return 1;
        }
        if (i-2==k && j+1==l){
            return 1;
        }
    }
    if (preMove=='3'){//bishop moves
//[ ][ ][ ][ ][ ][ ][ ][3]
//[3][ ][ ][ ][ ][ ][3][ ]
//[ ][3][ ][ ][ ][3][ ][ ]
//[ ][ ][3][ ][3][ ][ ][ ]
//[ ][ ][ ] 3 [ ][ ][ ][ ]
//[ ][ ][3][ ][3][ ][ ][ ]
//[ ][3][ ][ ][ ][3][ ][ ]
//[3][ ][ ][ ][ ][ ][3][ ]
        if (i>k && j<l){
            for (temp=i-1, temp1=j+1; ; temp--, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j<l){
            for (temp=i+1, temp1=j+1; ; temp++, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j>l){
            for (temp=i+1, temp1=j-1; ; temp++, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i>k && j>l){
            for (temp=i-1, temp1=j-1; ; temp--, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
    }
    if (preMove=='9'){//queen moves
//[ ][ ][ ][9][ ][ ][ ][9]
//[9][ ][ ][9][ ][ ][9][ ]
//[ ][9][ ][9][ ][9][ ][ ]
//[ ][ ][9][9][9][ ][ ][ ]
//[9][9][9] 9 [9][9][9][9]
//[ ][ ][9][9][9][ ][ ][ ]
//[ ][9][ ][9][ ][9][ ][ ]
//[9][ ][ ][9][ ][ ][9][ ]
        if (i>k && j==l){
            temp=i-1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp-1;
            }
        }
        if (i<k && j==l){
            temp=i+1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp+1;
            }
        }
        if (i==k){
            if (j>l){
                temp=j-1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp-1;
                }
            }
            if (j<l){
                temp=j+1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp+1;
                }
            }
        }
        if (i>k && j<l){
            for (temp=i-1, temp1=j+1; ; temp--, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j<l){
            for (temp=i+1, temp1=j+1; ; temp++, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j>l){
            for (temp=i+1, temp1=j-1; ; temp++, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i>k && j>l){
            for (temp=i-1, temp1=j-1; ; temp--, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
    }
    if (preMove=='0'){//king moves                      //[0][0][0]
        if (i==k && j+1==l){                            //[0] 0 [0]
            return 1;                                   //[0][0][0]
        }
        if (i+1==k && j+1==l){
            return 1;
        }
        if (i+1==k && j==l){
            return 1;
        }
        if (i+1==k && j-1==l){
            return 1;
        }
        if (i==k && j-1==l){
            return 1;
        }
        if (i-1==k && j-1==l){
            return 1;
        }
        if (i-1==k && j==l){
            return 1;
        }
        if (i-1==k && j+1==l){
            return 1;
        }
        if (i==k && j-2==l && xd==0 || i==k && j+2==l && xd==0){          //castle
            return castleWhite(l);
        }
    }
    return 0;
}

//is started by blackMove function if there is possibility for certain move
//check if player made a gg move, if it returns 1 the move was gg
//i, j are co-ordinates of piece before move (high, width)
//k, l are co-ordinates of piece after move (high, width)
//preMove tells what symbol was at (i,j)
//afterMove tells what symbol was at (k,l)
int xdd=0;
int blackCorrectMoves(char preMove, int afterMove, int i, int j, int k, int l){
    int temp, temp1;
    if (preMove=='P'){//pawn moves, 1 forward, kill slant
        if (i==1){
            if (k==i+2 && l==j && afterMove==' ' && board[2][j]==' ' || k==i+1 && j==l && afterMove==' '){
                return 1;
            }
        }
        if (i==4){
            if (k==i+1 && l==j-1 && board[i][j-1]=='1' && board[k][l]==' ' && whiteFlight[l]==1){
                board[i][j-1]=' ';
                return 1;
            }
            if (k==i+1 && l==j+1 && board[i][j+1]=='1' && board[k][l]==' ' && whiteFlight[l]==1){
                board[i][j+1]=' ';
                return 1;
            }
            if (k==i+1 && l==j && afterMove==' '){
                return 1;
            }
            if (k==i+1 && l==j-1 && afterMove!=' ' || k==i+1 && l==j+1 && afterMove!=' '){
                return 1;
            }
        }
        else {
            if (k==i+1 && l==j && afterMove==' '){
                return 1;
            }
            if (k==i+1 && l==j-1 && afterMove!=' ' || k==i+1 && l==j+1 && afterMove!=' '){
                return 1;
            }
        }
    }
    if (preMove=='W'){//rook moves
//[ ][ ][ ][W][ ][ ][ ][ ]
//[ ][ ][ ][W][ ][ ][ ][ ]
//[ ][ ][ ][W][ ][ ][ ][ ]
//[ ][ ][ ][W][ ][ ][ ][ ]
//[W][W][W] W [W][W][W][W]
//[ ][ ][ ][W][ ][ ][ ][ ]
//[ ][ ][ ][W][ ][ ][ ][ ]
//[ ][ ][ ][W][ ][ ][ ][ ]
        if (i>k && j==l){
            temp=i-1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp-1;
            }
        }
        if (i<k && j==l){
            temp=i+1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp+1;
            }
        }
        if (i==k){
            if (j>l){
                temp=j-1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp-1;
                }
            }
            if (j<l){
                temp=j+1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp+1;
                }
            }
        }
    }
    if (preMove=='S'){//knight moves
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][S][ ][S][ ][ ][ ]
//[ ][S][ ][ ][ ][S][ ][ ]
//[ ][ ][ ] S [ ][ ][ ][ ]
//[ ][S][ ][ ][ ][S][ ][ ]
//[ ][ ][S][ ][S][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
        if (i-1==k && j+2==l){
            return 1;
        }
        if (i+1==k && j+2==l){
            return 1;
        }
        if (i+2==k && j+1==l){
            return 1;
        }
        if (i+2==k && j-1==l){
            return 1;
        }
        if (i+1==k && j-2==l){
            return 1;
        }
        if (i-1==k && j-2==l){
            return 1;
        }
        if (i-2==k && j-1==l){
            return 1;
        }
        if (i-2==k && j+1==l){
            return 1;
        }
    }
    if (preMove=='G'){//bishop moves
//[ ][ ][ ][ ][ ][ ][ ][G]
//[G][ ][ ][ ][ ][ ][G][ ]
//[ ][G][ ][ ][ ][G][ ][ ]
//[ ][ ][G][ ][G][ ][ ][ ]
//[ ][ ][ ] G [ ][ ][ ][ ]
//[ ][ ][G][ ][G][ ][ ][ ]
//[ ][G][ ][ ][ ][G][ ][ ]
//[G][ ][ ][ ][ ][ ][G][ ]
        if (i>k && j<l){
            for (temp=i-1, temp1=j+1; ; temp--, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j<l){
            for (temp=i+1, temp1=j+1; ; temp++, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j>l){
            for (temp=i+1, temp1=j-1; ; temp++, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i>k && j>l){
            for (temp=i-1, temp1=j-1; ; temp--, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
    }
    if (preMove=='H'){//queen moves
//[ ][ ][ ][H][ ][ ][ ][H]
//[H][ ][ ][H][ ][ ][H][ ]
//[ ][H][ ][H][ ][H][ ][ ]
//[ ][ ][H][H][H][ ][ ][ ]
//[H][H][H] H [H][H][H][H]
//[ ][ ][H][H][H][ ][ ][ ]
//[ ][H][ ][H][ ][H][ ][ ]
//[H][ ][ ][H][ ][ ][H][ ]
        if (i>k && j==l){
            temp=i-1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp-1;
            }
        }
        if (i<k && j==l){
            temp=i+1;
            while (1){
                if (temp==k){
                    return 1;
                }
                if (board[temp][j]!=' '){
                    return 0;
                }
                temp=temp+1;
            }
        }
        if (i==k){
            if (j>l){
                temp=j-1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp-1;
                }
            }
            if (j<l){
                temp=j+1;
                while (1){
                    if (temp==l){
                        return 1;
                    }
                    if (board[i][temp]!=' '){
                        return 0;
                    }
                    temp=temp+1;
                }
            }
        }
        if (i>k && j<l){
            for (temp=i-1, temp1=j+1; ; temp--, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j<l){
            for (temp=i+1, temp1=j+1; ; temp++, temp1++) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i<k && j>l){
            for (temp=i+1, temp1=j-1; ; temp++, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
        if (i>k && j>l){
            for (temp=i-1, temp1=j-1; ; temp--, temp1--) {
                if (temp1==l && temp==k){
                    return 1;
                }
                if (board[temp][temp1]!=' '){
                    return 0;
                }
            }
        }
    }
    if (preMove=='K'){//king moves                      //[K][K][K]
        if (i==k && j+1==l){                            //[K] K [K]
            return 1;                                   //[K][K][K]
        }
        if (i+1==k && j+1==l){
            return 1;
        }
        if (i+1==k && j==l){
            return 1;
        }
        if (i+1==k && j-1==l){
            return 1;
        }
        if (i==k && j-1==l){
            return 1;
        }
        if (i-1==k && j-1==l){
            return 1;
        }
        if (i-1==k && j==l){
            return 1;
        }
        if (i-1==k && j+1==l){
            return 1;
        }
        if (i==k && j-2==l & xdd==0|| i==k && j+2==l & xdd==0){          //castle
            return castleBlack(l);
        }
    }
    return 0;
}

//is started by whiteMove function after "making test move"
//check if player made a gg move, if it returns 0 the move was gg
//i, j are co-ordinates of piece before move (high, width)
//k, l are co-ordinates of piece after move (high, width)
//preMove tells what symbol was at (i,j)
//afterMove tells what symbol was at (k,l)
//x1 and y1 are used to remember where is the white king
//x2 and y2 are used to remember where piece is
//count is used to remember how big is free space between king and piece
int checkTheCheckOnWhite(int preMove, char afterMove, int i, int j, int k, int l, char b[8][8], int kc){
    char tempBoard[8][8];
    int x, y, z, c, x1, y1, x2, y2, count;
    for (x = 0; x < 8; x++) {                   //copying the board to tempBoard
        for (y = 0; y < 8; y++) {
            tempBoard[x][y]=board[x][y];
        }
    }
    if (kc==2){
        for (x = 0; x < 8; x++) {                   //copying the board to tempBoard
            for (y = 0; y < 8; y++) {
                tempBoard[x][y]=b[x][y];
            }
        }
    }
    tempBoard[i][j]=' ';                        //cleaning the place before move
    tempBoard[k][l]=preMove;                    //making the move
    for (x = 0; x < 8; x++) {                   //looking for white king
        for (y = 0; y < 8; y++) {
            if (tempBoard[x][y]=='0'){
                x1=x;
                y1=y;
            }
        }
    }
    //checking if pawn can kill king
    if (tempBoard[x1-1][y1-1]=='P' || tempBoard[x1-1][y1+1]=='P'){
        return 1;
    }
    //checking if rook can kill king
    for (x = 0; x < 8; x++) {
        if (tempBoard[x][y1]=='W'){ //looking for rock
            count=0;
            x2=x;
            if (x1>x2){                             //situation:    W
                for (z=x2+1; z<x1; z++) {           //              0
                    if (tempBoard[z][y1]==' '){     //rook is above the king
                        count++;
                    }
                    if (count==x1-x2-1){
                        return 1;
                    }
                }
                if (x1-1==x2){
                    return 1;
                }
            }
            if (x1<x2){                                 //situation:    O
                for (z=x1+1; z<x2; z++) {               //              W
                    if (tempBoard[z][y1]==' '){         //rook is under the king
                        count++;
                    }
                    if (count==x2-x1-1){
                        return 1;
                    }
                }
                if (x1+1==x2){
                    return 1;
                }
            }
        }
        if (tempBoard[x1][x]=='W'){
            count=0;
            y2=x;
            if (y1>y2){                             //situation:    WO
                for (c=y2+1; c<y1; c++) {           //rook is on the left of the king
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y1-y2-1){
                        return 1;
                    }
                }
                if (y1-1==y2){
                    return 1;
                }
            }
            if (y1<y2){                             //situation:    OW
                for (c=y1+1; c<y2; c++) {           //rook is on the right of the king
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y2-y1-1){
                        return 1;
                    }
                }
                if (y1+1==y2){
                    return 1;
                }
            }
        }
    }
    //knight check, any of those:
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][S][ ][S][ ][ ][ ]
//[ ][S][ ][ ][ ][S][ ][ ]
//[ ][ ][ ] 0 [ ][ ][ ][ ]
//[ ][S][ ][ ][ ][S][ ][ ]
//[ ][ ][S][ ][S][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
    if (tempBoard[x1+2][y1+1]=='S'){
        return 1;
    }
    if (tempBoard[x1+2][y1-1]=='S'){
        return 1;
    }
    if (tempBoard[x1+1][y1-2]=='S'){
        return 1;
    }
    if (tempBoard[x1-1][y1-2]=='S'){
        return 1;
    }
    if (tempBoard[x1-2][y1-1]=='S'){
        return 1;
    }
    if (tempBoard[x1-2][y1+1]=='S'){
        return 1;
    }
    if (tempBoard[x1-1][y1+2]=='S'){
        return 1;
    }
    if (tempBoard[x1+1][y1+2]=='S'){
        return 1;
    }
    //bishop check
    for (x=0; x<8; x++) {
        for (y=0; y<8; y++) {
            if (tempBoard[x][y]=='G'){                              //looking for bishop
                count=0;
                x2=x;
                y2=y;
                if (x1>x2 && y1<y2) {                               //situation:    G
                    if (x1-x2==y2-y1) {                             //             O
                        for (z=0; z<y2-y1-1; z++) {                 //bishop on the top right of the king
                            if (tempBoard[x1-1-z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1<y2) {                               //situation:    O
                    if (x2-x1==y2-y1) {                             //               G
                        for (z=0; z<y2-y1-1; z++) {                 //bishop on the down right of the king
                            if (tempBoard[x1+1+z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1>y2) {                               //situation:    O
                    if (x2-x1==y1-y2) {                             //             G
                        for (z=0; z<y1-y2-1; z++) {                 //bishop on the down left of the king
                            if (tempBoard[x1+1+z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y2+1==y1){
                            return 1;
                        }
                    }
                }
                if (x1>x2 && y1>y2) {                               //situation:    G
                    if (x1-x2==y1-y2) {                             //               O
                        for (z=0; z<y1-y2-1; z++) {                 //bishop on the top left of the king
                            if (tempBoard[x1-1-z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y2+1==y1){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    //Queen check, copied from bishop and rook
    for (x = 0; x < 8; x++) {
        if (tempBoard[x][y1]=='H'){
            count=0;
            x2=x;
            if (x1>x2){
                for (z=x2+1; z<x1; z++) {
                    if (tempBoard[z][y1]==' '){
                        count++;
                    }
                    if (count==x1-x2-1){
                        return 1;
                    }
                }
                if (x1-1==x2){
                    return 1;
                }
            }
            if (x1<x2){
                for (z=x1+1; z<x2; z++) {
                    if (tempBoard[z][y1]==' '){
                        count++;
                    }
                    if (count==x2-x1-1){
                        return 1;
                    }
                }
                if (x1+1==x2){
                    return 1;
                }
            }
        }
        if (tempBoard[x1][x]=='H'){
            count=0;
            y2=x;
            if (y1>y2){
                for (c=y2+1; c<y1; c++) {
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y1-y2-1){
                        return 1;
                    }
                }
                if (y1-1==y2){
                    return 1;
                }
            }
            if (y1<y2){
                for (c=y1+1; c<y2; c++) {
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y2-y1-1){
                        return 1;
                    }
                }
                if (y1+1==y2){
                    return 1;
                }
            }
        }
    }
    for (x=0; x<8; x++) {
        for (y=0; y<8; y++) {
            if (tempBoard[x][y]=='H'){
                count=0;
                x2=x;
                y2=y;
                if (x1>x2 && y1<y2) {
                    if (x1-x2==y2-y1) {
                        for (z=0; z<y2-y1-1; z++) {
                            if (tempBoard[x1-1-z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1<y2) {
                    if (x2-x1==y2-y1) {
                        for (z=0; z<y2-y1-1; z++) {
                            if (tempBoard[x1+1+z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1>y2) {
                    if (x2-x1==y1-y2) {
                        for (z=0; z<y1-y2-1; z++) {
                            if (tempBoard[x1+1+z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y2+1==y1){
                            return 1;
                        }
                    }
                }
                if (x1>x2 && y1>y2) {
                    if (x1-x2==y1-y2) {
                        for (z=0; z<y1-y2-1; z++) {
                            if (tempBoard[x1-1-z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y2+1==y1){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    //King check, any of those
//[O][O][O]
//[O] K [O]
//[O][O][O]
    for (x=0; x<8; x++) {
        for (y=0; y<8; y++) {
            if (tempBoard[x][y]=='K'){
                x2=x;
                y2=y;
                if (x1==x2 && y1+1==y2){
                    return 1;
                }
                if (x1+1==x2 && y1+1==y2){
                    return 1;
                }
                if (x1+1==x2 && y1==y2){
                    return 1;
                }
                if (x1+1==x2 && y1-1==y2){
                    return 1;
                }
                if (x1==x2 && y1-1==y2){
                    return 1;
                }
                if (x1-1==x2 && y1-1==y2){
                    return 1;
                }
                if (x1-1==x2 && y1==y2){
                    return 1;
                }
                if (x1-1==x2 && y1+1==y2){
                    return 1;
                }
            }
        }
    }
    return 0;
}

//is started by blackMove function after "making test move"
//check if player made a gg move, if it returns 0 the move was gg
//i, j are co-ordinates of piece before move (high, width)
//k, l are co-ordinates of piece after move (high, width)
//preMove tells what symbol was at (i,j)
//afterMove tells what symbol was at (k,l)
//x1 and y1 are used to remember where is the black king
//x2 and y2 are used to remember where piece is
//count is used to remember how big is free space between king and piece
int checkTheCheckOnBlack(char preMove, int afterMove, int i, int j, int k, int l, char b[8][8], int kc){
    char tempBoard[8][8];
    int x, y, z, c, x1, y1, x2, y2, count;
    for (x = 0; x < 8; x++) {                   //copying the board to tempBoard
        for (y = 0; y < 8; y++) {
            tempBoard[x][y]=board[x][y];
        }
    }
    if (kc==2){
        for (x = 0; x < 8; x++) {                   //copying the board to tempBoard
            for (y = 0; y < 8; y++) {
                tempBoard[x][y]=b[x][y];
            }
        }
    }
    tempBoard[i][j]=' ';                        //cleaning the place before move
    tempBoard[k][l]=preMove;                    //making the move
    for (x = 0; x < 8; x++) {                   //looking for black king
        for (y = 0; y < 8; y++) {
            if (tempBoard[x][y]=='K'){
                x1=x;
                y1=y;
            }
        }
    }
    //checking if pawn can kill king
    if (tempBoard[x1+1][y1+1]=='1' || tempBoard[x1+1][y1-1]=='1'){
        return 1;
    }
    //checking if rook can kill king
    for (x = 0; x < 8; x++) {
        if (tempBoard[x][y1]=='5'){                     //looking for rock
            count=0;
            x2=x;
            if (x1>x2){
                //situation:    5
                //              K
                //rook is above the king
                for (z=x2+1; z<x1; z++) {
                    if (tempBoard[z][y1]==' '){
                        count++;
                    }
                    if (count==x1-x2-1){
                        return 1;
                    }
                }
                if (x1-1==x2){
                    return 1;
                }
            }
            if (x1<x2){
                //situation:    K
                //              5
                //rook is under the king
                for (z=x1+1; z<x2; z++) {
                    if (tempBoard[z][y1]==' '){
                        count++;
                    }
                    if (count==x2-x1-1){
                        return 1;
                    }
                }
                if (x1+1==x2){
                    return 1;
                }
            }
        }
        if (tempBoard[x1][x]=='5'){
            count=0;
            y2=x;
            if (y1>y2){
                //situation:    5K
                //rook is on the left of the king
                for (c=y2+1; c<y1; c++) {
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y1-y2-1){
                        return 1;
                    }
                }
                if (y1-1==y2){
                    return 1;
                }
            }
            if (y1<y2){
                //situation:    K5
                //rook is on the right of the king
                for (c=y1+1; c<y2; c++) {
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y2-y1-1){
                        return 1;
                    }
                }
                if (y1+1==y2){
                    return 1;
                }
            }
        }
    }
    //knight check, any of those:
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
//[ ][ ][4][ ][4][ ][ ][ ]
//[ ][4][ ][ ][ ][4][ ][ ]
//[ ][ ][ ] K [ ][ ][ ][ ]
//[ ][4][ ][ ][ ][4][ ][ ]
//[ ][ ][4][ ][4][ ][ ][ ]
//[ ][ ][ ][ ][ ][ ][ ][ ]
    if (tempBoard[x1+2][y1+1]=='4'){
        return 1;
    }
    if (tempBoard[x1+2][y1-1]=='4'){
        return 1;
    }
    if (tempBoard[x1+1][y1-2]=='4'){
        return 1;
    }
    if (tempBoard[x1-1][y1-2]=='4'){
        return 1;
    }
    if (tempBoard[x1-2][y1-1]=='4'){
        return 1;
    }
    if (tempBoard[x1-2][y1+1]=='4'){
        return 1;
    }
    if (tempBoard[x1-1][y1+2]=='4'){
        return 1;
    }
    if (tempBoard[x1+1][y1+2]=='4'){
        return 1;
    }
    //bishop check
    for (x=0; x<8; x++) {
        for (y=0; y<8; y++) {
            if (tempBoard[x][y]=='3'){                                  //looking for bishop
                count=0;
                x2=x;
                y2=y;
                if (x1>x2 && y1<y2) {
                    //situation:    3
                    //             K
                    //bishop on the top right of the king
                    if (x1-x2==y2-y1) {
                        for (z=0; z<y2-y1-1; z++) {
                            if (tempBoard[x1-1-z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1<y2) {
                    //situation:    K
                    //               3
                    //bishop on the down right of the king
                    if (x2-x1==y2-y1) {
                        for (z=0; z<y2-y1-1; z++) {
                            if (tempBoard[x1+1+z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1>y2) {
                    //situation:    K
                    //             3
                    //bishop on the down left of the king
                    if (x2-x1==y1-y2) {
                        for (z=0; z<y1-y2-1; z++) {
                            if (tempBoard[x1+1+z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y2+1==y1){
                            return 1;
                        }
                    }
                }
                if (x1>x2 && y1>y2) {
                    //situation:    3
                    //               K
                    //bishop on the top left of the king
                    if (x1-x2==y1-y2) {
                        for (z=0; z<y1-y2-1; z++) {
                            if (tempBoard[x1-1-z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y2+1==y1){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    //Queen check, copied from bishop and rook
    for (x = 0; x < 8; x++) {
        if (tempBoard[x][y1]=='9'){
            count=0;
            x2=x;
            if (x1>x2){
                for (z=x2+1; z<x1; z++) {
                    if (tempBoard[z][y1]==' '){
                        count++;
                    }
                    if (count==x1-x2-1){
                        return 1;
                    }
                }
                if (x1-1==x2){
                    return 1;
                }
            }
            if (x1<x2){
                for (z=x1+1; z<x2; z++) {
                    if (tempBoard[z][y1]==' '){
                        count++;
                    }
                    if (count==x2-x1-1){
                        return 1;
                    }
                }
                if (x1+1==x2){
                    return 1;
                }
            }
        }
        if (tempBoard[x1][x]=='9'){
            count=0;
            y2=x;
            if (y1>y2){
                for (c=y2+1; c<y1; c++) {
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y1-y2-1){
                        return 1;
                    }
                }
                if (y1-1==y2){
                    return 1;
                }
            }
            if (y1<y2){
                for (c=y1+1; c<y2; c++) {
                    if (tempBoard[x1][c]==' '){
                        count++;
                    }
                    if (count==y2-y1-1){
                        return 1;
                    }
                }
                if (y1+1==y2){
                    return 1;
                }
            }
        }
    }
    for (x=0; x<8; x++) {
        for (y=0; y<8; y++) {
            if (tempBoard[x][y]=='9'){
                count=0;
                x2=x;
                y2=y;
                if (x1>x2 && y1<y2) {
                    if (x1-x2==y2-y1) {
                        for (z=0; z<y2-y1-1; z++) {
                            if (tempBoard[x1-1-z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1<y2) {
                    if (x2-x1==y2-y1) {
                        for (z=0; z<y2-y1-1; z++) {
                            if (tempBoard[x1+1+z][y1+1+z]==' '){
                                count++;
                            }
                            if (count==y2-y1-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y1+1==y2){
                            return 1;
                        }
                    }
                }
                if (x1<x2 && y1>y2) {
                    if (x2-x1==y1-y2) {
                        for (z=0; z<y1-y2-1; z++) {
                            if (tempBoard[x1+1+z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x1+1==x2 && y2+1==y1){
                            return 1;
                        }
                    }
                }
                if (x1>x2 && y1>y2) {
                    if (x1-x2==y1-y2) {
                        for (z=0; z<y1-y2-1; z++) {
                            if (tempBoard[x1-1-z][y1-1-z]==' '){
                                count++;
                            }
                            if (count==y1-y2-1){
                                return 1;
                            }
                        }
                        if (x2+1==x1 && y2+1==y1){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    //King check, any of those
//[K][K][K]
//[K] 0 [K]
//[K][K][K]
    for (x=0; x<8; x++) {
        for (y=0; y<8; y++) {
            if (tempBoard[x][y]=='0'){
                x2=x;
                y2=y;
                if (x1==x2 && y1+1==y2){
                    return 1;
                }
                if (x1+1==x2 && y1+1==y2){
                    return 1;
                }
                if (x1+1==x2 && y1==y2){
                    return 1;
                }
                if (x1+1==x2 && y1-1==y2){
                    return 1;
                }
                if (x1==x2 && y1-1==y2){
                    return 1;
                }
                if (x1-1==x2 && y1-1==y2){
                    return 1;
                }
                if (x1-1==x2 && y1==y2){
                    return 1;
                }
                if (x1-1==x2 && y1+1==y2){
                    return 1;
                }
            }
        }
    }
    return 0;
}

//i, j are co-ordinates of piece before move (high, width)
//k, l are co-ordinates of piece after move (high, width)
//preMove tells what symbol was at (i,j)
//afterMove tells what symbol was at (k,l)
void blackMove(char b[8][8]){
    int i, j, k, l, x, afterMove;
    char preMove;
    xdd=0;
    printf("Black move!\n");
    while (1) {
        printf("Type square from which u wanna make a move:");
        scanf("%d %d", &i, &j);
        //check if player typed co-ordinates of black piece
        if (b[i][j] == 'P' || b[i][j] == 'W' || b[i][j] == 'S' || b[i][j] == 'G' || b[i][j] == 'H' || b[i][j] == 'K') {
            preMove = b[i][j];
            printf("Type square to which u wanna make a move:");
            scanf("%d %d", &k, &l);
            //check if player typed co-ordinates of free space or of white piece
            if (b[k][l] == '1' || b[k][l] == '5' || b[k][l] == '4' || b[k][l] == '3' || b[k][l] == '9' || b[k][l]==' ') {
                afterMove = b[k][l];
                //check if player want to make correct move
                //and check if there is no check on white after that move
                if (blackCorrectMoves(preMove, afterMove, i, j, k, l)==1 && checkTheCheckOnBlack(preMove, afterMove, i, j, k, l, board, 1)==0) {
                    //making the move
                    b[k][l] = preMove;
                    //this short loop is used to switch black pawn by running blackPawnSwitch function
                    for (x= 0; x<8; x++) {
                        if (board[7][x] == 'P') {
                            blackPawnSwitch(k, l);
                        }
                    }
                    break;
                }
                    //if he didnt he have to start move loop again from the beginning
                else {
                    printf("BAD MOVE!\n");
                }
            }
                //if he didnt he have to start move loop again from the beginning
            else {
                printf("BAD MOVE!\n");
            }
        }
            //if he didnt he have to start move loop again from the beginning
        else {
            printf("BAD MOVE!\n");
        }
    }
    //disappear the old piece
    board[i][j]=' ';
    //check if black can do a castle in his move
    castleCheckBlack(board);
    //printing the board
    writee(board);
    xdd++;
}

//i, j are co-ordinates of piece before move (high, width)
//k, l are co-ordinates of piece after move (high, width)
//preMove tells what symbol was at (i,j)
//afterMove tells what symbol was at (k,l)
void whiteMove(char b[8][8]){
    int i, j, k, l, x, preMove;
    char afterMove;
    xd=0;
    printf("White move!\n");
    while (1) {
        printf("Type square from which u wanna make a move:");
        scanf("%d %d", &i, &j);
        //check if player typed co-ordinates of white piece
        if (b[i][j]=='1' || b[i][j]=='5' || b[i][j]=='4' || b[i][j]=='3' || b[i][j]=='9' || b[i][j]=='0') {
            preMove = b[i][j];
            printf("Type square to which u wanna make a move:");
            scanf("%d %d", &k, &l);
            //check if player typed co-ordinates of free space or of black piece
            if (b[k][l] == ' ' || b[k][l] == 'W' || b[k][l] == 'S' || b[k][l] == 'G' || b[k][l] == 'H' || b[k][l] == 'P'){
                afterMove = b[k][l];
                //check if player want to make correct move
                //and check if there is no check on white after that move
                if (whiteCorrectMoves(preMove, afterMove, i, j, k, l)==1 && checkTheCheckOnWhite(preMove, afterMove, i, j, k, l, board, 1)==0) {
                    //making the move
                    b[k][l] = preMove;
                    //this short loop is used to switch white pawn by running whitePawnSwitch function
                    for (x= 0; x<8; x++) {
                        if (board[0][x]=='1') {
                            whitePawnSwitch(k, l);
                        }
                    }
                    break;
                }
                    //if he didnt he have to start move loop again from the beginning
                else {
                    printf("BAD MOVE!\n");
                }
            }
                //if he didnt he have to start move loop again from the beginning
            else {
                printf("BAD MOVE!\n");
            }
        }
            //if he didnt he have to start move loop again from the beginning
        else {
            printf("BAD MOVE!\n");
        }
    }
    //disappear the old piece
    board[i][j]=' ';
    //check if white can do a castle in his next move
    castleCheckWhite(board);
    //printing the board
    writee(board);
    xd++;
}

int minimaxBlack(char b[8][8], int *p, int *q, int *r, int *s, int deep, int turn){//, int alfa, int beta) {
    int result = ratingV1(b), maxResult, minResult, i, j, k, l, afterMoveBlack, preMoveWhite, x, y;
    int ptmp, qtmp, rtmp, stmp;
    char preMoveBlack, afterMoveWhite, before;
    if (deep<=0 || result>=WIN || result<=LOSE) {
        return result;
    }
    if (turn) {
        for (i=0, maxResult = 200*LOSE; i<8; i++) {
            for (j = 0; j < 8; j++) {
                if (b[i][j] == 'P' || b[i][j] == 'W' || b[i][j] == 'S' || b[i][j] == 'G' || b[i][j] == 'H' ||
                    b[i][j] == 'K') {
                    preMoveBlack = b[i][j];
                    for (k = 0; k < 8; k++) {
                        for (l = 0; l < 8; l++) {
                            if (b[k][l] == '1' || b[k][l] == '5' || b[k][l] == '4' || b[k][l] == '3' ||
                                b[k][l] == '9' || b[k][l] == ' ') {
                                afterMoveBlack = b[k][l];
                                if (blackCorrectMoves(preMoveBlack, afterMoveBlack, i, j, k, l) == 1 &&
                                    checkTheCheckOnBlack(preMoveBlack, afterMoveBlack, i, j, k, l, b, 2) == 0) {
                                    b[k][l] = preMoveBlack;
                                    b[i][j] = ' ';
                                    //castleCheckBlackV1(btemp);              //check if black can do a castle in his move
                                    result = minimaxBlack(b, &ptmp, &qtmp, &rtmp, &stmp, deep-1, !turn);//, alfa, beta);
                                    b[i][j] = preMoveBlack;
                                    b[k][l] = afterMoveBlack;
                                    if (result>maxResult) {
                                        maxResult = result;
                                        *p = i;
                                        *q = j;
                                        *r = k;
                                        *s = l;
                                    }
                                    /*if(maxResult>alfa){
                                        alfa=maxResult;
                                    }
                                    if(beta<=alfa){
                                        continue;
                                    }*/
                                }
                            }
                        }
                    }
                }
                if(b[i][j]==' '){
                    continue;
                }
            }
        }
        return maxResult;
    }
    else {
        for (i = 0, minResult = 200 * WIN; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (b[i][j] == '1' || b[i][j] == '5' || b[i][j] == '4' || b[i][j] == '3' || b[i][j] == '9' ||
                    b[i][j] == '0') {
                    preMoveWhite = b[i][j];
                    for (k = 0; k < 8; k++) {
                        for (l = 0; l < 8; l++) {
                            if (b[k][l] == ' ' || b[k][l] == 'W' || b[k][l] == 'S' || b[k][l] == 'G' ||
                                b[k][l] == 'H' || b[k][l] == 'P') {
                                afterMoveWhite = b[k][l];
                                if (whiteCorrectMoves(preMoveWhite, afterMoveWhite, i, j, k, l) == 1 &&
                                    checkTheCheckOnWhite(preMoveWhite, afterMoveWhite, i, j, k, l, b, 2) == 0) {
                                    b[k][l] = preMoveWhite;
                                    b[i][j] = ' ';
                                    //castleCheckWhiteV1(btemp);             //check if white can do a castle in his next move
                                    result = minimaxBlack(b, &ptmp, &qtmp, &rtmp, &stmp, deep-1, !turn);//, alfa, beta);
                                    b[i][j] = preMoveWhite;
                                    b[k][l] = afterMoveWhite;
                                    if (result < minResult) {
                                        minResult = result;
                                        *p = i;
                                        *q = j;
                                        *r = k;
                                        *s = l;
                                    }/*
                                    if(minResult<beta){
                                        beta=minResult;
                                    }
                                    if(beta<=alfa){
                                        continue;
                                    }*/
                                }
                            }
                        }
                    }
                }
                if(b[i][j]==' '){
                    continue;
                }
            }
        }
        return minResult;
    }
}

int main() {
    int x;
    setbuf(stdout, 0);                               //CLion bug
    int type=0;                                             //setting game type for 0
    type=start(type);                                       //choosing game type
    if (type==1) {                                          //player vs player
        clear(board);                                       //cleaning the board
        writee(board);                                      //printing the board
        beforeMoves(board);                                 //setting pieces
        writee(board);                                      //printing the board
        while (1) {                                         //if u dont see any gg move you have to surrender -> end program
            whiteFlightBeatBeforeMove(board);               //check if black will have possibility to do flight kill on white after his move
            whiteMove(board);                               //white move
            whiteFlightBeat(board);                         //check if black can do flight kill on white after his move
            blackFlightBeatBeforeMove(board);               //check if white will have possibility to do flight kill on black after his move
            blackMove(board);                               //black move
            blackFlightBeat(board);                         //check if white can do flight kill on black after his move
        }
    }
    if (type==2){                                               //player vs pc
        int p, q, r, s;                                         //those remembers i, ,j, k, l of PC
        char before;                                            //it is used just to make a move
        type=start(type);                                       //choosing game type
        if (type==1) {                                          //you gonna play white
            clear(board);                                       //cleaning the board
            writee(board);                                      //printing the board
            beforeMoves(board);                                 //setting pieces
            writee(board);                                      //printing the board
            while (1) {                                         //if u dont see any gg move you have to surrender -> end program
                whiteFlightBeatBeforeMove(board);               //check if black will have possibility to do flight kill on white after his move
                whiteMove(board);                               //white move
                whiteFlightBeat(board);                         //check if black can do flight kill on white after his move
                castleCheckBlack(board);                        //check if black can do a castle in his move
                blackFlightBeatBeforeMove(board);               //check if white will have possibility to do flight kill on black after his move
                minimaxBlack(board, &p, &q, &r, &s, A, 1);//, 1000*LOSE, 1000*WIN);   //black move
                printf("PC: (%d,%d)-->(%d,%d)\n", p, q, r, s);
                if (board[p][q]!=' ') {
                    before = board[p][q];
                    board[p][q] = ' ';
                    board[r][s] = before;
                    if (p == 0 && q == 4 && r == 0 && s == 6) {
                        board[0][5] = 'W';                                                        //moove the rook
                        board[0][7] = ' ';                                                        //clean the board
                    }
                    if (p == 0 && q == 4 && r == 0 && s == 2) {
                        board[0][3] = 'W';                                                        //moove the rook
                        board[0][0] = ' ';                                                        //clean the board
                    }
                    for (x = 0; x < 8; x++) {
                        if (board[7][x] == 'P') {
                            blackPawnSwitch(r, s);
                        }
                    }
                }
                else{
                    printf("PC SURRENDER, YOU WON!");
                    return 0;
                }
                castleCheckWhite(board);                        //check if white can do a castle in his next move
                castleCheckBlack(board);                        //check if black can do a castle in his next move
                writee(board);                                  //writing the board
                blackFlightBeat(board);                         //check if white can do flight kill on black after his move
            }
        }
        if (type==2){                                           //you gonna play black
            printf("not ready yet!!!");
            clear(board);                                       //cleaning the board
            writee(board);                                      //printing the board
            beforeMoves(board);                                 //setting pieces
            writee(board);                                      //printing the board
            while (1) {                                         //if u dont see any gg move you have to surrender -> end program
                castleCheckWhite(board);                        //check if white can do a castle in his move
                whiteFlightBeatBeforeMove(board);               //check if black will have possibility to do flight kill on white after his move
                //whiteMove(board);                               //white move
                whiteFlightBeat(board);                         //check if black can do flight kill on white after his move
                castleCheckBlack(board);                        //check if black can do a castle in his move
                blackFlightBeatBeforeMove(board);               //check if white will have possibility to do flight kill on black after his move
                blackMove(board);                               //black move
                blackFlightBeat(board);                         //check if white can do flight kill on black after his move
            }
        }
    }
    else return 0;
}