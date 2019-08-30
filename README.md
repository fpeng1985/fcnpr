<h1>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
<img src="https://gitlab.com/formigoni-ufv/lesc/ropper/raw/master/logo-ropper.png" alt="Ropper-logo" width="128"/>
<br><br>
&emsp;&emsp;&emsp;&emsp;&emsp;
Ropper - A Placement and Routing tool written in C++
</h1>

## About
This tool is inspired by [Dr. Mathias Soeken's work](https://msoeken.github.io/index.html).

The work was split into several submodules, the public ones can be
accessed throught this [link](https://gitlab.com/formigoni).

Our work was published in the *32nd Symposium on Integrated Circuits and
Systems Design*. In case you would like to make a citation, please use
the latex code below, thank you.

```latex
@article{ropper,
author = {Ruan Evangelista Formigoni and Ricardo S. Ferreira and Jos\'{e} Augusto M. Nacif},
year = {2019},
month = {7},
title = {Ropper: A Placement and Routing Framework for Field-Coupled Nanotechnologies},
journal = {32nd Symposium on Integrated Circuits and Systems Design (SBCCI '19), August 26--30, 2019, Sao Paulo, Brazil},
doi = {10.1145/3338852.3339838}
}
```

## Installation and Usage

### Compiling

It is possible to compile with one of two UIs or just compile the software as a library:
#### UI (args): This option introduces the usage shown below:
```shell
build/main -i circuits/C17.v -o output.json
```

To compile with this UI use the command:
```shell
cmake -D UI=args -H. -Bbuild && cmake --build build
```

#### UI (alice): This option uses the [alice](https://github.com/msoeken/alice) command shell library.
To compile with this UI use the command:
```shell
cmake -D UI=alice -H. -Bbuild && cmake --build build
```

#### Library: Allows the usage as a library.
```shell
cmake -D UI=library -H. -Bbuild && cmake --build build
```

If no interface is specified, the library option is set as the default one.

~~For more detailed installation instructions, usage and configuration read the wiki.~~

## Algorithms

## Statistics

### Test runs

| Circuit   | Gates   | Placement Algorithm    | Routing Algorithm    | Exec. Time                     | Version   |
| :-------: | :-----: | :--------------------: | :-----------------:  | :----------------------------: | :-------: |
| C17       | 11      | Simulated Annealing    | Breadth-First Search | 8.0602 seconds (avg. of 10)    | Alpha-0.1 |
| C17       | 11      | Simulated Annealing    | Dynamic Programming  | 0.935628 seconds (avg. of 100) | Alpha-0.2 |

## Planned Features

- [x] Dynamic field area calculations.
- [ ] ~~Compile-time field distance calculations with *constexpr*.~~
*Template metaprogramming in TBA library*.

- [ ] ~~Algorithms~~
	* Routing
		- [x] Breadth-First Search
		- [x] Dynamic Programming
	* Placement
		- [x] Simulated Annealing
		- [ ] ~~Ant colony optimization~~
*Moved to TBA library*.
		- [ ] ~~Firefly algorithm~~
*Moved to TBA library*.
		- [ ] ~~Particle swarm optimization~~
*Moved to TBA library*.
- [x] Json format P&R output information.
- [ ] ~~Cairo *.png* graphical output.~~ *Using SVG Graphics in TBA library*.

<sub>TBA = To Be Announced</sub>
