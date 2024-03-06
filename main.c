#include <SDL2/SDL.h>
#include <sdl2/sdl_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#define air 50
#define Tlen 10

void error(char* txt)
{
    fprintf(stderr, txt,
                SDL_GetError());
}

SDL_Texture* loadImage(SDL_Renderer* render, char* img)
{
    SDL_Surface *surf=NULL;
    SDL_Texture *texture=NULL;
    surf=SDL_LoadBMP(img);
    if(NULL==surf)
    {
        error("erreur sdl_loadbmp %s");
        return NULL;
    }
    texture=SDL_CreateTextureFromSurface(render,surf);
    SDL_FreeSurface(surf);
    if(NULL==texture)
    {
        error("erreur sdl_createtexturefromsurface %s");
        return NULL;
    }
    return texture;
}

void inittial(int tab[Tlen][Tlen])
{
    int i,j;
    for(i=0;i<Tlen;i++)
    {
        for(j=0;j<Tlen;j++)
        {
            if(i==0 || j==0 || i==9 || j==9)
                tab[i][j]=-1;
            else if(((i)%2==0 && (j)%2==0) || ((i)%2!=0 && (j)%2!=0))
                    tab[i][j]=0;
            else
            {
                if(j<4)
                    tab[i][j]=2;
                else if(j<6)
                    tab[i][j]=1;
                else
                    tab[i][j]=3;
            }
        }
    }
}

void reinit(int tab[Tlen][Tlen], int tr)
{
    int i,j;
    for(i=1;i<Tlen-1;i++)
        for(j=1;j<Tlen-1;j++)
        {
            if(tab[i][j]==20 || tab[i][j]==21 || tab[i][j]==22 ||
               tab[i][j]==30 || tab[i][j]==31 || tab[i][j]==32 ||
               tab[i][j]==12 || tab[i][j]==11 || tab[i][j]==10 ||
               tab[i][j]==13 || tab[i][j]==14)
               tab[i][j]=1;
            if(tab[i][j]==28)
                tab[i][j]=2;
            if(tab[i][j]==38)
                tab[i][j]=3;
            if(tab[i][j]==48)
                tab[i][j]=4;
            if(tab[i][j]==68)
                tab[i][j]=6;
        }
}

void aff_plato(int tab[Tlen][Tlen])
{
    int i,j;
    for(i=0;i<Tlen;i++)
    {
        for(j=0;j<Tlen;j++)
            printf("\t%d",tab[j][i]);
        printf("\n\n");
    }
}

SDL_Renderer *plateau(SDL_Renderer *ren, SDL_Texture *texture, int tab[Tlen][Tlen],SDL_Color color)
{
   int i,j;
   SDL_Texture *tmp = NULL;
   SDL_Texture *tmp1 = NULL;
   SDL_Texture *ktmp = NULL;
   SDL_Texture *ktmp1 = NULL;
   SDL_Rect src = {0,0,40,40}, drc = {52,52,40,40}, tb = {-60,-60,640,640}, plato = {50,50,400,400};
   SDL_SetRenderDrawColor(ren,color.r,color.g,color.b,color.a);
   SDL_RenderClear(ren);
   SDL_RenderCopy(ren,loadImage(ren,"fond.bmp"),NULL,&tb);
   SDL_RenderCopy(ren,texture,NULL,&plato);
   tmp = SDL_CreateTexture(ren,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,40,40);
   tmp1 = SDL_CreateTexture(ren,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,40,40);
   tmp = loadImage(ren,"pion2.bmp");
   tmp1 = loadImage(ren,"pion1.bmp");
   ktmp = loadImage(ren,"kpion2.bmp");
   ktmp1 = loadImage(ren,"kpion1.bmp");
   for(i=0;i<Tlen;i++)
   {
       for(j=0;j<Tlen;j++)
       {
           drc.x=50*i+5;
           drc.y=50*j+3;
           if(tab[i][j]==2 || tab[i][j]==-2)
            SDL_RenderCopy(ren,tmp,&src,&drc);
           if(tab[i][j]==3 || tab[i][j]==-3)
            SDL_RenderCopy(ren,tmp1,&src,&drc);
            if(tab[i][j]==4)
            SDL_RenderCopy(ren,ktmp,&src,&drc);
           if(tab[i][j]==6)
            SDL_RenderCopy(ren,ktmp1,&src,&drc);
       }
   }
   SDL_DestroyTexture(tmp);
   SDL_DestroyTexture(tmp1);
   return ren;
}

