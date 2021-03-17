if [ -z "$1" ]
  then
    echo "No argument supplied"
    return 1;
fi
cd data || return 1;

for ((i = 1 ; i <= $1 ; i++))
do
  python ../checker.py -f $i.txt
done