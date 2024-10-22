#include <iostream>
#include "sl.h"
#include <windows.h>
#include <mmsystem.h>
#include <cstdlib>
#include <time.h>

using namespace std;


struct Piece
{
    int Island[4][4];
    bool Compass;
    int Orientation;
    int Poz;
    bool Placed=false;
};

struct Challenge
{
    int CompassO;
    int FV[9];
    int WinFV[9];
    int WinCompassO;
};

struct BoardType
{
    int dim;
    Piece P[17];
    int Q[5];
    Challenge ChallengeNr[10];
};

struct Texture
{
    int compass[5];
    int piece[9];
    int island[17];
    bool TextureNotLoaded=true;
    int wintex;
};

struct Game
{
    int Board[7][7];
    int iBoard[7][7];
    Texture tex;
    BoardType B;
    int MouseX;
    int MouseY;
    bool win=false;
    bool game_song=false;
    bool win_song=false;
    bool menu_song=false;
};

void RotateMatrix(Piece &X)
{
    for (int i=1; i<=3; i++)
    {
        for (int j=i+1; j<=3; j++)
        {
            swap(X.Island[i][j],X.Island[j][i]);
        }
    }

    for (int i=1;i<=3;i++)
    {
        int start=1;
        int end=3;
        while(start<end)
        {
            swap(X.Island[i][start],X.Island[i][end]);
            start++;
            end--;
        }
    }
}

void InitialRotate(Game &game)
{
    for(int i=1;i<5;i++) // pentru piesa 1,2,3,4
        for(int j=1;j<5;j++) // rotim si salvam fiecare stadiu rotit
        {
            RotateMatrix(game.B.P[i]);
            if(j<4)
                for(int x=1;x<4;x++)
                    for(int y=1;y<4;y++)
                        game.B.P[i+j*4].Island[x][y]=game.B.P[i].Island[x][y];
        }
}

void BoardPreset(Game &game)
{
    game.Board[1][1]=0;
    game.Board[1][2]=6;
    game.Board[1][3]=1;
    game.Board[1][4]=6;
    game.Board[1][5]=4;
    game.Board[1][6]=0;
    game.Board[2][1]=2;
    game.Board[2][2]=0;
    game.Board[2][3]=7;
    game.Board[2][4]=0;
    game.Board[2][5]=2;
    game.Board[2][6]=7;
    game.Board[3][1]=5;
    game.Board[3][2]=0;
    game.Board[3][3]=0;
    game.Board[3][4]=5;
    game.Board[3][5]=3;
    game.Board[3][6]=0;
    game.Board[4][1]=6;
    game.Board[4][2]=1;
    game.Board[4][3]=3;
    game.Board[4][4]=0;
    game.Board[4][5]=0;
    game.Board[4][6]=0;
    game.Board[5][1]=2;
    game.Board[5][2]=7;
    game.Board[5][3]=0;
    game.Board[5][4]=4;
    game.Board[5][5]=6;
    game.Board[5][6]=8;
    game.Board[6][1]=3;
    game.Board[6][2]=4;
    game.Board[6][3]=8;
    game.Board[6][4]=2;
    game.Board[6][5]=3;
    game.Board[6][6]=7;
    for(int i=1;i<=6;i++)
        for(int j=1;j<=6;j++)
            game.iBoard[i][j]=game.Board[i][j];
    game.B.P[1].Island[1][1]=0;
    game.B.P[1].Island[1][2]=1;
    game.B.P[1].Island[1][3]=0;
    game.B.P[1].Island[2][1]=0;
    game.B.P[1].Island[2][2]=0;
    game.B.P[1].Island[2][3]=0;
    game.B.P[1].Island[3][1]=0;
    game.B.P[1].Island[3][2]=1;
    game.B.P[1].Island[3][3]=0;
    game.B.P[2].Island[1][1]=0;
    game.B.P[2].Island[1][2]=0;
    game.B.P[2].Island[1][3]=0;
    game.B.P[2].Island[2][1]=0;
    game.B.P[2].Island[2][2]=1;
    game.B.P[2].Island[2][3]=0;
    game.B.P[2].Island[3][1]=0;
    game.B.P[2].Island[3][2]=1;
    game.B.P[2].Island[3][3]=0;
    game.B.P[3].Island[1][1]=0;
    game.B.P[3].Island[1][2]=0;
    game.B.P[3].Island[1][3]=1;
    game.B.P[3].Island[2][1]=0;
    game.B.P[3].Island[2][2]=0;
    game.B.P[3].Island[2][3]=0;
    game.B.P[3].Island[3][1]=1;
    game.B.P[3].Island[3][2]=0;
    game.B.P[3].Island[3][3]=0;
    game.B.P[4].Island[1][1]=0;
    game.B.P[4].Island[1][2]=0;
    game.B.P[4].Island[1][3]=1;
    game.B.P[4].Island[2][1]=0;
    game.B.P[4].Island[2][2]=0;
    game.B.P[4].Island[2][3]=0;
    game.B.P[4].Island[3][1]=0;
    game.B.P[4].Island[3][2]=1;
    game.B.P[4].Island[3][3]=0;

    for(int i=1;i<9;i++)
        game.B.ChallengeNr[1].FV[i]=0;
    game.B.ChallengeNr[1].FV[2]=2;
    game.B.ChallengeNr[1].FV[3]=3;
    game.B.ChallengeNr[1].CompassO=2;
    InitialRotate(game);
}

