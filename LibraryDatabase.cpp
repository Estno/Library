#include "Library.h"

int main() {
    Library library;
    library.init();
    while(library.running) {
        library.menu();
    }
    return 0;
}
