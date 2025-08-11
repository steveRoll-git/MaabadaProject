; Start loop here to print all the numbers

.entry factor

factor: mov #0, r1
LOOP:   add r5, r1

; jmp logic
      dec r5
      cmp r5, #0
bne LOOP
jmp END


END: rts
