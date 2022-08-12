\ Rudimentary assembler (25Jul2022).

vocabulary bleem-assembler
also bleem-assembler  definitions

4 constant VMWORD-BYTES  \ Bytes per word.
8192 constant CODE-BYTES \ Size of code buffer.

variable codebuff  CODE-BYTES allot \ Assembly code buffer.
variable origin  0 origin !         \ Origin address.
variable asmip   0 asmip !          \ Assemble address.
variable fwdjmp  0 fwdjmp !         \ Forward jump flag.
variable byteop  0 byteop !         \ Byte length instruction.
: incIp  VMWORD-BYTES asmip +! ;

: vmword,  ( u -- )
  s" // " type  dup dup hex u. decimal s" : " type u. cr
  codebuff  asmip @ +  >r
  dup           $ff and r>   dup 1 + >r  c!
  dup 8  rshift $ff and r>   dup 1 + >r  c!
  dup 16 rshift $ff and r>   dup 1 + >r  c!
      24 rshift $ff and r>               c!
  incIp ;

: byte,  ( u -- )
  codebuff  asmip @ +  c!
  1 asmip +! ;

: vmword!  ( u addr -- )
  >r
  dup           $ff and r>   dup 1 + >r  c!
  dup 8  rshift $ff and r>   dup 1 + >r  c!
  dup 16 rshift $ff and r>   dup 1 + >r  c!
      24 rshift $ff and r>               c! ;

: dc  ( u-vmword-offset -- ) \ Dump code.
  VMWORD-BYTES *  codebuff +  48 dump ; 

: dc2 ( u-offset u org -- )
  s" uint32_t i = " type u.  [char] ; emit cr
  0 do
    dup  codebuff + c@  s" CPU->ram[i++] = " type  u.  [char] ; emit cr
    1+
  loop drop ;

: dc3 ( u-offset u -- )
  0 do
    dup  codebuff + c@  u.  [char] , emit cr
    1+
  loop drop ;

: start-rom
  s" #include <stdint.h>" type cr
  s" uint8_t rom[] = {" type cr ;
: end-rom  s" };" type cr ;

: dd  ( u-vmword-offset -- ) \ Dump data.
  VMWORD-BYTES *  codebuff +  384 dump ; 

\ Comment. -----
: '  ['] s" execute 2drop ;

\ Register indexes.
0 constant %ip
1 constant %r1
2 constant %r2
3 constant %r3
4 constant %r4
5 constant %r5
6 constant %r6
7 constant %r7
8 constant %r8
9 constant %sp
10 constant %fp

\ Directives. -----

: #ORG  origin ! ;
: #LABEL  create  asmip @  origin @ + , does> @ ; 
: #FWD  fwdjmp on ;
: #BYTE  byteop on ;
: #<PATCH  dup dup  asmip @  origin @ +
  swap -  16 lshift
  swap    origin @ -  codebuff + @  $ffffffff and  or
  swap    origin @ -  codebuff +  vmword! ;

variable dtmp
: reverse  ( i*x i -- i*y ) 0 do i roll loop ;
: #DATA8  ( ... u -- )
  #LABEL  dup dtmp !  reverse
  dtmp @  0 do byte, loop cr ;

: #DATA16  ( ... u -- )
  #LABEL  dup dtmp !  reverse
  dtmp @  0 do
    dup  $ff and  byte,
         8 rshift  $ff and  byte,
  loop cr ;

\ Instruction encoding. -----

: encode-op  ;
: encode-reg1  8 lshift $f00 and ;
: encode-reg2  12 lshift $f000 and ;
: encode-reg3  16 lshift $f0000 and ;
: encode-imm16  16 lshift $ffff0000 and ;
: encode-imm20  12 lshift $fffff000 and ;
: 2or or or ;
: 3or or or or ;

\ Instructions. -----

: hlt
  $ff encode-op  vmword, ;

(
: movi16r \ $ffff r1 movi16r
  encode-reg1 swap  encode-imm16 swap  $10 encode-op  2or vmword, ;
)

: movir \ $fffff r1 movi16r
  encode-reg1 swap  encode-imm20 swap  $11 encode-op  2or vmword, ;

: movrr \ r1 r2 movrr
  encode-reg2 swap  encode-reg1 swap  $12 encode-op  2or vmword, ;

: movrm \ r1 $ffff r2 movrm
  encode-reg1 swap  encode-imm16 swap  >r >r encode-reg2 r> r>  $13 encode-op  3or vmword, ;

