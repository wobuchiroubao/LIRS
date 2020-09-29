# LIRS

The "main" file is test_LIRS.cpp.
To accemble project use "g++ test_LIRS.cpp tests.cpp" in terminal.

program's output is two txt-files: burst_read.txt and loop_read.txt - 1st column represents cache's size, 2nd - number of hits for Belady's algorithm, 3rd - hits for LIRS algorithm.
After, you can launch graphs.p (if you've installed gnuplot) with command "gnuplot graphs.p" and get graphs for .txt-files' data.
