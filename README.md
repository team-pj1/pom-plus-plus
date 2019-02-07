# pom++

## Summary

This is a C++ pommerman port. It is **incompatible with [PyPommerman](https://github.com/MultiAgentLearning/playground)**.
It is a header-only library, and written with C++11 in mind however may work with legacy versions.

## Structure

- main.cpp: This holds example code for a basic environment
- include/pommerman: The directory holding the code for the project

## Incomplete Features

- Python Bindings
- Radio (Optional)
- Match Recording

## Known Bugs

- On some systems, the program only works when build for `x64`. If built for `Win32` it has problems with `<random.h>`'s methods or fails to obtain input using `__getch()`.
- If partial observation are used then it crashes on agent movement

## Incompatibilities

|    Difference    |     pom++      | PyPommerman |
| :--------------: | :------------: | :---------: |
|  Type of Model   |   Turn-based   | Step-based  |
|    Bomb data     |     Vector     |   Matrix    |
|    AgentDummy    |     Absent     |   Present   |
| Board Generation | Legality-check |  Recursive  |
|    Game Mode     | Parameterized  |   Preset    |

## Changelog

### v1.2.1

- Change `pom::Random` from pointer to object in `pom::forward_model`
- Remove `step_count` from `pom::Stats`
- Fix Agent Effector Bug
- Improve Benchmark to be more accurate

### v1.2

- RNG changed from `std::uniform_int_distribution` to modular
- Added support partial observations
- Fixed bomb kicking implementation
- Fixed flame-looping bug
- Added support for `/=` and `-=` operators to `pom::Coordinate`
- Added support for `[]` operator to `pom::board`
- Added support for `fill()` function to `pom::Matrix`
- Aligned `pom::direction_offset` to `pom::Actions`
- Added support to `keyboard_agent` for `unix` (Using `ncurses`)
- Added `print_agent` for agent_observation testing
