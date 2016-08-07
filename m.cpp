#include <unistd.h> //sleep
#include <malloc.h>

#include <string>
#include <vector>
#include <iostream>

using namespace std;

void func(int longstring){
  {
    std::vector<std::string> strvec;
    string s="This is gonna be a long string just to fill up the memory used by this fucking pthread";
    string s4=s+s+s+s;
    cout << s.size() << endl;
    cout << s4.size() << endl;
    for(long i = 0; i < 1024000; ++i){
      strvec.push_back( longstring==0 ?
          "This is gonna be a long string just to fill up the memory used by this fucking pthread"
          :
          "This is gonna be a long string just to fill up the memory used by this fucking pthread"
          );
    }
    std::cout << "finished loading 1st\n";
    cin.get(); // to monitor any changes    
    std::cout << "out of scope\n";
    //malloc_trim(0);
  }
}

int main(int argc, char** argv){
#if 1
  func(0);
  //func(1);
  //func(1);
  //func(1);
  //func(0);
#else
  {
    sleep(10); // to monitor any changes    
    std::vector<int> strvec(1000000,1);
    std::cout << "finished loading 1st\n";
    sleep(10); // to monitor any changes    
  }
#endif
  std::cout << "Test's finished.\n";
  cin.get();
  return 0;
}
