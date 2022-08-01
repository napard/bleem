\ Charset generator (31 Jul2022).

vocabulary charsetgen
also charsetgen  definitions

variable bitshift  0 bitshift !
: gen-bit
  bitshift @ lshift
  1 bitshift +!
  bitshift @ 8 >= if
    0 bitshift !
    or or or or or or or  hex u. decimal
  then ;

: _ 0 gen-bit ;
: X 1 gen-bit ;

_ _ X X X X _ _
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ X X X _ 
_ X X _ X X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ X _ 
_ _ X X X X _ _ cr

_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X X X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ cr

_ X X X X X _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ X X X X _ _ _ 
_ X X X X _ _ _ 
_ X X _ X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ X X _ _ 
_ X X X X _ _ _ cr

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ cr

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ X X _ X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ cr

_ _ _ X X X X _ 
_ _ _ X X X X _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ X X _ X X _ _ 
_ _ X X X _ _ _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ X X _ _ 
_ X X X X _ _ _ 
_ X X X _ _ _ _ 
_ X X X X _ _ _ 
_ X X _ X X _ _ 
_ X X _ _ X X _ cr

_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ cr


_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X X _ X X X 
_ X X X X X X X 
_ X X _ X _ X X 
_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X _ _ _ X X cr

_ X X _ _ X X _ 
_ X X X _ X X _ 
_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ X X X _ 
_ X X _ X X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ X X X X X _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X _ _ _ _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ _ X X X _ cr

_ X X X X X _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X X X X _ _ 
_ X X X X _ _ 
_ X X _ X X _ _ 
_ X X _ _ X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ _ X X X X _ _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ X X X X X X _ 
_ X X X X X X _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ cr

_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X _ _ _ X X 
_ X X _ X _ X X 
_ X X X X X X X 
_ X X X X X X X 
_ X X X _ X X X 
_ X X _ _ _ X X cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ cr

_ X X X X X X _ 
_ X X X X X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X X X _ _ cr

_ _ _ _ X X _ _ 
_ _ _ X _ _ X _ 
_ _ X X _ _ _ _ 
_ X X X X X _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ X _ 
X X X X X X _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ X X X X _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X X _ _ 
_ X X X X X X _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X _ _ _ _ 
_ _ X X _ _ _ _ 
_ X X X X X X X 
_ X X X X X X X 
_ _ X X _ _ _ _ 
_ _ _ X _ _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ cr

_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ cr

_ X X _ _ X X _ 
_ X X _ _ X X _ 
X X X X X X X X 
_ X X _ _ X X _ 
X X X X X X X X 
X X X X X X X X 
_ X X _ _ X X _ 
_ X X _ _ X X _ cr

_ _ _ X X _ _ _ 
_ _ X X X X X _ 
_ X X _ _ _ _ _ 
_ _ X X X X _ _ 
_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ X X X X X _ _ 
_ _ _ X X _ _ _ cr

_ X X _ _ _ X _ 
_ X X _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ X X _ 
_ X _ _ _ X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ _ X X X _ _ _ 
_ X X _ _ X X X 
_ X X _ _ X X _ 
_ _ X X X X X X cr

_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ cr

_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ X X _ _ cr

_ _ X X _ _ _ _ 
_ _ X X _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
X X X X X X X X 
_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ X X X X X X _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ X X X X X X _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ cr

_ _ _ _ _ _ _ _ 
_ _ _ _ _ _ X X 
_ _ _ _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ X X X _ 
_ X X X _ X X _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ X X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ X X X X X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ X X _ _ _ _ 
_ X X _ _ _ _ _ 
_ X X X X X X _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ X X X _ _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ _ _ _ X X X _ 
_ _ _ X X X X _ 
_ X X _ _ X X _ 
_ X X X X X X X 
_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ cr

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ _ _ _ 
_ X X X X X _ _ 
_ _ _ _ _ X X _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ _ _ _ 
_ X X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ X X X X X X _ 
_ X X X X X X _ 
_ X X _ _ X X _ 
_ _ _ _ X X _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ 
_ _ _ X X _ _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr

_ _ X X X X _ _ 
_ _ X X X X _ _ 
_ X X _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X X _ 
_ _ _ _ _ X X _ 
_ X X _ _ X X _ 
_ _ X X X X _ _ cr


