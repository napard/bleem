\ (02Ago2022).

s" assembler.fs" included

$000ff7f0 constant VIDEO_CTRL_REG

    $1000 #ORG

    VIDEO_CTRL_REG %r2
                movir
    1 %r1       movir
    %r1 %r2     movrr*

    hlt

\ Dump code.
start-rom
0 8192 dc3
end-rom







