// Construcción del FM index de un texto
//
// Prerrequisitos: Tener la biblioteca SDSL instalada
//
// Compilación: g++ -O3 -o fmi FM-index.cpp -lsdsl -ldivsufsort -ldivsufsort64

#include <algorithm>
#include <iostream>
#include <sdsl/suffix_arrays.hpp>
#include <string>

using namespace sdsl;
using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Uso: " << argv[0] << " [archivos]" << endl;
    return 1;
  }

  // PASO 1: leer archivos y concatenarlos
  vector<string> file_names;
  string seq;
  for (int i = 1; i < argc; i++) {
    file_names.push_back(argv[i]);
    int_vector<> tmp;
    load_vector_from_file(tmp, argv[i], 1);
    string str(tmp.begin(), tmp.end());
    seq += str + (char)3;
  }
  // cout << "DEBUG:" << seq << endl;

  // PASO 2: construir FM-index
  csa_wt<wt_huff<>> fm_index;
  construct_im(fm_index, seq, 1);

  // PASO 3: encontrar posiciones del patron
  string patron = "Incorrect IRQ";
  auto posiciones = locate(fm_index, patron.begin(), patron.end());
  sort(posiciones.begin(), posiciones.end());
  cout << "POSICIONES PATRON:" << endl;
  for (size_t i = 0; i < posiciones.size(); ++i) {
    cout << posiciones[i] << endl;
  }

  // PASO 4: encontrar posiciones de los delimitadores de archivos
  string delim = "\3";
  auto delim_positions = locate(fm_index, delim.begin(), delim.end());
  sort(delim_positions.begin(), delim_positions.end());
  cout << "POSICIONES delimitador:" << endl;
  for (size_t i = 0; i < delim_positions.size(); ++i) {
    cout << delim_positions[i] << endl;
  }

  // PASO 5: calcular nombres de archivos en que se encuentra patron
  set<string> files;
  for (int i = 0; i < posiciones.size(); i++) {
    int pos = lower_bound(delim_positions.begin(), delim_positions.end(),
                          posiciones[i]) -
              delim_positions.begin();
    files.insert(file_names[pos]);
  }

  for (auto f : files) cout << f << endl;

  return 0;
}
