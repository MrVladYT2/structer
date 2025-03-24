#include <iostream>
using namespace std;
int extendedEuclidean(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int gcd = extendedEuclidean(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}
int modularInverse(int a, int n) {
    int x, y;
    int gcd = extendedEuclidean(a, n, x, y);
    if (gcd != 1) {
        return 0; 
    } else {
        return (x % n + n) % n; 
    }
}
int main() {
    int a, n;
    cin >> a >> n;
    int inverse = modularInverse(a, n);
    cout << inverse << endl;
    cout << "Калинкин Владислав Андреевич" << endl;
    cout << "090301-ПОВА-о24" << endl;

    return 0;
}
