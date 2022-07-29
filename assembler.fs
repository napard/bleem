\ Rudimentary assembler (25Jul2022).

vocabulary assembler
also assembler  definitions

\ Comment. -----
: '  cr ['] s" execute  s" // " type type ;

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

4 constant VMWORD-BYTES

variable origin  0 origin !
variable asmip   0 asmip !
: incIp  VMWORD-BYTES asmip +! ;

: #ORG  origin ! ;
: #LABEL create  asmip @  origin @ + , does> @ ; 

\ Instruction encoding. -----

\ : encode-op  hex s" CPU->ram[i++] = 0x" type  u.  s" ;" type  cr decimal ;
\ : encode-op  hex u. decimal ;
: encode-op ;

: encode-reg1  hex  8 lshift $f00 and ( u. decimal) ;
: encode-reg2  hex  12 lshift $f000 and ( u. decimal) ;
: encode-reg3  hex  16 lshift $f0000 and ( u. decimal) ;
: encode-imm16  hex  16 lshift $ffff0000 and ( u. decimal) ;
: or. or s" 0x" type u. decimal ;
: 2or. or or s" 0x" type u. decimal ;
: 3or. or or or s" 0x" type u. decimal ;

\ Instructions. -----

: movi16r \ $ffff r1 movi16r
  cr encode-reg1 swap  encode-imm16 swap  $10 encode-op 2or.
  s" //movi16r" type incIp ;

: movrr \ r1 r2 movrr
  cr encode-reg2 swap  encode-reg1 swap  $11 encode-op 2or.
  s" //movrr" type incIp ;

: movrm \ r1 $ffff r2 movrm
  cr encode-reg1 swap  encode-imm16 swap  >r >r encode-reg2 r> r>  $12 encode-op 3or.
  s" //movrm" type incIp ;

: movmr \ r1 $ffff r2 movrm
  cr encode-reg2 swap  encode-imm16 swap  >r >r encode-reg1 r> r>  $13 encode-op 3or.
  s" //movmr" type incIp ;

: movr*r \ r1 r2 movr*r
  cr encode-reg2 swap  encode-reg1 swap  $15 encode-op 2or.
  s" //movr*r" type incIp ;

: addrr \ r1 r2 r3 addrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $24 encode-op 3or.
  s" //addrr" type incIp ;

: subrr \ r1 r2 r3 subrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $28 encode-op 3or.
  s" //subrr" type incIp ;

: incr \ r1 incr
  cr encode-reg1  $29 encode-op or.
  s" //incr" type incIp ;

: decr \ r1 decr
  cr encode-reg1  $2a encode-op or.
  s" //decr" type incIp ;

: mulrr \ r1 r2 r3 mulrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $2c encode-op 3or.
  s" //mulrr" type incIp ;

: lshi16r \ $ffff r1 r2 lshi16r
  cr encode-reg2 swap  encode-reg1  >r >r encode-imm16 r> r>  $30 encode-op 3or.
  s" //lshi16r" type incIp ;

: lshrr \ r1 r2 r3 lshrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $31 encode-op 3or.
  s" //lshrr" type incIp ;

: rshi16r \ $ffff r1 r2 rshi16r
  cr encode-reg2 swap  encode-reg1  >r >r encode-imm16 r> r>  $32 encode-op 3or.
  s" //rshi16r" type incIp ;

: rshrr \ r1 r2 r3 rshrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $33 encode-op 3or.
  s" //rshrr" type incIp ;

: andrr \ r1 r2 r3 andrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $35 encode-op 3or.
  s" //andrr" type incIp ;

: orrr \ r1 r2 r3 orrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $37 encode-op 3or.
  s" //orrr" type incIp ;

: xorrr \ r1 r2 r3 xorrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $39 encode-op 3or.
  s" //xorrr" type incIp ;

: notr \ r1 notr
  cr encode-reg1  $3a encode-op or.
  s" //notr" type incIp ;

: jnei16 \ $ffff jnei16
  cr encode-imm16  $40 encode-op or.
  s" //jnei16" type incIp ;

: jei16
  cr encode-imm16  $41 encode-op or.
  s" //jei16" type incIp ;

: jlti16
  cr encode-imm16  $42 encode-op or.
  s" //jlti16" type incIp ;

: jlei16
  cr encode-imm16  $43 encode-op or.
  s" //jlei16" type incIp ;

: jgti16
  cr encode-imm16  $44 encode-op or.
  s" //jgti16" type incIp ;

: jgei16
  cr encode-imm16  $45 encode-op or.
  s" //jgei16" type incIp ;

: jcyi16
  cr encode-imm16  $46 encode-op or.
  s" //jcyi16" type incIp ;

: jncyi16
  cr encode-imm16  $47 encode-op or.
  s" //jncyi16" type incIp ;

: adcrr \ r1 r2 r3 addcr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $c4 encode-op 3or.
  s" //addcr" type incIp ;

: sbcrr \ r1 r2 r3 sbcrr
  cr encode-reg3 swap  encode-reg2  >r >r encode-reg1 r> r>  $c8 encode-op 3or.
  s" //sbcrr" type incIp ;

\ Test -----------------------------------------------------------------------------------------------------------------

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

cr



