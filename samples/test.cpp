#include <iostream>
using namespace std;

int unusedHelper(){
    return 5;
}

int main(){
    int x=10;
    int y=20;
    if(x){
        int z=30;
    }
    return 0;
}