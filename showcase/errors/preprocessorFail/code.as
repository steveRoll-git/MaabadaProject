mov r5, #123

; Invalid macro name.
mcro stop

; Extraneous text after macro definition.
mcro testMcro extraText

; Extraneous text after `mcroend`.
mcroend extraText

; Line is longer than 80 characters.
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa