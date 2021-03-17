cd data || return 1;

for i in 1 2 3 4 5 6 7 8
do
  python ../generator.py -f $i.txt -c $((i * 1000))
done