# riscv32-sim

A cycle-accurate RV32I RISC-V simulator designed for visualizing microarchitectural state and hazard behavior. This project aims to implement a modular pipeline to study advanced architectural features, including out-of-order execution and speculative branch prediction.

## Implementation Status

| Feature | Status | Description |
| --- | --- | --- |
| **RV32I ISA** | Partial | Base integer instruction set support. |
| **5-Stage Pipeline** | Functional | Separate IF, ID, EX, MEM, and WB stages. |
| **Hazard Handling** | Manual | Requires `NOP` instructions in source assembly for RAW/Control hazards. |
| **Scoreboard** | In-Dev | Tracking register status for Out-of-Order execution. |
| **Branch Prediction** | In-Dev | Speculative execution and pipeline flushing logic. |
| **Cycle Accuracy** | Goal | Currently 1-cycle-per-stage; multi-cycle latency modeling is planned. |

## Project Structure

* `src/`: C source files for the simulator core and modular components.
* `build/`: Build artifacts, object files, and the final binary.
* `tests/data/src/`: RISC-V assembly files used for verification.

## Building the Project

The project uses a Makefile to manage the build process. To compile the simulator:

```bash
make
```

The executable `rvsim` will be generated in the `build/` directory.

## Usage

The simulator executes instructions by loading them directly into a simulated memory space. It accepts two primary input formats: raw RISC-V assembly files (`.s`) or binary files (`.bin`) containing a flat text segment of instructions.

When an assembly file is provided, the simulator uses the cross-compiler toolchain defined in the source (e.g., `riscv64-elf-gcc` or `riscv-none-elf-gcc`) to assemble and extract the raw machine code before loading and disassembling it for execution.

### Running the Simulator

Execute the binary from the project root using the following syntax:

```bash
./build/rvsim <path_to_file>
```

**Example with Assembly:**

```bash
./build/rvsim tests/data/src/branch.s
```

**Example with Binary:**

```bash
./build/rvsim tests/data/bin/add1.bin
```

### Toolchain Requirements

To process assembly files, the simulator requires a RISC-V cross-compiler available in your system path. The internal logic automatically selects the appropriate commands based on your OS:

* **Linux/macOS:** Uses `riscv64-elf-gcc` and `riscv64-elf-objcopy`.
* **Windows:** Uses `riscv-none-elf-gcc` and `riscv-none-elf-objcopy`.

## Testing

To run a specific assembly test case, use the `TSRC` (test source) variable with `make test`. This command automates the build and execution of a source file located in `./tests/data/src/`.

**Example:**

```bash
make test TSRC=branch.s
```

**Note on Verification:**
Testing is currently a manual process. The `make` command only handles execution; you must manually compare the simulator's output against your expected results to verify architectural correctness and pipeline behavior.

To clean the build directory:

```bash
make clean
```

## Technical Specification

* **ISA:** RV32I (Base Integer Instruction Set).
* **Hazard Handling:** Currently requires manual NOP insertion in the source assembly/instructions. Hardware-level detection and pipeline stalling are under development.
* **Execution Model:** 5-stage pipeline with a standalone decoder. Out-of-order Scoreboard system, branch prediction and hazard analysis are in development.
