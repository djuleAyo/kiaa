#include <iostream>
#include <vector>
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
    std:fill(visited.begin(), visited.end(), false);

    // Alociramo memoriju za V vectora, jer znamo da ce ih biti V, a svaki od njih cemo povecavati za po jedan element pomocu vector.push_back()
    adjacency_list.resize(V);
  }

  // Funkcija koja dodaje granu u -> v u graf
  void add_edge(int u, int v)
  {
    // Sused cvora u je cvor v
    adjacency_list[u].push_back(v);
  }

  // Obilazak grafa u dubinu
  void DFS(int u)
  {
    // Kazemo da je cvor koji trenutno obradjujemo vec posecen
    visited[u] = true;

    // Uzimamo iteratore na pocetak i kraj kolekcije kako bismo prosli kroz sve susede
    // Ako obratimo paznju videcemo da uzimamo adjacency_list[u].begin() i adjacency_list[u].end(), odnosno uzimamo vector suseda cvora u i obradjujemo njih
    auto begin = adjacency_list[u].begin();
    auto end = adjacency_list[u].end();

    while (begin != end) {
      // Ako smo vec posetili cvor necemo ponovo u njega ici, trazimo neposecene cvorove. Za njih pozivamo DFS rekurzivno.
      // Ovde se krije i uslov izlaska iz rekurzije, jer kada nema vise cvorova koji nisu poseceni necemo ici dalje, tj necemo pozivati DFS ponovo
      // begin i end su iteratori (pokazivacke promenljive), pa da bismo dobili vrednost koju cuva begin moramo da ga dereferenciramo, i zato imamo *begin
      if (!visited[*begin])
        DFS(*begin);
      // Krecemo se kroz kolekciju
      begin++;
    }
  }

  Graph reverse_edges()
  {
    // pravimo novi graf sa takodje V cvorova
    Graph g(this->V);

    // Idemo kroz sve cvorove grafa
    for (int i = 0; i < V; i++)
      // Idemo kroz sve susede i-tog cvora
      for (int u : adjacency_list[i])
        // Dodajemo suprotnu granu u odnosnu na onu koja postoji. Ako smo npr imali liste povezanosti
        // 0 -> 1 -> 2
        // 1 -> 2
        // 2 -> 0
        // Spoljasnja petlja po i ide redom kroz ove liste, unutrasnja po u ide kroz svaku listu, tako da ako je i == 0, idemo kroz listu suseda cvora i
        // I onda znamo da imamo grane i -> u, zato u novi graf g dodajemo granu u -> v
        g.add_edge(u, i);

    // Vracamo graf g
    return g;
  }

  // Funckija koja broji neposecene cvorove
  int count_unvisited_nodes()
  {
    return std::count_if(visited.begin(), visited.end(), [](bool visited){ return visited == false; });
  }

  bool kosaraju()
  {
    // Pokrecemo DFS iz cvora 0
    DFS(0);

    // Prebrojavamo koliko imamo elemenata u vektoru visited za koje vazi da je vrednost false, tj brojimo koliko ima cvorova koji nisu poseceni. Ako do nekog cvora nismo
    // stigli, znaci da graf nije jako povezan.
    if (count_unvisited_nodes() > 0)
      return false;

    // Pravimo novi graf za koji cemo opet pokrenuti DFS
    Graph g = reverse_edges();

    // Pozivamo DFS za novi graf
    g.DFS(0);

    // Proveravamo koliko u njemu ima neposecenih cvorova nakon DFS-a
    if (g.count_unvisited_nodes() > 0)
      return false;

    // Vracamo potvrdan odgovor na pitanje da li je graf jako povezan
    return true;
  }

private:
  // Lista susedstva. Imamo vector vector-a, sto znaci za svaki od cvorova [0,V) imamo po jednu listu koja cuva susede odgovarajuceg cvora
  std::vector<std::vector<int>> adjacency_list;
  // Broj cvorova grafa
  int V;
  // Lista posecenih cvorova. Da ne bismo ulazili u beskonacnu rekurziju ne zelimo da obilazimo cvorove koje smo vec obisli na putu kojim smo krenuli, zato cuvamo listu
  // posecenih cvorova
  std::vector<bool> visited;
};

int main ()
{
  Graph g(5);

  // Jeste
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  g.add_edge(3, 0);
  g.add_edge(2, 4);
  g.add_edge(4, 2);

  // Nije
  // g.add_edge(0, 1);
  // g.add_edge(1, 2);
  // g.add_edge(2, 3);

  std::cout << (g.kosaraju() ? "Graph is strongly connected!\n" : "Graph is not strongly connected!\n");

  return 0;
}
