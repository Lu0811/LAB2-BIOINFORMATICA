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

class NeedlemanWunsch {
private:
    string seq1, seq2;
    int match, mismatch, gap;
    vector<vector<int>> scoreMatrix;
    vector<Alignment> alignments;

    void initializeMatrix() {
        int n = seq1.size();
        int m = seq2.size();
        scoreMatrix.assign(n+1, vector<int>(m+1, 0));
        for (int i = 1; i <= n; ++i) scoreMatrix[i][0] = i * gap;
        for (int j = 1; j <= m; ++j) scoreMatrix[0][j] = j * gap;
    }

    void fillMatrix() {
        int n = seq1.size();
        int m = seq2.size();

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                int score_diag = scoreMatrix[i-1][j-1] + (seq1[i-1] == seq2[j-1] ? match : mismatch);
                int score_up = scoreMatrix[i-1][j] + gap;
                int score_left = scoreMatrix[i][j-1] + gap;
                scoreMatrix[i][j] = max({score_diag, score_up, score_left});
            }
        }
    }

    void backtrack(int i, int j, string aligned1, string aligned2) {
        if (i == 0 && j == 0) {
            reverse(aligned1.begin(), aligned1.end());
            reverse(aligned2.begin(), aligned2.end());
            alignments.push_back({aligned1, aligned2});
            return;
        }

        int currentScore = scoreMatrix[i][j];

        if (i > 0 && j > 0) {
            int score_diag = scoreMatrix[i-1][j-1] + (seq1[i-1] == seq2[j-1] ? match : mismatch);
            if (currentScore == score_diag) {
                backtrack(i-1, j-1, aligned1 + seq1[i-1], aligned2 + seq2[j-1]);
            }
        }
        if (i > 0) {
            int score_up = scoreMatrix[i-1][j] + gap;
            if (currentScore == score_up) {
                backtrack(i-1, j, aligned1 + seq1[i-1], aligned2 + "-");
            }
        }
        if (j > 0) {
            int score_left = scoreMatrix[i][j-1] + gap;
            if (currentScore == score_left) {
                backtrack(i, j-1, aligned1 + "-", aligned2 + seq2[j-1]);
            }
        }
    }

public:
    NeedlemanWunsch(const string& s1, const string& s2, int m, int mm, int g) :
        seq1(s1), seq2(s2), match(m), mismatch(mm), gap(g) {}

    void align() {
        initializeMatrix();
        fillMatrix();
        alignments.clear();
        backtrack(seq1.size(), seq2.size(), "", "");
    }

    int getFinalScore() const {
        return scoreMatrix[seq1.size()][seq2.size()];
    }

    void printAndSaveResults(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "Error al abrir el archivo " << filename << " para escribir.\n";
            return;
        }

        cout << "Secuencia 1: " << seq1 << "\n";
        cout << "Secuencia 2: " << seq2 << "\n\n";

        file << "Secuencia 1: " << seq1 << "\n";
        file << "Secuencia 2: " << seq2 << "\n\n";

        cout << "Score final: " << getFinalScore() << "\n\n";
        file << "Score final: " << getFinalScore() << "\n\n";

        cout << "Matriz de scores:\n";
        file << "Matriz de scores:\n";
        for (const auto& row : scoreMatrix) {
            for (int val : row) {
                cout << val << "\t";
                file << val << "\t";
            }
            cout << "\n";
            file << "\n";
        }

        cout << "\nCantidad de alineamientos: " << alignments.size() << "\n\n";
        file << "\nCantidad de alineamientos: " << alignments.size() << "\n\n";

        cout << "Alineamientos generados:\n";
        file << "Alineamientos generados:\n";
        for (const auto& aln : alignments) {
            cout << aln.seq1_aligned << "\n" << aln.seq2_aligned << "\n\n";
            file << aln.seq1_aligned << "\n" << aln.seq2_aligned << "\n\n";
        }

        file.close();
        cout << "Resultados guardados en '" << filename << "'\n";
    }
};

bool esSubstring(const string& s1, const string& s2) {
    if (s1.find(s2) != string::npos) return true;
    if (s2.find(s1) != string::npos) return true;
    return false;
}

int main() {
    string s1, s2;
    int match, mismatch, gap;

    cout << "Ingrese la primera secuencia: ";
    getline(cin, s1);

    cout << "Ingrese la segunda secuencia: ";
    getline(cin, s2);

    cout << "Ingrese el valor para match (ejemplo +1): ";
    cin >> match;

    cout << "Ingrese el valor para mismatch (ejemplo -1): ";
    cin >> mismatch;

    cout << "Ingrese el valor para gap (ejemplo -2): ";
    cin >> gap;

    cout << "\n¿Una cadena es substring de la otra? ";
    cout << (esSubstring(s1, s2) ? "Sí\n\n" : "No\n\n");

    NeedlemanWunsch nw(s1, s2, match, mismatch, gap);
    nw.align();

    nw.printAndSaveResults("resultado_alineamiento.txt");

    return 0;
}
