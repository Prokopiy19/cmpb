## Dependencies

Building requires libcurl and nlohmann-json.

## Building

Run following commands to build the library and program:

```sh
./configure
make
make install
```

## Using the library

To link the program with the library, you can use pkg-config:

```sh
g++ main.cpp `pkg-config libcmpb --cflags --libs`
```

## Running

Run the program:

    dptest {branch} {branch}

where {branch} is a branch name.

For example, you can enter this:

    dptest sisyphus p10

Program will generate .json files with names 'results_{arch}.json' for each architecture.
