set terminal postscript eps enhanced color font "Helvetica, 16"
set output 'burst_read.eps'

set grid xtics ytics

set xlabel "cache size"
set ylabel "hits"
set title "Burst read" font "Helvetica Bold, 16"

set xrange[0:305]
set yrange[0:8000]
plot "burst_read.txt" using 1:2 title "Belady's algorithm" with lines lc rgb "green", \
		 "burst_read.txt" using 1:2 notitle with points, \
		 "burst_read.txt" using 1:3 title "LIRS" with lines lc rgb "red", \
		 "burst_read.txt" using 1:3 notitle with points


set output "loop_read.eps"

set grid xtics ytics

set xlabel "cache size"
set ylabel "hits"
set title "Loop read" font "Helvetica Bold, 16"

set xrange[0:305]
set yrange[0:8000]
plot "loop_read.txt" using 1:2 title "Belady's algorithm" with lines lc rgb "green", \
		 "loop_read.txt" using 1:2 notitle with points, \
		 "loop_read.txt" using 1:3 title "LIRS" with lines lc rgb "red", \
		 "loop_read.txt" using 1:3 notitle with points
