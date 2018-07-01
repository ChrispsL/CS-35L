Single-thread
real    0m59.918s
user    0m59.753s
sys     0m0.005s
2-threads
real    0m33.909s
user    1m7.753s
sys     0m0.012s
4-threads
real    0m18.565s
user    1m12.020s
sys     0m0.005s
8-threads
real    0m11.791s
user    1m19.247s
sys     0m0.007s

Parallel/multi-threading makes the ray tracing much faster.
Though, user time goes up because of overhead such as creating threads.
The performance increase isn't perfectly double.

A couple issues I had for awhile was outputting the correct values (scaled_color), but I ended up storing all the values in a 3-dimensional array.
Also, when trying to make the scene a global variable, I forgot to change the declaration of it in main to just an assignment 
	(i.e. main() was creating another scene that was not being used by the parallel function).
