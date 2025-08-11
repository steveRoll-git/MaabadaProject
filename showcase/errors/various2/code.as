someEntry: mov r1, r2

; Label already defined as entry.
.entry someEntry
.entry someEntry

; Label not defined.
.entry nonExistent

; Invalid number.
.data 1, 2, asdf

; Invalid matrix syntax.
mov r1, someMat[r2 [r3]

; Matrix rows and columns can only be accessed with registers.
mov r1, someMat[r2][someEntry]

; An external label cannot be defined as an entry.
.entry someLabel
.extern someLabel

; Invalid string syntax.
.string "oh no
.string oh no"

; Extraneous text after directive.
.entry asdf extra

; Program is too large after this point.
.mat[100][100]

; Word is too large.
.data 1, 2, 1000

; Immediate number is too large.
mov #1000, r1
