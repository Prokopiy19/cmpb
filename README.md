## Building

Run following commands to build library and program:

```sh
cd library
make
cd ../CLI
make
```

## Running

First, make sure you are in directory CLI. Then run the program:

    ./prog {branch} {branch}

where {branch} is a branch name.

For example, you can enter this:

    ./prog sisyphus p10

Program will generate .json files with names 'results_{arch}.json' for each architecture.
