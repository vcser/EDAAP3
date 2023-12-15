// // Construcción del suffix array y la BWT de un texto
// //
// // Prerrequisitos: Tener la biblioteca SDSL instalada
// //
// // Compilación: g++ -O3 -o sa sa.cpp -lsdsl -ldivsufsort -ldivsufsort64

#include <algorithm>
#include <iostream>
#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <vector>
#include <chrono>

using namespace sdsl;
using namespace std;

vector<string> file_names;

// Función para buscar todas las ocurrencias de un patrón en un Suffix Array
std::vector<int> search_all_occurrences(const std::string& pattern,
                                        const std::string& txt,
                                        const sdsl::int_vector<>& sa) {
  int n = txt.length();
  int m = pattern.length();
  std::vector<int> occurrences;

  // Búsqueda binaria en el Suffix Array
  int low = 0, high = n - 1;
  while (low <= high) {
    int mid = (low + high) / 2;
    int suffix = sa[mid];
    std::string suffix_str = txt.substr(suffix, m);

    if (pattern == suffix_str) {
      // Se encontró una coincidencia, ahora busca más ocurrencias
      occurrences.push_back(suffix);

      // Busca hacia la izquierda
      int left = mid - 1;
      while (left >= 0 && txt.substr(sa[left], m) == pattern) {
        occurrences.push_back(sa[left]);
        left--;
      }

      // Busca hacia la derecha
      int right = mid + 1;
      while (right < n && txt.substr(sa[right], m) == pattern) {
        occurrences.push_back(sa[right]);
        right++;
      }

      return occurrences;
    } else if (pattern < suffix_str) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  return occurrences;
}

vector<string> doc_locate(string T, string p) {
  // PASO 2: construir suffix array
  int n = T.size();
  int_vector<> sa(1, 0, bits::hi(n) + 1);
  sa.resize(n);
  algorithm::calculate_sa((const unsigned char*)T.data(), n, sa);

  // PASO 3: encontrar posiciones del patron
  auto posiciones = search_all_occurrences(p, T, sa);
  sort(posiciones.begin(), posiciones.end());

  // PASO 4: encontrar posiciones de los delimitadores de archivos
  string delim = "\3";
  auto delim_positions = search_all_occurrences(delim, T, sa);
  sort(delim_positions.begin(), delim_positions.end());

  // PASO 5: calcular nombres de archivos en que se encuentra patron
  set<string> files;
  for (int i = 0; i < posiciones.size(); i++) {
    int pos = lower_bound(delim_positions.begin(), delim_positions.end(),
                          posiciones[i]) -
              delim_positions.begin();
    files.insert(file_names[pos]);
  }

  return vector<string>(files.begin(), files.end());
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cout << "Uso: " << argv[0] << "<archivo patron> [archivos]" << endl;
    return 1;
  }

  // PASO 0: leer archivo patron
  int_vector<> tmp;
  load_vector_from_file(tmp, argv[1], 1);
  string pattern(tmp.begin(), tmp.end());

  // PASO 1: leer archivos y concatenarlos
  string seq;
  for (int i = 2; i < argc; i++) {
    file_names.push_back(argv[i]);
    int_vector<> tmp;
    load_vector_from_file(tmp, argv[i], 1);
    string str(tmp.begin(), tmp.end());
    seq += str + (char)3;
  }

  #define ITER 30

  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < ITER; i++) {
    auto files = doc_locate(seq, pattern);
  }
  auto stop = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(stop - start).count() / ITER;

  cout << argv[2] << "," << pattern.size() << "," << elapsed << "\n";

  // for (auto f : files) cout << f << endl;

  return 0;
}
