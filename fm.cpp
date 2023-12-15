// Construcción del FM index de un texto
//
// Prerrequisitos: Tener la biblioteca SDSL instalada
//
// Compilación: g++ -O3 -o fmi FM-index.cpp -lsdsl -ldivsufsort -ldivsufsort64

#include <algorithm>
#include <iostream>
#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <chrono>

using namespace sdsl;
using namespace std;

vector<string> file_names;

vector<string> doc_locate(string T, string p) {
  // PASO 2: construir FM-index
  csa_wt<wt_huff<>> fm_index;
  construct_im(fm_index, T, 1);

  // PASO 3: encontrar posiciones del patron
  auto posiciones = locate(fm_index, p.begin(), p.end());
  sort(posiciones.begin(), posiciones.end());

  // PASO 4: encontrar posiciones de los delimitadores de archivos
  string delim = "\3";
  auto delim_positions = locate(fm_index, delim.begin(), delim.end());
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
