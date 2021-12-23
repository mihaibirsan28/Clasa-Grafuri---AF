#include <bits/stdc++.h>

#define Nmax 100010

using namespace std;

//ifstream fin("apm.in");
//ofstream fout("apm.out");


ifstream fin("ciclueuler.in");
ofstream fout("ciclueuler.out");


class disjoint
{
private:
    int n;
    int par[Nmax];
    int dim[Nmax];

public:

    disjoint():n(0)
    {

    }

    disjoint(int x):n(x)
    {

    }

    int findd(int x)
    {
        while(x != par[x])
        {
            x = par[x];
        }
        return x;
    }

    void unite(int x, int y)
    {
        x = findd(x);
        y = findd(y);
        if(dim[x] >= dim[y])
        {
            par[y] = x;
            dim[x] += dim[y];
        }
        else
        {
            par[x] = y;
            dim[y] += dim[x];
        }
    }

    void init()
    {
        for(int i=1; i<=n; i++)
        {
            par[i] = i;
            dim[i] = 1;
        }
    }
};


class Graf
{
private:
    int n;
    int m;
    vector<vector<int>> gr;   //graf -> lista de adiacenta
    vector<vector<int>> gr_t; //graf transpus

    int viz[Nmax] ;

public:
    Graf(): n(0), m(0)
    {

    }

    Graf(int x, int y = 0): n(x), m(y)
    {

    }

    Graf(int x, int y, const vector<vector<int>>& v): n(x), m(y), gr(v)
    {

    }

    void adaugarea_muchie(int x, int y)
    {
        gr[x].push_back(y);
    }

    //dezvizitatam toate nodurile
    void nevizitate()
    {
        for(int i = 1; i <= n; i++)
            viz[i] = 0;
    }
    //bfs
    void BFS(int start, int dist[])
    {
        int c[Nmax];
        int pr, ul;
        pr = 0;
        ul = -1;
        int n_curent;
        dist[start] = 0;
        c[++ul] = start;
        while(pr <= ul)
        {
            n_curent = c[pr++];
            for(auto i:gr[n_curent]) //verificam toti vecinii nodului curent
                if(dist[i] == -1)
                {
                    dist[i] = dist[n_curent] + 1;
                    c[++ul] = i;
                }
        }
    }


    //dfs
    void DFS(int x)
    {
        //int nr_componente_conexe;
        viz[x] = 1;
        for(int i = 0; i < gr[x].size(); ++i)
        {
            if(viz[gr[x][i]] == 0)
            {
                viz[gr[x][i]] = 1;
                DFS(gr[x][i]);
            }
        }
    }

    //numar componente conexe
    int nr_comp_conexe()
    {
        int nr_componente_conexe = 0;
        for(int i = 1; i <= n; i++)
        {
            if(viz[i] == 0)
            {
                DFS(i);
                nr_componente_conexe += 1;
            }
        }
        return nr_componente_conexe;
    }


    //sortare topologica
    void sortare_topologica(int x, vector<int> topo)
    {
        viz[x] = 1;
        for(int i = 0; i < gr[x].size(); ++i)
        {
            if(viz[gr[x][i]] == 0)
                sortare_topologica(gr[x][i], topo);
        }
        topo.push_back(x);
    }

    //Componente tare conexe
    void DFS2(int x,int ct,vector<vector<int>> ctc,  int comp[])
    {
        viz[x] = 1;
        comp[x] = ct;
        ctc[ct].push_back(x);
        for(int i = 0; i< gr_t[x].size(); i++)
        {
            if(viz[gr_t[x][i]] == 0)
            {
                DFS2(gr_t[x][i],ct,ctc,comp);
            }
        }
    }

    void componenteTareConexe(int ct, int comp[], const vector<vector<int>>& ctc,  vector<int> topo)
    {
        nevizitate();
        for(int i=1;i<=n;++i)
        {
            if(viz[i] == 0)
            {
                sortare_topologica(i, topo);
            }
        }
        reverse(topo.begin(), topo.end());
        for(auto i:topo)
        {
            if(comp[i] == 0)
            {
                ct += 1;
                DFS2(i, ct, ctc, comp);
            }
        }
    }
    //Havel Hakimi
    bool HavelHakimi(vector<int> &grade)
    {
        sort(grade.begin(), grade.end(), greater<>());
        while(grade.size() > 0)
        {
            if(grade[0] + 1 > grade.size())
                return false;

            for(int i = 1; i <= grade[0]; i++)
            {
                grade[i] -= 1;
                if(grade[i] < 0)
                {
                    return false;
                }
            }
            grade.erase(grade.begin());
            sort(grade.begin(), grade.end(), greater<>());
        }
        return true;
    }
    //Paduri de multimi disjuncte


