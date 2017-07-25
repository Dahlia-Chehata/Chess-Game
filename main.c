#include <stdio.h>
#include <stdlib.h>


int i,j,k=0,l,m=0,n,simulation=0;
char Board[10][10];
char EmptyBoard[10][10];
char Alternative[10][10];
char piecesout1[16];
char piecesout2[16];
char input[4]; //TO GET INPUT FROM THE USER
struct turn
{
    char board[10][10];
    char piecesout1[16];
    char piecesout2[16];
    int k;
    int m;
};
struct turn turns[1000];//TO SAVE MOVES

int maxTurn = 0;
int currentTurn = 0;
void copyPiece( char pieces[16], char pieces2[16]);
void copyBoard( char board[10][10], char board2[10][10]);
int player=1;
int row_1,col_1,row_2,col_2; //TO ACCESS ANY ELEMENT IN BOARD
void initialize_Board();
void displayPlatform();
int check_col_1(char x);
int check_row_1(char x);
int check_col_2(char x);
int check_row_2(char x);
char promotion;
void moveValidation();
void saveGame(void);
void loadGame(void);
void undo();
void redo();
int Apply_move(int row_1,int col_1,int row_2,int col_2);
int King1_row();
int King1_column();
int King2_row();
int King2_column();
int CheckKing_1(int x,int y);
int CheckKing_2(int x,int y);
int Check_mate_king1();
int Check_mate_king2();


int main()
{
    int resume=1;
    int checkKing_1,checkKing_2,check_mate_King1,check_mate_King2;

    for(i=0; i<10; i+=2)
        for(j=1; j<10; j+=2)
        {
            EmptyBoard[i][j]='.';
            EmptyBoard[i][j+1]='_';
        }

    for(i=1; i<10; i+=2)
        for(j=0; j<10; j+=2)
        {
            EmptyBoard[i][j]='.';
            EmptyBoard[i][j+1]='_';
        }



    initialize_Board();//FILL IN BOARD
    if(!simulation)
    {
        copyBoard( Board, turns[currentTurn].board);
        copyPiece( piecesout1, turns[currentTurn].piecesout1);
        copyPiece( piecesout2, turns[currentTurn].piecesout2);
        turns[currentTurn].k = k;
        turns[currentTurn].m = m;
        currentTurn++;
        maxTurn = currentTurn;
    }
    system("cls");
    displayPlatform(); //DISPLAY THE INITIAL VIEW OF THE GAME
    while(resume)
    {
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Alternative[i][j]=Board[i][j];
        if(player==1)
        {
            checkKing_1=CheckKing_1(King1_row(),King1_column());
            if(checkKing_1==1)
            {
                check_mate_King1=Check_mate_king1();
                if(!check_mate_King1)
                {
                    printf("Player(1)-Check !!\n");
                    simulation=0;
                    for(i=0; i<10; i++)
                        for(j=0; j<10; j++)
                            Board[i][j]=Alternative[i][j];
                }
                else
                {
                    printf("Check_mate player(1)!\nplayer(2) wins");
                    break;
                }
            }
            else
            {

                check_mate_King1=Check_mate_king1();
                if(check_mate_King1)
                {
                    printf("This game is draw by Stale_mate !!");
                    break;
                }
            }
        }
        else
        {
            checkKing_2=CheckKing_2(King2_row(),King2_column());
            if(checkKing_2==1)
            {
                check_mate_King2=Check_mate_king2();
                if(!check_mate_King2)
                {
                    printf("Player(2)-Check !!\n");
                    simulation=0;
                    for(i=0; i<10; i++)
                        for(j=0; j<10; j++)
                            Board[i][j]=Alternative[i][j];
                }
                else
                {
                    printf("Check_mate player(2)!\nplayer(1) wins");
                    break;
                }
            }
            else
            {
                check_mate_King2=Check_mate_king2();
                if(check_mate_King2)
                {
                    printf("This game is draw by Stale_mate !!");
                    break;
                }
            }
        }
        printf("Player(%d)\'s turn, enter your move: ",player);
        fflush(stdin);
        scanf("%c%c%c%c",&input[0],&input[1],&input[2],&input[3]);


        if(input[0]=='s'&&input[1]=='a'&&input[2]=='v'&&input[3]=='e')
        {
            fflush(stdin);
            saveGame();
        }
        else if(input[0]=='l'&&input[1]=='o'&&input[2]=='a'&&input[3]=='d')
        {
            fflush(stdin);
            loadGame();

        }
        else if(input[0]=='u'&&input[1]=='n'&&input[2]=='d'&&input[3]=='o')
        {
            if(maxTurn == currentTurn)
                currentTurn--;
            fflush(stdin);
            undo();
        }
        else if(input[0]=='r'&&input[1]=='e'&&input[2]=='d'&&input[3]=='o')
        {
            fflush(stdin);
            redo();
        }
        else
        {
            fflush(stdin);
            moveValidation();
            system("cls");
            displayPlatform();

            //SWITCH PLAYER:
            if(player==1)
                player++;
            else
                player=1;
        }
    }
    return 0;
}
//saving the game
void saveGame(void)
{

    FILE* save;
    save = fopen("save.txt","w");

    // use w to write

    if(save == NULL)

    {
        printf("ERROR: failed to open file");
        return;

    }


    fwrite(Board,sizeof(Board),sizeof(char),save);
    fwrite(piecesout1,sizeof(piecesout1),sizeof(char),save);
    fwrite(piecesout2,sizeof(piecesout2),sizeof(char),save);
    fwrite(&k,1,4,save);
    fwrite(&m,1,4,save);

    /*first parameter is the name of array to be written
    second parameter is number of elements replaced by sizeof(Board)
    third parameter is the size of the data type which is 1 byte here where char is 1 byte
    fourth parameter is the pointer to the file
    */
    fclose(save);

}
//loading the game

void loadGame(void)