void Remove(int quadrant,Game &game)
{
        int i,j;
        if(quadrant==1)
        {
            for(i=1;i<=3;i++)
                for(j=1;j<=3;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else if(quadrant==2)
        {
            for(i=1;i<=3;i++)
                for(j=4;j<=6;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else if(quadrant==3)
        {
            for(i=4;i<=6;i++)
                for(j=1;j<=3;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else if(quadrant==4)
        {
            for(i=4;i<=6;i++)
                for(j=4;j<=6;j++)
                    game.Board[i][j]=game.iBoard[i][j];
        }
        else
        {
            cout<<"eroare cadran la remove"<<endl;
        }
}

int Placement(Piece X, int quadrant, Game &game)
{
        Remove(quadrant,game);
        if(quadrant==1)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i][j]=game.Board[i][j]*X.Island[i][j];
        }
        else if(quadrant==2)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i][j+3]=game.Board[i][j+3]*X.Island[i][j];
        }
        else if(quadrant==3)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i+3][j]=game.Board[i+3][j]*X.Island[i][j];
        }
        else if(quadrant==4)
        {
            for(int i=1;i<=3;i++)
                for(int j=1;j<=3;j++)
                    game.Board[i+3][j+3]=game.Board[i+3][j+3]*X.Island[i][j];
        }
        else
        {
            cout<<"eroare cadran la placement"<<endl;
        }
        return 0;
}

void RandomizedPreset(Game &game)
{
    int randP1,randP2,randP3,randP4;
    int randQ1,randQ2,randQ3,randQ4;
    srand(time(0));
    for(int i=1;i<rand()%100;i++)
    {
        do
        {
            randP1=rand()%16+1;
            randP2=rand()%16+1;
            randP3=rand()%16+1;
            randP4=rand()%16+1;
        }while(randP1%4==randP2%4 || randP1%4==randP3%4 || randP1%4==randP4%4 || randP2%4==randP3%4 || randP2%4==randP4%4 || randP3%4==randP4%4);
    }
    if(randP1%4==0)
        game.B.ChallengeNr[1].CompassO=randP1/4;
    else if(randP2%4==0)
        game.B.ChallengeNr[1].CompassO=randP2/4;
    else if(randP3%4==0)
        game.B.ChallengeNr[1].CompassO=randP3/4;
    else if(randP4%4==0)
        game.B.ChallengeNr[1].CompassO=randP4/4;
    for(int i=1;i<rand()%100;i++)
    {
        do
        {
            randQ1=rand()%4+1;
            randQ2=rand()%4+1;
            randQ3=rand()%4+1;
            randQ4=rand()%4+1;
        }while(randQ1==randQ2 || randQ1==randQ3 || randQ1==randQ4 || randQ2==randQ3 || randQ2==randQ4 || randQ3==randQ4);
    }
    Placement(game.B.P[randP1],randQ1,game);
    Placement(game.B.P[randP2],randQ2,game);
    Placement(game.B.P[randP3],randQ3,game);
    Placement(game.B.P[randP4],randQ4,game);
    for(int i=0;i<9;i++)
        game.B.ChallengeNr[1].FV[i]=0;

    for(int i=1;i<=6;i++)
        for(int j=1;j<=6;j++)
            game.B.ChallengeNr[1].FV[game.Board[i][j]]++;

    cout<<"solutia este: "<<randP1<<" "<<randP2<<" "<<randP3<<" "<<randP4<<endl;
    Remove(1,game);
    Remove(2,game);
    Remove(3,game);
    Remove(4,game);
}

