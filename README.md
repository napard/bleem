# Bleem

Bleem will be a fantasy but serious computer. It's a simple 32-bit RISC machine.
It borrows concepts from retro computers like the Commodore64 and more modern aproaches like the Oberon RISC machine among others.

It will eventually have a default high level programming language that's a mix between Oberon-07, BASIC and Megezeux Robotic (no, it's not a joke ¯\\_(ツ)_/¯).

But first of all, Bleem is a personal vision and framework to experiment with low-fat computing and gaming.

It is by no means an attempt to emulate a possible hardware implementation, so, it's serious, but as serious as it can be. There are possibly many aspects that are not viable in a real implementation, I'm not a hardware expert.

-----

## Specifications
</br>

### CPU

* Fantasy 32-bit RISC machine.
* 8 general purpose registers.
* Special registers: instruction pointer, stack pointer, frame pointer.
* Status register: N, C, V, Z
* [ ] Interrupt mechanism support.

### Memory

* 1MB flat addresable memory space.
* Memory mapped IO with simple access abstraction.
* No MMU, no memory protection.

### Video output

- [X] Currently uses SDL2 for video output.
* Video mode 1:
    - [ ] 40x25 tiled console.
    - [ ] 8x8 pixel character.
    - [ ] 3 editable character sets.
    - [ ] Selectable foreground and background color per tile.
* Video mode 2:
    - [ ] Resolution: 320x200.
    - [ ] Color depth: 256 colors, pixel format RRRGGGBB.