    //Arbore partial de cost minim -> folosim algoritmul kruskal
    vector<pair<int, pair<int, int>>> kruskal(vector<pair<int, pair<int, int>>>& muchii)
    {
        disjoint disj(n);
        vector<pair<int, pair<int, int>>> subgr;
        sort(muchii.begin(), muchii.end());
        disj.init();
        for(auto &muchie : muchii)
        {
            if(disj.findd(muchie.second.first) != disj.findd(muchie.second.second))
            {
                disj.unite(muchie.second.first, muchie.second.second);
                subgr.push_back({muchie.first, {muchie.second.first, muchie.second.second}});
            }
        }
        return subgr;
    }

    //Diametrul unui arbore
    void dfsParcurgere(int x, int d, int &dmax, int &nmax)
    {
        if(d > dmax)
        {
            dmax = d;
            nmax = x;
        }
        viz[x] = 1;
        for(auto k:gr[x])
        {
            if(viz[k] == 0)
            {
                dfsParcurgere(k, d+1, dmax, nmax);
            }
        }
    }

    int diamentru()
    {
        int nmax = 0, dmax = 0;
        dfsParcurgere(1, 0, dmax, nmax);
        int n1 = nmax;
        nevizitate();
        dmax = 0;
        nmax = 0;
        dfsParcurgere(n1, 0, dmax, nmax);
        return dmax + 1;
    }



    void royFloyd(int c[][101])
    {
        int i,j,k;
        for( k = 1; k <= n; k++ )
        {
            for( i = 1; i <= n; i++ )
            {
                for( j = 1; j <= n; j++ )
                {
                    //if (c[i][k] && c[k][j])
                    if (c[i][j] > c[i][k] + c[k][j])
                        c[i][j] = c[i][k] + c[k][j];
                }
            }
        }
    }



    void ciclu_Euler(int start, vector<pair<int,int>> g[], vector<int> &ciclu)
    {
        stack<int> st;
        st.push(start); //adaugam nodul de la care pornim in stiva
        while(!st.empty()) //cat timp avem elem in stiva
        {
            int n = st.top();
            if(!g[n].empty())
            {

                int vecin = g[n][g[n].size()-1].first; //ultimul vecin al nodului din lista de adiacenta
                int nr = g[n][g[n].size()-1].second; //nr pentru muchia respectiva
                g[n].pop_back();
                if(viz[nr] == 0)//daca muchia respectiva nu a mai fost vizitata
                {
                    viz[nr] = 1;
                    st.push(vecin);
                }
            }
            else
            {
                ciclu.push_back(n);
                st.pop();
            }
        }
    }

    void exista_ciclu_Euler(vector<pair<int,int>> g[])
    {
        vector<int> ciclu;
        for(int i = 0; i <= n; i++)
        {
            if(g[i].size() % 2 == 1)//verificam daca exista noduri cu grad impar
            {
                fout << "-1";
                return ;
            }
        }
        ciclu_Euler(1, g, ciclu);

        for(int i = 0; i < ciclu.size() - 1; i++)
        {
            fout << ciclu[i] << " ";
        }
    }

};


int main()
{

    int nr_componente_conexe = 0;
    int par[Nmax];
    int dim[Nmax];
    vector<int> topo;
    int dist[Nmax] = {-1};
    int n,m,x,y;
    vector<vector<int>> v(n + 1);
    fin >> n >> m;
    for(int i = 1; i <= m; i++)
    {
        fin >> x >> y;
        v[x].push_back(y);
        v[y].push_back(x);
    }
    Graf graf(n,m, v);
    //sau
    /*
    fin >> n >> m;
    Graf graf(n, m)
    for(int i = 1; i <= m; i++)
    {
        fin >> x >> y;
        graf.adaugarea_muchie(x,y);
        graf.adaugarea_muchie(y,x);
    }
    */
}
