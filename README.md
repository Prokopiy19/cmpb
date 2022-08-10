## Dependencies

Building requires libcurl and nlohmann-json.

## Building

Run following commands to build the library and program:

```sh
make
make install
```

## Running

Run the program:

    bin/prog {branch} {branch}

where {branch} is a branch name.

For example, you can enter this:

    bin/prog sisyphus p10

Program will generate .json files with names 'results_{arch}.json' for each architecture.