void switchP(Piece X1, Piece X2, int quad1, int quad2,Game &game)
{
    Remove(quad1,game);
    Remove(quad2,game);
    Placement(X1,quad2,game);
    Placement(X2,quad1,game);
}

void ChooseTexture(int centerX,int centerY,int Length,int i,int j,Game &game)
{
    if(game.tex.TextureNotLoaded)
    {
        game.tex.TextureNotLoaded=false;
        game.tex.piece[1]=slLoadTexture("1.png");
        game.tex.piece[2]=slLoadTexture("2.png");
        game.tex.piece[3]=slLoadTexture("3.png");
        game.tex.piece[4]=slLoadTexture("4.png");
        game.tex.piece[5]=slLoadTexture("5.png");
        game.tex.piece[6]=slLoadTexture("6.png");
        game.tex.piece[7]=slLoadTexture("7.png");
        game.tex.piece[8]=slLoadTexture("8.png");
        game.tex.island[1]=slLoadTexture("IS1.png");
        game.tex.island[2]=slLoadTexture("IS2.png");
        game.tex.island[3]=slLoadTexture("IS3.png");
        game.tex.island[4]=slLoadTexture("IS4.png");
        game.tex.island[5]=slLoadTexture("IS5.png");
        game.tex.island[6]=slLoadTexture("IS6.png");
        game.tex.island[7]=slLoadTexture("IS7.png");
        game.tex.island[8]=slLoadTexture("IS8.png");
        game.tex.island[9]=slLoadTexture("IS9.png");
        game.tex.island[10]=slLoadTexture("IS10.png");
        game.tex.island[11]=slLoadTexture("IS11.png");
        game.tex.island[12]=slLoadTexture("IS12.png");
        game.tex.island[13]=slLoadTexture("IS13.png");
        game.tex.island[14]=slLoadTexture("IS14.png");
        game.tex.island[15]=slLoadTexture("IS15.png");
        game.tex.island[16]=slLoadTexture("IS16.png");
        game.tex.compass[1]=slLoadTexture("C1.png");
        game.tex.compass[2]=slLoadTexture("C2.png");
        game.tex.compass[3]=slLoadTexture("C3.png");
        game.tex.compass[4]=slLoadTexture("C4.png");
        game.tex.wintex=slLoadTexture("WinTex.png");
    }
    slSetForeColor(1,1,1,1);
    if(game.Board[i][j]==1)
    {
        slSprite(game.tex.piece[1],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==2)
    {
        slSprite(game.tex.piece[2],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==3)
    {
        slSprite(game.tex.piece[3],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==4)
    {
        slSprite(game.tex.piece[4],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==5)
    {
        slSprite(game.tex.piece[5],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==6)
    {
        slSprite(game.tex.piece[6],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==7)
    {
        slSprite(game.tex.piece[7],centerX,centerY,Length,Length);
    }
    else if(game.Board[i][j]==8)
    {
        slSprite(game.tex.piece[8],centerX,centerY,Length,Length);
    }
}



void PlaceGameTexture(int centerX, int centerY,int ratio,int quadrant,Game &game)
{
    int M=ratio/45,i,j;
    slSetForeColor(0,0,0,1);

    if(quadrant==1)
    {
        i=1; j=1;
    }
    else if(quadrant==2)
    {
        i=1; j=4;
    }
    else if(quadrant==3)
    {
        i=4;j=1;
    }
    else if(quadrant==4)
    {
        i=4;j=4;
    }
    else
    {
        cout<<"eroare in functia Place graphic la cadran"<<endl<<endl;
        exit(0);
    }

    ChooseTexture(centerX-M*2,centerY+M*2,M*2,i,j,game);
    ChooseTexture(centerX,centerY+M*2,M*2,i,j+1,game);
    ChooseTexture(centerX+M*2,centerY+M*2,M*2,i,j+2,game);
    ChooseTexture(centerX-M*2,centerY,M*2,i+1,j,game);
    ChooseTexture(centerX,centerY,M*2,i+1,j+1,game);
    ChooseTexture(centerX+M*2,centerY,M*2,i+1,j+2,game);
    ChooseTexture(centerX-M*2,centerY-M*2,M*2,i+2,j,game);
    ChooseTexture(centerX,centerY-M*2,M*2,i+2,j+1,game);
    ChooseTexture(centerX+M*2,centerY-M*2,M*2,i+2,j+2,game);
}

void PlaceIslandsOffBoard(int width,int length, int ratio, Game &game)
{
    slSetForeColor(1,1,1,1);
    if(!game.B.P[1].Placed)
        slSprite(game.tex.island[1],width/2+width/2.6,length/6*5.2,ratio/12,ratio/12);
    if(!game.B.P[2].Placed)
        slSprite(game.tex.island[2],width/2+width/2.6,length/6*3.7,ratio/12,ratio/12);
    if(!game.B.P[3].Placed)
        slSprite(game.tex.island[3],width/2+width/2.6,length/6*2.2,ratio/12,ratio/12);
    if(!game.B.P[4].Placed)
        slSprite(game.tex.island[4],width/2+width/2.6,length/6*0.7,ratio/12,ratio/12);
}

void IslandChecker(int width,int length, int ratio,Game &game)
{
    slSetForeColor(1,1,1,1);
    if(game.B.Q[1])
    {
        slSprite(game.tex.island[game.B.Q[1]],width/2-ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);
    }
    if(game.B.Q[2])
    {
        slSprite(game.tex.island[game.B.Q[2]],width/2+ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);
    }
    if(game.B.Q[3])
    {
        slSprite(game.tex.island[game.B.Q[3]],width/2-ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);
    }
    if(game.B.Q[4])
    {
        slSprite(game.tex.island[game.B.Q[4]],width/2+ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);
    }
}

void RemoveIsland(int width,int length,int ratio,Game &game)
{
    if(slGetKey(SL_KEY_BACKSPACE))
    {
        if(game.B.Q[1] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            Remove(1,game);
            if(game.B.Q[1]%4==0)
                game.B.Q[1]=4;
            else
                game.B.Q[1]=game.B.Q[1]%4;
            game.B.P[game.B.Q[1]].Placed=false;
            game.B.Q[1]=0;
        }
        else if(game.B.Q[2] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            Remove(2,game);
            if(game.B.Q[2]%4==0)
                game.B.Q[2]=4;
            else
                game.B.Q[2]=game.B.Q[2]%4;
            game.B.P[game.B.Q[2]].Placed=false;
            game.B.Q[2]=0;
        }
        else if(game.B.Q[3] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            Remove(3,game);
            if(game.B.Q[3]%4==0)
                game.B.Q[3]=4;
            else
                game.B.Q[3]=game.B.Q[3]%4;
            game.B.P[game.B.Q[3]].Placed=false;
            game.B.Q[3]=0;
        }
        else if(game.B.Q[4] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            Remove(4,game);
            if(game.B.Q[4]%4==0)
                game.B.Q[4]=4;
            else
                game.B.Q[4]=game.B.Q[4]%4;
            game.B.P[game.B.Q[4]].Placed=false;
            game.B.Q[4]=0;
        }
    }
}

void PlaceChallenges(int width,int length, int ratio,Game &game)
{
    slSetForeColor(1,1,1,1);
    int i,j,cnt=9;
    if(game.B.ChallengeNr[1].CompassO)
    {
        slSprite(game.tex.compass[game.B.ChallengeNr[1].CompassO],width/16,length/10*cnt,ratio/45*2,ratio/45*2);
        cnt--;
    }
    for(i=1;i<=8;i++)
    {
        j=0;
        while(game.B.ChallengeNr[1].FV[i]!=j && game.B.ChallengeNr[1].FV[i]<5)
        {
            slSprite(game.tex.piece[i],width/16,length/10*cnt,ratio/45*2,ratio/45*2);
            cnt--;
            j++;
        }
    }
}

void DrawBoard(int width,int length,int ratio,Game &game,int &ScreenState)
{

    slSetBackColor(0.949,0.761,0.537);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/16,length/2,ratio/10,length); //coloana challenge
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/16,length/2,ratio/10,length);

    slSetForeColor(0.475,0.894,0.998,1);
    slRectangleFill(width/2,length/2,ratio/3,ratio/3); // patratul mare
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2,length/2,ratio/3,ratio/3);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2-ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 1
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2-ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2+ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 2
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2+ratio/13,length/2+ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2-ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 3
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2-ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2+ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5); // patrat cadran 4
    slSetForeColor(0,0,0,1);
    slRectangleOutline(width/2+ratio/13,length/2-ratio/13,ratio/7.5,ratio/7.5);

    slSetForeColor(0.475,0.894,0.998,1);
    slRectangleFill(width/5.5,length/2,ratio/30,ratio/10); //
    slSetForeColor(0,0,0,1);
    slText(width/5.5,length/2,"Menu");
    slRectangleOutline(width/5.5,length/2,ratio/30,ratio/10);

    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        if(slGetMouseX()>= width/5.5-ratio/30 and slGetMouseX()<=width/5.5+ratio/30 and slGetMouseY()<=length/2+ratio/10 and slGetMouseY()>=length/2-ratio/10)
        {
            ScreenState = 1;
        }
    }

    PlaceGameTexture(width/2-ratio/13,length/2+ratio/13,ratio,1,game);
    PlaceGameTexture(width/2+ratio/13,length/2+ratio/13,ratio,2,game);
    PlaceGameTexture(width/2-ratio/13,length/2-ratio/13,ratio,3,game);
    PlaceGameTexture(width/2+ratio/13,length/2-ratio/13,ratio,4,game);

    PlaceIslandsOffBoard(width,length,ratio,game);
    PlaceChallenges(width,length,ratio,game);

    IslandChecker(width,length,ratio,game);

    RemoveIsland(width,length,ratio,game);
}

int StartDragging(int width,int length, int ratio, Game &game)
{
    slSetForeColor(1,1,1,1);
    if(game.MouseX>=width/2+width/2.6-ratio/24 && game.MouseX<=width/2+width/2.6+ratio/24)
    {
        if(!game.B.P[1].Placed && game.MouseY<=length/6*5.2+ratio/24 && game.MouseY>=length/6*5.2-ratio/24)
        {
            game.B.P[1].Placed=true;
            return 1;
        }
        else if(!game.B.P[2].Placed && game.MouseY<=length/6*3.7+ratio/24 && game.MouseY>=length/6*3.7-ratio/24)
        {
            game.B.P[2].Placed=true;
            return 2;
        }
        else if(!game.B.P[3].Placed && game.MouseY<=length/6*2.2+ratio/24 && game.MouseY>=length/6*2.2-ratio/24)
        {
            game.B.P[3].Placed=true;
            return 3;
        }
        else if(!game.B.P[4].Placed && game.MouseY<=length/6*0.7+ratio/24 && game.MouseY>=length/6*0.7-ratio/24)
        {
            game.B.P[4].Placed=true;
            return 4;
        }
    }
    return 0;
}

void Drop(int width,int length,int ratio, int &dragging, Game &game)
{
    if(!slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {

        if(game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[1]].Placed=false;
            game.B.Q[1]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[1]],1,game);
        }
        else if(game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/+ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[2]].Placed=false;
            game.B.Q[2]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[2]],2,game);
        }
        else if(game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[3]].Placed=false;
            game.B.Q[3]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[3]],3,game);
        }
        else if(game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.P[game.B.Q[4]].Placed=false;
            game.B.Q[4]=dragging;
            dragging=0;
            Placement(game.B.P[game.B.Q[4]],4,game);
        }
    }
}

