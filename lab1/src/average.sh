#!/bin/bash

sum = 0
while read arg
do
sum=$(( $sum + $arg ))
count=$(( $count + 1))
done
average=$(bc<<<"scale=2;$sum/$count")
echo "sum $sum"
echo "count $count"
echo "average $average"