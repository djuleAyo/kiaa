#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <stack>

#define INFINITY INT_MAX

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

    // Alociramo prostor za niz nizova, tj matricu, u C++-u moramo da imamo eksplicitno kastovanje u (int **), odsnosno (int *) inace se g++ buni
    matrix = (int **)malloc(V * sizeof(int *));
    // Svaki od elemenata matrice je novi niz, pa se alocira i memorija za njih
    for (int i = 0; i < V; i++)
      matrix[i] = (int *)calloc(sizeof(int), V);

    // Alociramo prostor za V bool vrednosti
    included_in_MST.resize(V);
    // Na pocetku kazemo da nijedan cvor nije ukljucen u MST
    std::fill(included_in_MST.begin(), included_in_MST.end(), false);

    // Alociramo prostor za v int vrednosti
    parents.resize(V);
    // Na pocetku kazemo da su svi roditelji -1
    std::fill(parents.begin(), parents.end(), -1);

    // Alociramo prostor za v int vrednosti
    edges.resize(V);
    // Na pocetku kazemo da su sve "minimalne grane" INFINITY
    std::fill(edges.begin(), edges.end(), INFINITY);
  }

  // Funkcija za nalazenje grane najmanje tezine koja nije vec ubacena u MST
  int min_edge_not_included()
  {
    // Minimum je INFINITY a indeks minimuma -1 na pocetku
    int min = INFINITY;
    int min_index = -1;

    // Idemo kroz sve cvorove
    for (int i = 0; i < V; i++)
      // Ako cvor vec nije ukljucen u MST, i ako je grana koja vodi ka njemu manja od trenutnog minimuma, menjamo minimum i pamtimo koji je to cvor koji sledeci treba
      // obraditi, tj koji je to cvor ka kome vodi minimalna grana
      if (included_in_MST[i] == false && edges[i] < min) {
        min_index = i;
        min = edges[i];
      }

    return min_index;
  }

  // Funkcija koja dodaje grane u -> v i v -> u u graf. Obe grane su tezine weight
  void add_edge(int u, int v, int weight)
  {
    matrix[u][v] = weight;
    matrix[v][u] = weight;
  }

  // Primov algoritam
  void prim(int u)
  {
    // Tezina grane do pocetnog cvora je 0
    edges[u] = 0;

    int node;

    // MST ima ukupno V - 1 grana pa petlja ide do V - 1
    for (int i = 0; i < V - 1; i++) {
      // Uzimamo cvor do koga treba da odemo, tj onaj ka kome vodi grana minimalne tezine koja nije vec u MST-u
      node = min_edge_not_included();

      // Ubacimo cvor u MST
      included_in_MST[node] = true;

      // Prodjemo kroz sve susede cvora ndoe i eventualno azuriramo vrednosti grana koje vode ka njima, ukoliko je vrednost grane manja od trenutno minimalne grane koja ide
      // ka odgovarajucem susedu
      for (int v = 0; v < V; v++) {
        if (matrix[node][v] && included_in_MST[v] == false && matrix[node][v] < edges[v]) {
          parents[v] = node;
          edges[v] = matrix[node][v];
        }
      }
    }

    // Cena MST-a je 0 na pocetku
    int cost = 0;

    // Idemo kroz sve cvorove i dodajemo tezine grana koje su dodate u MST, to su bas grane koje povezuju svaki cvor i sa cvorom parents[i]. Jedino ne racunamo u jer je
    // njegov roditelj -1 i on je pocetni cvor do kog ne vodi nijedna grana

    for (int i = 0; i < V; i++)
      if (i != u)
        cost += matrix[i][parents[i]];

    // Ispisujemo ukupnu cenu povezivanja svih gradova, odnosno sumu svih grana MST-a
    std::cout << cost << std::endl;
  }

private:
  // Broj cvorova grafa
  int V;
  // Umesto vektora distances cuvacemo matricu jer ce nam i lavirint biti dat kao matrica, pa cemo lako azurirati vrednost distances za svaki od cvorova
  int **matrix;
  // Vektor koji ce nam govoriti koji cvorovi su ukljuceni u minimalno razapinjuce stablo
  std::vector<bool> included_in_MST;
  // Vektor koji govori koji je roditeljski cvor svakog od cvorova
  std::vector<int> parents;
  // Vektor koji za svaki cvor govori o vrednosti grane koja se ukljucuje u MST a povezuje njega i njegovog roditelja
  std::vector<int> edges;
};

int main ()
{
  Graph g(5);

  g.add_edge(0, 1, 1);
  g.add_edge(0, 2, 2);
  g.add_edge(0, 3, 3);
  g.add_edge(0, 4, 4);
  g.add_edge(1, 2, 5);
  g.add_edge(1, 4, 7);
  g.add_edge(2, 3, 6);

  g.prim(0);

  return 0;
}
