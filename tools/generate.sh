DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ls -1 $DIR/include/ | xargs -n 1 -P $(nproc) -I{} -t bash -c "python $DIR/te.py $DIR/include/{} | clang-format-5.0 -style=file > $DIR/../include/rtg/{}"
