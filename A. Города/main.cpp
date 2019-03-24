//
//  A. Города
//
//Требуется отыскать самый короткий маршрут между городами. Из города может выходить дорога, которая возвращается в этот же город.
//Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
//N ≤ 10000, M ≤ 250000.
//Длина каждой дороги ≤ 10000.

#include<iostream>
#include<vector>
#include<queue>
struct dist_vert                                       //вершина и её расстояние до начальной
{
    dist_vert(int vert_, int dist_): vert(vert_), dist(dist_){}
    int vert = 0;
    int dist = 0;
    const bool operator < (const dist_vert& value) const;
};
const bool dist_vert::operator < (const dist_vert& value) const
{
    if(this->dist < value.dist)
    {
        return false;
    }
    else
    {
        return true;
    }
}
struct children                                         //нам нужно запомнить вершины соседние с нашей и веса этих ребер
{                                                       //для этого используем эту структуру
    std::vector <int> next;
    std::vector <int> weight;
    int size();
};
int children::size()
{
    return next.size();
}
struct ListGraph {
    explicit ListGraph(int count);
    ~ListGraph() = default;
    void AddEdge(int from, int to, int weight);
    int VerticesCount() const;
    void GetNextVertices(int vertex, children& vertices) const;
private:
    std::vector<children> Next_Vertices;
};

ListGraph::ListGraph(int count)                                                     //конструктор пустого графа
{
    Next_Vertices.resize(count);
}

int ListGraph::VerticesCount() const
{
    return Next_Vertices.size();
}

void ListGraph::AddEdge(int from, int to, int weight)                               //добавление ребра
{
    Next_Vertices[from].next.push_back(to);
    Next_Vertices[from].weight.push_back(weight);
    Next_Vertices[to].next.push_back(from);
    Next_Vertices[to].weight.push_back(weight);
}

void ListGraph::GetNextVertices(int vertex, children& vertices) const                   //получаем детей вершины
{
    vertices = Next_Vertices[vertex];
}

int dijkstra (ListGraph& graph, int start, int end, int inf)            //пишем дейкстру
{
    int N = graph.VerticesCount();                                      //число вершин графа
    std::vector<int>distanc;                                            //тут храняться длина за которую мы умеем доходить до каждой вершины на данный момент
    std::priority_queue<dist_vert>order_queue;                          //приоритетная очередь для дейкстры
    distanc.resize(N);
    for(int i = 0; i < N; ++i)
    {
        distanc[i] = inf;                                                   //сначала не умеем доходить == умеем доходить за бесконечную длину
    }
    distanc[start] = 0;                                                     //в начальную вершину умеем доходить за 0
    dist_vert first_vertices(start, distanc[start]);                        //собирём первую вершину
    order_queue.push(first_vertices);                                       //и запушим её
    while (!order_queue.empty())
    {
        dist_vert now_vertices = order_queue.top();                         //вытаскиваем из очереди самую близкую к началу
        order_queue.pop();
        if (now_vertices.dist != distanc[now_vertices.vert])                //это чтобы не ходить в родителей
        {
            continue;
        }
        children next;                                                      //сюда запишем всех детей вытащенной вершины
        graph.GetNextVertices(now_vertices.vert, next);                     //возьмём этих детей
        for(int i = 0; i < next.size(); ++i)
        {
            int child_number = next.next[i];
            int edge = next.weight[i];
            if(distanc[now_vertices.vert] + edge < distanc[child_number])       //если путь через эту вершину короче, то идём через неё
            {
                distanc[child_number] = distanc[now_vertices.vert] + edge;
                dist_vert child(child_number, distanc[child_number]);
                order_queue.push(child);
            }
        }
    }
    return distanc[end];                                                    //тут наш ответ
}

int main()
{
    int N = 0;
    int M = 0;
    std::cin >> N >> M;
    ListGraph graph(N);
    int inf = 0;
    for(int i = 0; i < M ; ++i)
    {
        int from = 0;
        int to = 0;
        int weight = 0;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        inf += weight;
    }
    int start = 0;
    int end = 0;
    std::cin >> start >> end;
    std::cout << dijkstra(graph, start, end, inf);
}


