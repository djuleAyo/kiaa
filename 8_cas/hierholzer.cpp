#include <iostream>
#include <vector>
#include <stack>

// Klasa kojom cemo predstavljati graf
class Graph
{
public:
  // Konstruktor za graf koji samo prima broj cvorova grafa
  Graph(int V)
  {
    // Broj cvorova grafa je jednak prosledjenom argumentu
    this->V = V;

    // Alociramo memoriju za V vectora, jer znamo da ce ih biti V, a svaki od njih cemo povecavati za po jedan element pomocu vector.push_back()
    adjacency_list.resize(V);
  }

  // Funkcija koja dodaje granu u -> v u graf
  void add_edge(int u, int v)
  {
    // Sused cvora u je cvor v
    adjacency_list[u].push_back(v);
  }

  void hierholzer ()
  {
    // Stavljamo cvor iz koga krecemo u trenutnu putanju
    stack.push(0);
    // Trenutni cvor koji razmatramo je bas pocetni
    int v = 0;

    // Pomocna promenljiva za cuvanje cvorova
    int tmp;

    // Dok god na steku ima cvorova
    while (stack.size()) {
      // Ako iz trenutnog cvora imamo bar jos jednu granu
      if (adjacency_list[v].size()) {

        // Dodajemo cvor v na stek
        stack.push(v);

        // Uzimamo jednu granu iz cvora v, pamtimo ka kom cvoru ona vodi i izbacujemo je
        tmp = adjacency_list[v].back();
        adjacency_list[v].pop_back();

        // Naredni cvor koji se razmatra je bas tmp, tj cvor u koji smo otisli iz cvora v
        v = tmp;
      }
      // Ako iz trenutnog cvora nema vise grana
      else {
        // Ako smo dosli ovde znaci da nema vise grana iz ovog cvora i ona treba da bude dodat u Hamiltonov put/ciklus
        euler_path.push_back(v);

        // Vracamo se nazad u cvor iz koga smo dosli i njega razmatramo, trazimo novu granu ili ako ih vise nema, dodajemo njega u putanju itd.
        v = stack.top();
        stack.pop();
      }
    }

    int i;

    // Nakon sto smo prosli kroz sve cvorove ispisujemo putanju koju smo nasli
    for (i = euler_path.size() - 1; i >= 1; i--)
      std::cout << euler_path[i] << " -> ";

    std::cout << euler_path[i] << std::endl;
  }

private:
  // Lista susedstva. Imamo vector vector-a, sto znaci za svaki od cvorova [0,V) imamo po jednu listu koja cuva susede odgovarajuceg cvora
  std::vector<std::vector<int>> adjacency_list;
  // Broj cvorova grafa
  int V;
  // Stek na koji smestamo trenutnu putanju dok ne dodjemo do cvora koji treba da se stavi u konacnu putanju
  std::stack<int> stack;
  // Vektor koji nam predstavlja konacnu putanju, tj Hamiltonov ciklus
  std::vector<int> euler_path;
};

int main ()
{
  Graph g(5);

  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 0);
  g.add_edge(3, 4);
  g.add_edge(4, 1);

  g.hierholzer();

  return 0;
}