: movmr \ r1 $ffff r2 movmr
  encode-reg2 swap  encode-imm16 swap  >r >r encode-reg1 r> r>  $14 encode-op  3or vmword, ;

: movr*r \ r1 r2 movr*r
  encode-reg2 swap  encode-reg1 swap
  byteop @  if byteop off  $1c else $1b then  encode-op
  2or vmword, ;

: movrr* \ r1 r2 movrr*
  encode-reg2 swap  encode-reg1 swap
  byteop @  if byteop off  $1e else $1d then  encode-op
  2or vmword, ;

\          ^
\ FIXED ---|

: addrr \ r1 r2 r3 addrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $24 encode-op  3or vmword, ;

: subrr \ r1 r2 r3 subrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $28 encode-op  3or vmword, ;

: incr \ r1 incr
  encode-reg1  $29 encode-op  or vmword, ;

: decr \ r1 decr
  encode-reg1  $2a encode-op  or vmword, ;

: mulrr \ r1 r2 r3 mulrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $2c encode-op  3or vmword, ;

: lshi16r \ $ffff r1 r2 lshi16r
  encode-reg2 swap  encode-reg1  >r >r encode-imm16 r> r>  $30 encode-op  3or vmword, ;

: lshrr \ r1 r2 r3 lshrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $31 encode-op  3or vmword, ;

: rshi16r \ $ffff r1 r2 rshi16r
  encode-reg2 swap  encode-reg1  >r >r encode-imm16 r> r>  $32 encode-op  3or vmword, ;

: rshrr \ r1 r2 r3 rshrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $33 encode-op  3or vmword, ;

: andrr \ r1 r2 r3 andrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $35 encode-op  3or vmword, ;

: orrr \ r1 r2 r3 orrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $37 encode-op  3or vmword, ;

: xorrr \ r1 r2 r3 xorrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $39 encode-op  3or vmword, ;

: notr \ r1 notr
  encode-reg1  $3a encode-op  or vmword, ;

: fwd-label  create  asmip @  origin @ + ,  does> @ ;
: emit-fwd-jmp  ( opc -- )
  fwdjmp @ if
    fwdjmp off
    fwd-label
    0 encode-imm16 encode-op  or vmword,
    r> drop \ Early return from caller.
  else
    drop
  then ;

: jnei16 \ $ffff jnei16
  $30 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $30 encode-op  or vmword, ;

: jei16
  $41 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $40 encode-op  or vmword, ;

: jlti16
  $42 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $42 encode-op  or vmword, ;

: jlei16
  $43 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $43 encode-op  or vmword, ;

: jgti16
  $44 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $44 encode-op  or vmword, ;

: jgei16
  $45 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $45 encode-op  or vmword, ;

: jcyi16
  $46 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $46 encode-op  or vmword, ;

: jncyi16
  $47 emit-fwd-jmp
  origin @ -  asmip @  -  encode-imm16  $47 encode-op  or vmword, ;

: adcrr \ r1 r2 r3 addcr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $c4 encode-op  3or vmword, ;

: sbcrr \ r1 r2 r3 sbcrr
  encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $c8 encode-op  3or vmword, ;

\ Test -----------------------------------------------------------------------------------------------------------------

( \ EXAMPLE 1
$1000 #ORG

#LABEL _start
    $fc00 %r3       movi16r
    %r3 %r7         movrr
    %r5 %r7         movrr
#LABEL _test
    %r7 $ffee %r5   movrm
    %r1 _test %r5   movmr
    %r4 %r5         movr*r
    ' Test comment.
    %r1 %r2 %r3     addrr
    %r1 %r2 %r3     subrr
    %r7 incr
    %r1 %r2 %r8     mulrr
    $ab67 %r3 %r4   lshi16r
    %r1 %r3 %r7     lshrr
    %r2 %r3 %r7     orrr
    %r8             notr
    $1237           jnei16
    %r4 %r5 %r6     sbcrr
)

( \ EXAMPLE 2
$1000 #ORG

#LABEL _start
    %r1 %r2         movrr
    #FWD   jnei16     tt
    _start jnei16
    %r3 %r4         movrr
    %r5 %r6         movrr
    tt #<PATCH
    hlt
)

( \ EXAMPLE 3
$1000 #ORG

1 2 3 4 5   5 #DATA8 my_data8
$1234 $5678 2 #DATA16 my_data16
)





