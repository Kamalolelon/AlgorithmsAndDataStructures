#include <iostream>
#include <string>

using namespace std;

void NaiveSearch(const string& text, const string& pattern, int* result, int& count) {
    count = 0;

    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; i++) {
        bool match = true;

        for (int j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            result[count++] = i;
        }
    }
}

int main() {
    string text = "ABABABABACAB";
    string pattern = "ABAB";

    int result[100];
    int count;

    NaiveSearch(text, pattern, result, count);

    cout << "Wzorzec \"" << pattern << "\" znaleziono na indeksach: ";
    for (int i = 0; i < count; i++) {
        cout << result[i] << " ";
    }
  

    return 0;
}
