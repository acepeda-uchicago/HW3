#include <iostream>
struct HelloWorld{
    HelloWorld(){
        std::cout << "Hello World!" << std::endl << "\n";
    }
    HelloWorld(int x, const std::string& y){
        for (int i = 0; i < x; i++){
            std::cout << y << std::endl ;
        }
        std::cout << "\n";
    }
    HelloWorld(const std::string& y, int x){
        std::cout << y;
        for (int i = 0; i < x; i++){
            std::cout << "!";
        }
        std::cout << std::endl << "\n";
    }
};

// answer to standard question
HelloWorld example_1;

// order of constructor changes cout
// print 3x
HelloWorld example_2(3, "Hello World!");
// print w/ 3 !'s
HelloWorld example_3("Hello World", 3);


int main()
{ return 0;
}