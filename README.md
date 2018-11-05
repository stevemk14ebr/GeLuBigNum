
# Genaille Lucas Rulers
These rulers are a pre-computed lookup table for 1 digit multiplies by an N digit multiplicand. This 1 by N attribute can be used
to scale to N by M multiplies by breaking the multiplier into it's place-components, apply the lut, then adding all the split chunks 
back together. 

Here's a great VSauce video about them: https://www.youtube.com/watch?v=OxRMyPsLtTk

# The library
Only basic operators are implemented (add, sub, and multiply). This library exposes a multiply() and sum() function that takes an arbitrary length string and does the operation. This could be extended to division with another lut. Numbers are encoded in what is basically Binary Coded Decimal (BCD) plus a small control structure for sign and encoding unpacking. We acheive 2 chars per byte, and can do all math operations without copies by directly operating on the digits in their BCD form (1 nibble = 1 char). 

# Why
The rulers are interesting, and if you don't care about speed the code size is quite good. That said this isn't THAT slow, it
can handle a 30 char number * 30 char number in around a millisecond or two, I did do a little profiler guided optimization. And memory usage is < 2 MB most of the time (could be significantly improved here).

I also recommend taking a look at the rulers themselves. There's quite a few hidden paterrns and they're quite beautiful. You can actually generate all the numbers of the rulers with 3 or 4 nested for loops, they do indeed have an underlying structure.
