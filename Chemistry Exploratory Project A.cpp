#include<array>
#include<cassert>
#include<fstream>
#include<iostream>
#include<random>
#include<thread>
#include<vector>
#include<windows.h>
using namespace std;
#define l 300//250//720 //195
#define b 300//160//160 //45
#define E ' '
#define C 'C'
#define O 'O'
#define ORIGINAL 15
#define C_col 150
#define O_col 100
#define End_message_space 0
#define max_iter 100000000
#define mf_bin 0.01
#define vis_out 0

ofstream out("out.txt");

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int y, int x)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(console,coord);
}

int main()
{
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

    while(yCO<=1)
    {
        system("cls");
        if(vis_out)
        gotoxy(b+End_message_space,0);
        cout<<"yCO="<<yCO;
        array<string,b> grid;
        gotoxy(0,0);
        for(int i=0;i<b;i++)
        {
            grid[i]=string(l,E);
            if(vis_out)
            cout<<grid[i]<<'\n';
        }
        int voids=l*b,CO_area=0,O_area=0,CO2_prod=0;
        default_random_engine generator;
        uniform_real_distribution<double> mf_parameter(0,yCO+yO2);
        long long it_no=0;
        long double ACF_O=0,ACF_CO=0;

        int z=max_iter;
        while(z--)
        {
            if(mf_parameter(generator)<yCO)    // Carbon Monoxide
            {
                int x=rand()%l,y=rand()%b;
                if(grid[y][x]==E)
                {
                    vector<pair<int,int>> v;

                    if(grid[(y+1)%b][x]==O)
                        v.push_back({(y+1)%b,x});
                    if(grid[(y-1+b)%b][x]==O)
                        v.push_back({(y-1+b)%b,x});
                    if(grid[y][(x+1)%l]==O)
                        v.push_back({y,(x+1)%l});
                    if(grid[y][(x-1+l)%l]==O)
                        v.push_back({y,(x-1+l)%l});

                    if(v.empty())
                    {
                        if(vis_out)
                        SetConsoleTextAttribute(console,C_col);
                        grid[y][x]=C;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<C;
                        }
                        voids--;
                        CO_area++;
                        if(vis_out)
                        SetConsoleTextAttribute(console,ORIGINAL);
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
                        O_area--;
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
                        if(vis_out)
                        SetConsoleTextAttribute(console,O_col);
                        grid[y][x]=O;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<O;
                        }
                        voids--;
                        O_area++;
                        if(vis_out)
                        SetConsoleTextAttribute(console,ORIGINAL);
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
                        if(vis_out)
                        SetConsoleTextAttribute(console,O_col);
                        grid[y2][x2]=O;
                        if(vis_out)
                        {
                        gotoxy(y2,x2);
                        cout<<O;
                        }
                        voids--;
                        O_area++;
                        if(vis_out)
                        SetConsoleTextAttribute(console,ORIGINAL);
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
                if(grid[y][x]==E)
                {
                    vector<pair<int,int>> v;

                    if(grid[(y+1)%b][x]==O)
                        v.push_back({(y+1)%b,x});
                    if(grid[(y-1+b)%b][x]==O)
                        v.push_back({(y-1+b)%b,x});
                    if(grid[y][(x+1)%l]==O)
                        v.push_back({y,(x+1)%l});
                    if(grid[y][(x-1+l)%l]==O)
                        v.push_back({y,(x-1+l)%l});

                    if(v.empty())
                    {
                        if(vis_out)
                        SetConsoleTextAttribute(console,C_col);
                        grid[y][x]=C;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<C;
                        }
                        voids--;
                        CO_area++;
                        if(vis_out)
                        SetConsoleTextAttribute(console,ORIGINAL);
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
                        O_area--;
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
                        if(vis_out)
                        SetConsoleTextAttribute(console,O_col);
                        grid[y][x]=O;
                        if(vis_out)
                        {
                        gotoxy(y,x);
                        cout<<O;
                        }
                        voids--;
                        O_area++;
                        if(vis_out)
                        SetConsoleTextAttribute(console,ORIGINAL);
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
                        if(vis_out)
                        SetConsoleTextAttribute(console,O_col);
                        grid[y2][x2]=O;
                        if(vis_out)
                        {
                        gotoxy(y2,x2);
                        cout<<O;
                        }
                        voids--;
                        O_area++;
                        if(vis_out)
                        SetConsoleTextAttribute(console,ORIGINAL);
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
            //this_thread::sleep_for(1ns);
            it_no++;
        }
        ACF_O/=max_iter;
        ACF_O/=l*b;
        ACF_CO/=max_iter;
        ACF_CO/=l*b;
        if(vis_out)
        gotoxy(b+End_message_space,0);
        cout<<"\rSimulation Ended"<<' '<<yCO;
        out<<yCO<<' '<<ACF_CO<<' '<<ACF_O<<' '<<(float)CO2_prod/max_iter<<'\n';
        //system("pause");
        yCO+=mf_bin;
        yO2-=mf_bin;
    }
}
