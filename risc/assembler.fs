\ Rudimentary assembler (25Jul2022).

vocabulary assembler
also assembler  definitions

\ Comment. -----
: '  ['] s" execute  s" // " type type cr ;

\ Register indexes.
0 constant rACC
1 constant rIP
2 constant r1
3 constant r2
4 constant r3
5 constant r4
6 constant r5
7 constant r6
8 constant r7
9 constant r8
10 constant rSP
11 constant rFP

\ Instruction encoding. -----

\ Encode byte.
: byte.  hex s" CPU->ram[i++] = 0x" type  u.  s" ;" type  cr decimal ;
\ : byte.  hex u. decimal ;
\ Encode machine word. -----
: encode-word  dup  $ff and byte.  8 rshift  $ff and byte. ;

\ Instructions. -----

: movlr  $10 byte.  swap  encode-word  byte.  s" //movlr" type cr ;
: movrr  $11 byte.  swap  byte. byte.  s" //movrr" type cr ;
: movrm  $12 byte.  swap  byte.  encode-word  s" //movrm" type cr ;
: movmr  $13 byte.  swap  encode-word  byte.  s" //movmr" type cr ;
: addrr  $14 byte.  swap  byte. byte.  s" //addrr" type cr ;
: hlt    $15 byte.  s" //hlt" type cr ;
: jne    $16 byte.  swap encode-word encode-word  s" //jne" type cr ;
: pushl  $17 byte.  encode-word  s" //pushl" type cr ;
: pushr  $18 byte.  byte.  s" //pushr" type cr ;
: pop    $19 byte.  byte.  s" //pop" type cr ;
: calll  $1a byte.  encode-word  s" //calll" type cr ;
: callr  $1b byte.  byte.  s" //callr" type cr ;
: ret    $1c byte.  s" //ret" type cr ;
cr cr

\ End. -----

(
$3fff r1 movlr
666 r7 movlr
r1 r7 movrr
r1 $1234 movrm
$abcd r2 movmr
r4 r5 addrr
9 4096 jne
255 pushl
r5 pushr
r6 pop
$100 calll
r1 callr
ret
hlt
)

$3333 pushl
$2222 pushl
$1111 pushl
$1234 r1 movlr
$5678 r4 movlr

$0000 pushl \ args count
$3000 calll
$4444 pushl

' my_subrutine
$0102 pushl
$0304 pushl
$0506 pushl
$0708 r1 movlr
$090a r8 movlr
ret

( char X r1 movlr
r1 $3000 movrm
hlt )

cr



