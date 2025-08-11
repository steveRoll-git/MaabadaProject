MAIN: mov #10 , r5


.extern factor

jsr factor

add #48, r1
prn r1

stop
