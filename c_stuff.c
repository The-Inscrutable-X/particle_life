#include <stdio.h> 
 
int _(int i) { 
    int _i; 
    printf("%d^2=%d\n", i+1, _i=i+(--i?i+_(i):i)+1); 
    return _i; 
} 
 
int main() { 
    _(10); 
} 