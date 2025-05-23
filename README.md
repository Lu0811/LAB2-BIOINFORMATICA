# LAB2-BIOINFORMATICA
##  1. Inclusión de librerías y definición de estructuras

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

struct Alignment {
    string seq1_aligned;
    string seq2_aligned;
};
```

* `#include <...>`: se importan librerías necesarias.

  * `iostream`: para entrada/salida.
  * `vector`: para usar matrices y listas dinámicas.
  * `string`: para trabajar con cadenas de caracteres.
  * `algorithm`: para usar funciones como `max` y `reverse`.
  * `fstream`: para leer y escribir archivos.

* `using namespace std;`: para evitar escribir `std::` todo el tiempo.

* `struct Alignment`: estructura que almacena dos secuencias alineadas.

---

##  2. Clase `NeedlemanWunsch`

### Atributos privados

```cpp
string seq1, seq2;
int match, mismatch, gap;
vector<vector<int>> scoreMatrix;
vector<Alignment> alignments;
```

* `seq1`, `seq2`: secuencias a alinear.
* `match`, `mismatch`, `gap`: puntajes ingresados por el usuario.
* `scoreMatrix`: matriz que almacena los puntajes acumulados.
* `alignments`: vector de todos los alineamientos óptimos.

---

### 2.1 Inicialización de la matriz

```cpp
void initializeMatrix() {
    ...
}
```

* Crea una matriz de `(n+1)x(m+1)` con ceros.
* Llena la primera fila y columna con penalizaciones por espacio (`gap`).
* Esto establece la base del alineamiento.

---

### 2.2 Llenado de la matriz de puntuaciones

```cpp
void fillMatrix() {
    ...
}
```

Para cada celda `scoreMatrix[i][j]`, calcula el mejor puntaje según:

* Diagonal: `scoreMatrix[i-1][j-1] + match/mismatch`
* Arriba: `scoreMatrix[i-1][j] + gap`
* Izquierda: `scoreMatrix[i][j-1] + gap`

Toma el **máximo** de esos valores y lo guarda.

---

### 2.3 Backtracking (reconstrucción de alineamientos)

```cpp
void backtrack(int i, int j, string aligned1, string aligned2) {
    ...
}
```

* Función **recursiva** que recorre la matriz desde el final `(i,j)` hasta `(0,0)`.
* Si llega al inicio, guarda la alineación completa en `alignments`.
* Explora todos los caminos válidos para reconstruir **todos los alineamientos óptimos**:

  * Diagonal: letra vs letra.
  * Arriba: letra vs guion (`-`).
  * Izquierda: guion vs letra.

---

### 2.4 Método `align()`

```cpp
void align() {
    initializeMatrix();
    fillMatrix();
    alignments.clear();
    backtrack(seq1.size(), seq2.size(), "", "");
}
```

* Orquesta todo el proceso:

  1. Inicializa la matriz.
  2. Llena la matriz.
  3. Ejecuta backtracking para obtener los alineamientos.

---

### 2.5 Imprimir y guardar resultados

```cpp
void printAndSaveResults(const string& filename) const {
    ...
}
```

* Imprime en consola y guarda en archivo:

  * Las secuencias originales.
  * Score final del alineamiento.
  * La matriz completa.
  * Todos los alineamientos óptimos.

---

## 3. Función auxiliar `esSubstring`

```cpp
bool esSubstring(const string& s1, const string& s2) {
    if (s1.find(s2) != string::npos) return true;
    if (s2.find(s1) != string::npos) return true;
    return false;
}
```

* Verifica si una cadena está **contenida** dentro de la otra.
* Usa `.find()` para buscar subcadenas.

---

## 4. Función `main()`

```cpp
int main() {
    ...
}
```

1. Pide al usuario:

   * Dos secuencias.
   * Puntajes para match, mismatch y gap.
2. Llama a `esSubstring` para informar si hay relación entre secuencias.
3. Crea un objeto `NeedlemanWunsch` con los datos.
4. Ejecuta el método `align()` para realizar el alineamiento.
5. Muestra los resultados por pantalla y los guarda en un archivo `.txt`.

---

##  Resumen visual del flujo

```
Usuario → [main()] → [NeedlemanWunsch::align()]
               ↓
       Inicializa matriz
               ↓
        Llena matriz de scores
               ↓
        Realiza backtracking
               ↓
      Muestra + guarda resultados
```
