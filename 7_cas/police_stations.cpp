#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <stack>

#define INFINITY INT_MAX

// Struktura koja predefinise operator () kako bismo u priority_queue-u kao treci argument mogli da prosledimo ovu strukturu
struct compare
{
  // Vracamo informaciju o tome da li je duzina puta para p1 veca od duzine puta para p2
  bool operator()(std::pair<int, int> &p1, std::pair<int, int> &p2)
  {
    return p1.second > p2.second;
  }
};

class Graph
{
public:
  // Konstruktor za graf koji samo prima broj cvorova grafa
  Graph(int V)
  {
    // Broj cvorova grafa je jednak prosledjenom argumentu
    this->V = V;
    // Alociramo mesto za V bool vrednosti koje ce cuvati posecene cvorove

    // Alociramo memoriju za V vectora, jer znamo da ce ih biti V, a svaki od njih cemo povecavati za po jedan element pomocu vector.push_back()
    adjacency_list.resize(V);

    // Alociramo prostor za V integer-a
    distances.resize(V);
    // Na pocetku kazemo da su svi udaljeni beskonacno
    std::fill(distances.begin(), distances.end(), INFINITY);

    // Alociramo prostor za V bool vrednosti
    path_found.resize(V);
    // Na pocetku kazemo da ni do jednog cvora nemamo put
    std::fill(path_found.begin(), path_found.end(), false);

    // Alociramo prostor za V integer-a
    shortest_path.resize(V);
    // Na pocetku su putanje do svakog cvora beskonacne
    std::fill(shortest_path.begin(), shortest_path.end(), INFINITY);
  }

  // Funkcija koja dodaje grane u -> v  i v -> u u graf
  void add_edge(int u, int v)
  {
    // Funkcija std::make_pair kreira par vrednosti koje se prosledjuju kao argumenti

    // Sused cvora u je cvor v, a tezina je weight
    adjacency_list[u].push_back(v);
    // Sused cvora v je cvor u a tezina je weight
    adjacency_list[v].push_back(u);
  }

  void dijkstra()
  {
    std::pair<int, int> tmp;

    // Dok god ima elemenata u heap-u
    while (!heap.empty()) {
      // Uzimamo koreni element heap-a
      tmp = heap.top();

      // Skidamo element sa vrha heap-a
      heap.pop();

      // Kako moze da se desi da smo neki cvor vise puta ubacili u heap, ako opet naidjemo na cvor koji je vec obradjen, tj do kog smo nasli najkraci put, necemo ponovo
      // da ga obradjujemo
      if (path_found[tmp.first])
        continue;

      // Kada udjemo u cvor znamo da smo nasli najkraci put do njega pa to i kazemo
      path_found[tmp.first] = true;

      // Prolazimo kroz listu suseda cvora tmp
      // tmp je par ciji je prvi element cvor a drugi cena najkraceg puta do tog cvora, tako da svuda gde nam treba cvor imamo tmp.first
      for (int neighbour : adjacency_list[tmp.first]) {
        // Ako preko cvora tmp imamo kraci put do nekog suseda onda azuriramo najkraci put do tog suseda i dodajemo ga na heap
        if (distances[neighbour] > distances[tmp.first] + 1) {
          // Azuriramo udaljenost cvora neighbour
          distances[neighbour] = distances[tmp.first] + 1;
          // Dodajemo cvor neighbour zajedno sa novom tezinom puta u heap
          heap.push(std::make_pair(neighbour, distances[neighbour]));
        }
      }
    }
  }

  void closest_police_stations()
  {
    // U heap dodajemo sve one gradove koji imaju policijske stanice, i njihova udaljenost do najblize stanice je 0
    for (int station : police_stations) {
      heap.push(std::make_pair(station, 0));
      distances[station] = 0;
    }

    dijkstra();

    // Samo ispisujemo udaljenosti za svaki od gradova do najblize policijske stanice
    for (int i = 0; i < V; i++)
      std::cout << i << " " << distances[i] << std::endl;
      // std::cout << "Closest police station from city " << i << " is " << distances[i] << " kilometers away" << std::endl;
  }

  void add_police_station(int station)
  {
    // Dodajemo policijsku stanicu u vektor policijskih stanica
    police_stations.push_back(station);
  }

private:
  // Lista susedstva. Imamo vector parova gde je prvi element cvor ka kome void grana a drugi element tezina grane
  std::vector<std::vector<int>> adjacency_list;
  // Broj cvorova grafa
  int V;
  // Vektor koji ce da cuva tezinu puta od pocetnog cvora do svakog odgovarajuceg cvora
  std::vector<int> distances;
  // Vektor koji ce nam govoriti do kog cvora smo vec nasli najkraci put
  std::vector<bool> path_found;
  // Vektor koji za svaki cvor cuva duzinu najkraceg puta do tog cvora
  std::vector<int> shortest_path;
  // Vektor koji cuva policijske stanice
  std::vector<int> police_stations;
  // Pravimo min-heap za cuvanje svih cvorova grafa. Svaki cvor heap-a ce biti cvor kao i najkrace rastojanje do njega koje je do tad nadjeno
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, compare> heap;
};

int main ()
{
  Graph g(6);

  g.add_edge(0, 1);
  g.add_edge(0, 5);
  g.add_edge(1, 2);
  g.add_edge(1, 5);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(2, 5);
  g.add_edge(3, 4);


  g.add_police_station(0);
  g.add_police_station(4);

  g.closest_police_stations();

  return 0;
}
