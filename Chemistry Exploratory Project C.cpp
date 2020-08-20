#include<array>
#include<cassert>
#include<fstream>
#include<iostream>
#include<random>
#include<thread>
#include<vector>
#include<windows.h>
using namespace std;
#define r 20
#define l ((r<<1)+1)//250//720 //195
#define b ((r<<1)+1)//160//160 //45
#define E ' '
#define C 'C'
#define O 'O'
#define OX 'X'
#define ORIGINAL 15
#define C_col 150
#define O_col 100
#define OX_col 200
#define End_message_space 0
#define max_iter 10000000
#define mf_bin 0.01
#define CT 0.0
#define vis_out 0

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void pr(ostream& o,char c)
{
    o<<c;
}

ostream& operator<<(ostream& o,char c)
{
    switch(c)
    {
        case C:SetConsoleTextAttribute(console,C_col);break;
        case O:SetConsoleTextAttribute(console,O_col);break;
        case OX:SetConsoleTextAttribute(console,OX_col);break;
    }
    pr(o,c);
    SetConsoleTextAttribute(console,ORIGINAL);
    return o;
}

ofstream out("out.txt");

void gotoxy(int y, int x)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(console,coord);
}

void oxidize_all(array<string,b> &grid)
{
    gotoxy(0,0);
    for(int i=0;i<b;i++)
    {
        for(int j=0;j<l;j++)
        {
            if(grid[i][j]=='O')
                grid[i][j]='X';
        }
    }
    if(vis_out)
    {
        for(int i=0;i<b;i++)
        {
            for(int j=0;j<l;j++)
                cout<<grid[i][j];
            cout<<'\n';
        }
    }
}

