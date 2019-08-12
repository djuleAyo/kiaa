#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <stack>
#include <set>

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

// Klasa kojom cemo predstavljati graf
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

    // Alociramo prostor za V bool vrednosti
    path_found.resize(V);
  }

  // Funkcija koja dodaje grane u -> v  i v -> u u graf
  void add_edge(int u, int v, int weight)
  {
    // Funkcija std::make_pair kreira par vrednosti koje se prosledjuju kao argumenti

    // Sused cvora u je cvor v, a tezina je weight
    adjacency_list[u].push_back(std::make_pair(v, weight));
    // Sused cvora v je cvor u a tezina je weight
    adjacency_list[v].push_back(std::make_pair(u, weight));
  }

  int dijkstra(int u, int v, int avoid)
  {
    // Pravimo min-heap za cuvanje svih cvorova grafa. Svaki cvor heap-a ce biti cvor kao i najkrace rastojanje do njega koje je do tad nadjeno
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, compare> heap;

    // Na pocetku kazemo da su svi udaljeni beskonacno
    std::fill(distances.begin(), distances.end(), INFINITY);

    // Na pocetku kazemo da ni do jednog cvora nemamo put
    std::fill(path_found.begin(), path_found.end(), false);

    // Dodajemo cvor u na heap
    heap.push(std::make_pair(u, distances[u]));

    // Udaljenost pocetnog cvora od sebe samog je 0
    distances[u] = 0;

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
      for (std::pair<int, int> &neighbour : adjacency_list[tmp.first]) {

        // Ako je cvor preko koga zelimo da idemo onaj koji zelimo da izbegnemo samo nastavljamo dalje, tj necemo ici kroz taj cvor
        if (neighbour.first == avoid)
          continue;

        // Ako preko cvora tmp imamo kraci put do nekog suseda onda azuriramo najkraci put do tog suseda i dodajemo ga na heap
        // Druga opcija je da imamo put jednake tezine koji sadrzi manje grana
        if (distances[neighbour.first] > distances[tmp.first] + neighbour.second) {
          // Azuriramo tezinu puta do cvora neighbour
          distances[neighbour.first] = distances[tmp.first] + neighbour.second;
          // Dodajemo cvor neighbour zajedno sa novom tezinom puta u heap
          heap.push(std::make_pair(neighbour.first, distances[neighbour.first]));
        }
      }
    }
    return distances[v];
  }

  int shortest_path_via_node(int start, int finish, int middle)
  {
    // Kada zelimo da idemo preko nekog cvora ne moramo da izbegnemo nijedan, pa kao treci argument saljemo -1
    return dijkstra(start, middle, -1) + dijkstra(middle, finish, -1);
  }

  int shortest_path_avoid(int start, int finish, int avoid)
  {
    // Samo vracamo put od start do finish izbegavajuci cvor avoid
    return dijkstra(start, finish, avoid);
  }

private:
  // Lista susedstva. Imamo vector parova gde je prvi element cvor ka kome void grana a drugi element tezina grane
  std::vector<std::vector<std::pair<int, int>>> adjacency_list;
  // Broj cvorova grafa
  int V;
  // Vektor koji ce da cuva tezinu puta od pocetnog cvora do svakog odgovarajuceg cvora
  std::vector<int> distances;
  // Vektor koji ce nam govoriti do kog cvora smo vec nasli najkraci put
  std::vector<bool> path_found;
  // Skup koja ce cuvati grane koje su vec dodate u putanju kako ih ne bismo ponovo razmatrali. Par ce biti jedinstven pa u slozenosti O(log n) mozemo dobiti
  // informaciju da li granu vec imamo
  std::set<std::pair<int, int>> edges;
};

int main ()
{
  Graph g(4);

  g.add_edge(0, 1, 1);
  g.add_edge(1, 3, 3);
  g.add_edge(1, 2, 2);
  // Dodati ovu granu i videti ponasanje
  // g.add_edge(0, 2, 8);

  std::cout << g.shortest_path_via_node(0, 2, 3) << std::endl;

  int result = g.shortest_path_avoid(0, 2, 1);

  if (result == INFINITY)
    std::cout << "No path!\n";
  else
    std::cout << result << std::endl;

  return 0;
}
