#!/bin/sh
set -v
# wrong type
echo 'abcde' | cargo run
echo '#' | cargo run
# empty line
echo '' | cargo run
# broken reclen
echo ':0' | cargo run
echo ':x' | cargo run
echo ':0x' | cargo run
# broken addr (ignored)
echo ':00xxxx' | cargo run
# broken rectype
echo ':001111' | cargo run
echo ':001111x' | cargo run
echo ':0011110x' | cargo run
# non-null rectype
echo ':00111101' | cargo run
echo ':00111104' | cargo run
echo ':001111FF' | cargo run
# broken data
echo ':01FFFF00' | cargo run
echo ':01FFFF00x' | cargo run
echo ':01FFFF00Fx' | cargo run
echo ':0AFFFF00112233445566778899' | cargo run
echo ':0AFFFF00112233445566778899x' | cargo run
echo ':0AFFFF00112233445566778899Ax' | cargo run

