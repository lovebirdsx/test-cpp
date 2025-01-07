find src tests | entr -c sh -c '
    start=$(date +%s%3N)
    cmake -S . -B build && cmake --build build && ./build/tests/TestApp
    end=$(date +%s%3N)
    echo "Elapsed time: $((end - start)) ms"
'