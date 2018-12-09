#include <iostream>
#include <list>

//https://bytes.com/topic/c/answers/133169-return-memory-os

struct Big {
  int array[64];
};

using namespace std;

int main()
{
  {
    list<Big> tempList;

    cout << "going to start adding now ..." << endl;

    for(long i=0; i<= 1000000; i++)
    {
      tempList.push_back(Big());
    }

    cout << "done adding. press enter." << endl;
    cin.get();
  }

  cout << "out of scope. press enter." << endl;
  cin.get();

  return 0;
}

