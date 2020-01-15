**Deviations
#Team 11

Each allocate call creates a new object_info_t which takes up 32 bytes of memory
. This does not follow the function specifications requiring the overhead to be a maximum of 8 bytes for each allocation. Unfortunately we had no ideas regarding how to fix this as a single pointer in c takes up 8 bytes of memory and there was no way for us to create this library without at least 2 pointers and some variable to hold the reference counter.

Apart from this we do not believe that our program deviates from the project requirements.

