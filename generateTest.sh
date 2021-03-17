if [ -z "$1" ]
  then
    echo "No argument supplied"
    return 1;
fi
rm -rf data
mkdir -p data
cd data || return 1;

for ((i = 1 ; i <= $1 ; i++))
do
  python ../generator.py -f $i.txt -c $((i * 1000))
done