{

    FILE* load;

    load = fopen("save.txt","r");

    if(load == NULL)


    {

        printf("ERROR: failed to open file");
        return;

    }

    fread(Board,100,1,load);
    fread(piecesout1,16,1,load);
    fread(piecesout2,16,1,load);
    fread(&k,1,4,load);
    fread(&m,1,4,load);
    if(player==1)
        player=1;
    else
        player++;
    system("cls");
    displayPlatform();
    fclose(load);


}
//copying board to another array
void copyBoard( char board[10][10], char board2[10][10])
{
    int counter=0,counter2=0;
    for(counter = 0; counter < 10; counter++)
    {
        for(counter2 = 0; counter2 < 10; counter2++)
        {
            board2[counter][counter2] = board[counter][counter2];

        }
    }
}
//copying pieces out to another array
void copyPiece( char pieces[16], char pieces2[16])
{
    int counter;
    for(counter = 0; counter < 16; counter++)
    {
        pieces2[counter] = pieces[counter];
    }
}
//UNDO
void undo()
{
    if( currentTurn > 0)
    {
        currentTurn--;
        copyBoard(turns[currentTurn].board, Board);
        copyPiece(turns[currentTurn].piecesout1,piecesout1);
        copyPiece(turns[currentTurn].piecesout2, piecesout2);
        k = turns[currentTurn].k;
        m = turns[currentTurn].m;
        if(player==1)
            player++;
        else
            player=1;
        system("cls");
        displayPlatform();
    }
    else
    {
        system("cls");
        displayPlatform();
        printf("You can't undo any further.\n");
    }
}
//REDO
void redo()
{
    if( currentTurn < maxTurn-1)
    {
        currentTurn++;
        copyBoard(turns[currentTurn].board, Board);
        copyPiece(turns[currentTurn].piecesout1,piecesout1);
        copyPiece(turns[currentTurn].piecesout2, piecesout2);
        k = turns[currentTurn].k;
        m = turns[currentTurn].m;
        if(player==1)
            player++;
        else
            player=1;
        system("cls");
        displayPlatform();
    }
    else
    {
        system("cls");
        displayPlatform();
        printf("You can't redo any further.\n");
    }

}

//FILLING THE BOARD:
void initialize_Board()
{

    for(i=1 ; i<9 ; i+=2)
        for(j=2 ; j<9 ; j+=2)
        {
            Board[i][j]='.';
            Board[j][i]='.';
        }

    for(i=1 ; i<9 ; i+=2)
        for(j=1 ; j<9 ; j+=2)
        {
            Board[i][j]='_';
            Board[j][i]='_';
        }

    for(i=2 ; i<9 ; i+=2)
        for(j=2 ; j<9 ; j+=2)
            Board[i][j]='_';

    for(j=1 ; j<9 ; j++)
    {
        Board[0][j]=64+j;
        Board[9][j]=64+j;
        Board[2][j]=80;
        Board[7][j]=112;
    }

    for(i=1 ; i<9 ; i++)
    {
        Board[i][0]=57-i;
        Board[i][9]=57-i;
    }

    Board[1][1]=Board[1][8]=82;
    Board[1][2]=Board[1][7]=78;
    Board[1][3]=Board[1][6]=66;
    Board[1][4]=81;
    Board[1][5]=75;

    Board[8][1]=Board[8][8]=114;
    Board[8][2]=Board[8][7]=110;
    Board[8][3]=Board[8][6]=98;
    Board[8][4]=113;
    Board[8][5]=107;

    Board[0][0]=Board[0][9]=Board[9][0]=Board[9][9]=32;

}

//VIEWING THE BOARD:
void displayPlatform()
{

    printf("======================================");
    printf("\n");
    for(l=0 ; l<m ; l++)

    {
        printf (" %c ",piecesout2[l]);


    }

    printf("\n");

    printf("======================================");
    printf("\n\n");
    for(i=0 ; i<10 ; i++)
    {
        for(j=0 ; j<10 ; j++)
        {
            printf(" %c ",Board[i][j]);

        }
        printf("\n\n");
    }
    printf("=====================================");
    printf("\n");

    for(l=0 ; l<k; l++)
    {


        printf(" %c ",piecesout1[l]);

    }


    printf("\n");
    printf("=====================================");

    printf("\n\n");



}

//CONVERTING INPUTS FROM CHAR TO INT TO DEAL WITH THE BOARD:
int check_col_1(char x)
{
    switch (x)
    {
    case('A'):
        col_1=1;
        break;
    case('B'):
        col_1=2;
        break;
    case('C'):
        col_1=3;
        break;
    case('D'):
        col_1=4;
        break;
    case('E'):
        col_1=5;
        break;
    case('F'):
        col_1=6;
        break;
    case('G'):
        col_1=7;
        break;
    case('H'):
        col_1=8;
        break;
    case('s'):
        break;
    case('l'):
        break;
    case('u'):
        break;
    case('n'):
        break;
    default:
        col_1=0;
    }
    return col_1;

}

int check_row_1(char x)
{
    switch (x)
    {
    case('8'):
        row_1=1;
        break;
    case('7'):
        row_1=2;
        break;
    case('6'):
        row_1=3;
        break;
    case('5'):
        row_1=4;
        break;
    case('4'):
        row_1=5;
        break;
    case('3'):
        row_1=6;
        break;
    case('2'):
        row_1=7;
        break;
    case('1'):
        row_1=8;
        break;
    case('a'):
        break;
    case('o'):
        break;
    case('n'):
        break;
    case('e'):
        break;
    default:
        row_1=0;

    }
    return row_1;
}

int check_col_2(char x)
{
    switch (x)
    {
    case('A'):
        col_2=1;
        break;
    case('B'):
        col_2=2;
        break;
    case('C'):
        col_2=3;
        break;
    case('D'):
        col_2=4;
        break;
    case('E'):
        col_2=5;
        break;
    case('F'):
        col_2=6;
        break;
    case('G'):
        col_2=7;
        break;
    case('H'):
        col_2=8;
        break;
    case('v'):
        break;
    case('a'):
        break;
    case('d'):
        break;

    default:
        col_2=0;
    }
    return col_2;
}

int check_row_2(char x)
{
    switch (x)
    {
    case('8'):
        row_2=1;
        break;
    case('7'):
        row_2=2;
        break;
    case('6'):
        row_2=3;
        break;
    case('5'):
        row_2=4;
        break;
    case('4'):
        row_2=5;
        break;
    case('3'):
        row_2=6;
        break;
    case('2'):
        row_2=7;
        break;
    case('1'):
        row_2=8;
        break;
    case ('e'):
        break;
    case('d') :
        break;
    case('o'):
        break;

    default:
        row_2=0;
    }
    return row_2;
}

