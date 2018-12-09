# STL Memory Pool and Malloc Subsystem Analysis

- by Henry Wu

GCC STL memory management has two stages.
 
Before g++3.4, STL uses two strategies to allocate memory, although HP(Visual C++), SGI(GCC), RW(C++Builder) STLs have different implementations. To avoid defragmentation and speed up memory allocation, STL matains a memory pool with free memory blocks connnected by a free list. The source code is at [bits/stl_alloc.h](https://github.com/henrywoo/STLandMallocMemory/blob/master/g%2B%2B/3.3/bits/stl_alloc.h#L332). This two-strategies way increases the complexity of implementation, but reduced memeory defragmentation.
 
Starting from g++ 3.4, GCC STL by default uses operator new/delete, which finally call glibc's malloc/free [here](https://github.molgen.mpg.de/git-mirror/glibc/blob/master/malloc/malloc.c#L22), for memeory allocation. As malloc system also uses pool technique. We can still observe the pooling effect. The previous STL pool code was moved to [pool_allocator.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/ext/pool_allocator.h#L84).

The freelist properties are defined as below. We can see its size is 16, and max bytes is 128.
```cpp
 78     class __pool_alloc_base
 79     { 
 80     protected:
 81 
 82       enum { _S_align = 8 };
 83       enum { _S_max_bytes = 128 };
 84       enum { _S_free_list_size = (size_t)_S_max_bytes / (size_t)_S_align };
        ...
```

[Here](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/ext/pool_allocator.h#L243) is the logic of STL memory `allocation`. If `GLIBCXX_FORCE_NEW` is defined or object size is greater than 128 Bytes, just use new/delete(or malloc/free) sub-system; otherwise, try to get an object from the freelist in memory pool. If the object is from memory pool, after deallocation, it is released back to the pool. [Here]((https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/ext/pool_allocator.h#L279)) is the logic of `deallocation`.




 
The following is my test results generated from code [stl_mem.cpp](src/stl_mem.cpp) and [malloc_mem.cpp](src/malloc_mem.cpp), where I track memory usage of VM/VSS(Virtual Set Size) and RSS(Resident Set Size) `after STL container is deallocated(or goes out of scope)` or `after memory is reclaimed by free in malloc/free subsystem`. These two metrics are often used by many monitoring tools in companies to track program's memory usage.


## vector

![](img/vector.png)

The x axis is the size of string, y axis is the RSS memory. The red-dot line is RSS after all STL objects are released. From the red-dot line, we can see when object size is less than or equal to 128 bytes, the memory usage keeps increasing even after object goes out of scope.

## List

![](img/list.png)

For list, the memory usage keeps increasing.

## deque

![](img/deque.png)

Deque is actually an unrolled linked list plus a map(not STL map). So it is between vector and list. We can see from the graphs above, compared with vector, deque has around 30M overhead in RSS.

## Other versions of compilers and OSes

I tested with different compilers: `GCC 4.8`, `GCC 5`, `GCC 6` and `GCC 7`, `Clang++ 6.0` and different operating systemes: `IBM AIX 7.1`, `Sun Solaris 10`. `GCC 7` and `Clang++ 6.0` have almost the same behavior, but others behave very very differently.

Also STL, or its underlying malloc system, tries to reuse same objects as possible as it can. I tested strings with all '0' and random strings. The all-zero strings make program hold more memory in the pool.

![](img/gcc4.8_random_string.png)


## Conclusion

Both STL and malloc subsystem use pool technique to optimize memory allocation/deallocation. Sometimes it just won't return memory back to operating system, but this is __`NOT` memory leak__, since all the memory are reusable for the process.

Although it is not memory leak, it could cause some issue in production environment. Because the program cannot control how much memory is retained in the pool in that process, it may hold the system memory and starve other processes. Since the memory is not returned to OS, it is `not reusable for other processes` in system-wide. In that case, we need to __`improve our algorithm`__ or __`try other memory allocation tools`__([tcmalloc](http://goog-perftools.sourceforge.net/doc/tcmalloc.html), [jemalloc](http://people.freebsd.org/~jasone/jemalloc/bsdcan2006/jemalloc.pdf), [nedmalloc](http://www.nedprod.com/programs/portable/nedmalloc/index.html), [Hoard](http://www.hoard.org/)...) or just simply __`increase system memory`__ :-)!


- Lesson learned:

Especially for data intensive application, before deploying to production, don't forget to test it fully to avoid get on-called at night due to OOM issue. No memory leak doesn't mean no memory hog.

