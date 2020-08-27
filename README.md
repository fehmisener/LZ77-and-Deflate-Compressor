# LZ77-and-Deflate-Compressor
This project is about  the implementation of the LZ77 and Deflate COMPRESSION  algorithm.
# General summary of the project
The project aims to compress and store the text document file with .txt extension, which is received as input.
To do this, LZSS (Lempel – Ziv – Storer – Szymanski) and DEFLATE algorithm are used. LZSS algorithm aims to compress by eliminating repetitive places

# How the lz77 algorithm works
LZ77 iterates sequentially through the input string and stores any new match into a search buffer. The process of compression can be divided in 3 steps:
Find the longest match of a string that starts at the current position with a pattern available in the search buffer.
Output a triple (o, l, c) where,
o: offset, represents the number of positions that we would need to move backwards in order to find the start of the matching string.
l: length, represents the length of the match.
c: character, represents the character that is found after the match.
Move the cursor l+1 positions to the right.
for more information: https://towardsdatascience.com/how-data-compression-works-exploring-lz77-3a2c2e06c097