//MOVE VALIDATION
void moveValidation()
{
label:
    ;
    int a,b,c,move,check;
    col_1 = check_col_1(input[0]);
    row_1 = check_row_1(input[1]);
    col_2 = check_col_2(input[2]);
    row_2 = check_row_2(input[3]);

    a = (Board[row_1][col_1]=='_' || Board[row_1][col_1]=='.' || (row_1==row_2 && col_1==col_2));
    b = (player==1 && (Board[row_1][col_1]=='P' || Board[row_1][col_1]=='R' || Board[row_1][col_1]=='N' || Board[row_1][col_1]=='B' ||
                       Board[row_1][col_1]=='Q' || Board[row_1][col_1]=='K' || Board[row_2][col_2]=='p' || Board[row_2][col_2]=='r' || Board[row_2][col_2]=='n' ||
                       Board[row_2][col_2]=='b' || Board[row_2][col_2]=='q' || Board[row_2][col_2]=='k'));
    c = (player==2 && (Board[row_2][col_2]=='P' || Board[row_2][col_2]=='R' || Board[row_2][col_2]=='N' || Board[row_2][col_2]=='B' ||
                       Board[row_2][col_2]=='Q' || Board[row_2][col_2]=='K' || Board[row_1][col_1]=='p' || Board[row_1][col_1]=='r' || Board[row_1][col_1]=='n' ||
                       Board[row_1][col_1]=='b' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='k'));
    if(player==1)
    {
        simulation=1;
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Alternative[i][j]=Board[i][j];
        move = Apply_move(row_1,col_1,row_2,col_2);
        check=CheckKing_1(King1_row(),King1_column());
    }
    else
    {
        simulation=1;
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Alternative[i][j]=Board[i][j];
        move = Apply_move(row_1,col_1,row_2,col_2);
        check=CheckKing_2(King2_row(),King2_column());
    }
    if(a==0 && b==0 && c==0 && check==0)
    {
        simulation=0;
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Board[i][j]=Alternative[i][j];
        move = Apply_move(row_1,col_1,row_2,col_2);
    }
    else
    {
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Board[i][j]=Alternative[i][j];
        printf("\aYour input is incorrect, please enter a valid move: ");
        fflush(stdin);
        scanf("%c%c%c%c",&input[0],&input[1],&input[2],&input[3]);
        if(input[0]=='s'&&input[1]=='a'&&input[2]=='v'&&input[3]=='e')
        {
            fflush(stdin);
            saveGame();
        }
        else if(input[0]=='l'&&input[1]=='o'&&input[2]=='a'&&input[3]=='d')
        {
            fflush(stdin);
            loadGame();

        }
        else if(input[0]=='u'&&input[1]=='n'&&input[2]=='d'&&input[3]=='o')
        {
            fflush(stdin);
            undo();
        }
        else if(input[0]=='r'&&input[1]=='e'&&input[2]=='d'&&input[3]=='o')
        {
            fflush(stdin);
            redo();
        }
        else
        {
            fflush(stdin);
            goto label;
        }
    }
    system("cls");
    displayPlatform();

    while(col_1==0 || row_1==0 || col_2==0 || row_2==0 || a || b || c || move==0)
    {
        printf("\aYour input is incorrect, please enter a valid move: ");
        scanf("%c%c%c%c",&input[0],&input[1],&input[2],&input[3]);
        if(input[0]=='s'&&input[1]=='a'&&input[2]=='v'&&input[3]=='e')
        {
            fflush(stdin);
            saveGame();
        }
        else if(input[0]=='l'&&input[1]=='o'&&input[2]=='a'&&input[3]=='d')
        {
            fflush(stdin);
            loadGame();

        }
        else if(input[0]=='u'&&input[1]=='n'&&input[2]=='d'&&input[3]=='o')
        {
            fflush(stdin);
            undo();
        }
        else if(input[0]=='r'&&input[1]=='e'&&input[2]=='d'&&input[3]=='o')
        {
            fflush(stdin);
            redo();
        }
        else
        {
            fflush(stdin);
            col_1 = check_col_1(input[0]);
            row_1 = check_row_1(input[1]);
            col_2 = check_col_2(input[2]);
            row_2 = check_row_2(input[3]);
            a = (Board[row_1][col_1]=='_' || Board[row_1][col_1]=='.' || (row_1==row_2 && col_1==col_2));
            b = (player==1 && (Board[row_1][col_1]=='P' || Board[row_1][col_1]=='R' || Board[row_1][col_1]=='N' || Board[row_1][col_1]=='B' ||
                               Board[row_1][col_1]=='Q' || Board[row_1][col_1]=='K' || Board[row_2][col_2]=='p' || Board[row_2][col_2]=='r' || Board[row_2][col_2]=='n' ||
                               Board[row_2][col_2]=='b' || Board[row_2][col_2]=='q' || Board[row_2][col_2]=='k'));
            c = (player==2 && (Board[row_2][col_2]=='P' || Board[row_2][col_2]=='R' || Board[row_2][col_2]=='N' || Board[row_2][col_2]=='B' ||
                               Board[row_2][col_2]=='Q' || Board[row_2][col_2]=='K' || Board[row_1][col_1]=='p' || Board[row_1][col_1]=='r' || Board[row_1][col_1]=='n' ||
                               Board[row_1][col_1]=='b' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='k'));

            move = Apply_move(row_1,col_1,row_2,col_2);

        }

    }

}

