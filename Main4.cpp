#include <bits/stdc++.h>
using namespace std;

#define x first
#define y second
typedef pair<int, int> pos;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count()); // random

int n, m;
pos boat, cur;
vector<vector<double>> P, q, P0, q0, a, t, p[5], Ppre;
vector<vector<vector<vector<double>>>> dis;
double time_stamp;
const double eps = 1e-9;

const pos dpos[] = {{0, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0}};
// 0, right, up, left, down

double Qt(pos i)
{
    return 1.0 - (1.0 - q[i.x][i.y]) / t[i.x][i.y] / a[i.x][i.y] * (exp(-a[i.x][i.y] * time_stamp) - exp(-a[i.x][i.y] * (time_stamp + t[i.x][i.y])));
}

double Psurrounding(pos i)
{
    double ret = 0;
    for (int k = 1; k <= 2; ++k)
        if (p[k][i.x][i.y] < 0)
            ret -= p[k][i.x][i.y] * Ppre[i.x + dpos[k].x][i.y + dpos[k].y];
    for (int k = 3; k <= 4; ++k)
        if (p[k][i.x][i.y] > 0)
            ret += p[k][i.x][i.y] * Ppre[i.x + dpos[k].x][i.y + dpos[k].y];
    return ret;
}

void read()
{
    cin >> n >> m;

    P.assign(n, vector<double>(m));
    q.assign(n, vector<double>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> P[i][j];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> q[i][j];
    P0 = P, q0 = q;

    a.assign(n, vector<double>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> a[i][j], a[i][j] = max(a[i][j], eps);

    t.assign(n, vector<double>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> t[i][j];

    dis.assign(n, vector<vector<vector<double>>>(m, vector<vector<double>>(n, vector<double>(m))));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < n; ++k)
                for (int l = 0; l < m; ++l)
                    cin >> dis[i][j][k][l];

    for (int k = 0; k < 5; ++k)
        p[k].assign(n, vector<double>(m, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> p[0][i][j];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            cin >> p[1][i][j];
            if (j + 1 < m)
                p[3][i][j + 1] = p[1][i][j];
        }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            cin >> p[2][i][j];
            if (i - 1 >= 0)
                p[4][i - 1][j] = p[2][i][j];
        }
}

pos boat0()
{
    int tmp = rnd() % 1000000;
    double sum = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            sum += P[i][j] * 1000000;
            if (sum >= tmp)
                return {i, j};
        }
    return {n - 1, m - 1}; // unnececessary
}

void init()
{
    P = P0, q = q0, boat = boat0();
    time_stamp = 0;
}

pos select_p()
{
    pos ret = {0, 0};
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (P[ret.x][ret.y] < P[i][j])
                ret = {i, j};
    return ret;
}

pos select_q()
{
    pos ret = {0, 0};
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (P[ret.x][ret.y] * (1 - Qt(ret)) < P[i][j] * (1 - Qt({i, j})))
                ret = {i, j};
    return ret;
}

bool find()
{
    if (cur != boat)
        return false;
    // cur == boat
    int tmp = rnd() % 1000000;
    return tmp >= q[boat.x][boat.y] * 1000000;
}

void change_p()
{
    Ppre = P;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            P[i][j] = Ppre[i][j] * p[0][i][j] / (1.0 - Ppre[cur.x][cur.y] * (1.0 - Qt(cur)));
    P[cur.x][cur.y] *= Qt(cur);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            P[i][j] += Psurrounding({i, j});
}

void change_boat()
{
    int tmp = rnd() % 1000000;
    double sum = 0;
    sum += p[0][boat.x][boat.y] * 1000000;
    if (sum >= tmp)
        return;

    for (int i = 1; i <= 2; ++i)
    {
        if (p[i][boat.x][boat.y] <= 0)
            continue;
        sum += p[i][boat.x][boat.y] * 1000000;
        if (sum >= tmp)
        {
            boat.x += dpos[i].x, boat.y += dpos[i].y;
            return;
        }
    }

    for (int i = 3; i <= 4; ++i)
    {
        if (p[i][boat.x][boat.y] >= 0)
            continue;
        sum -= p[i][boat.x][boat.y] * 1000000;
        if (sum >= tmp)
        {
            boat.x += dpos[i].x, boat.y += dpos[i].y;
            return;
        }
    }
}

pos select() { return select_p(); }

int run()
{
    init();

    int cnt = 0;
    while (true)
    {
        ++cnt;
        if (cnt > 100)
            return -1;

        pos pre = cur;
        cur = select();
        if (cnt > 1)
            time_stamp += dis[pre.x][pre.y][cur.x][cur.y];

        if (find())
            return cnt;

        change_p();
        change_boat();
        time_stamp += t[cur.x][cur.y];
    }
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    read();

    int sum = 0, cnt = 100000, fail = 0;
    for (int i = 0; i < cnt; ++i)
    {
        int result = run();
        if (result == -1)
            ++fail;
        else
            sum += result;
    }

    cout << fixed << setprecision(8);
    cout << "Fail rate: " << 1.0 * fail / cnt << '\n';
    cout << "Succeed avg: " << 1.0 * sum / (cnt - fail) << '\n';

    return 0;
}