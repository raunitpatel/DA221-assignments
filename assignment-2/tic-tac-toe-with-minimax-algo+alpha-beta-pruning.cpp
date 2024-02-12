#include<bits/stdc++.h>
using namespace std;
#define SIZE 3
#define COMPUTER 1
#define HUMAN 2
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

void show_instruct(){
    cout<<"\n Choose a cell numbered from 1 to 9\n";
    cout<<"\t\t\t 1 | 2 | 3 \n";
    cout<<"\t\t\t 4 | 5 | 6 \n";
    cout<<"\t\t\t 7 | 8 | 9 \n";
}
void show_board(char board[][SIZE]){
    cout<<"\t\t\t "<<board[0][0]<<" | "<<board[0][1]<<" | "<<board[0][2]<<endl;
    cout<<"\t\t\t "<<board[1][0]<<" | "<<board[1][1]<<" | "<<board[1][2]<<endl;
    cout<<"\t\t\t "<<board[2][0]<<" | "<<board[2][1]<<" | "<<board[2][2]<<endl;
}
void init_board(char board[][SIZE]){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            board[i][j]='*';
        }
    }
}
void declare_winner(int whoseturn){
    if(whoseturn==COMPUTER) cout<<"COMPUTER WON THE GAME\n";
    else cout<<"HUMAN WON THE GAME\n";
}
bool row_crossed(char board[][SIZE]){
    for(int i=0;i<SIZE;i++){
        if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][0]!='*' )return true;
    }
    return false;
}
bool column_crossed(char board[][SIZE]){
    for(int i=0;i<SIZE;i++){
        if(board[0][i]==board[1][i] && board[1][i]==board[2][i] && board[1][i]!='*' )return true;
    }
    return false;
}
bool diagonal_crossed(char board[][SIZE]){
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[0][0]!='*' )return true;
    if(board[0][2]==board[1][1] && board[1][1]==board[2][0] && board[0][2]!='*' )return true;
    return false;
}
bool game_over(char board[][SIZE]){
    return (row_crossed(board) || column_crossed(board) || diagonal_crossed(board));
}

int openLinesForPlayer(char board[][SIZE],char player){
    int openLines=0;
    char opponent =(player==COMPUTERMOVE)?HUMANMOVE:COMPUTERMOVE;
    for(int i=0;i<SIZE;i++){
        bool rowOpen =true,colOpen=true;
        for(int j=0;j<SIZE;j++){
            if(board[i][j]==opponent)rowOpen=false;
            if(board[j][i]==opponent)colOpen=false;
        }
        openLines+=rowOpen+colOpen;
    }
    bool diag1Open =true,diag2Open=true;
    for(int i=0;i<SIZE;i++){
        if(board[i][i]==opponent)diag1Open=false;
        if(board[i][SIZE-i-1]==opponent)diag2Open=false;
    }
    openLines+=diag1Open+diag2Open;
    return openLines;
}
bool check_winner(char board[][SIZE], char player) {
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }
    for (int j = 0; j < SIZE; ++j) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}
int evaluate_fn(char board[][SIZE]){
    int open_for_AI=openLinesForPlayer(board,COMPUTERMOVE);
    int open_for_human=openLinesForPlayer(board,HUMANMOVE);
    if(check_winner(board,COMPUTER)) return INT_MAX;
    else if(check_winner(board,HUMAN)) return INT_MIN;
    return open_for_AI-open_for_human;
}

