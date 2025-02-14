#include <iostream>
#include <string>

using namespace std;


void BuildLPS(const string& pattern, int* lps) {
    int m = pattern.length();
    int len = 0; 
    lps[0] = 0;  

    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else {
            if (len != 0) {
                len = lps[len - 1]; // Powrót do poprzedniego prefiksu
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMPSearch(const string& text, const string& pattern) {
    int n = text.length();  
    int m = pattern.length();  

    if (m == 0) {
        return;
    }

    int* lps = new int[m];
    BuildLPS(pattern, lps);  

    int i = 0; 
    int j = 0;

    while (i < n) {
        if (text[i] == pattern[j]) {  // Jeœli znaki pasuj¹
            i++;  
            j++; 
        }

        if (j == m) {  // Jeœli ca³y wzorzec zosta³ dopasowany
            cout << "Wzorzec znaleziono na indeksie: " << i - j << endl;
            j = lps[j - 1];  
        }
        else if (i < n && text[i] != pattern[j]) {  // Jeœli znaki nie pasuj¹
            if (j != 0) {
                j = lps[j - 1];  
            } else {
                i++;  
            }
        }
    }

    delete[] lps;  
}

int main() {
    string text = "ABABABABACAB";
    string pattern = "ABAB";

    KMPSearch(text, pattern);

    return 0;
}