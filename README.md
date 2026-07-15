# Opcoder

![Language](https://img.shields.io/badge/Language-C%20%20ASM--x64-blue)
![License](https://img.shields.io/github/license/lina-torovoltas/Opcoder)
![GitHub last commit](https://img.shields.io/github/last-commit/lina-torovoltas/Opcoder)
![GitHub release](https://img.shields.io/github/v/release/lina-torovoltas/Opcoder)</br>


Opcoder is a tool for parsing and executing custom opcode files.</br>
It provides JIT execution, binary extraction, and opcode disassembly.


## Usage

Opcoder accepts `.opc` files as input.

```bash
$ ./opcoder input.opc -e              # execute in memory
$ ./opcoder input.opc -d              # execute + dump register state
$ ./opcoder input.opc -o output.bin   # save parsed bytecode as binary
$ ./opcoder input.opc -a output.dasm  # disassembly with addresses/bytes
$ ./opcoder input.opc -ao output.dasm # plain disassembly (mnemonics only)
```

Options can be combined:
```bash
$ ./opcoder print.opc -d -o print.bin -ao print.dasm
# executes, dumps registers, saves binary, and generates plain disassembly
```


## Build dependencies

- `make` utility for building
- `gcc` or `tcc` compiler for Linux
- `clang` and `mingw-w64` for Windows cross-compilation


## Building

1. Clone the repository and navigate to the project folder.
2. Install the `capstone` library.
3. Run `make` to build.


## Contributing

Contributions are welcome!</br>
If thou hast found a bug or wishest to propose an improvement,</br>
feel free to open an issue or submit a pull request.

***
Developed by <a href="https://github.com/lina-torovoltas" style="color:#ff4f00">Lina Torovoltas</a> — © 2026 All rights reserved.
