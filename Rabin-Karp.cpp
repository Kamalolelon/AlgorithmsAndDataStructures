#include <iostream>
#include <string>

using namespace std;

int FunHash(const string& name) {
    int sum = 0;
    for (int i = 0; i < name.length(); i++) {
        char c = toupper(name[i]); //biblioteka cctype, ale tutaj iostream te¿ jakoœ obejmuje
        sum += (c - 'A' + 1);
    }
    return sum % 13;
}

void RabinKarp(const string& text, const string& pattern, int* result, int& count) {
    count = 0;

    int n = text.length(); //od 1 liczy
    int m = pattern.length();

    if (m > n) {
        return;
    }

    int patternHash = FunHash(pattern);
    int windowHash = FunHash(text.substr(0, m));  //substr(string,start,length, text o d³ugoœci m)
    /* int windowHash = 0;
     for (int i = 0; i < m; i++) {
         char c = toupper(text[i]);
         windowHash += (c - 'A' + 1);
     }
     windowHash %= 13;*/

    for (int i = 0; i <= n - m; i++) {
        if (windowHash == patternHash) {
            //if (text.substr(i, m) == pattern)
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

        if (i < n - m) {
            windowHash = (windowHash - text[i] + text[i + m]) % 13;
            if (windowHash < 0) {
                windowHash += 13;
            }
        }
    }
}

int main() {
    string text = "ABABABABACAB";
    string pattern = "ABAB";

    int result[100];
    int count;

    RabinKarp(text, pattern, result, count);

    cout << "Wzorzec \"" << pattern << "\" znaleziono na indeksach: ";
    for (int i = 0; i < count; i++) {
        cout << result[i] << " ";
    }


    return 0;
}