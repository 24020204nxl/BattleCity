#include <iostream>
#include <string>
using namespace std;
bool lachuthuong(char c) {
    return c >= 'a' && c <= 'z';
}
bool lachuin(char c) {
    return c >= 'A' && c <= 'Z';
}
bool lachu(char c) {
    return lachuthuong||lachuin;
}
char thuongthanhin(char c) {
    if (lachuthuong(c)) {
        return c - 'a' + 'A';
    }
    return c;
}
void viethoa(string &s) {
    bool tumoi= true;
    n=s.length();
    for (int i = 0; i < n; i++) {
        if (lachu(s[i])) {
            if (tumoi) {
                s[i] = thuongthanhin(str[i]);
               tumoi= false;
            }
        } else {
            tumoi= true;
        }
    }
}
int main() {
    string str;
    getline(cin, str);
    viethoa(str);
    cout << "Chuỗi sau khi viết hoa đầu từ: " << str << endl;
    return 0;
}
