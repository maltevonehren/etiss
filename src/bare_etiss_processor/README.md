# Bare-Metal Simulation

This folder contains a basic example project that uses ETISS.

The target software that should be simulated must firstly be build for the core.
ETISS has example target software, see [/examples/SW/README.md](/examples/SW/README.md) for build instructions.

When the simulation is running,
ETISS will translate the compiled binary code into host executable code
and simulate the behavior of target bare-metal core.

## Getting Started

When using example software from ETISS, make sure it is being build
(see [/examples/SW/README.md](/examples/SW/README.md)).

When building your own target software, build the target software for the target architecture
and define a configuration file with the details of the core to be simulated
(memory layout, enabled plugins, etc.).
See `ETISS.ini` for details of the configuration file.

### LINUX SYSTEM

The easiest way to execute the example processor is using the `run_helper.sh` script located in `<etiss_install_dir>/bin/run_helper.sh`.
This script adds environment variables and starts the main executable.
It can also be used to easily change configuration such as logger verbosity level.
Executing the run_helper script with no arguments prints the usage information.

Execute the `hello_world` example for RISC-V (rv32gc) with the following command from inside `<etiss_install_dir>`:

	$ bin/run_helper.sh examples/SW/rv32gc/bin/hello_world config examples/SW/rv32gc/etc/memsegs.ini 

