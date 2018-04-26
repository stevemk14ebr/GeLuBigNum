
# Genaille Lucas Rulers
These rulers are a pre-computed lookup table for 1 digit multiplies by an N digit multiplicand. This 1 by N attribute can be used
to scale to N by M multiplies by breaking the multiplier into it's place-components, apply the lut, then adding all the split chunks 
back together. 

Here's a great VSauce video about them: https://www.youtube.com/watch?v=OxRMyPsLtTk

# The library
This library is naive in everyway and unoptimized. Big digits are stored directly as their character representation instead of a more 
efficient packed encoding. Also only two operators are implemented (add and multiply).

# Why
The rulers are interesting, and if you don't care about speed the code size is quite good. That said this isn't THAT slow, it
can handle a 30 char number * 30 char number in around a millisecond or two. 

I also recommend taking a look at the rulers themselves. There's quite a few hidden paterrns and they're quite beautiful. You can actually generate all the numbers of the rulers with 3 or 4 nested for loops, they do indeed have an underlying structure.
