if [ $# -ne 1 ]
then
    echo "`basename $0` DIR"
    exit 1
fi

DIR=$1
if [ ! -d $DIR ]
then
    echo "\"$DIR\" is not a directory"
    exit 1
fi

cd $DIR
LAST=./.changes-timestamp
if [ -e $LAST ]
then
    T=`cat $LAST`
else
    T=`  git log --reverse --date=iso-strict \
                 --format=%ad -- . \
       | head -1`
fi

date +%FT%T > $LAST

git diff -r HEAD@{$T} -- .