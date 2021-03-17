if [ -z "$1" ]
  then
    echo "No latency supplied"
    return 1;
fi

if [ -z "$2" ]
  then
    echo "No files num supplied"
    return 1;
fi

ARGS="$1"
for ((i = 1 ; i <= $2 ; i++))
do
  ARGS="$ARGS data/$i.txt"
done
echo $ARGS
build/coSort $ARGS