SDL_Renderer *pos_jouable(int x,int y,int pl,int tab[Tlen][Tlen], SDL_Renderer *ren)
{
    SDL_Rect drc = {0,0,50,50};
    SDL_Texture *texture = NULL;
    texture = SDL_CreateTexture(ren,SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,60,60);
    //SDL_SetRenderTarget(render,texture);
    //SDL_SetRenderDrawColor(render,0,255,0,255);
    texture = loadImage(ren,"posi.bmp");
    if(pl==2)
    {
        if(tab[x+1][y+1]==1)
        {
            drc.x=(x+1)*50;
            drc.y=(y+1)*50;
            tab[x+1][y+1]=20;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
        if((tab[x+1][y+1]==3 || tab[x+1][y+1]==6) && tab[x+2][y+2]==1)
        {
            drc.x=(x+2)*50;
            drc.y=(y+2)*50;
            tab[x+2][y+2]=22;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
        if(tab[x-1][y+1]==1)
        {
            drc.x=(x-1)*50;
            drc.y=(y+1)*50;
            tab[x-1][y+1]=20;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
        if((tab[x-1][y+1]==3 || tab[x-1][y+1]==6) && tab[x-2][y+2]==1)
        {
            drc.x=(x-2)*50;
            drc.y=(y+2)*50;
            tab[x-2][y+2]=21;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
    }
    if(pl==3)
    {
        if(tab[x+1][y-1]==1)
        {
            drc.x=(x+1)*50;
            drc.y=(y-1)*50;
            tab[x+1][y-1]=30;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
        if((tab[x+1][y-1]==2 || tab[x+1][y-1]==4) && tab[x+2][y-2]==1)
        {
            drc.x=(x+2)*50;
            drc.y=(y-2)*50;
            tab[x+2][y-2]=32;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
        if(tab[x-1][y-1]==1)
        {
            drc.x=(x-1)*50;
            drc.y=(y-1)*50;
            tab[x-1][y-1]=30;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
        if((tab[x-1][y-1]==2 || tab[x-1][y-1]==4) && tab[x-2][y-2]==1)
        {
            drc.x=(x-2)*50;
            drc.y=(y-2)*50;
            tab[x-2][y-2]=31;
            SDL_RenderCopy(ren,texture,NULL,&drc);
        }
    }
    if(pl==4 || pl==6)
    {
        int i=1,t=0;
        while(t==0)
        {
            if(tab[x+i][y+i]==1)
            {
                drc.x=(x+i)*50;
                drc.y=(y+i)*50;
                tab[x+i][y+i]=10;
                if(tab[x+i-1][y+i-1]%10==8 || tab[x+i-1][y+i-1]==12)
                   tab[x+i][y+i]=12;
                SDL_RenderCopy(ren,texture,NULL,&drc);
            }
            else if(((tab[x+i][y+i]==3 ||tab[x+i][y+i]==6) && pl==4) || ((tab[x+i][y+i]==2 || tab[x+i][y+i]==4) && pl==6))
            {
                if(tab[x+1+i][y+1+i]==1)
                {
                    drc.x=(x+i+1)*50;
                    drc.y=(y+i+1)*50;
                    tab[x+i][y+i]=8+10*tab[x+i][y+i];
                    SDL_RenderCopy(ren,texture,NULL,&drc);
                }
                else t=1;
            }
            else t=1;
            i++;
        }
        i=1; t=0;
        while(t==0)
        {
            if(tab[x-i][y+i]==1)
            {
                drc.x=(x-i)*50;
                drc.y=(y+i)*50;
                tab[x-i][y+i]=10;
                if(tab[x-i+1][y+i-1]%10==8 || tab[x-i+1][y+i-1]==11)
                    tab[x-i][y+i]=11;
                SDL_RenderCopy(ren,texture,NULL,&drc);
            }
            else if(((tab[x-i][y+i]==3 ||tab[x-i][y+i]==6) && pl==4) || ((tab[x-i][y+i]==2 || tab[x-i][y+i]==4) && pl==6))
            {
                if(tab[x-1-i][y+1+i]==1)
                {
                    drc.x=(x-i-1)*50;
                    drc.y=(y+i+1)*50;
                    tab[x-i][y+i]=8+10*tab[x-i][y+i];
                    SDL_RenderCopy(ren,texture,NULL,&drc);
                }
                else t=1;
            }
            else t=1;
            i++;
        }
        i=1; t=0;
        while(t==0)
        {
            if(tab[x+i][y-i]==1)
            {
                drc.x=(x+i)*50;
                drc.y=(y-i)*50;
                tab[x+i][y-i]=10;
                if(tab[x+i-1][y-i+1]%10==8 || tab[x+i-1][y-i+1]==13)
                   tab[x+i][y-i]=13;
                SDL_RenderCopy(ren,texture,NULL,&drc);
            }
            else if(((tab[x+i][y-i]==3 ||tab[x+i][y-i]==6) && pl==4) || ((tab[x+i][y-i]==2 || tab[x+i][y-i]==4) && pl==6))
            {
                if(tab[x+1+i][y-1-i]==1)
                {
                    drc.x=(x+i+1)*50;
                    drc.y=(y-i-1)*50;
                    tab[x+i][y-i]=8+10*tab[x+i][y-i];
                    SDL_RenderCopy(ren,texture,NULL,&drc);
                }
                else t=1;
            }
            else t=1;
            i++;
        }
        i=1; t=0;
        while(t==0)
        {
            if(tab[x-i][y-i]==1)
            {
                drc.x=(x-i)*50;
                drc.y=(y-i)*50;
                tab[x-i][y-i]=10;
                if(tab[x-i+1][y-i+1]%10==8 || tab[x-i+1][y-i+1]==14)
                    tab[x-i][y-i]=14;
                SDL_RenderCopy(ren,texture,NULL,&drc);
            }
            else if(((tab[x-i][y-i]==3 ||tab[x-i][y-i]==6) && pl==4) || ((tab[x-i][y-i]==2 || tab[x-i][y-i]==4) && pl==6))
            {
                if(tab[x-1-i][y-1-i]==1)
                {
                    drc.x=(x-i-1)*50;
                    drc.y=(y-i-1)*50;
                    tab[x-i][y-i]=8+10*tab[x-i][y-i];
                    SDL_RenderCopy(ren,texture,NULL,&drc);
                }
                else t=1;
            }
            else t=1;
            i++;
        }
    }
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_DestroyTexture(texture);
    return ren;
}

int pion_out(int tab[Tlen][Tlen], int ply)
{
   int i;
   if(ply==1)
   {
       for(i=0;i<Tlen;i++)
        if(tab[i][0]==-1)
            return i;
        if(tab[0][1]==-1)
            return 10;
        if(tab[0][2]==-1)
        {
            tab[0][3]=23;
            return 20;
        }
   }
   if(ply==2)
   {
        for(i=0;i<Tlen;i++)
        if(tab[i][Tlen-1]==-1)
            return i;
        if(tab[Tlen-1][Tlen-2]==-1)
            return 10;
        if(tab[Tlen-1][Tlen-3]==-1)
        {
            tab[Tlen-1][Tlen-4]=23;
            return 20;
        }
   }

}

int depl_pion_abs(int x, int y, int tab[Tlen][Tlen], int tr)
{
    SDL_Event event;
    int i=0,xt, yt;
    x=x/50;
    y=y/50;
    if(x>10 || x==0 || y>10 || y==0 || tab[x][y] == 0 || tab[x][y]==1)
        { printf("-- x = %d   y = %d v = %d\n",x,y,tab[x][y]);return tr;}
    if(tab[x][y]==2 && tr==1)
    {
        printf("choisir ou deplacer\n");
        while(i==0)
        {
            SDL_WaitEvent(&event);
            if(event.type == SDL_QUIT)
                SDL_Quit();
            else
                if(event.type == SDL_MOUSEBUTTONUP)
                    i=1;
        }
        xt = event.button.x/50;
        yt = event.button.y/50;

        if(tab[xt][yt]==20)
        {
            tab[x][y]=1;
            tab[xt][yt]=2;
            if(yt==Tlen-2)
                tab[xt][yt]=4;
            return 2;
        }
        if(tab[xt][yt]==21)
        {
            tab[x][y]=1;
            tab[x-1][y+1]=1;
            tab[xt][yt]=2;
            if(yt==Tlen-2)
                tab[xt][yt]=4;
            if(pion_out(tab,tr)==10)
            {
                tab[0][1]=-3;
            }else
            if(pion_out(tab,tr)==20)
            {
                tab[0][2]=-3;
            }else
            {
                tab[pion_out(tab,tr)][0]=-3;
            }
            return 2;
        }
        if(tab[xt][yt]==22)
        {
            tab[x][y]=1;
            tab[x+1][y+1]=1;
            tab[xt][yt]=2;
            if(yt==Tlen-2)
                tab[xt][yt]=4;
            if(pion_out(tab,tr)==10)
            {
                tab[0][1]=-3;
            }else
            if(pion_out(tab,tr)==20)
            {
                tab[0][2]=-3;
            }else
            {
                tab[pion_out(tab,tr)][0]=-3;
            }
            return 2;
        }
        printf("deplacement impossible 2\n");
        return 1;
    }
    if((tab[x][y]==2|| tab[x][y]==4) && tr==2)
    {
        printf("mauvais pion\n");
        return 2;
    }
    if(tab[x][y]==3 && tr==2)
    {
        printf("choisir ou deplacer\n");
        while(i==0)
        {
            SDL_WaitEvent(&event);
            if(event.type == SDL_QUIT)
                SDL_Quit();
            else
                if(event.type == SDL_MOUSEBUTTONUP)
                    i=1;
        }
        xt = event.button.x/50;
        yt = event.button.y/50;

        if(tab[xt][yt]==30)
        {
            tab[x][y]=1;
            tab[xt][yt]=3;
            if(yt==1)
                tab[xt][yt]=6;
            return 1;
        }
        if(tab[xt][yt]==31)
        {
            tab[x][y]=1;
            tab[x-1][y-1]=1;
            tab[xt][yt]=3;
            if(yt==1)
                tab[xt][yt]=6;
            if(pion_out(tab,tr)==10)
            {
                tab[Tlen-1][Tlen-2]=-2;
            }else
            if(pion_out(tab,tr)==20)
            {
                tab[Tlen-1][Tlen-3]=-2;
            }else
            {
                tab[pion_out(tab,tr)][Tlen-1]=-2;
            }
            return 1;
        }
        if(tab[xt][yt]==32)
        {
            tab[x][y]=1;
            tab[x+1][y-1]=1;
            tab[xt][yt]=3;
            if(yt==1)
                tab[xt][yt]=6;
            if(pion_out(tab,tr)==10)
            {
                tab[Tlen-1][Tlen-2]=-2;
            }else
            if(pion_out(tab,tr)==20)
            {
                tab[Tlen-1][Tlen-3]=-2;
            }else
            {
                tab[pion_out(tab,tr)][Tlen-1]=-2;
            }
            return 1;
        }
        printf("deplacement impossible 2\n");
        return 2;
    }
    if((tab[x][y]==3 || tab[x][y]==6) && tr==1)
    {
        printf("mauvais pion\n");
        return 1;
    }

    if(tab[x][y]==4 && tr==1)
    {
        printf("choisir ou deplacer\n");
        while(i==0)
        {
            SDL_WaitEvent(&event);
            if(event.type == SDL_QUIT)
                SDL_Quit();
            else
                if(event.type == SDL_MOUSEBUTTONUP)
                    i=1;
        }
        xt = event.button.x/50;
        yt = event.button.y/50;
        if(tab[xt][yt]==10)
        {
            tab[x][y]=1;
            tab[xt][yt]=4;
            return 2;
        }
        if(tab[xt][yt]==11)
        {
            tab[x][y]=1;
            tab[xt][yt]=4;
            i=1;
            while(x-i != xt)
            {
                if(tab[x-i][y+i]==38 || tab[x-i][y+i]==68)
                {
                    tab[x-i][y+i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[0][1]=-3;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[0][2]=-3;
                    }else
                    {
                        tab[pion_out(tab,tr)][0]=-3;
                    }
                }
                i++;
            }
            return 2;
        }
        if(tab[xt][yt]==12)
        {
            tab[x][y]=1;
            tab[xt][yt]=4;
            i=1;
            while(x+i != xt)
            {
                if(tab[x+i][y+i]==38 || tab[x+i][y+i]==68)
                {
                    tab[x+i][y+i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[0][1]=-3;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[0][2]=-3;
                    }else
                    {
                        tab[pion_out(tab,tr)][0]=-3;
                    }
                }
                i++;
            }
            return 2;
        }
        if(tab[xt][yt]==13)
        {
            tab[x][y]=1;
            tab[xt][yt]=4;
            i=1;
            while(x+i != xt)
            {
                if(tab[x+i][y-i]==38 || tab[x+i][y-i]==68)
                {
                    tab[x+i][y-i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[0][1]=-3;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[0][2]=-3;
                    }else
                    {
                        tab[pion_out(tab,tr)][0]=-3;
                    }
                }
                i++;
            }
            return 2;
        }
        if(tab[xt][yt]==14)
        {
            tab[x][y]=1;
            tab[xt][yt]=4;
            i=1;
            while(x-i != xt)
            {
                if(tab[x-i][y-i]==38 || tab[x-i][y-i]==68)
                {
                    tab[x-i][y-i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[0][1]=-3;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[0][2]=-3;
                    }else
                    {
                        tab[pion_out(tab,tr)][0]=-3;
                    }
                }
                i++;
            }
            return 2;
        }
        printf("deplacement impossible 2\n");
        return 2;
    }

    if(tab[x][y]==6 && tr==2)
    {
        printf("choisir ou deplacer\n");
        while(i==0)
        {
            SDL_WaitEvent(&event);
            if(event.type == SDL_QUIT)
                SDL_Quit();
            else
                if(event.type == SDL_MOUSEBUTTONUP)
                    i=1;
        }
        xt = event.button.x/50;
        yt = event.button.y/50;
        if(tab[xt][yt]==10)
        {
            tab[x][y]=1;
            tab[xt][yt]=6;
            return 1;
        }
        if(tab[xt][yt]==11)
        {
            tab[x][y]=1;
            tab[xt][yt]=6;
            i=1;
            while(x-i != xt)
            {
                if(tab[x-i][y+i]==28 || tab[x-i][y+i]==48)
                {
                    tab[x-i][y+i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[Tlen-1][Tlen-2]=-2;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[Tlen-1][Tlen-3]=-2;
                    }else
                    {
                        tab[pion_out(tab,tr)][Tlen-1]=-2;
                    }
                }
                i++;
            }
            return 1;
        }
        if(tab[xt][yt]==12)
        {
            tab[x][y]=1;
            tab[xt][yt]=6;
            i=1;
            while(x+i != xt)
            {
                if(tab[x+i][y+i]==28 || tab[x+i][y+i]==48)
                {
                    tab[x+i][y+i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[Tlen-1][Tlen-2]=-2;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[Tlen-1][Tlen-3]=-2;
                    }else
                    {
                        tab[pion_out(tab,tr)][Tlen-1]=-2;
                    }
                }
                i++;
            }
            return 1;
        }
        if(tab[xt][yt]==13)
        {
            tab[x][y]=1;
            tab[xt][yt]=6;
            i=1;
            while(x+i != xt)
            {
                if(tab[x+i][y-i]==28 || tab[x+i][y-i]==48)
                {
                    tab[x+i][y-i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[Tlen-1][Tlen-2]=-2;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[Tlen-1][Tlen-3]=-2;
                    }else
                    {
                        tab[pion_out(tab,tr)][Tlen-1]=-2;
                    }
                }
                i++;
            }
            return 1;
        }
        if(tab[xt][yt]==14)
        {
            tab[x][y]=1;
            tab[xt][yt]=6;
            i=1;
            while(x-i != xt)
            {
                if(tab[x-i][y-i]==28 || tab[x-i][y-i]==48)
                {
                    tab[x-i][y-i]=1;
                    if(pion_out(tab,tr)==10)
                    {
                        tab[Tlen-1][Tlen-2]=-2;
                    }else
                    if(pion_out(tab,tr)==20)
                    {
                        tab[Tlen-1][Tlen-3]=-2;
                    }else
                    {
                        tab[pion_out(tab,tr)][Tlen-1]=-2;
                    }
                }
                i++;
            }
            return 1;
        }
        printf("deplacement impossible 2\n");
        return 2;
    }
}

int fin(int tab[Tlen][Tlen],int tr,SDL_Renderer *ren)
{
    int i,j;
    if(tab[0][3]==23)
        return 10;
    if(tab[Tlen-1][Tlen-4]==23)
        return 20;
    if(tr==1)
    {
        for(i=1;i<Tlen-1;i++)
            for(j=1;j<Tlen-1;j++)
            {
                if(tab[i][j]==2)
                    pos_jouable(i,j,2,tab,ren);
                if(tab[i][j]==4)
                    pos_jouable(i,j,4,tab,ren);
            }
        for(i=1;i<Tlen-1;i++)
            for(j=1;j<Tlen-1;j++)
                if(tab[i][j]==10 || tab[i][j]==11 || tab[i][j]==12 ||
                   tab[i][j]==13 || tab[i][j]==14 || tab[i][j]==20 ||
                   tab[i][j]==21 || tab[i][j]==22)
                    {
                        reinit(tab,tr);
                        return 0;
                    }
        return 20;
    }
    if(tr==2)
    {
        for(i=1;i<Tlen-1;i++)
            for(j=1;j<Tlen-1;j++)
            {
                if(tab[i][j]==3)
                    pos_jouable(i,j,3,tab,ren);
                if(tab[i][j]==6)
                    pos_jouable(i,j,6,tab,ren);
            }
        for(i=1;i<Tlen-1;i++)
            for(j=1;j<Tlen-1;j++)
                if(tab[i][j]==10 || tab[i][j]==11 || tab[i][j]==12 ||
                   tab[i][j]==13 || tab[i][j]==14 || tab[i][j]==30 ||
                   tab[i][j]==31 || tab[i][j]==32)
                    {
                        reinit(tab,tr);
                        return 0;
                    }
        return 10;
    }
}

int main(int argc, char *argv[])
{
    int i, Tplato[Tlen][Tlen];
    int tour=2;
    SDL_Event event;
    SDL_Surface *surf=NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Color fond = {50,81,7,255};
    SDL_Texture *texture = NULL;
    TTF_Font *texte = NULL;
    int statut = EXIT_FAILURE;

    inittial(Tplato);
    aff_plato(Tplato);
    if(0 != SDL_Init(SDL_INIT_VIDEO))
         {error("Erreur SDL_Init : %s"); goto Quit;}

/*    if(-1 == TTF_Init())
    {
        fprintf(stderr,"erreur TTF_Init %s",TTF_GetError);
        goto Quit;
    }*/
//    texte = TTF_OpenFont("Rattoney.ttf",25);
    window = SDL_CreateWindow("Test", 10,40,
                              640, 640, SDL_WINDOW_SHOWN);
    if(NULL == window)
        {error("Erreur SDL_CreateWindow : %s");goto Quit;}

    render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(NULL == render)
        {error("Erreur SDL_CreateRenderer : %s");goto Quit;}

    texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_RGBA8888,
                      SDL_TEXTUREACCESS_TARGET,500,500);
    if(NULL==texture)
    {
        error("erreur sdl_createtexture %s");
        goto Quit;
    }
//    surf = TTF_RenderText_Blended(texte,"pour un debut",fond);
    texture = loadImage(render,"echi1.bmp");
    render = plateau(render,texture,Tplato,fond);
    SDL_RenderPresent(render);
    i=0;
    int winner;
    while(i==0)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            i=1;
        else
            if(event.type == SDL_MOUSEBUTTONUP)
        {
            SDL_RenderClear(render);
            render = plateau(render,texture,Tplato,fond);
            printf("x = %d   y = %d\n",event.button.x,event.button.y);
            SDL_RenderPresent(pos_jouable(event.button.x/50,event.button.y/50,Tplato[event.button.x/50][event.button.y/50],Tplato,render));
            printf("\ntableau des positions jouables\n");
            aff_plato(Tplato);
            tour = depl_pion_abs(event.button.x,event.button.y,Tplato,tour);
            printf("\ntableau du coup joué\n");
            aff_plato(Tplato);
            reinit(Tplato,tour);
            printf("\ntableau reinitialise\n");
            aff_plato(Tplato);
            SDL_RenderClear(render);
            render = plateau(render,texture,Tplato,fond);
            SDL_RenderPresent(render);
            i=fin(Tplato, tour, render);
            if(i!=0)
                winner=i;
        }
    }
    statut = EXIT_SUCCESS;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
//    TTF_CloseFont(texte);
Quit:
//    TTF_Quit();
    SDL_Quit();
    return statut;
}
