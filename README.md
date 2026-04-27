Project 3 – Pthreads Programming

Assignment 1:
- Implemented parallel substring search using Pthreads
- Used mutex to safely update global count

Assignment 2:
- Implemented producer-consumer problem
- Used condition variables and circular buffer (size = 5)

Assignment 3:
- Optimized list-forming program
- Reduced locking from K times to 1 time per thread
- Achieved better performance with larger K values

Performance Results:
K = 100000

4 threads:
Original: 30566 µs
Optimized: 15966 µs

8 threads:
Original: 81669 µs
Optimized: 32127 µs
