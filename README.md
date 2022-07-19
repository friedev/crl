# croguelike

A basic roguelike implemented in C using termbox2, intended to serve as a template for more interesting roguelikes.

Very early in development; see TODO.md for planned features.

## Dependencies

- C99
- Make
- termbox2 (installed globally, i.e. `make install`)

## Installation

Build locally:

```sh
make
```

Install globally (as root):

```sh
make install
```

## Usage

```sh
./roguelike
```

croguelike accepts no command line arguments.

## Troubleshooting

If incremental code changes are causing include errors, try running `make clean` to delete the generated `*.d` files.

## Contributing

Patches are welcome, but if you want to add any major features specific to a particular game, please create a fork.

## License

croguelike is licensed under the GNU Affero General Public License 3.0.
See LICENSE.txt for the full license text.
