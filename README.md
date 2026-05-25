# crl

Unfinished prototype of a traditional roguelike game using C and [termbox2](https://github.com/termbox/termbox2),

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
crl
```

crl accepts no command line arguments.

### Controls

- Move: `hjklyubn`
- Wait: `.`
- Pick up item: `g`
- Drop item: `d`

## Development

### Troubleshooting

If incremental code changes are causing include errors, try running `make clean` to delete the generated `*.d` files.

## License

crl is licensed under the MIT License.
See [LICENSE.txt](LICENSE.txt) for the full license text.
