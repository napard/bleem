\ Charset generator (31Jul2022).

vocabulary bleem-charsetgen
also bleem-charsetgen  definitions

variable bitshift  0 bitshift !
: gen-bit
  bitshift @ lshift
  1 bitshift +!
  bitshift @ 8 >= if
    0 bitshift !
    or or or or or or or  s" 0x" type  hex u. decimal  [char] , emit
  then ;
: start-charset
  s" #include <stdint.h>" type cr
  s" uint8_t charset0[] = {" type cr ;
: end-charset  s" };" type cr ;
: #  ( cr s" // ---------- " type) cr ;

: _ 0 gen-bit ;
: X 1 gen-bit ;

start-charset

_ _ X X X X _ _
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ X X X _ 
_ X X _ X X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ X _ 
_ _ X X X X _ _ #

_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X X X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ #

_ X X X X X _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ X X X X _ _ _ 
_ X X X X _ _ _ 
_ X X _ X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ X X _ _ 
_ X X X X _ _ _ #

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ #

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ X X _ X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ #

_ _ _ X X X X _ 
_ _ _ X X X X _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ X X _ X X _ _ 
_ _ X X X _ _ _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ X X _ _ 
_ X X X X _ _ _ 
_ X X X _ _ _ _ 
_ X X X X _ _ _ 
_ X X _ X X _ _ 
_ X X _ _ X X _ #

_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ #

_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X X _ X X X 
_ X X X X X X X 
_ X X _ X _ X X 
_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X _ _ _ X X #

_ X X _ _ X X _ 
_ X X X _ X X _ 
_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ X X X _ 
_ X X _ X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ X X X X X _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ _ X X X _ #

_ X X X X X _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ 
_ X X X X _ _ _
_ X X _ X X _ _ 
_ X X _ _ X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ _ X X X X _ _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ X X X X X X _ 
_ X X X X X X _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ #

_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X _ X _ X X 
_ X X X X X X X 
_ X X X X X X X 
_ X X X _ X X X 
_ X X _ _ _ X X #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ #

_ X X X X X X _ 
_ X X X X X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X X X _ _ #

_ _ _ _ X X _ _ 
_ _ _ X _ _ X _ 
_ _ X X _ _ _ _ 
_ X X X X X _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ X _ 
X X X X X X _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ X X X X _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ 
_ X X X X X X _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X _ _ _ _ 
_ _ X X _ _ _ _ 
_ X X X X X X X 
_ X X X X X X X 
_ _ X X _ _ _ _ 
_ _ _ X _ _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ #

_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ #

_ X X _ _ X X _ 
_ X X _ _ X X _ 
X X X X X X X X 
_ X X _ _ X X _ 
X X X X X X X X 
X X X X X X X X 
_ X X _ _ X X _ 
_ X X _ _ X X _ #

_ _ _ X X _ _ _ 
_ _ X X X X X _ 
_ X X _ _ _ _ _ 
_ _ X X X X _ _ 
_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ X X X X X _ _ 
_ _ _ X X _ _ _ #

_ X X _ _ _ X _ 
_ X X _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ X X _ 
_ X _ _ _ X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ X X X _ _ _ 
_ X X _ _ X X X 
_ X X _ _ X X _ 
_ _ X X X X X X #

_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ #

_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ X X _ _ #

_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
X X X X X X X X 
_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ X X X X X X _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ X X X X X X _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ X X 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ X X X _ 
_ X X X _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ X X X X X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ X X X _ _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X X _ 
_ _ _ X X X X _ 
_ X X _ _ X X _ 
_ X X X X X X X 
_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ #

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ _ _ _ 
_ X X X X X _ _ 
_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X X _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ #

_ _ _ _ X X X _ 
_ _ _ _ X X X _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ X X X _ #

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ X X X X X X _ 
_ _ _ _ _ _ _ _ 
_ X X X X X X _ 
_ X X X X X X _ 
_ _ _ _ _ _ _ _ #

_ X X X _ _ _ _ 
_ X X X _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ X X X _ _ _ _ #

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ #

end-charset

