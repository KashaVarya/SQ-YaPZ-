class C1 {
    int a;
}

class C2 : private C1 {
    double b;
    int a;
}

class C3: public C1{}
class C4 : protected C2, public C3 {
    float d;
}
    
