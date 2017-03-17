set terminal pdf
set output "matrixMisspredictions.pdf"
set xlabel "number of elements"
set ylabel "branch miss predictions"

plot "matrix.data" using 2:($1 ==1  ? $14 : 1/0) with linespoints pointtype 7 title "iterative", \
     "matrix.data" using 2:($1 ==2  ? $14 : 1/0) with linespoints pointtype 7 title "recursive",\
     "matrix.data" using 2:($1 ==3  ? $14 : 1/0) with linespoints pointtype 7 title "transpose"

set output "matrixRuntime.pdf"
set xlabel "number of elements"
set ylabel "execution time (s)"

plot "matrix.data" using 2:($1 ==1  ? $3 : 1/0) with linespoints pointtype 7 title "iterative", \
     "matrix.data" using 2:($1 ==2  ? $3 : 1/0) with linespoints pointtype 7 title "recursive",\
     "matrix.data" using 2:($1 ==3  ? $3 : 1/0) with linespoints pointtype 7 title "transpose"

set output "matrixCacheMiss.pdf"
set xlabel "number of elements"
set ylabel "D1 read cache misses"

plot "matrix.data" using 2:($1 ==1  ? $8 : 1/0) with linespoints pointtype 7 title "iterative", \
     "matrix.data" using 2:($1 ==2  ? $8 : 1/0) with linespoints pointtype 7 title "recursive",\
     "matrix.data" using 2:($1 ==3  ? $8 : 1/0) with linespoints pointtype 7 title "transpose"

set output "matrixInstructions.pdf"
set xlabel "number of elements"
set ylabel "number of instructions"

plot "matrix.data" using 2:($1 ==1  ? $4 : 1/0) with linespoints pointtype 7 title "iterative", \
     "matrix.data" using 2:($1 ==2  ? $4 : 1/0) with linespoints pointtype 7 title "recursive",\
     "matrix.data" using 2:($1 ==3  ? $4 : 1/0) with linespoints pointtype 7 title "transpose"

set output "matrixDataReads.pdf"
set xlabel "number of elements"
set ylabel "number of Data Reads"

plot "matrix.data" using 2:($1 ==1  ? $5 : 1/0) with linespoints pointtype 7 title "iterative", \
     "matrix.data" using 2:($1 ==2  ? $5 : 1/0) with linespoints pointtype 7 title "recursive",\
     "matrix.data" using 2:($1 ==3  ? $5 : 1/0) with linespoints pointtype 7 title "transpose"

set output "matrixDataWrites.pdf"
set xlabel "number of elements"
set ylabel "number of Data Writes"

plot "matrix.data" using 2:($1 ==1  ? $6 : 1/0) with linespoints pointtype 7 title "iterative", \
     "matrix.data" using 2:($1 ==2  ? $6 : 1/0) with linespoints pointtype 7 title "recursive",\
     "matrix.data" using 2:($1 ==3  ? $6 : 1/0) with linespoints pointtype 7 title "transpose"
