#include <unistd.h> //sleep
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
#if 1
  {
    getchar(); // to monitor any changes    
    char *p = new char[1024000*5*64*2];
    for(int i=0;i<1024000*5*64*2;++i) p[i]='0';
    std::cout << "finished loading 1st\n";
    getchar(); // to monitor any changes    
    delete [] p;
    std::cout << "out of scope\n";
  }
  {
    getchar(); // to monitor any changes    
    char *p = new char[1024000*5*64*2];
    for(int i=0;i<1024000*5*64*2;++i) p[i]='0';
    std::cout << "finished loading 1st\n";
    getchar(); // to monitor any changes    
    delete [] p;
    std::cout << "out of scope\n";
  }
  {
    getchar(); // to monitor any changes    
    char *p = new char[1024000*5*64*2];
    for(int i=0;i<1024000*5*64*2;++i) p[i]='0';
    std::cout << "finished loading 1st\n";
    getchar(); // to monitor any changes    
    delete [] p;
    std::cout << "out of scope\n";
  }
  {
    getchar(); // to monitor any changes    
    char *p = new char[1024000*5*64*2];
    for(int i=0;i<1024000*5*64*2;++i) p[i]='0';
    std::cout << "finished loading 1st\n";
    getchar(); // to monitor any changes    
    delete [] p;
    std::cout << "out of scope\n";
  }
  {
    getchar(); // to monitor any changes    
    char *p = new char[1024000*5*64*2];
    for(int i=0;i<1024000*5*64*2;++i) p[i]='0';
    std::cout << "finished loading 1st\n";
    getchar(); // to monitor any changes    
    delete [] p;
    std::cout << "out of scope\n";
  }
#else
  {
    getchar(); // to monitor any changes    
    std::vector<int> strvec(1000000,1);
    std::cout << "finished loading 1st\n";
    getchar(); // to monitor any changes    
  }
#endif
  std::cout << "Test's finished.\n";
  return 0;
}
