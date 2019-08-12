#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

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
    visited.resize(V);
    // Na pocetku nijedan cvor nije posecen
    /********* C++ deo *********/
    // Funkcija std::fill() prima iterator na pocetak kolekcije i kraj kolekcije i vrednost kojom treba popuniti celu kolekciju
    std::fill(visited.begin(), visited.end(), false);

    // Alociramo prostor za niz nizova, tj matricu, u C++-u moramo da imamo eksplicitno kastovanje u (int **), odsnosno (int *) inace se g++ buni
    matrix = (int **)malloc(V * sizeof(int *));
    // Svaki od elemenata matrice je novi niz, pa se alocira i memorija za njih
    for (int i = 0; i < V; i++)
      matrix[i] = (int *)calloc(sizeof(int), V);

    // Imamo V cvorova pa nam je vektor ulazinh stepena velicine V
    in_degrees.resize(V);
    // Na pocetku svi cvorovi imaju ulazni stepen 0
    std::fill(in_degrees.begin(), in_degrees.end(), 0);
  }

  // Funkcija koja dodaje granu u -> v u graf, i menja ulazni stepen cvora
  void add_edge(int u, int v)
  {
    // Sused cvora u je cvor v
    matrix[u][v] = 1;

    // Kada dodamo granu u -> v znaci da se ulazni stepen cvora v povecava za 1
    in_degrees[v]++;
  }

  void topological_sorting()
  {
    // Red koji koristimo za cuvanje cvorova koje treba obraditi
    std::queue<int> nodes;

    // Odmah dodamo sve cvorove ciji je ulazni stepen 0
    for (int i = 0; i < V; i++)
      if (!in_degrees[i]) {
        nodes.push(i);
      }
    // Pomocni cvor u koji smestamo ono sto izvucemo iz reda
    int tmp;

    while (nodes.size()) {
      // Uzimamo cvor sa pocetka reda
      tmp = nodes.front();

      // Skidamo cvor sa pocetka reda
      nodes.pop();

      // Dodajemo cvor u topoloski poredak
      topological_sort.push_back(tmp);

      // Prolazimo kroz sve cvorove koji su susedni cvoru tmp, smanjujemo im ulazni stepen, i onda ako je neki dosao do ulaznog stepena 0, znaci da smo obisli sve njegove
      // susede tako da se i on moze dodati na kraj reda
      for (int i = 0; i < V; i++) {
        if (matrix[tmp][i] != 0) {
          in_degrees[i]--;

          if (!in_degrees[i])
            nodes.push(i);
        }
      }
    }
  }

  // Funkcija kojom proveravamo da li graf ima Hamiltonov ciklus
  bool has_hamiltonian_path()
  {
    // Pozivamo prvo topolosko sortiranje
    topological_sorting();

    // Idemo kroz poredak u topoloskom sortiranju i proveravamo da li imamo granu izmedju svaka dva uzastopna cvora, i to od manjeg ka vecem. Tj ako nam je
    // u pre v u topoloskom poretku onda trazimo granu u -> v
    for (int i = 0; i < V - 1; i++) {
      if (matrix[topological_sort[i]][topological_sort[i + 1]] == 0)
        return false;
    }

    return true;
  }

private:
  // Matrica susedstva
  int **matrix;
  // Broj cvorova grafa
  int V;
  // Lista posecenih cvorova. Da ne bismo ulazili u beskonacnu rekurziju ne zelimo da obilazimo cvorove koje smo vec obisli na putu kojim smo krenuli, zato cuvamo listu
  // posecenih cvorova
  std::vector<bool> visited;
  // Struktura stek u koju cemo smestati cvorove grafa
  std::vector<int> in_degrees;
  // Vektor koji cuva topoloski poredak cvorova
  std::vector<int> topological_sort;
};

int main ()
{
  Graph g(4);

  // Ima
  // g.add_edge(0, 1);
  // g.add_edge(1, 2);
  // g.add_edge(2, 3);

  // Nema
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(0, 3);

  std::cout << (g.has_hamiltonian_path() ? "Graph has Hamiltonian path! :)\n" : "Graph has not Hamiltonian path! :(\n");

  return 0;
}