//APPLYING THE CORRECT MOVE:
int Apply_move(int row_1,int col_1,int row_2,int col_2)
{
    int flag=1;
    //CASE(1) OF ROOK/QUEEN MOVE

    if((Board[row_1][col_1]=='r' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='R' || Board[row_1][col_1]=='Q') && col_1==col_2 && row_2>row_1)
    {
        for(i=row_1+1 ; (i<row_2 && flag) ; i++)
        {
            if(Board[i][col_1]!= '.' && Board[i][col_1]!= '_')
                flag=0;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='r')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }



                    return 1;
                }
                else if(Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m++;
                    }
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {


                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if(Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }

                    return 1;
                }
            }

            else if(Board[row_1][col_1]=='R')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }

            else if(Board[row_1][col_1]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //CASE(2) OF ROOK/QUEEN MOVE
    else if((Board[row_1][col_1]=='r' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='R' || Board[row_1][col_1]=='Q') && col_1==col_2 && row_2<row_1)
    {
        for(i=row_1-1 ; (i>row_2 && flag) ; i--)
        {
            if(Board[i][col_1]!= '.' && Board[i][col_1]!= '_')
                flag=0;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='r')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')

                {
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }

            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='R')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if (Board[row_2][col_2]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //CASE(3) OF ROOK/QUEEN MOVE
    else if((Board[row_1][col_1]=='r' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='R' || Board[row_1][col_1]=='Q') && row_1==row_2 && col_2>col_1)
    {
        for(i=col_1+1 ; (i<col_2 && flag) ; i++)
        {
            if(Board[row_1][i]!= '.' && Board[row_1][i]!= '_')
                flag=0;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='r')
            {

                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')

                {
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }

                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;

                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='R')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }

            else if (Board[row_1][col_1]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //CASE(4) OF ROOK/QUEEN MOVE
    else if((Board[row_1][col_1]=='r' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='R' || Board[row_1][col_1]=='Q') && row_1==row_2 && col_2<col_1)
    {
        for(i=col_1-1 ; (i>col_2 && flag) ; i--)
        {
            if(Board[row_1][i]!= '.' && Board[row_1][i]!= '_')
                flag=0;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='r')

            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='r';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }

            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }

            else if(Board[row_1][col_1]=='R')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='R';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if (Board[row_1][col_1]=='Q')
            {

                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    copyBoard( Board, turns[currentTurn].board);
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //CASE(1) OF BISHOP/QUEEN MOVE
    else if((Board[row_1][col_1]=='b' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='B' || Board[row_1][col_1]=='Q') && (row_1-row_2)==(col_1-col_2) && row_2>row_1)
    {
        j=col_1+1;
        for(i=row_1+1 ; (i<row_2 && flag) ; i++)
        {
            if(Board[i][j]!= '.' && Board[i][j]!= '_')
                flag=0;
            else
                j++;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='b')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='q')
            {

                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='B')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {
                    Board[row_2][col_2]='B';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='B';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }


    //CASE(2) OF BISHOP/QUEEN MOVE
    else if((Board[row_1][col_1]=='b' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='B' || Board[row_1][col_1]=='Q') && (row_1-row_2)==(-1*(col_1-col_2)) && row_2>row_1)
    {
        j=col_1-1;
        for(i=row_1+1 ; (i<row_2 && flag) ; i++)
        {
            if(Board[i][j]!= '.' && Board[i][j]!= '_')
                flag=0;
            else
                j--;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='b')

            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')

                {
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='B')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {
                    Board[row_2][col_2]='B';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='B';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if (Board[row_1][col_1]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //CASE(3) OF BISHOP/QUEEN MOVE
    else if((Board[row_1][col_1]=='b' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='B' || Board[row_1][col_1]=='Q') && (row_1-row_2)==(col_1-col_2) && row_2<row_1)
    {
        j=col_1-1;
        for(i=row_1-1 ; (i>row_2 && flag) ; i--)
        {
            if(Board[i][j]!= '.' && Board[i][j]!= '_')
                flag=0;
            else
                j--;
        }
        if(flag==1)
        {
            if(Board[row_1][col_1]=='b')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {


                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='B')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='B';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='B';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if (Board[row_2][col_2]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //CASE(4) OF BISHOP/QUEEN MOVE
    else if((Board[row_1][col_1]=='b' || Board[row_1][col_1]=='q' || Board[row_1][col_1]=='B' || Board[row_1][col_1]=='Q') && (row_1-row_2)==(-1*(col_1-col_2)) && row_2<row_1)
    {
        j=col_1+1;
        for(i=row_1-1 ; (i>row_2 && flag) ; i--)
        {
            if(Board[i][j]!= '.' && Board[i][j]!= '_')
                flag=0;
            else
                j++;
        }
        if(flag==1)
        {

            if(Board[row_1][col_1]=='b')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='b';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
                {
                    if(!simulation)
                    {
                        piecesout2[m]=Board[row_2][col_2];
                        m ++;
                    }
                    Board[row_2][col_2]='q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else if(Board[row_1][col_1]=='B')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')

                    Board[row_2][col_2]='B';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
            else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
            {
                if(!simulation)
                {
                    piecesout1[k]=Board[row_2][col_2];
                    k ++;
                }
                Board[row_2][col_2]='B';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }

            else if (Board[row_1][col_1]=='Q')
            {
                if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
                {

                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
                {
                    if(!simulation)
                    {
                        piecesout1[k]=Board[row_2][col_2];
                        k ++;
                    }
                    Board[row_2][col_2]='Q';
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
        }
        return 0;
    }

    //KNIGHT MOVE
    else if((Board[row_1][col_1]=='n' || Board[row_1][col_1]=='N') && (
                (row_2==(row_1+1) && col_2==(col_1+2)) ||
                (row_2==(row_1+1) && col_2==(col_1-2)) ||
                (row_2==(row_1-1) && col_2==(col_1+2)) ||
                (row_2==(row_1-1) && col_2==(col_1-2)) ||
                (row_2==(row_1+2) && col_2==(col_1+1)) ||
                (row_2==(row_1+2) && col_2==(col_1-1)) ||
                (row_2==(row_1-2) && col_2==(col_1+1)) ||
                (row_2==(row_1-2) && col_2==(col_1-1)) ))
    {
        if(Board[row_1][col_1]=='n')
        {
            if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
            {

                Board[row_2][col_2]='n';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
            else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
            {
                if(!simulation)
                {
                    piecesout2[m]=Board[row_2][col_2];
                    m ++;
                }
                Board[row_2][col_2]='n';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }
        else if(Board[row_1][col_1]=='N')
        {
            if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')

            {
                Board[row_2][col_2]='N';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
            else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
            {
                if(!simulation)
                {
                    piecesout1[k]=Board[row_2][col_2];
                    k ++;
                }
                Board[row_2][col_2]='N';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }
        return 0;
    }

    //KING MOVE
    else if((Board[row_1][col_1]=='k' || Board[row_1][col_1]=='K') && (
                (row_1==row_2 && (col_1-col_2==1)) ||
                (row_1==row_2 && (col_1-col_2==-1)) ||
                (col_1==col_2 && (row_1-row_2==1)) ||
                (col_1==col_2 && (row_1-row_2==-1)) ||
                (row_1==(row_2+1) && col_1==(col_2+1)) ||
                (row_1==(row_2+1) && col_1==(col_2-1)) ||
                (row_1==(row_2-1) && col_1==(col_2+1)) ||
                (row_1==(row_2-1) && col_1==(col_2-1)) ))
    {
        if(Board[row_1][col_1]=='k')
        {
            if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
            {

                Board[row_2][col_2]='k';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
            else if (Board[row_2][col_2]=='P'||Board[row_2][col_2]=='Q'||Board[row_2][col_2]=='R'||Board[row_2][col_2]=='B'||Board[row_2][col_2]=='K'||Board[row_2][col_2]=='N')
            {
                if(!simulation)
                {
                    piecesout2[m]=Board[row_2][col_2];
                    m ++;
                }
                Board[row_2][col_2]='k';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }
        else if(Board[row_1][col_1]=='K')

        {
            if(Board[row_2][col_2]=='.'||Board[row_2][col_2]=='_')
            {

                Board[row_2][col_2]='K';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
            else if (Board[row_2][col_2]=='p'||Board[row_2][col_2]=='q'||Board[row_2][col_2]=='r'||Board[row_2][col_2]=='b'||Board[row_2][col_2]=='k'||Board[row_2][col_2]=='n')
            {
                if(!simulation)
                {
                    piecesout1[k]=Board[row_2][col_2];
                    k ++;
                }
                Board[row_2][col_2]='K';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }
        return 0;
    }

    //PAWN MOVE ,, PLAYER(1)
    else if(Board[row_1][col_1]=='p')
    {
        if((col_1==col_2 && row_1==(row_2+1) && (Board[row_2][col_2]=='.' || Board[row_2][col_2]=='_')&& row_2!=1&&row_1!=2)
                ||(col_1==col_2 && row_1==(row_2+2) && (Board[row_2+1][col_1]=='.' || Board[row_2+1][col_1]=='_')&& (Board[row_2][col_2]=='.' || Board[row_2][col_2]=='_') && row_1==7))
        {
            Board[row_2][col_2]='p';
            Board[row_1][col_1]=EmptyBoard[row_1][col_1];
            if(!simulation)
            {
                copyBoard( Board, turns[currentTurn].board);
                copyPiece( piecesout1, turns[currentTurn].piecesout1);
                copyPiece( piecesout2, turns[currentTurn].piecesout2);
                turns[currentTurn].k = k;
                turns[currentTurn].m = m;
                currentTurn++;
                maxTurn = currentTurn;
            }
            return 1;
        }
        //pawn promotion for player1 (case 1)
        else if(col_1==col_2 &&  (Board[row_2][col_2]=='.' || Board[row_2][col_2]=='_')&& row_2==1&&row_1==2)

        {
            if(!simulation)
            {
                printf("\nenter your promotion:");
                fflush(stdin);
                scanf("%c",&promotion);
                if(promotion =='n'||promotion =='q'||promotion =='r'||promotion =='b')
                {
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else
                {
                    while (promotion !='n'&&promotion !='q'&&promotion !='r'&&promotion !='b')
                    {
                        printf("\n\anot a valid promotion:");
                        fflush(stdin);
                        scanf("%c",&promotion);
                    }
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else
            {
                Board[row_2][col_2]='p';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }

        }
        else if((col_1==(col_2+1) || (col_1==(col_2-1))) && row_1==(row_2+1) && (Board[row_2][col_2]=='P' || Board[row_2][col_2]=='R' || Board[row_2][col_2]=='N'
                || Board[row_2][col_2]=='B' || Board[row_2][col_2]=='Q' || Board[row_2][col_2]=='K')&& row_2!=1&&row_1!=2)
        {
            if(!simulation)
            {
                piecesout2[m]=Board[row_2][col_2];
                m ++;
            }
            Board[row_2][col_2]='p';
            Board[row_1][col_1]=EmptyBoard[row_1][col_1];
            if(!simulation)
            {
                copyBoard( Board, turns[currentTurn].board);
                copyPiece( piecesout1, turns[currentTurn].piecesout1);
                copyPiece( piecesout2, turns[currentTurn].piecesout2);
                turns[currentTurn].k = k;
                turns[currentTurn].m = m;
                currentTurn++;
                maxTurn = currentTurn;
            }
            return 1;
        }
        //pawn promotion for player1 (case 2)
        else if((col_1==(col_2+1) || (col_1==(col_2-1)))  && (Board[row_2][col_2]=='P' || Board[row_2][col_2]=='R' || Board[row_2][col_2]=='N'
                || Board[row_2][col_2]=='B' || Board[row_2][col_2]=='Q' || Board[row_2][col_2]=='K')&& row_2==1&&row_1==2)
        {
            if(!simulation)
            {
                printf("\nenter your promotion:");
                fflush(stdin);
                scanf("%c",&promotion);
                if(promotion =='n'||promotion =='q'||promotion =='r'||promotion =='b')
                {
                    piecesout2[m]=Board[row_2][col_2];
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    m++;
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else
                {
                    while (promotion !='n'&&promotion !='q'&&promotion !='r'&&promotion !='b')
                    {
                        printf("\n\anot a valid promotion:");
                        fflush(stdin);
                        scanf("%c",&promotion);
                    }
                    piecesout2[m]=Board[row_2][col_2];
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    m++;
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else
            {
                Board[row_2][col_2]='p';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }
        return 0;
    }
    //PAWN MOVE ,, PLAYER(2)
    else if(Board[row_1][col_1]=='P')
    {
        if ((col_1==col_2 && row_1==(row_2-1)  && (Board[row_2][col_2]=='.' || Board[row_2][col_2]=='_')&&row_1!=7&&row_2!=8) ||
                (col_1==col_2 && row_1==(row_2-2) && (Board[row_2-1][col_1]=='.' || Board[row_2-1][col_1]=='_')&& (Board[row_2][col_2]=='.' || Board[row_2][col_2]=='_') && row_1==2))
        {
            Board[row_2][col_2]='P';
            Board[row_1][col_1]=EmptyBoard[row_1][col_1];
            if(!simulation)
            {
                copyBoard( Board, turns[currentTurn].board);
                copyPiece( piecesout1, turns[currentTurn].piecesout1);
                copyPiece( piecesout2, turns[currentTurn].piecesout2);
                turns[currentTurn].k = k;
                turns[currentTurn].m = m;
                currentTurn++;
                maxTurn = currentTurn;
            }
            return 1;
        }

        //pawn promotion for player2 (case 1)
        else  if  (col_1==col_2  && (Board[row_2][col_2]=='.' || Board[row_2][col_2]=='_')&&row_2==8&&row_1==7)
        {
            if(!simulation)
            {
                printf("\nenter your promotion:");
                fflush(stdin);
                scanf("%c",&promotion);
                if(promotion =='N'||promotion =='Q'||promotion =='R'||promotion =='B')
                {
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else
                {
                    while (promotion !='N'&&promotion !='Q'&&promotion !='R'&&promotion !='B')
                    {
                        printf("\n\anot a valid promotion:");
                        fflush(stdin);
                        scanf("%c",&promotion);
                    }
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else
            {
                Board[row_2][col_2]='p';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }

        else if((col_1==(col_2+1) || (col_1==(col_2-1))) && row_1==(row_2-1) && (Board[row_2][col_2]=='p' || Board[row_2][col_2]=='r' || Board[row_2][col_2]=='n'
                || Board[row_2][col_2]=='b' || Board[row_2][col_2]=='q' || Board[row_2][col_2]=='k')&&row_1!=7&&row_2!=8)
        {
            if(!simulation)
            {
                piecesout1[k]=Board[row_2][col_2];
                k ++;
            }
            Board[row_2][col_2]='P';
            Board[row_1][col_1]=EmptyBoard[row_1][col_1];
            if(!simulation)
            {
                copyBoard( Board, turns[currentTurn].board);
                copyPiece( piecesout1, turns[currentTurn].piecesout1);
                copyPiece( piecesout2, turns[currentTurn].piecesout2);
                turns[currentTurn].k = k;
                turns[currentTurn].m = m;
                currentTurn++;
                maxTurn = currentTurn;
            }
            return 1;
        }
        //pawn promotion for player2 (case 2)
        else  if((col_1==(col_2+1) || (col_1==(col_2-1))) && row_2==8&&row_1==7 && (Board[row_2][col_2]=='p' || Board[row_2][col_2]=='r' || Board[row_2][col_2]=='n'
                 || Board[row_2][col_2]=='b' || Board[row_2][col_2]=='q' || Board[row_2][col_2]=='k'))
        {
            if(!simulation)
            {
                printf("\nenter your promotion:");
                fflush(stdin);
                scanf("%c",&promotion);
                if(promotion =='N'||promotion =='Q'||promotion =='R'||promotion =='B')
                {
                    piecesout1[k]=Board[row_2][col_2];
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    k++;
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
                else
                {
                    while (promotion !='N'&&promotion !='Q'&&promotion !='R'&&promotion !='B')
                    {
                        printf("\n\anot a valid promotion:");
                        fflush(stdin);
                        scanf("%c",&promotion);
                    }
                    piecesout1[k]=Board[row_2][col_2];
                    Board[row_2][col_2]=promotion;
                    Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                    k++;
                    if(!simulation)
                    {
                        copyBoard( Board, turns[currentTurn].board);
                        copyPiece( piecesout1, turns[currentTurn].piecesout1);
                        copyPiece( piecesout2, turns[currentTurn].piecesout2);
                        turns[currentTurn].k = k;
                        turns[currentTurn].m = m;
                        currentTurn++;
                        maxTurn = currentTurn;
                    }
                    return 1;
                }
            }
            else
            {
                Board[row_2][col_2]='P';
                Board[row_1][col_1]=EmptyBoard[row_1][col_1];
                if(!simulation)
                {
                    copyBoard( Board, turns[currentTurn].board);
                    copyPiece( piecesout1, turns[currentTurn].piecesout1);
                    copyPiece( piecesout2, turns[currentTurn].piecesout2);
                    turns[currentTurn].k = k;
                    turns[currentTurn].m = m;
                    currentTurn++;
                    maxTurn = currentTurn;
                }
                return 1;
            }
        }
        else
            return 0;

    }
    return 0;
}

int King1_row()
{
    //LOCATING THE ROW OF PLAYER(1)'S KING
    int king=0; //x_y DETERMINE THE EXACT LOCATION OF KING ON THE BOARD
    for(i=1; (i<9 && !king); i++)
        for(j=1; (j<9 && !king); j++)
        {
            if(Board[i][j]=='k')
                king=1;
        }
    return(i-1);
}

int King1_column()
{
    //LOCATING THE COLUMN OF PLAYER(1)'S KING
    int king=0; //x_y DETERMINE THE EXACT LOCATION OF KING ON THE BOARD
    for(i=1; (i<9 && !king); i++)
        for(j=1; (j<9 && !king); j++)
        {
            if(Board[i][j]=='k')
                king=1;
        }
    return(j-1);
}


int CheckKing_1(int x,int y)
{
    int check;
    //ROOK OR QUEEN ON THE RIGHT
    int check_1=0,found=0;
    for(i=y+1; (i<9 && !found && !check_1); i++)
    {
        if(Board[x][i]!='.' && Board[x][i]!='_')
        {
            if(Board[x][i]=='R' || Board[x][i]=='Q')
                check_1=1;
            else
                found=1;
        }
    }

    //ROOK OR QUEEN ON THE LEFT
    int check_2=0;
    found=0;
    for(i=y-1; (i>0 && !found && !check_2); i--)
    {
        if(Board[x][i]!='.' && Board[x][i]!='_')
        {
            if(Board[x][i]=='R' || Board[x][i]=='Q')
                check_2=1;
            else
                found=1;
        }
    }

    //ROOK OR QUEEN UP
    int check_3=0;
    found=0;
    for(i=x-1; (i>0 && !found && !check_3); i--)
    {
        if(Board[i][y]!='.' && Board[i][y]!='_')
        {
            if(Board[i][y]=='R' || Board[i][y]=='Q')
                check_3=1;
            else
                found=1;
        }
    }

    //ROOK OR QUEEN DOWN
    int check_4=0;
    found=0;
    for(i=x+1; (i<9 && !found && !check_4); i++)
    {
        if(Board[i][y]!='.' && Board[i][y]!='_')
        {
            if(Board[i][y]=='R' || Board[i][y]=='Q')
                check_4=1;
            else
                found=1;
        }
    }

    //BISHOP OR QUEEN LOWER-RIGHT CORNER
    found=0;
    int check_5=0;
    j=y+1;
    for(i=x+1; (i<9 && !found && !check_5); i++)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='B' || Board[i][j]=='Q')
                check_5=1;
            else
                found=1;
        }
        j++;
    }

    //BISHOP OR QUEEN UPPER-LEFT CORNER
    found=0;
    int check_6=0;
    j=y-1;
    for(i=x-1; (i>0 && !found && !check_6); i--)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='B' || Board[i][j]=='Q')
                check_6=1;
            else
                found=1;
        }
        j--;
    }

    //BISHOP OR QUEEN LOWER-LEFT CORNER
    found=0;
    int check_7=0;
    j=y-1;
    for(i=x+1; (i<9 && !found && !check_7); i++)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='B' || Board[i][j]=='Q')
                check_7=1;
            else
                found=1;
        }
        j--;
    }

    //BISHOP OR QUEEN UPPER-RIGHT CORNER
    found=0;
    int check_8=0;
    j=y+1;
    for(i=x-1; (i>0 && !found && !check_8); i--)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='B' || Board[i][j]=='Q')
                check_8=1;
            else
                found=1;
        }
        j++;
    }

    //PAWN THREATENING PLAYER(1)'S KING
    int check_9=0;
    if(Board[x-1][y-1]=='P' || Board[x-1][y+1]=='P')
        check_9=1;

    //KNIGHTS
    int check_10=0;
    if(Board[x+2][y+1]=='N' || Board[x+2][y-1]=='N' || Board[x-2][y+1]=='N' || Board[x-2][y-1]=='N' || Board[x+1][y+2]=='N' || Board[x+1][y-2]=='N' ||
            Board[x-1][y+2]=='N' || Board[x-1][y-2]=='N' )
        check_10=1;

    //CHECK_TEST
    check = check_1||check_2||check_3||check_4||check_5||check_6||check_7||check_8||check_9||check_10;

    return check;
}


int King2_row()
{
    //LOCATING HTE ROW OF PLAYER(2)'S KING
    int king=0; //x_y DETERMINE THE EXACT LOCATION OF KING ON THE BOARD
    for(i=1; (i<9 && !king); i++)
        for(j=1; (j<9 && !king); j++)
        {
            if(Board[i][j]=='K')
                king=1;
        }
    return(i-1);
}

int King2_column()
{
    //LOCATING THE COLUMN OF PLAYER(2)'S KING
    int king=0; //x_y DETERMINE THE EXACT LOCATION OF KING ON THE BOARD
    for(i=1; (i<9 && !king); i++)
        for(j=1; (j<9 && !king); j++)
        {
            if(Board[i][j]=='K')
                king=1;
        }
    return(j-1);
}


int CheckKing_2(int x,int y)
{
    int check;
    //ROOK OR QUEEN ON THE RIGHT
    int check_1=0;
    int found=0;
    for(i=y+1; (i<9 && !found && !check_1); i++)
    {
        if(Board[x][i]!='.' && Board[x][i]!='_')
        {
            if(Board[x][i]=='r' || Board[x][i]=='q')
                check_1=1;
            else
                found=1;
        }
    }

    //ROOK OR QUEEN ON THE LEFT
    int check_2=0;
    found=0;
    for(i=y-1; (i>0 && !found && !check_2); i--)
    {
        if(Board[x][i]!='.' && Board[x][i]!='_')
        {
            if(Board[x][i]=='r' || Board[x][i]=='q')
                check_2=1;
            else
                found=1;
        }
    }

    //ROOK OR QUEEN UP
    int check_3=0;
    found=0;
    for(i=x-1; (i>0 && !found && !check_3); i--)
    {
        if(Board[i][y]!='.' && Board[i][y]!='_')
        {
            if(Board[i][y]=='r' || Board[i][y]=='q')
                check_3=1;
            else
                found=1;
        }
    }

    //ROOK OR QUEEN DOWN
    int check_4=0;
    found=0;
    for(i=x+1; (i<9 && !found && !check_4); i++)
    {
        if(Board[i][y]!='.' && Board[i][y]!='_')
        {
            if(Board[i][y]=='r' || Board[i][y]=='q')
                check_4=1;
            else
                found=1;
        }
    }

    //BISHOP OR QUEEN LOWER-RIGHT CORNER
    int check_5=0;
    found=0;
    j=y+1;
    for(i=x+1; (i<9 && !found && !check_5); i++)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='b' || Board[i][j]=='q')
                check_5=1;
            else
                found=1;
        }
        j++;
    }

    //BISHOP OR QUEEN UPPER-LEFT CORNER
    int check_6=0;
    found=0;
    j=y-1;
    for(i=x-1; (i>0 && !found && !check_6); i--)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='b' || Board[i][j]=='q')
                check_6=1;
            else
                found=1;
        }
        j--;
    }

    //BISHOP OR QUEEN LOWER-LEFT CORNER
    int check_7=0;
    found=0;
    j=y-1;
    for(i=x+1; (i<9 && !found && !check_7); i++)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='b' || Board[i][j]=='q')
                check_7=1;
            else
                found=1;
        }
        j--;
    }

    //BISHOP OR QUEEN UPPER-RIGHT CORNER
    int check_8=0;
    found=0;
    j=y+1;
    for(i=x-1; (i>0 && !found && !check_8); i--)
    {
        if(Board[i][j]!='.' && Board[i][j]!='_')
        {
            if(Board[i][j]=='b' || Board[i][j]=='q')
                check_8=1;
            else
                found=1;
        }
        j++;
    }

    //PAWN THREATENING PLAYER(2)'S KING
    int check_9=0;
    if(Board[x+1][y+1]=='p' || Board[x+1][y-1]=='p')
        check_9=1;

    //KNIGHTS
    int check_10=0;
    if(Board[x+2][y+1]=='n' || Board[x+2][y-1]=='n' || Board[x-2][y+1]=='n' || Board[x-2][y-1]=='n' || Board[x+1][y+2]=='n' || Board[x+1][y-2]=='n' ||
            Board[x-1][y+2]=='n' || Board[x-1][y-2]=='n' )
        check_10=1;

    //CHECK_TEST
    check = check_1||check_2||check_3||check_4||check_5||check_6||check_7||check_8||check_9||check_10;

    return check;
}
//CHECK_MATE (KING1)
int Check_mate_king1()
{
    int x,y,i1,j1,i2,j2;
    x=King1_row();
    y=King1_column();

    int check_1,out_1;
    if((x-1)>0 && (x-1)<8 && Board[x-1][y]!='b' && Board[x-1][y]!='r' && Board[x-1][y]!='q' && Board[x-1][y]!='n' && Board[x-1][y]!='p')
        check_1=CheckKing_1(x-1,y);
    else
        out_1=1;

    int check_2,out_2;
    if((x+1)>1 && (x+1)<9 && Board[x+1][y]!='b' && Board[x+1][y]!='r' && Board[x+1][y]!='q' && Board[x+1][y]!='n' && Board[x+1][y]!='p')
        check_2=CheckKing_1(x+1,y);
    else
        out_2=1;

    int check_3,out_3;
    if((y-1)>0 && (y-1)<8 && Board[x][y-1]!='b' && Board[x][y-1]!='r' && Board[x][y-1]!='q' && Board[x][y-1]!='n' && Board[x][y-1]!='p')
        check_3=CheckKing_1(x,y-1);
    else
        out_3=1;

    int check_4,out_4;
    if((y+1)>1 && (y+1)<9 && Board[x][y+1]!='b' && Board[x][y+1]!='r' && Board[x][y+1]!='q' && Board[x][y+1]!='n' && Board[x][y+1]!='p')
        check_4=CheckKing_1(x,y+1);
    else
        out_4=1;

    int check_5,out_5;
    if((x+1)>1 && (x+1)<9 && (y+1)>1 && (y+1)<9 && Board[x+1][y+1]!='b' && Board[x+1][y+1]!='r' && Board[x+1][y+1]!='q' && Board[x+1][y+1]!='n' && Board[x+1][y+1]!='p')
        check_5=CheckKing_1(x+1,y+1);
    else
        out_5=1;

    int check_6,out_6;
    if((x-1)>0 && (x-1)<8 && (y-1)>0 && (y-1)<8 && Board[x-1][y-1]!='b' && Board[x-1][y-1]!='r' && Board[x-1][y-1]!='q' && Board[x-1][y-1]!='n' && Board[x-1][y-1]!='p')
        check_6=CheckKing_1(x-1,y-1);
    else
        out_6=1;

    int check_7,out_7;
    if((x+1)>1 && (x+1)<9 && (y-1)>0 && (y-1)<8 && Board[x+1][y-1]!='b' && Board[x+1][y-1]!='r' && Board[x+1][y-1]!='q' && Board[x+1][y-1]!='n' && Board[x+1][y-1]!='p')
        check_7=CheckKing_1(x+1,y-1);
    else
        out_7=1;

    int check_8,out_8;
    if((x-1)>0 && (x-1)<8 && (y+1)>1 && (y+1)<9 && Board[x-1][y+1]!='b' && Board[x-1][y+1]!='r' && Board[x-1][y+1]!='q' && Board[x-1][y+1]!='n' && Board[x-1][y+1]!='p')
        check_8=CheckKing_1(x-1,y+1);
    else
        out_8=1;

    int check_around = (check_1||out_1)&&(check_2||out_2)&&(check_3||out_3)&&(check_4||out_4)&&(check_5||out_5)&&(check_6||out_6)&&(check_7||out_7)&&(check_8||out_8);
    if(1)
    {
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Alternative[i][j]=Board[i][j];

        simulation=1;
        char temp;
        int move,check_in_simultion;
        int safe=0;
        for(i1=1; (i1<9 &&!safe); i1++)
            for(j1=1; (j1<9 &&!safe); j1++)
            {
                if(Board[i1][j1]=='p' || Board[i1][j1]=='n' || Board[i1][j1]=='r' || Board[i1][j1]=='b' || Board[i1][j1]=='q')
                {
                    for(i2=1; (i2<9 &&!safe); i2++)
                        for(j2=1; (j2<9 &&!safe); j2++)
                        {
                            temp=Board[i2][j2];
                            move=Apply_move(i1,j1,i2,j2);
                            if(move)
                            {
                                check_in_simultion=CheckKing_1(x,y);
                                if(!check_in_simultion)
                                {
                                    safe=1;
                                    for(i=0; i<10; i++)
                                        for(j=0; j<10; j++)
                                            Board[i][j]=Alternative[i][j];
                                    return 0;
                                }
                                else
                                {
                                    Board[i1][j1]=Board[i2][j2];
                                    Board[i2][j2]=temp;
                                    for(i=0; i<10; i++)
                                        for(j=0; j<10; j++)
                                            Board[i][j]=Alternative[i][j];
                                }

                            }
                        }
                }
            }
        if(safe==0)
            return 1;
    }
    else
        return 0;
}

//CHECK_MATE (KING2)
int Check_mate_king2()
{
    int x,y,i1,j1,i2,j2;
    x=King2_row();
    y=King2_column();

    int check_1,out_1;
    if((x-1)>0 && (x-1)<8 && Board[x-1][y]!='B' && Board[x-1][y]!='R' && Board[x-1][y]!='Q' && Board[x-1][y]!='N' && Board[x-1][y]!='P')
        check_1=CheckKing_2(x-1,y);
    else
        out_1=1;

    int check_2,out_2;
    if((x+1)>1 && (x+1)<9 && Board[x+1][y]!='B' && Board[x+1][y]!='R' && Board[x+1][y]!='Q' && Board[x+1][y]!='N' && Board[x+1][y]!='P')
        check_2=CheckKing_2(x+1,y);
    else
        out_2=1;

    int check_3,out_3;
    if((y-1)>0 && (y-1)<8 && Board[x][y-1]!='B' && Board[x][y-1]!='R' && Board[x][y-1]!='Q' && Board[x][y-1]!='N' && Board[x][y-1]!='P')
        check_3=CheckKing_2(x,y-1);
    else
        out_3=1;

    int check_4,out_4;
    if((y+1)>1 && (y+1)<9 && Board[x][y+1]!='B' && Board[x][y+1]!='R' && Board[x][y+1]!='Q' && Board[x][y+1]!='N' && Board[x][y+1]!='P')
        check_4=CheckKing_2(x,y+1);
    else
        out_4=1;

    int check_5,out_5;
    if((x+1)>1 && (x+1)<9 && (y+1)>1 && (y+1)<9 && Board[x+1][y+1]!='B' && Board[x+1][y+1]!='R' && Board[x+1][y+1]!='Q' && Board[x+1][y+1]!='N' && Board[x+1][y+1]!='P')
        check_5=CheckKing_2(x+1,y+1);
    else
        out_5=1;

    int check_6,out_6;
    if((x-1)>0 && (x-1)<8 && (y-1)>0 && (y-1)<8 && Board[x-1][y-1]!='B' && Board[x-1][y-1]!='R' && Board[x-1][y-1]!='Q' && Board[x-1][y-1]!='N' && Board[x-1][y-1]!='P')
        check_6=CheckKing_2(x-1,y-1);
    else
        out_6=1;

    int check_7,out_7;
    if((x+1)>1 && (x+1)<9 && (y-1)>0 && (y-1)<8 && Board[x+1][y-1]!='B' && Board[x+1][y-1]!='R' && Board[x+1][y-1]!='Q' && Board[x+1][y-1]!='N' && Board[x+1][y-1]!='P')
        check_7=CheckKing_2(x+1,y-1);
    else
        out_7=1;

    int check_8,out_8;
    if((x-1)>0 && (x-1)<8 && (y+1)>1 && (y+1)<9 && Board[x-1][y+1]!='B' && Board[x-1][y+1]!='R' && Board[x-1][y+1]!='Q' && Board[x-1][y+1]!='N' && Board[x-1][y+1]!='P')
        check_8=CheckKing_2(x-1,y+1);
    else
        out_8=1;

    int check_around = (check_1||out_1)&&(check_2||out_2)&&(check_3||out_3)&&(check_4||out_4)&&(check_5||out_5)&&(check_6||out_6)&&(check_7||out_7)&&(check_8||out_8);
    if(1)
    {
        for(i=0; i<10; i++)
            for(j=0; j<10; j++)
                Alternative[i][j]=Board[i][j];

        simulation=1;
        char temp;
        int move,check_in_simultion;
        int safe=0;
        for(i1=1; (i1<9 &&!safe); i1++)
            for(j1=1; (j1<9 &&!safe); j1++)
            {
                if(Board[i1][j1]=='P' || Board[i1][j1]=='N' || Board[i1][j1]=='R' || Board[i1][j1]=='B' || Board[i1][j1]=='Q')
                {
                    for(i2=1; (i2<9 &&!safe); i2++)
                        for(j2=1; (j2<9 &&!safe); j2++)
                        {

                            temp=Board[i2][j2];
                            move=Apply_move(i1,j1,i2,j2);
                            if(move)
                            {
                                check_in_simultion=CheckKing_2(x,y);
                                if(!check_in_simultion)
                                {
                                    safe=1;
                                    for(i=0; i<10; i++)
                                        for(j=0; j<10; j++)
                                            Board[i][j]=Alternative[i][j];
                                    return 0;
                                }
                                else
                                {
                                    Board[i1][j1]=Board[i2][j2];
                                    Board[i2][j2]=temp;
                                    for(i=0; i<10; i++)
                                        for(j=0; j<10; j++)
                                            Board[i][j]=Alternative[i][j];
                                }
                            }
                        }
                }
            }
        if(safe==0)
            return 1;
    }
    else
        return 0;
}