void Drag(int width,int length,int ratio,int &dragging,Game &game)
{
    slSetForeColor(1,1,1,1);
    if(dragging==0 && slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        dragging=StartDragging(width,length,ratio,game);
    }
    else if(dragging==1)
    {
        slSprite(game.tex.island[1],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    else if(dragging==2)
    {
        slSprite(game.tex.island[2],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    else if(dragging==3)
    {
        slSprite(game.tex.island[3],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    else if(dragging==4)
    {
        slSprite(game.tex.island[4],slGetMouseX(),slGetMouseY(),ratio/12,ratio/12);
    }
    if(dragging)
    {
        Drop(width,length,ratio,dragging,game);
    }
}

void DrawMenu(int width,int length,int ratio,Game &game,int &ScreenState)
{
    slSetBackColor(0.949,0.761,0.537);
    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2*1.5,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2*1.5,"Play");
    slRectangleOutline(width/2,length/2*1.5,ratio/15*1.5,ratio/15);
    //
    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2,"Settings");
    slRectangleOutline(width/2,length/2,ratio/15*1.5,ratio/15);
    slSetForeColor(0.4,0.765,0.851,1);
    //
    slRectangleFill(width/2,length/2*0.5,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2*0.5,"Exit");
    slRectangleOutline(width/2,length/2*0.5,ratio/15*1.5,ratio/15);
    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        if(slGetMouseX()>=width/2-width/16 and slGetMouseX()<=width/2+width/16)
            {
                if(slGetMouseY()<=length/2*1.5+ratio/30*1.5 and slGetMouseY()>=length/2*1.5-ratio/30*1.5 )
                {
                    ScreenState = 0;
                }

                else if(slGetMouseY()<=length/2+ratio/30*1.5  and slGetMouseY()>=length/2-ratio/30*1.5 )
                {
                    ScreenState = 2;
                }
                if(slGetMouseY()<=length/2*0.5+ratio/30*1.5  and slGetMouseY()>=length/2*0.5-ratio/30*1.5 and ScreenState == 1)
                {
                    ScreenState = 3;
                }
            }


    }
}
void DrawSettings(int width,int length,int ratio,Game &game,int &ScreenState)
{
    slSetBackColor(0.949,0.761,0.537);
    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2*1.5,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/10*9,"Settings");
    slText(width/2,length/2*1.5,"Video");
    slRectangleOutline(width/2,length/2*1.5,ratio/15*1.5,ratio/15);
    //
    slSetForeColor(0.4,0.765,0.851,1);
    slRectangleFill(width/2,length/2,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2,"Audio");
    slRectangleOutline(width/2,length/2,ratio/15*1.5,ratio/15);
    slSetForeColor(0.4,0.765,0.851,1);
    //
    slRectangleFill(width/2,length/2*0.5,ratio/15*1.5,ratio/15);
    slSetForeColor(0,0,0,1);
    slText(width/2,length/2*0.5,"Menu");
    slRectangleOutline(width/2,length/2*0.5,ratio/15*1.5,ratio/15);
    if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
    {
        if(slGetMouseX()>=width/2-width/16 and slGetMouseX()<=width/2+width/16)
            {
                if(slGetMouseY()<=length/2*0.5+ratio/30*1.5 and slGetMouseY()>=length/2*0.5-ratio/30*1.5)
                {
                    ScreenState = 1;
                    Sleep(100);
                }
            }


    }
}

void RotateGraphics(int width,int length,int ratio,Game &game)
{
    if(slGetMouseButton(SL_MOUSE_BUTTON_RIGHT))
    {
        if(game.B.Q[1] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.Q[1]=game.B.Q[1]+4;
            if(game.B.Q[1]>16)
                game.B.Q[1]=game.B.Q[1]%16;
            Placement(game.B.P[game.B.Q[1]],1,game);
        }
        else if(game.B.Q[2] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2+ratio/13+ratio/15 && game.MouseY>=length/2+ratio/13-ratio/15)
        {
            game.B.Q[2]=game.B.Q[2]+4;
            if(game.B.Q[2]>16)
                game.B.Q[2]=game.B.Q[2]%16;
            Placement(game.B.P[game.B.Q[2]],2,game);
        }
        else if(game.B.Q[3] && game.MouseX<=width/2-ratio/13+ratio/15 && game.MouseX>=width/2-ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.Q[3]=game.B.Q[3]+4;
            if(game.B.Q[3]>16)
                game.B.Q[3]=game.B.Q[3]%16;
            Placement(game.B.P[game.B.Q[3]],3,game);
        }
        else if(game.B.Q[4] && game.MouseX<=width/2+ratio/13+ratio/15 && game.MouseX>=width/2+ratio/13-ratio/15 && game.MouseY<=length/2-ratio/13+ratio/15 && game.MouseY>=length/2-ratio/13-ratio/15)
        {
            game.B.Q[4]=game.B.Q[4]+4;
            if(game.B.Q[4]>16)
                game.B.Q[4]=game.B.Q[4]%16;
            Placement(game.B.P[game.B.Q[4]],4,game);
        }
        Sleep(250);
    }
}

bool WinCondition(Game &game)
{
    int i,j;
    for(i=0;i<9;i++)
        game.B.ChallengeNr[1].WinFV[i]=0;
    for(i=1;i<=6;i++)
        for(j=1;j<=6;j++)
            game.B.ChallengeNr[1].WinFV[game.Board[i][j]]++;
    for(i=1;i<9;i++)
        if(game.B.ChallengeNr[1].WinFV[i]!=game.B.ChallengeNr[1].FV[i])
        {
            return false;
        }
    for(i=1;i<=4;i++)
        if(game.B.Q[i]%4==0 && game.B.Q[i])
        {
            game.B.ChallengeNr[1].WinCompassO=game.B.Q[i]/4;
        }
    if(game.B.ChallengeNr[1].WinCompassO!=game.B.ChallengeNr[1].CompassO)
    {
        return false;
    }
    return true;
}

void DrawWinScreen(int width,int length, int ratio,Game &game)
{
    slSprite(game.tex.wintex,width/2,length/2,ratio/3,ratio/3);
    slRender();
}


void StartGraphics(Game &game)
{
    int x=GetSystemMetrics(SM_CXSCREEN),y= GetSystemMetrics(SM_CYSCREEN), ratio=x/y*2200, dragging=0;

    slWindow(x,y,"Test grafic",1);
    slSetFont(slLoadFont("whitrabt.ttf"),24);
    slSetTextAlign(SL_ALIGN_CENTER);


    game.B.Q[1]=0;
    game.B.Q[2]=0;
    game.B.Q[3]=0;
    game.B.Q[4]=0;

    int ScreenState = 1;
    /*
        0 -> Game
        1 -> Menu
        2 -> Settings
        3 -> Exit
        4 -> WinScreen
    */

    while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
    {
        if(slGetDeltaTime()>0.35)
            cout<<"sub 30 frame-uri"<<endl;
        if(ScreenState==1)
        {
            if(!game.menu_song)
            {
                game.win_song=false;
                game.menu_song=true;                                          //muzica
                //PlaySound("State1.wav",NULL,SND_SYNC);
            }

            int i,j;
            for( i=1;i<=6;i++)
                for( j=1;j<=6;j++)
                    game.Board[i][j]=game.iBoard[i][j];
            for(i=1;i<=4;i++)                                               //reset joc
                game.B.Q[i]=0;
            for(i=1;i<=16;i++)
                game.B.P[i].Placed=false;

            RandomizedPreset(game);

            DrawMenu(x,y,ratio,game,ScreenState);
            slRender();
        }
        else if(ScreenState==0)
        {
                if(!game.game_song)
                {
                    game.menu_song=false;
                    game.game_song=true;                                        // muzica
                    //PlaySound("State0.wa",NULL,SND_ASYNC);
                }
                game.MouseX=slGetMouseX();
                game.MouseY=slGetMouseY();

                if(game.win)
                {
                   ScreenState=4;
                }

                DrawBoard(x,y,ratio,game,ScreenState);
                Drag(x,y,ratio,dragging,game);
                RotateGraphics(x,y,ratio,game);
                game.win=WinCondition(game);
                slRender();
        }
        else if(ScreenState==2)
        {
                DrawSettings(x,y,ratio,game,ScreenState);
                slRender();
        }
        else if(ScreenState == 3)
        {
            break;
        }
        else if(ScreenState==4)
        {
            if(!game.win_song)
            {
                game.win_song=true;
                game.game_song=false;                                   //muzica 4
                //PlaySound("State4.wav",NULL,SND_ASYNC);
            }

            Sleep(500);

            DrawWinScreen(x,y,ratio,game);


            game.win=false;

            Sleep(3000);

            ScreenState=1;
        }
    }
    slClose();
}


int main()
{
    Game game;
    BoardPreset(game);


    StartGraphics(game);

    return 0;
}
