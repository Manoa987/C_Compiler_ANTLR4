#include <inttypes.h>
int toto(int a) {
    int b = a+2;
    return b;
}

int main() {
    int a = 0;
    int b = toto(a);
    return b;
}
