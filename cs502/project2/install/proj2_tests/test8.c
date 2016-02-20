void f3(){
    int a3;
    int b3 = a3;
}
void f2(){
    int a2;
    int b2 = a2;
}
void f1(){
    int a1;
    int b1 = a1;
    f2();
}
main(){
    int a0;
    int b0 = a0;
    f1();
}
