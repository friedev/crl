# croguelike

A basic roguelike implemented in C using ncurses, intended to serve as a template for more interesting roguelikes.

Very early in development; see TODO.md for planned features.

## Dependencies

- C99
- Make
- ncurses

## Installation

Build locally:

```sh
make
```

Install globally (run as root):

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

If you want to submit a patch, please follow these guidelines:

- Run the project to test for bugs.

## License

croguelike is licensed under the GNU Affero General Public License 3.0.
See LICENSE.txt for the full license text.
