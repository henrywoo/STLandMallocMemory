//
// g++-7 stl_mem.cpp -O3 -o mememe -std=c++11 && ./mememe
//
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <list>
#include <deque>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
//#define ALL_ZERO_STRING
#define STL_CONTAINER vector

int sz[1024]={}, idx_sz=0;
int rss0[1024]={}, idx_rss0=0;
int rss1[1024]={}, idx_rss1=0;

void process_mem_usage(double& vm_usage, double& resident_set) {
  vm_usage     = 0.0;
  resident_set = 0.0;
  ifstream stat_stream("/proc/self/stat",ios_base::in);
  string pid, comm, state, ppid, pgrp, session, tty_nr;
  string tpgid, flags, minflt, cminflt, majflt, cmajflt;
  string utime, stime, cutime, cstime, priority, nice;
  string O, itrealvalue, starttime;
  unsigned long vsize;
  long rss;
  stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
              >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
              >> utime >> stime >> cutime >> cstime >> priority >> nice
              >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
  stat_stream.close();
  long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
  vm_usage     = vsize / 1024.0;
  resident_set = rss * page_size_kb;
}

void test_stl_mem(int k){
  srand(time(NULL));
  STL_CONTAINER<string> strvec;
  static const int LOOPSIZE = 1000000; // 1million
  for(long long i = 0; i < LOOPSIZE; ++i){
    string out(k,'0');
#ifndef ALL_ZERO_STRING
    out[rand()%k] = rand()%26+'0'; // all strings are different, mostly!
#endif
    strvec.push_back(out);
  }
  double vm, rss;
  process_mem_usage(vm, rss);
  sz[idx_sz++]=k, rss0[idx_rss0++]=rss;
}

int main(int argc, char** argv){
  for (int k=8; k<=800;k+=8){
    test_stl_mem(k);
    double vm1, rss;
    process_mem_usage(vm1, rss);
    rss1[idx_rss1++] = rss;
  }
  for(int i=0;i<idx_sz;i++) cout << sz[i] << ",";
  cout << "\n" << string(80,'-') << endl;
  for(int i=0;i<idx_rss0;i++) cout << rss0[i] << ",";
  cout << "\n" << string(80,'-') << endl;
  for(int i=0;i<idx_rss1;i++) cout << rss1[i] << ",";
  cout << "\n" << string(80,'-') << endl;
  return 0;
}
