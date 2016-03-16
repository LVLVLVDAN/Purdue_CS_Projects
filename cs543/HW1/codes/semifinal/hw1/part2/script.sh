#!/bin/bash
echo "1234567" > seed.txt
for number in {1..10}
do
./part2
echo $number
number=$((number+1))
done
