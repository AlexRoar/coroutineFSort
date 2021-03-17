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
sh generateTest.sh $2
sh sort.sh $1 $2
sh checkTest.sh