int main()
{
    // Assumptions are made in drawing the circle
    out.setf(ios::fixed);
    system("pause");
    float yCO=0.0,yO2=1.0;
    /*do
    {
        system("cls");
        cout<<"Enter the mole fraction of CO: ";
        cin>>yCO;
        cout<<"Enter the mole fraction of O2: ";
        cin>>yO2;
        if(yCO+yO2<0||yCO+yO2>1)
        {
            cout<<"\nYou entered wrong values of mole fractions. Please enter them again.\n";
            system("pause");
        }
    }while(yCO+yO2<=0||yCO+yO2>1);*/
    //yCO=0.5;yO2=0.5;
    while(yCO<=1)
    {
        system("cls");
        if(vis_out)
        gotoxy(b+End_message_space,0);
        cout<<"yCO="<<yCO;
        array<string,b> grid;
        int voids=l*b,CO_area=0,O_area=0,init_CO_area=0,init_O_area=0,CO2_prod=0;
        gotoxy(0,0);
        for(int i=0;i<b;i++)
        {
            grid[i]=string(l,E);
            for(int j=0;j<l;j++)
            {
                if(((l/2.0)-(j+0.5))*((l/2.0)-(j+0.5))+((b/2.0)-(i+0.5))*((b/2.0)-(i+0.5))>=r*r)
                {
                    grid[i][j]=O;
                    voids--;
                    init_O_area++;
                    O_area++;
                }
            }
        }
        if(O_area>CT*(l*b))
        {
            for(int i=0;i<b;i++)
            {
                for(int j=0;j<l;j++)
                {
                    if(grid[i][j]==O)
                        grid[i][j]=OX;
                }
            }
        }
        if(vis_out)
        {
            for(int i=0;i<b;i++)
            {
                for(int j=0;j<l;j++)
                    cout<<grid[i][j];
                cout<<'\n';
            }
        }
        int cat_sites=voids;
        //return 0;
        default_random_engine generator;
        uniform_real_distribution<double> mf_parameter(0,yCO+yO2);
        long long it_no=0;
        long double ACF_O=0,ACF_CO=0;
        long double theta_s=0; //Time averaged fraction of catalyst sites that are unoccupied.

        int z=max_iter;
        while(z--)
        {
            if(it_no==max_iter)
                break;

            if(mf_parameter(generator)<yCO)    // Carbon Monoxide
            {
                int x=rand()%l,y=rand()%b;
                if(grid[y][x]==OX&&(((l/2.0)-(x+0.5))*((l/2.0)-(x+0.5))+((b/2.0)-(y+0.5))*((b/2.0)-(y+0.5))<r*r))
                {
                    grid[y][x]=E;
                    if(vis_out)
                    {
                    gotoxy(y,x);
                    cout<<E;
                    }
                    O_area--;
                    voids++;
                }
                else if(grid[y][x]==E)
                {
                    vector<pair<int,int>> v;

                    if(grid[(y+1)%b][x]==O||grid[(y+1)%b][x]==OX)
                        v.push_back({(y+1)%b,x});
                    if(grid[(y-1+b)%b][x]==O||grid[(y-1+b)%b][x]==OX)
                        v.push_back({(y-1+b)%b,x});
                    if(grid[y][(x+1)%l]==O||grid[y][(x+1)%l]==OX)
                        v.push_back({y,(x+1)%l});
                    if(grid[y][(x-1+l)%l]==O||grid[y][(x-1+l)%l]==OX)
                        v.push_back({y,(x-1+l)%l});

                    if(v.empty())
                    {
                        grid[y][x]=C;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<C;
                        }
                        voids--;
                        CO_area++;
                    }
                    else
                    {
                        int z=rand()%v.size();
                        if(((l/2.0)-(v[z].second+0.5))*((l/2.0)-(v[z].second+0.5))+((b/2.0)-(v[z].first+0.5))*((b/2.0)-(v[z].first+0.5))<r*r)
                        {
                            grid[v[z].first][v[z].second]=E;
                            voids++;
                            O_area--;
                        }
                        if(vis_out&&(grid[v[z].first][v[z].second]==E))
                        {
                            gotoxy(v[z].first,v[z].second);
                            cout<<grid[v[z].first][v[z].second];
                        }
                    }
                }
            }
            else    //Oxygen
            {
                int x=rand()%l,y=rand()%b;
                bool V=rand()&1;
                if(grid[y][x]==E&&((V?grid[(y+1)%b][x]:grid[y][(x+1)%l])==E))
                {
                    int x2=(V?x:(x+1)%l),y2=(V?(y+1)%b:y);

                    vector<pair<int,int>> v;

                    if(grid[(y+1)%b][x]==C)
                        v.push_back({(y+1)%b,x});
                    if(grid[(y-1+b)%b][x]==C)
                        v.push_back({(y-1+b)%b,x});
                    if(grid[y][(x+1)%l]==C)
                        v.push_back({y,(x+1)%l});
                    if(grid[y][(x-1+l)%l]==C)
                        v.push_back({y,(x-1+l)%l});

                    if(v.empty())
                    {
                        grid[y][x]=(O_area>CT*(l*b))?OX:O;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<grid[y][x];
                        }
                        voids--;
                        O_area++;
                        if(O_area>CT*(l*b)&&O_area-1<=CT*(l*b))
                            oxidize_all(grid);
                    }
                    else
                    {
                        int z=rand()%v.size();
                        grid[v[z].first][v[z].second]=E;
                        if(vis_out)
                        {
                        gotoxy(v[z].first,v[z].second);
                        cout<<E;
                        }
                        voids++;
                        CO_area--;
                    }
                    v.clear();

                    if(grid[(y2+1)%b][x2]==C)
                        v.push_back({(y2+1)%b,x2});
                    if(grid[(y2-1+b)%b][x2]==C)
                        v.push_back({(y2-1+b)%b,x2});
                    if(grid[y2][(x2+1)%l]==C)
                        v.push_back({y2,(x2+1)%l});
                    if(grid[y2][(x2-1+l)%l]==C)
                        v.push_back({y2,(x2-1+l)%l});

                    if(v.empty())
                    {
                        grid[y2][x2]=(O_area>CT*(l*b))?OX:O;
                        if(vis_out)
                        {
                        gotoxy(y2,x2);
                        cout<<grid[y2][x2];
                        }
                        voids--;
                        O_area++;
                        if(O_area>CT*(l*b)&&O_area-1<=CT*(l*b))
                            oxidize_all(grid);
                    }
                    else
                    {
                        int z=rand()%v.size();
                        grid[v[z].first][v[z].second]=E;
                        if(vis_out)
                        {
                        gotoxy(v[z].first,v[z].second);
                        cout<<E;
                        }
                        voids++;
                        CO_area--;
                    }
                }
            }
        }

        //Steady state
        while(1)
        {
            if(it_no==max_iter)
                break;

            if(mf_parameter(generator)<yCO)    // Carbon Monoxide
            {
                int x=rand()%l,y=rand()%b;
                if(grid[y][x]==OX&&(((l/2.0)-(x+0.5))*((l/2.0)-(x+0.5))+((b/2.0)-(y+0.5))*((b/2.0)-(y+0.5))<r*r))
                {
                    grid[y][x]=E;
                    if(vis_out)
                    {
                    gotoxy(y,x);
                    cout<<E;
                    }
                    O_area--;
                    voids++;
                }
                else if(grid[y][x]==E)
                {
                    vector<pair<int,int>> v;

                    if(grid[(y+1)%b][x]==O||grid[(y+1)%b][x]==OX)
                        v.push_back({(y+1)%b,x});
                    if(grid[(y-1+b)%b][x]==O||grid[(y-1+b)%b][x]==OX)
                        v.push_back({(y-1+b)%b,x});
                    if(grid[y][(x+1)%l]==O||grid[y][(x+1)%l]==OX)
                        v.push_back({y,(x+1)%l});
                    if(grid[y][(x-1+l)%l]==O||grid[y][(x-1+l)%l]==OX)
                        v.push_back({y,(x-1+l)%l});

                    if(v.empty())
                    {
                        grid[y][x]=C;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<C;
                        }
                        voids--;
                        CO_area++;
                    }
                    else
                    {
                        int z=rand()%v.size();
                        CO2_prod++;
                        if(((l/2.0)-(v[z].second+0.5))*((l/2.0)-(v[z].second+0.5))+((b/2.0)-(v[z].first+0.5))*((b/2.0)-(v[z].first+0.5))<r*r)
                        {
                            grid[v[z].first][v[z].second]=E;
                            voids++;
                            O_area--;
                        }
                        if(vis_out&&(grid[v[z].first][v[z].second]==E))
                        {
                            gotoxy(v[z].first,v[z].second);
                            cout<<grid[v[z].first][v[z].second];
                        }
                    }
                }
            }
            else    //Oxygen
            {
                int x=rand()%l,y=rand()%b;
                bool V=rand()&1;
                if(grid[y][x]==E&&((V?grid[(y+1)%b][x]:grid[y][(x+1)%l])==E))
                {
                    int x2=(V?x:(x+1)%l),y2=(V?(y+1)%b:y);

                    vector<pair<int,int>> v;

                    if(grid[(y+1)%b][x]==C)
                        v.push_back({(y+1)%b,x});
                    if(grid[(y-1+b)%b][x]==C)
                        v.push_back({(y-1+b)%b,x});
                    if(grid[y][(x+1)%l]==C)
                        v.push_back({y,(x+1)%l});
                    if(grid[y][(x-1+l)%l]==C)
                        v.push_back({y,(x-1+l)%l});

                    if(v.empty())
                    {
                        grid[y][x]=(O_area>CT*(l*b))?OX:O;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<grid[y][x];
                        }
                        voids--;
                        O_area++;
                        if(O_area>CT*(l*b)&&O_area-1<=CT*(l*b))
                            oxidize_all(grid);
                    }
                    else
                    {
                        int z=rand()%v.size();
                        grid[v[z].first][v[z].second]=E;
                        if(vis_out)
                        {
                        gotoxy(v[z].first,v[z].second);
                        cout<<grid[y][x];
                        }
                        voids++;
                        CO2_prod++;
                        CO_area--;
                    }
                    v.clear();

                    if(grid[(y2+1)%b][x2]==C)
                        v.push_back({(y2+1)%b,x2});
                    if(grid[(y2-1+b)%b][x2]==C)
                        v.push_back({(y2-1+b)%b,x2});
                    if(grid[y2][(x2+1)%l]==C)
                        v.push_back({y2,(x2+1)%l});
                    if(grid[y2][(x2-1+l)%l]==C)
                        v.push_back({y2,(x2-1+l)%l});

                    if(v.empty())
                    {
                        grid[y2][x2]=(O_area>CT*(l*b))?OX:O;
                        if(vis_out)
                        {
                        gotoxy(y2,x2);
                        cout<<grid[y2][x2];
                        }
                        voids--;
                        O_area++;
                        if(O_area>CT*(l*b)&&O_area-1<=CT*(l*b))
                            oxidize_all(grid);
                    }
                    else
                    {
                        int z=rand()%v.size();
                        grid[v[z].first][v[z].second]=E;
                        if(vis_out)
                        {
                        gotoxy(v[z].first,v[z].second);
                        cout<<E;
                        }
                        voids++;
                        CO2_prod++;
                        CO_area--;
                    }
                }
            }
            ACF_O+=O_area;
            ACF_CO+=CO_area;
            theta_s+=voids;
            //this_thread::sleep_for(1ns);
            it_no++;
        }
        ACF_O/=max_iter;
        ACF_O/=l*b;
        ACF_CO/=max_iter;
        ACF_CO/=l*b;
        theta_s/=max_iter;
        theta_s/=cat_sites;
        if(vis_out)
        gotoxy(b+End_message_space,0);
        cout<<"\rSimulation Ended"<<' '<<yCO;
        //out<<yCO<<' '<<ACF_CO<<' '<<ACF_O<<' '<<(float)CO2_prod/max_iter<<'\n';
        out<<yCO<<' '<<yCO*theta_s<<'\n';
        //system("pause");
        yCO+=mf_bin;
        yO2-=mf_bin;
    }
}
