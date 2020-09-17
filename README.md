# ihexcrc
## CRC32 for Intellec Hex files
![C/C++ CI](https://github.com/knilch0r/ihexcrc/workflows/C/C++%20CI/badge.svg)

## What it does

This tool parses an [Intellec Hex](https://en.wikipedia.org/wiki/Intel_HEX) file line by line and generates
a [CRC-32](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) checksum over all data records.

Automotive-grade software deployment relies on such a CRC mechanism, so you might want to use it for
input files of automotive-grade software deployment tools.

## Usage

```
ihexcrc < myfile.hex > myfile.crc
```

## Expert usage

```
ihexcrc -v < myfile.hex
```
prints verbose output for every line that is ignored because it is not a data record.

## Building from source

Clone the repository, then run
```
make
```

## Running the test suite

```
make test
```

## License

GPL-2. See the file LICENSE.
