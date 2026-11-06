#!/bin/sh
set -eu
cd "$(dirname "$0")"
make
output=$(mktemp)
trap 'rm -f "$output"' EXIT HUP INT TERM
for input in tests_cases/*.in; do
    ./main < "$input" > "$output" 2>/dev/null
    diff -u "${input%.in}.out" "$output"
    printf 'PASS %s\n' "$input"
done
