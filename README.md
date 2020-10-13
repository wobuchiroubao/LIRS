# LIRS
To accemble project with Belady's cache use "make cache=Beladys" in terminal;  
To accemble with LIRS cache use "make cache=LIRS".

input: cache_size, entry_trace_size, entries themselves (entry_trace_size numbers of int type)  
output: one number (representing number of hits)

example:  
input:  
3  
10  
1 2 3 4 5 3 6 6 2 3  
output (for make cache=Beladys):  
4
output (for make cache=LIRS):  
3