int minimax(char board[][SIZE],int depth,int alpha,int beta,bool isAI){
    int score=0,bestScore=0;
    if(game_over(board)==true || depth>=9){
        return evaluate_fn(board);
    }
    else{
        if(isAI){
            bestScore=INT_MIN;
            for(int i=0;i<SIZE;i++){
                for(int j=0;j<SIZE;j++){
                    if(board[i][j]=='*'){
                        board[i][j]=COMPUTERMOVE;
                        score=minimax(board,depth+1,alpha,beta,false);
                        board[i][j]='*';
                        if(score>bestScore) bestScore=score;
                        alpha=max(alpha,bestScore);
                        if(beta<=alpha)break;
                    }
                }
                if (beta <= alpha) break;
            }
            return bestScore;
        }
        else {
            bestScore=INT_MAX;
            for(int i=0;i<SIZE;i++){
                for(int j=0;j<SIZE;j++){
                    if(board[i][j]=='*'){
                        board[i][j]=HUMANMOVE;
                        score=minimax(board,depth+1,alpha,beta,true);
                        board[i][j]='*';
                        if(score<bestScore) bestScore=score;
                        beta=min(beta,bestScore);
                        if(beta<=alpha)break;
                    }
                }
                if (beta <= alpha) break;
            }
            return bestScore;
        }
    }
}
int bestmove(char board[][SIZE], int moveIndex){
    int x=-1,y=-1;
    int score =0,bestscore=INT_MIN;
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(board[i][j]=='*'){
                board[i][j]=COMPUTERMOVE;
                score=minimax(board,moveIndex+1,INT_MIN,INT_MAX,false);
                board[i][j]='*';
                if(score>bestscore){
                    bestscore=score;
                    x=i;
                    y=j;
                }
            }
        }
    }
    return x*3+y;
}
void play_tto(int whoseturn){
    char board[SIZE][SIZE];
    int moveIndex =0,x=0,y=0;
    init_board(board);
    show_instruct();

    while(game_over(board)==false && moveIndex!=SIZE*SIZE){
        int n;
        if(whoseturn == COMPUTER){
            n =bestmove(board,moveIndex);
            x=n/SIZE;
            y=n%SIZE;
            board[x][y] =COMPUTERMOVE;
            cout<<"Computer has put a "<<COMPUTERMOVE<<" in cell "<<n+1<<endl;
            show_board(board);
            moveIndex++;
            whoseturn=HUMAN;
        }
        else if(whoseturn == HUMAN){
            cout<<"You can insert in following position : ";
            for(int i=0;i<SIZE;i++){
                for(int j=0;j<SIZE;j++){
                    if(board[i][j]=='*') cout<<i*3+j+1<<" ";
                }
            }
            cout<<"\n\n Enter the position = ";
            cin>>n;
            n--;
            x=n/SIZE;
            y=n%SIZE;
            if(board[x][y] == '*' && n<9 && n>=0){
                board[x][y]=HUMANMOVE;
                cout<<"Human has put a "<<HUMANMOVE<<" in cell "<<n+1<<endl;
                show_board(board);
                moveIndex++;
                whoseturn=COMPUTER;
            }
            else if(board[x][y] != '*' && n<9 && n>=0){
                cout<<"\nPosition is Occupied, please choose another position\n";
            }
            else{
                cout<<"\nInvalid Position\n";
            }
        }
    }
    if(game_over(board)==false && moveIndex==SIZE*SIZE){
        cout<<"GAME IS A DRAW\n\n";
    }
    else{
        if(whoseturn==COMPUTER)whoseturn=HUMAN;
        else whoseturn=COMPUTER;
        declare_winner(whoseturn);
    }
    cout<<"\n---------------------------------\n";
    cout<<"\n---------------------------------\n\n";

}
int main(){
    cout<<"\n---------------------------------\n\n";
    cout<<"\t TIC-TAC-TOE\n";
    cout<<"\n---------------------------------\n\n";
    char cont='y';
    do{
        char choice;
        cout<<"Do you want to start first(y/n) : ";
        cin>>choice;
        if(choice =='n')play_tto(COMPUTER);
        else if(choice =='y')play_tto(HUMAN);
        else cout<<"INVALID CHOICE\n";
        cout<<"Do you want to quit(y/n) : ";
        cin>>cont;
    }
    while(cont=='n');
}