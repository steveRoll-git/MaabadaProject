; This is a program that should print decreasing numbers to an output from 10 to 0
mov #10 , r5
; Start loop here to print all the numbers

LOOP: mov r5,r1
      ; adds ascii value to '0' to the number, making it printable via ASCII
      add #48, r1

      prn r1

      ; jmp logic
      dec r5
      cmp r5 ,#0
bne LOOP
jmp END

END: stop








