#include <iostream>
#include <cstdlib>
#include <climits>

#define INFINITY 9999

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

    // Kazemo da je svaki element matrice matrix jednak INFINITY osim elemenata na dijagonali koji su 0
    for (int i = 0; i < V; i++)
      for (int j = 0; j < V; j++) {
        if (i == j)
          matrix[i][j] = 0;
        else
          matrix[i][j] = INFINITY;
      }
  }

  // Dodajemo granu izmedju u i v koja je tezine weight
  void add_edge(int u, int v, int weight)
  {
    matrix[u][v] = weight;
  }

  void floyd_warshall()
  {
    // Uzimamo svaki od cvorova kao srednji cvor
    for (int k = 0; k < V; k++) {
      // Uzimamo svaki od cvorova kao pocetni cvor
      for (int i = 0; i < V; i++) {
        // Uzimamo svaki od cvorova kao kranji cvor
        for (int j = 0; j < V; j++) {
          // Ako je udaljenost pocetnog do krajnjeg cvora preko srednjeg manja od trenutne udaljenosti, idemo preko srednjeg cvora i azuriramo vrednost puta od pocetnog do
          // krajnjeg cvora
          if (matrix[i][j] > matrix[i][k] + matrix[k][j])
            matrix[i][j] = matrix[i][k] + matrix[k][j];
        }
      }
    }

    // Idemo kroz matricu povezanosti koja cuva najkrace puteve i samo vidimo da li je neki cvor do sebe "negativno udaljen" sto nam oznacava negativni ciklus
    for (int i = 0; i < V; i++)
      if (matrix[i][i] < 0) {
        std::cout << "YES!\n";
        return ;
      }

    std::cout << "NO!\n";
  }

private:
  // Broj cvorova grafa
  int V;
  // Matrica susedstva
  int **matrix;
};

int main ()
{
  Graph g(4);

  g.add_edge(0, 1, 1);
  g.add_edge(1, 2, -1);
  g.add_edge(2, 3, -1);
  g.add_edge(3, 0, -1);

  g.floyd_warshall();

  return 0;
}
