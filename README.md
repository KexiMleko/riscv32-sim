# riscv32-sim

A cycle-accurate RV32I RISC-V simulator designed for visualizing microarchitectural state and hazard behavior. This project aims to implement a modular pipeline to study advanced architectural features, including out-of-order execution and speculative branch prediction.

## Implementation Status

| Feature | Status | Description |
| --- | --- | --- |
| **RV32I ISA** | Functional | Base integer instruction set support. |
| **5-Stage Pipeline** | Functional | Separate IF, ID, EX, MEM, and WB stages. |
| **ALU Forwarding** | Functional | MEM→EX and WB→EX forwarding paths eliminate stalls for ALU RAW hazards. |
| **Hazard Detection** | Functional | Detects load-use hazards and stalls the pipeline via `pc_en`, `if_id_en`, and `ctrl_pass` gating. |
| **Branch Prediction** | Functional | 2-bit saturating predictor with BTB and pipeline flushing on misprediction. |
| **Tomasulo (OoO)** | Functional | Standalone out-of-order pipeline with reservation stations, CDB write-back, and load/store buffers for memory ops. |
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
* **Hazard Handling:** ALU RAW hazards are resolved by the forwarding unit (MEM→EX, WB→EX). Load-use hazards are caught by the hazard detection unit, which stalls IF/ID and injects a bubble into ID/EX for one cycle. Control hazards are handled by the branch predictor with flush-on-misprediction.
* **Execution Models:** Two selectable pipelines share a common front-end and decoder — a classic in-order 5-stage pipeline, and a Tomasulo-based out-of-order pipeline with reservation stations, a common data bus, and load/store buffers.