See [Snapshots](#snapshot) for example output.

`memsegs.ini` is passed as an additional configuration file. It contains details specific to the RISC-V examples (memory layout etc.).

### WINDOWS SYSTEM

Set the `PATH` variable to include files in the `<etiss_install_dir>/lib` folder, so that `ETISS.dll` will be found.

	$ $Env:Path="<etiss_install_dir>/lib;"+$Env:Path

When using bash on windows, follow the same steps is in [LINUX SYSTEM](#linux-system).

If you are not able to execute shell scripts in windows, you will have to pass the arguments manually:

	$ main --vp.elf_file=examples/SW/rv32gc/bin/hello_world -iexamples/base.ini -iexamples/SW/rv32gc/etc/memsegs.ini --jit.type=TCCJIT --etiss.loglevel=4

## Debugging

### Debug ETISS

	$ ./run_helper.sh <path_to_target_elf> gdb

### Debug target software

A **gdbserver** is integrated in ETISS, and can be used to debug the target
core when it runs target software.
The gdbserver can be enabled by passing `tgdb` to `run_helper.sh`
Or adding these options to a configuration file:

	[Plugin gdbserver];
		plugin.gdbserver.port=<port_number_you_defined>
		minPcAlign=<minimal_instruction_bits_alignment>

The gdbserver surveils underlying target software, send/receive commands
to/from target core. The ETISS simulation will be suspended at the
beginning of target software, waiting for incoming command, if **gdbserver**
is enabled.

#### Use gdbserver

Short version:

	$ ./run_helper.sh <path_to_target_elf> tgdb

Requires KDE "Konsole", but add option "noattach" to attach to the gdbserver manually (see below).

Long version:

Run the ETISS (main) using aforementioned script, then invoke corresponding
gdb in a new terminal. E.g. or1k-elf-gdb for or1k core. **Do not use system
default gdb directly for target core.** The target SW executable should be
given as an argument or given after entering gdb and using file command.
The executable is used to load symbol table. For example:

	$ <target_toolchain>-elf-gdb <SW_directory>/<target_SW>.elf

alternative:

	$ <target_toolchain>-elf-gdb
	$ (gdb) file <SW_directory>/<target_SW>.elf

Then you should connect gdb to gdbserver inside ETISS

	$ (gdb) target remote :<port_number_you_defined>  //local computer

or:

	$ (gdb) target remote <host_ip>:<port_number_you_defined>  // connect through internet to remote computer

As connection between gdb and gdbserver is established, standard gdb command
could be used to debug target SW.

  > RSP protocol commands (internal gdb commands) such as 'g', 'G'
  for read/write all registers, are invisible for gdb user. These commands
  cannot be recognized by gdb.

#### Using an IDE for debugging

As long as the IDE supports remote debugging with GDB, it will be able to connect to the GDBServer of ETISS.

<details><summary>Eclipse</summary>
<p>

For example use the following steps to debug with Eclipse (https://stackoverflow.com/a/45608937):

Setup:

- Get "Eclipse IDE for C/C++ Developers"
- Open Eclipse
- Menu: Run -> Debug Configurations...
- Select: C/C++ Remote Application
- Press "New Configuration" button on top
- At the bottom click on "Select other..."
- Check: Use configuration specific settings
- Select: GDB (DSF) Manual Remote Debugging Launcher
- Main tab: Enter C/C++ Application path
- Debugger tab -> Main sub-tab: Enter toolchain GDB path into "GDB debugger"
- Debugger tab -> Connection sub-tab: TCP, localhost, 2222
- Click Apply, Close

Usage:

- Run ETISS with GDBServer enabled
- Menu: Run -> Debug

</p>
</details>

<details><summary>Visual Studio Code</summary>
<p>

Create a new debugging configuration for your project by going to the debug side-view on the left. Click on the drop-down, select "Add Config (your project name)", select environment "C++ (GDB/LLDB)". Adjust the file that opens like the following:

	{
		// Use IntelliSense to learn about possible attributes.
		// Hover to view descriptions of existing attributes.
		// For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
		"version": "0.2.0",
		"configurations": [
			{
				"name": "ETISS Debug",
				"type": "cppdbg",
				"request": "launch",
				"program": "${workspaceFolder}/build/installed/examples/rv32gc/bin/hello_world",
				"miDebuggerServerAddress": "localhost:2222",
				"args": [],
				"stopAtEntry": false,
				"cwd": "${workspaceFolder}",
				"environment": [],
				"externalConsole": false,
				"MIMode": "gdb",
				"setupCommands": [
					{
						"description": "Enable pretty-printing for gdb",
						"text": "-enable-pretty-printing",
						"ignoreFailures": true
					}
				],
				"miDebuggerPath": "/usr/local/research/projects/SystemDesign/tools/riscv/current/bin/riscv32-unknown-elf-gdb"
			}
		]
	}

</p>
</details>

## Snapshot

The output during execution should look similar to this:

<details><summary>Click to expand</summary>
<p>

### `test_cases` output

Command used on Linux: 

	$ bin/run_helper.sh examples/SW/rv32gc/bin/test_cases config examples/SW/rv32gc/etc/memsegs.ini 

```
=== Setting up configurations ===
Initializer::loadIni(): Ini sucessfully loaded /home/malte/dev/etiss/build/installed/examples/base.ini
Initializer::loadIni(): Ini sucessfully loaded /tmp/etiss_dynamic_QXmliM5E12.ini
Initializer::loadIni(): Ini sucessfully loaded examples/SW/rv32gc/etc/memsegs.ini
  Load Configs from .ini files:
ETISS: Info: Created new config container: global
ETISS: Info:   [BoolConfigurations]
ETISS: Info:     arch.enable_semihosting=true,
ETISS: Info:     arch.or1k.ignore_sr_iee=false,
ETISS: Info:     etiss.enable_dmi=true,
ETISS: Info:     etiss.load_integrated_libraries=true,
ETISS: Info:     etiss.log_pc=false,
ETISS: Info:     jit.debug=false,
ETISS: Info:     jit.gcc.cleanup=true,
ETISS: Info:     jit.verify=false,
ETISS: Info:     testing=false,
ETISS: Info:   [IntConfigurations]
ETISS: Info:     arch.cpu_cycle_time_ps=31250 ; 32 MHz,
ETISS: Info:     arch.or1k.if_stall_cycles=0,
ETISS: Info:     etiss.max_block_size=100,
ETISS: Info:     ETISS::CPU_quantum_ps=100000,
ETISS: Info:     ETISS::sim_mode=0,
ETISS: Info:     ETISS::write_pc_trace_from_time_us=0,
ETISS: Info:     ETISS::write_pc_trace_until_time_us=3000000,
ETISS: Info:     simple_mem_system.memseg_length_00=0x00400000,
ETISS: Info:     simple_mem_system.memseg_length_01=0x00100000,
ETISS: Info:     simple_mem_system.memseg_origin_00=0x10000000,
ETISS: Info:     simple_mem_system.memseg_origin_01=0x20000000,
ETISS: Info:     vp::simulation_time_us=20000000,
ETISS: Info:   [StringConfigurations]
ETISS: Info:     etiss.output_path_prefix=,
ETISS: Info:     jit.type=TCCJIT,
ETISS: Info:     vp.elf_file=examples/SW/rv32gc/bin/hello_world,
=== Finished setting up configurations ===

=== Setting up test system ===
  Setting up Memory
ETISS: Info: New Memory segment added: 1 - [0x00000000103fffff - 0x0000000010000000]
ETISS: Info: New Memory segment added: 2 - [0x00000000200fffff - 0x0000000020000000]
ETISS: Info: Set ETISS architecture to RV32IMACFD as specified in ELF-file.
ETISS: Info: Initialized the memory segment 0 - RWX[0x0000000010000000 - 0x000000001000f9d7] from ELF-file
ETISS: Info: Initialized the memory segment 1 - RW[0x0000000020000000 - 0x0000000020000a97] from ELF-file
  Setting up CPUCore
  CPU start address: 0x10002500
=== Finished Setting up test system ===

=== Setting up plug-ins ===
ETISS: Info:  Adding JIT "TCCJIT"
=== Setting up plug-ins ===


=== Simulation start ===
Hello world!
CPU Time: 6.9375e-05s    Simulation Time: 0.193657s
CPU Cycles (estimated): 2220
MIPS (estimated): 0.0114636
=== Simulation end ===

CPU0 exited with exception: 0x80000000: Finished cpu execution. This is the proper way to exit from etiss::CPUCore::execute.
```

### GDB output in terminal
When utilizing **gdbserver** to debug target software.

```
GNU gdb (SiFive GDB-Metal 10.1.0-2020.12.7) 10.1
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "--host=x86_64-linux-gnu --target=riscv64-unknown-elf".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://github.com/sifive/freedom-tools/issues>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from examples/SW/rv32gc/bin/test_cases...
Remote debugging using :2222
0x1000db80 in _start ()
(gdb) b main
Breakpoint 1 at 0x10000068
(gdb) c
Continuing.

Breakpoint 1, 0x10000068 in main ()
(gdb) info registers    
ra             0x1000dbc6       0x1000dbc6 <_start+70>
sp             0x200fffe0       0x200fffe0
gp             0x20004df4       0x20004df4 <obuf+1652>
tp             0x0      0x0
t0             0x10007d60       268467552
t1             0xf      15
t2             0x0      0
fp             0x20100000       0x20100000
s1             0x0      0
a0             0x10011010       268505104
a1             0x0      0
a2             0x0      0
a3             0xffffffff       -1
a4             0xffffffff       -1
a5             0x10011000       268505088
a6             0x2      2
a7             0x0      0
s2             0x0      0
s3             0x0      0
s4             0x0      0
s5             0x0      0
s6             0x0      0
s7             0x0      0
s8             0x0      0
s9             0x0      0
s10            0x0      0
s11            0x0      0
t3             0x0      0
t4             0x0      0
t5             0x0      0
t6             0x0      0
pc             0x10000068       0x10000068 <main+24>
(gdb) c
Continuing.
Remote connection closed
(gdb) 
```


</p>
</details>

