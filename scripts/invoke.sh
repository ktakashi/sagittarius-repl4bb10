echo Invoking sash
LD_LIBRARY_PATH=./app/native/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
./app/native/lib/sash
