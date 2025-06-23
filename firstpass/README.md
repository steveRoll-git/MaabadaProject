# Hello World

some problems I've noticed.

- Instruction counter starts at 100, data pointer starts at 0, why?
- Readline AGAIN needs the entire line and tell me if there was a problem.
- Instruction counter starts at 0 in the mmn, The video and the example start at 100, what gives?

## Things I need to be sure about.

- I Need readline to give me an error if I read more than 80 characters.
- I need readline to tell me what type of of status I'm in, whitespace? comment? instruction? or data.
- I need to know 

- I can destory the line I get from FGETS,because I don't need to print it anywhere this time.


fgets => strtok (label?  | type of command), strtok(type of command | args?) =>intruction_t => proceed as told.

dc_table value is the SIZE of the value. ( in sentences.)
ic_table value tells you the LOCATION in memory.

Parser for line (keyword style (zero, one, two args), with optional label)
if label found => if its instctive put in IC as location, if DC, put in DC as weigth.

