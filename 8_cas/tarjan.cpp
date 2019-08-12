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

    // Na pocetku je time 0
    time = 0;

    // Alociramo mesto za V int vrednosti koje ce cuvati vremena obilaska cvorova
    times.resize(V);
    // Na pocetku svi imaju vreme obilaska -1
    /********* C++ deo *********/
    // Funkcija std::fill() prima iterator na pocetak kolekcije i kraj kolekcije i vrednost kojom treba popuniti celu kolekciju
    std::fill(times.begin(), times.end(), -1);

    // Alociramo mesto za V int vrednosti koje ce cuvati lower times obilaska cvorova
    lower_times.resize(V);
    // Na pocetku svi imaju lower times -1
    /********* C++ deo *********/
    // Funkcija std::fill() prima iterator na pocetak kolekcije i kraj kolekcije i vrednost kojom treba popuniti celu kolekciju
    std::fill(lower_times.begin(), lower_times.end(), -1);

    // Alociramo mesto za V bool vrednosti koje ce cuvati podatak o tome da li je cvor na steku vec
    in_stack.resize(V);
    // Na pocetku nijedan cvor nije na steku
    /********* C++ deo *********/
    // Funkcija std::fill() prima iterator na pocetak kolekcije i kraj kolekcije i vrednost kojom treba popuniti celu kolekciju
    std::fill(in_stack.begin(), in_stack.end(), false);
  }

  // Funkcija koja dodaje grane u -> v i v -> u u graf (posto je neusmeren)
  void add_edge(int u, int v)
  {
    // Sused cvora u je cvor v
    adjacency_list[u].push_back(v);
  }

  // Obilazak grafa u dubinu
  void DFS(int u)
  {
    // Trenutnom cvoru postavljamo i vreme obilaska i lower time na time
    times[u] = lower_times[u] = time;

    // Uvecavamo time za sledeci cvor
    time++;

    // Dodajemo cvor na stek
    stack.push(u);

    // Kazemo da cvor koji trenutno razmatramo jeste u steku
    in_stack[u] = true;

    // Uzimamo iteratore na pocetak i kraj kolekcije kako bismo prosli kroz sve susede
    // Ako obratimo paznju videcemo da uzimamo adjacency_list[u].begin() i adjacency_list[u].end(), odnosno uzimamo vector suseda cvora u i obradjujemo njih
    auto begin = adjacency_list[u].begin();
    auto end = adjacency_list[u].end();

    while (begin != end) {
      // Ako cvor nije vec posecen posecujemo ga
      if (times[*begin] == -1) {
        DFS(*begin);

        // Ako imamo suseda ciji je lower time manji od lower time-a trenutnog cvora onda azuriramo i lower time trenutnog cvora
        lower_times[u] = lower_times[*begin] < lower_times[u] ? lower_times[*begin] : lower_times[u];
      }
      // Ako je sused vec posecen i nalazi se u steku, mozda je njegovo lower time manje od lower time-a cvora u, pa samim tim treba azurirati lower time cvora u
      else if (in_stack[*begin] == true) {
        lower_times[u] = lower_times[u] < lower_times[*begin] ? lower_times[u] : lower_times[*begin];
      }

      // Krecemo se kroz kolekciju
      begin++;
    }

    // Ako smo dosli do korena neke komponente, indikator za to nam je da je njegov time jednak lower_time-u
    if (times[u] == lower_times[u]) {
      // Idemo "beskonacno"
      while (1) {
        // Uzimamo element sa vrha steka
        int v = stack.top();

        // Uklanjamo element sa vrha steka
        stack.pop();

        // Ispisujemo cvor
        std::cout << v << " ";

        // Kazemo da element vise nije u steku
        in_stack[v] = false;

        // Ako smo dosli do korena, onda smo obisli ceu komponentu i samo izlazimo iz "beskonacne" petlje
        if (v == u) {
          std::cout << "\n";
          break;
        }
      }
    }
  }

  private:
  // Lista susedstva. Imamo vector vector-a, sto znaci za svaki od cvorova [0,V) imamo po jednu listu koja cuva susede odgovarajuceg cvora
  std::vector<std::vector<int>> adjacency_list;
  // Broj cvorova grafa
  int V;
  // Vektor koji cuva vremena posecivanja cvorova u DFS obilasku
  std::vector<int> times;
  // Vektor koji cuva lower times za sve cvorove. Lower time je zapravo minamlna vrednost time-a za sve susedne cvorove cvora u
  std::vector<int> lower_times;
  // Stek na koji se smestaju cvorovi
  std::stack<int> stack;
  // Vektor koji nam govori za svaki od cvorova da li je u steku ili ne
  std::vector<bool> in_stack;
  // Promenljiva koja predstavlja vreme koje se dodeljuje svakom od cvorova
  int time;
};

int main ()
{
  Graph g(8);

  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 0);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(3, 5);
  g.add_edge(4, 2);
  g.add_edge(4, 5);
  g.add_edge(4, 6);
  g.add_edge(5, 7);
  g.add_edge(6, 5);
  g.add_edge(7, 6);

  // g.add_edge(0, 1);
  // g.add_edge(0, 2);
  // g.add_edge(0, 7);
  // g.add_edge(1, 3);
  // g.add_edge(1, 4);
  // g.add_edge(2, 5);
  // g.add_edge(4, 6);
  // g.add_edge(5, 8);
  // g.add_edge(6, 1);
  // g.add_edge(8, 0);

  // g.add_edge(0, 1);
  // g.add_edge(1, 2);
  // g.add_edge(2, 3);
  // g.add_edge(3, 1);
  // g.add_edge(3, 0);

  // g.add_edge(0, 1);
  // g.add_edge(1, 2);
  // g.add_edge(2, 3);
  // g.add_edge(3, 0);
  // g.add_edge(5, 0);
  // g.add_edge(5, 4);

  g.DFS(0);

  return 0;
}
