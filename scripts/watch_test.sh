cmake -S . -B build
find src tests | entr -c sh -c '
    start=$(date +%s%3N)
    cmake --build build --target TestApp && ./build/tests/TestApp
    end=$(date +%s%3N)
    echo "Elapsed time: $((end - start)) ms"
'