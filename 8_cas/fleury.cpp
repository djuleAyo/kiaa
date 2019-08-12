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

    // Alociramo prostor
    degrees.resize(V);

    // Postavljamo stepene na 0
    std::fill(degrees.begin(), degrees.end(), 0);
  }

  // Funkcija koja dodaje grane u -> v i v - > u u graf
  void add_edge(int u, int v)
  {
    // Sused cvora u je cvor v
    adjacency_list[u].push_back(v);

    // Sused cvora u je cvor v
    adjacency_list[v].push_back(u);

    // Povecavamo ulazni, odnosno izlazni stepen za oba cvora
    degrees[u]++;
    degrees[v]++;
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
      // Ovde jos dodatno proveravamo da li grana postoji, tj da li nije obrisana
      if (*begin != -1 && !visited[*begin])
        DFS(*begin);
      // Krecemo se kroz kolekciju
      begin++;
    }
  }

  // Funkcija koja proverava da li je graf povezan, tj da li su svi cvorovi koji imaju stepen veci od 0 povezani
  bool is_connected()
  {
    // Cvor iz koga krecemo DFS
    int start_node;

    // Prvo nadjemo cvor koji ima stepen veci od 0, uzmemo bilo koji takav cvor i iz njega mozemo da pokrenemo DFS, tako bi trebalo da prodjemo kroz sve cvorove koji imaju stepen razlicit od
    // 0. Ako neki ne obidjemo graf nam nije povezan i vracamo false
    for (int i = 0; i < V; i++)
      if (adjacency_list[i].size() > 0)
        start_node = i;

    // Pokrecemo DFS
    DFS(start_node);

    // Idemo kroz sve cvorove
    for (int i = 0; i < V; i++)
      // Ako ima neki neposeceni koji ima stepen veci od 0, vracamo false
      if (visited[i] == false && degrees[i] > 0)
        return false;

    return true;
  }

  // Funkcija koja vraca 0 ako graf nije Ojlerov, 1 ako graf ima Ojlerov put, ali nema Ojlerov ciklusi 2 ako ima Ojlerov ciklus
  bool is_eulerian()
  {
    int count_odd = num_of_odd_vertices();

    // Ako imamo vise od dva cvora sa neparnim stepenima vracamo 0, tj graf nije Ojlerov
    if (count_odd > 2)
      return 0;

    // Ako graf nije povezan, odnosno ako neke cvorove stepena veceg od 0 koji nisu povezani vracamo da graf nije Ojlerov
    if (!is_connected())
      return 0;

    // Vracamo informaciju o tome da li graf ima Ojlerov ciklus ili samo put, ako ima 0 cvorova neparnog stepena onda ima ciklus, inace put
    return count_odd == 0 ? 2 : 1;
  }

  // Funkcija koja broji koliko imamo cvorova neparnog stepena u grafu
  int num_of_odd_vertices()
  {
    // Broj cvorova neparnog stepena
    int count_odd = 0;

    // Ako imamo neki cvor sa ukupnim neparnim stepenom uvecavamo broj takvih cvorova
    for (int i = 0; i < V; i++)
      if (degrees[i] % 2)
        count_odd++;

    return count_odd;
  }

  // Funkcija koja vraca neki cvor neparnog stepena
  int find_node_with_odd_degree()
  {
    // Idemo kroz sve cvorove
    for (int i = 0; i < V; i++) {
      // Ako je stepen neparan vracamo cvor
      if (degrees[i] % 2)
        return i;
    }
  }

  // Funkcija koja vraca neki cvor parnog stepena
  int find_node_with_even_degree()
  {
    // Idemo kroz sve cvorove
    for (int i = 0; i < V; i++) {
      // Ako je stepen paran i nije 0 vracamo cvor
      if (degrees[i] % 2 == 0 && degrees[i])
        return i;
    }
  }

  void fleury()
  {
    // Brojimo koliko ima cvorova koji su neparnog stepena
    int count_odd = num_of_odd_vertices();

    // Cvor iz koga se krece;
    int start_node;

    // Ako nema nijednog, mozemo da krenemo iz bilo kog cvora koji nema stepen 0
    if (count_odd == 0) {
      start_node = find_node_with_even_degree();
      print_euler_path_or_cycle(start_node);
    }
    // Ako imamo 2 cvora koja su neparnog stepena krecemo iz bilo kog od ta dva
    else if (count_odd == 2) {
      start_node = find_node_with_odd_degree();
      print_euler_path_or_cycle(start_node);
    }
  }

  void print_euler_path_or_cycle(int u)
  {
    // Idemo kroz sve susede cvora u
    for (int v : adjacency_list[u]) {
      // Ako grana postoji tj nije obrisana i ako moze biti obrisana
      if (v != -1 && is_valid_edge(u, v)) {
        std::cout << u << " -> " << v << std::endl;
        remove_edge(u, v);
        print_euler_path_or_cycle(v);
      }
    }
  }

  // Funkcija koja proverava da li grana moze biti uklonjena. Funkcija gleda da li je grana most. Ako je most i ako je jedina grana iz cvora u onda je sve okej
  // ali ako je most a ima jos grana iz cvora u, onda ne smemo da uzmemo most, tj necemo je ukloniti
  bool is_valid_edge(int u, int v)
  {
    // Brojimo koliko imamo grana od cvora u, prebrojimo sve one koje nisu izbacene. Ako imam tacno jednu granu onda je u -> v jedina grana i nju onda smemo da
    // izbacimo. Ako imamo vise od jedne grane, onda gledamo da li je u -> v most. Ako jeste most ideo dalje u pretragu, jer ona ne sme biti izbacena, ako nije
    // most mozemo da je izbacimo
    int count_edges = std::count_if(adjacency_list[u].begin(), adjacency_list[u].end(), [](int x){ return x != -1; });

    if (count_edges == 1)
      return true;

    // Uklanjamo grane u -> v i v -> u
    remove_edge(u, v);

    // Postavimo sve cvorove na neposecene jer imamo vise DFS obilazaka grafa, kako bismo uvek imali "sveze" podatke, azuriramo ovaj vektor
    std:fill(visited.begin(), visited.end(), false);

    // Pokrecemo DFS iz u
    DFS(u);

    // Brisemo pojavu -1 iz vektora povezanosti cvora u. Ovim smo uklonili jednu granu koja je bila obrisana. I onda dodajemo granu u -> v.
    adjacency_list[u].erase(std::find(adjacency_list[u].begin(), adjacency_list[u].end(), -1));
    // Brisemo pojavu -1 iz vektora povezanosti cvora v. Ovim smo uklonili jednu granu koja je bila obrisana. I onda dodajemo granu v -> u.
    adjacency_list[v].erase(std::find(adjacency_list[v].begin(), adjacency_list[v].end(), -1));
    // Vracamo grane u -> v i v -> u
    add_edge(u, v);

    // Ako nismo posetili cvor v znaci da je grana u -> v bila most i da ne smemo da je uklanjamo
    if (!visited[v]) {
      return false;
    }

    // Inace vracamo true, sto znaci da grana u -> v moze da bude izbacena
    return true;
  }

  void remove_edge(int u, int v)
  {
    // Nalazimo gde se u listi povezanosti cvora u nalazi cvor v i na tu poziciju stavljamo -1, kao oznaku da je grana uklonjena
    auto it_u = std::find(adjacency_list[u].begin(), adjacency_list[u].end(), v);
    *it_u = -1;

    // Nalazimo gde se u listi povezanosti cvora v nalazi cvor u i na tu poziciju stavljamo -1, kao oznaku da je grana uklonjena
    auto it_v = std::find(adjacency_list[v].begin(), adjacency_list[v].end(), u);
    *it_v = -1;
  }

private:
  // Lista susedstva. Imamo vector vector-a, sto znaci za svaki od cvorova [0,V) imamo po jednu listu koja cuva susede odgovarajuceg cvora
  std::vector<std::vector<int>> adjacency_list;
  // Broj cvorova grafa
  int V;
  // Lista posecenih cvorova. Da ne bismo ulazili u beskonacnu rekurziju ne zelimo da obilazimo cvorove koje smo vec obisli na putu kojim smo krenuli, zato cuvamo listu
  // posecenih cvorova
  std::vector<bool> visited;
  // Vektor koji ce nam govoriti koliki je ulazni stepen svakog od cvorova
  // Graf je nepovezan, pa ce nam ovde biti uracunate i ulazne i izlazne grane, jer su nam u ovom slucaju jednake
  std::vector<int> degrees;
};

int main ()
{
  Graph g(5);

  // Put
  // g.add_edge(0, 1);
  // g.add_edge(0, 2);
  // g.add_edge(1, 2);
  // g.add_edge(2, 3);

  // Ciklus
  // g.add_edge(0, 1);
  // g.add_edge(1, 2);
  // g.add_edge(2, 0);

  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 4);

  g.fleury();

  return 0;
}
