\ (02Ago2022).

s" assembler.fs" included

$000ff7f0 constant VIDEO_CMD_REG
$000ff7f1 constant VIDEO_BGCOL_REG

    $1000 #ORG

#LABEL start
        ' Set clear color.
        VIDEO_BGCOL_REG %r2 movir
        $fc %r1 movir
#BYTE   %r1 %r2 movrr* 
        ' Clear screen.
        VIDEO_CMD_REG %r2 movir
        2 %r1 movir
        %r1 %r2 movrr*

        ' start jnei16

        hlt

\ Dump code.
start-rom
0 8192 dc3
end-rom







