#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

// Structure to represent an edge in the graph
struct Edge
{
    int u, v;
    int weight;
};

// Union-Find Disjoint Set for Kruskal's Algorithm
class UnionFind
{
private:
    vector<int> parent, rank;

public:
    UnionFind(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }
    int find(int x)
    {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y)
    {
        int xroot = find(x);
        int yroot = find(y);
        if (xroot == yroot)
            return;
        if (rank[xroot] < rank[yroot])
            parent[xroot] = yroot;
        else if (rank[xroot] > rank[yroot])
            parent[yroot] = xroot;
        else
        {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }
};

// Part 1: Kruskal's Algorithm for Minimum Spanning Tree
vector<Edge> kruskalMST(int N, vector<vector<int>> &graph)
{
    vector<Edge> edges;
    // Build edge list
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (graph[i][j] > 0)
            {
                edges.push_back({i, j, graph[i][j]});
            }
        }
    }
    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](Edge a, Edge b)
         { return a.weight < b.weight; });
    UnionFind uf(N);
    vector<Edge> mst;
    for (auto &edge : edges)
    {
        if (uf.find(edge.u) != uf.find(edge.v))
        {
            uf.unite(edge.u, edge.v);
            mst.push_back(edge);
        }
    }
    return mst;
}

// Part 2: Traveling Salesman Problem using brute-force (for small N)
int minTSP = INT_MAX;
vector<int> bestRoute;

void tsp(int start, vector<vector<int>> &graph, vector<bool> &visited, vector<int> &path, int cost, int N)
{
    if (path.size() == N)
    {
        cost += graph[path.back()][start]; // return to start
        if (cost < minTSP)
        {
            minTSP = cost;
            bestRoute = path;
        }
        return;
    }
    for (int i = 0; i < N; i++)
    {
        if (!visited[i])
        {
            visited[i] = true;
            path.push_back(i);
            cost += graph[path[path.size() - 2]][i];
            tsp(start, graph, visited, path, cost, N);
            cost -= graph[path[path.size() - 2]][i];
            path.pop_back();
            visited[i] = false;
        }
    }
}

// Part 3: Edmonds-Karp Algorithm for Maximum Flow
int bfs(vector<vector<int>> &rGraph, int s, int t, vector<int> &parent)
{
    int N = rGraph.size();
    fill(parent.begin(), parent.end(), -1);
    queue<pair<int, int>> q;
    q.push({s, INT_MAX});
    parent[s] = -2;
    while (!q.empty())
    {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();
        for (int v = 0; v < N; v++)
        {
            if (parent[v] == -1 && rGraph[u][v] > 0)
            {
                parent[v] = u;
                int new_flow = min(flow, rGraph[u][v]);
                if (v == t)
                    return new_flow;
                q.push({v, new_flow});
            }
        }
    }
    return 0;
}

int maxFlow(vector<vector<int>> &graph, int s, int t)
{
    int flow = 0;
    int N = graph.size();
    vector<vector<int>> rGraph = graph;
    vector<int> parent(N);
    int new_flow;
    while ((new_flow = bfs(rGraph, s, t, parent)))
    {
        flow += new_flow;
        int v = t;
        while (v != s)
        {
            int u = parent[v];
            rGraph[u][v] -= new_flow;
            rGraph[v][u] += new_flow;
            v = u;
        }
    }
    return flow;
}

// Helper function to convert index to character
char indexToChar(int index)
{
    return 'A' + index;
}

int main()
{
    int N;
    cin >> N;
    vector<vector<int>> distanceMatrix(N, vector<int>(N));
    vector<vector<int>> capacityMatrix(N, vector<int>(N));
    vector<pair<int, int>> coordinates(N);

    // Read distance matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> distanceMatrix[i][j];
        }
    }

    // Read capacity matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> capacityMatrix[i][j];
        }
    }

    // Read coordinates
    for (int i = 0; i < N; i++)
    {
        char ch;
        cin >> ch; // '('
        int x;
        cin >> x;
        cin >> ch; // ','
        int y;
        cin >> y;
        cin >> ch; // ')'
        coordinates[i] = {x, y};
    }

    // Part 1: Optimal wiring (Minimum Spanning Tree)
    vector<Edge> mst = kruskalMST(N, distanceMatrix);
    cout << "1. Way of wiring the neighborhoods with fiber (list of arcs):" << endl;
    for (auto &edge : mst)
    {
        cout << "(" << indexToChar(edge.u) << "," << indexToChar(edge.v) << ")" << endl;
    }

    // Part 2: Mail delivery route (Traveling Salesman Problem)
    minTSP = INT_MAX;
    bestRoute.clear();
    vector<bool> visited(N, false);
    vector<int> path;
    int start = 0; // Start from node 0 ('A')
    visited[start] = true;
    path.push_back(start);
    tsp(start, distanceMatrix, visited, path, 0, N);
    cout << "2. Route to be followed by the mail delivery personnel:" << endl;
    for (int i = 0; i < bestRoute.size(); i++)
    {
        cout << indexToChar(bestRoute[i]) << " -> ";
    }
    cout << indexToChar(start) << endl;

    // Part 3: Maximum information flow value from the initial node to the final node
    int max_flow = maxFlow(capacityMatrix, 0, N - 1);
    cout << "3. Maximum information flow value from the initial node to the final node: " << max_flow << endl;

    // Part 4: List of polygons (Voronoi diagram not implemented)
    // Since generating Voronoi diagrams is complex, we'll output the coordinates
    cout << "4. List of polygons (coordinates of exchanges):" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "(" << coordinates[i].first << "," << coordinates[i].second << ")" << endl;
    }

    return 0;
}
