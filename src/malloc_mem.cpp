//
// g++-7 malloc_mem.cpp -O3 -o mememe -std=c++11 && ./mememe
//
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>


using namespace std;

int sz[1024] = {}, idx_sz = 0;
int rss0[1024] = {}, idx_rss0 = 0;
int rss1[1024] = {}, idx_rss1 = 0;

void process_mem_usage(double &vm_usage, double &resident_set) {
  vm_usage = 0.0;
  resident_set = 0.0;
  ifstream stat_stream("/proc/self/stat", ios_base::in);
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
  vm_usage = vsize / 1024.0;
  resident_set = rss * page_size_kb;
}

void test_malloc_mem(int k) {
  static const int LOOPSIZE = 1000000; // 1million
  char **strvec = (char **)malloc(LOOPSIZE*sizeof(char*));
  for (long long i = 0; i < LOOPSIZE; ++i) {
    strvec[i] = (char *)malloc(k);
    strvec[i][0] = '0' + (rand() % 26);
    strvec[i][k-1] = '0' + (k % 26);
  }
  double vm, rss;
  process_mem_usage(vm, rss);
  sz[idx_sz++] = k, rss0[idx_rss0++] = rss;
  for (long long i = 0; i < LOOPSIZE; ++i) {
    free(strvec[i]);
  }
  free(strvec);
}

int main(int argc, char **argv) {
  for (int k = 8; k <= 800; k += 8) {
    test_malloc_mem(k);
    double vm1, rss;
    process_mem_usage(vm1, rss);
    rss1[idx_rss1++] = rss;
  }
  for (int i = 0; i < idx_sz; i++) cout << sz[i] << ",";
  cout << "\n" << string(80, '-') << endl;
  for (int i = 0; i < idx_rss0; i++) cout << rss0[i] << ",";
  cout << "\n" << string(80, '-') << endl;
  for (int i = 0; i < idx_rss1; i++) cout << rss1[i] << ",";
  cout << "\n" << string(80, '-') << endl;
  return 0;
}
