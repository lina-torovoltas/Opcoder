# Opcoder

![Language](https://img.shields.io/badge/Language-C%20%20ASM--x64-blue)
![License](https://img.shields.io/github/license/lina-torovoltas/Opcoder)
![GitHub last commit](https://img.shields.io/github/last-commit/lina-torovoltas/Opcoder)
![GitHub release](https://img.shields.io/github/v/release/lina-torovoltas/Opcoder)</br>


Opcoder is a toolchain for working with custom opcode files.</br>
It provides bytecode parsing, JIT execution, binary extraction, and opcode disassembly.

## Usage

Opcoder accepts `.opc` files as input.

Execute code directly in memory:
```bash
opcoder input.opc -e
```

Execute code and display register state after execution:
```bash
opcoder input.opc -d
```

Save parsed bytecode as a binary file:
```bash
opcoder input.opc -o output.bin
```

Generate disassembly with addresses and instruction bytes:
```bash
opcoder input.opc -a output.dasm
```

Generate plain disassembly containing only assembly mnemonics:
```bash
opcoder input.opc -ao output.dasm
```

Options can be combined together<br/>
For example:
```bash
opcoder print.opc -d -o print.bin -ao print.dasm
```

This command will execute the code, display register state after execution, save the binary output, and generate plain disassembly.

## Building

Ensure that `gcc`, `make`, and `capstone` are installed and available in your system PATH.</br>
Clone the repository and build Opcoder using make:

```bash
git clone https://github.com/lina-torovoltas/Opcoder
cd Opcoder
make
```


## Contributing

Contributions are welcome!</br>
If thou hast found a bug or wishest to propose an improvement,</br>
feel free to open an issue or submit a pull request.

***
Developed by <a href="https://github.com/lina-torovoltas" style="color:#ff4f00">Lina Torovoltas</a> — © 2025-2026 All rights reserved.