#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct Node
{
  Node()
  {
    // Cvor inicijalno nije list
    is_leaf = false;
    // Oba potomka svakog cvora su nullptr, tj ne postoje dok ne dodjemo u situaciju da moramo da ih kreiramo
    nodes[0] = nodes[1] = nullptr;
  }

  // Flag koji govori da li je cvor list
  bool is_leaf;
  // Nema potrebe ni za kakvim mapama, znamo da su brojevi binarni, tako da sigurno imamo najvise 2 potomka
  Node *nodes[2];
};

// Funckija za racunanje vrednosti broja iz stringa koji predstavlja binarnu reprezentaciju
int value(std::string &binary)
{
  int n = binary.size();

  int value = 0;

  for (int i = 0, j = n - 1;i < n;i++, j--)
    // -'0' da bismo zaista dobili cifru, jer su elementi binary karakteri i dobijali bismo ASCII vrednost
    value += std::pow(2,i) * (binary[j] - '0');
}

void add_number(Node *root, std::string &number, int i)
{
  // Ukoliko smo dosli do kraja broja, to znaci da je cvor u kome se trenutno nalazimo i list, tj predstavlja kraj broja, oznacavamo to i zavrsavamo rekurziju,
  // jer je ceo broj dodat u stablo
  if (i == number.size()) {
    root->is_leaf = true;
    return ;
  }

  // Ukoliko nemamo odgovarajuci cvor, kreiramo ga, - '0' da bismo zaista dobili 0 ili 1 a ne ASCII vrednost
  if (root->nodes[number[i] - '0'] == nullptr)
    root->nodes[number[i] - '0'] = new Node();

  // Rekurzivno nastavljamo za ostatak broja
  add_number(root->nodes[number[i] - '0'], number, i + 1);
}

// Trazimo maksimalnu vrednost XOR-a broja number sa svim drugim brojevima koji su vec u stablu
void find_XOR(Node *root, std::string &XOR, std::string &number, int i)
{
  if (root->is_leaf) {
    return ;
  }

  // Trenutna cifra broja number koju razmatramo
  // Ukoliko je moguce zelimo da idemo drugim putem u odnosnu na ovu cifru, dakle ako je on 0 mi zelimo da idemo preko 1, jer nam tako XOR daje 1, a maksimalan XOR ce biti onaj
  // sa najvise jedinica na pocetku
  int digit = number[i] - '0';

  // Ukoliko imamo cvor koji cuva cifru suprotnu od i-te cifre broja number, super, idemo tim putem, i u promenljivu XOR dodajemo 1
  if (root->nodes[std::abs(digit - 1)]) {
    XOR += '1';
    find_XOR(root->nodes[std::abs(digit - 1)], XOR, number, i + 1);
  }
  else {
    XOR += '1';
    find_XOR(root->nodes[digit], XOR, number, i + 1);
  }
}

int main ()
{
  std::vector<std::string> numbers = {"001", "010", "011", "100", "101", "110", "111"};

  int max = 0;
  std::string max_XOR;

  std::string XOR;

  Node *root = new Node();

  for (std::string &s : numbers) {
    XOR = "";
    add_number(root, s, 0);

    find_XOR(root, XOR, s, 0);

    if (value(XOR) > max) {
      max = value(XOR);
      max_XOR = XOR;
    }
  }

  std::cout << max_XOR << std::endl;

  return 0;
}
