// %%writefile graph.cpp  --> in first cell write 


%%writefile graph.cpp

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Graph with maximum 100 nodes
vector<int> graph[100];

// Visited array
bool visited[100];


// ================= BFS FUNCTION =================
void BFS(int start)
{
    queue<int> q;

    q.push(start);

    visited[start] = true;

    while (!q.empty())
    {
        int node = q.front();

        q.pop();

        cout << node << " ";

        // Parallel loop for adjacent nodes
        #pragma omp parallel for
        for (int i = 0; i < (int)graph[node].size(); i++)
        {
            int adj = graph[node][i];

            // Critical section for shared data
            #pragma omp critical
            {
                if (!visited[adj])
                {
                    visited[adj] = true;

                    q.push(adj);
                }
            }
        }
    }
}


// ================= DFS FUNCTION =================
void DFS(int start)
{
    stack<int> st;

    st.push(start);

    while (!st.empty())
    {
        int node = st.top();

        st.pop();

        bool alreadyVisited = false;

        // Critical section
        #pragma omp critical
        {
            if (visited[node])
            {
                alreadyVisited = true;
            }
            else
            {
                visited[node] = true;

                cout << node << " ";
            }
        }

        // Continue outside critical section
        if (alreadyVisited)
        {
            continue;
        }

        // Parallel loop for adjacent nodes
        #pragma omp parallel for
        for (int i = 0; i < (int)graph[node].size(); i++)
        {
            int adj = graph[node][i];

            #pragma omp critical
            {
                if (!visited[adj])
                {
                    st.push(adj);
                }
            }
        }
    }
}


// ================= MAIN FUNCTION =================
int main()
{
    int n, m;

    cout << "Enter number of vertices and edges: ";
    cin >> n >> m;

    cout << "Enter edges:\n";

    for (int i = 0; i < m; i++)
    {
        int u, v;

        cin >> u >> v;

        // Undirected graph
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Initialize visited array for BFS
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }

    // BFS Traversal
    cout << "\nBFS Traversal: ";
    BFS(0);

    // Reset visited array for DFS
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
    }

    // DFS Traversal
    cout << "\nDFS Traversal: ";
    DFS(0);

    return 0;
}


// !g++ -fopenmp graph.cpp -o graph  -> second cell 

// !./graph   -> third cell 
