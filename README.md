# binwords - A simple C library for managing binary codes.

*binwords* it's an easy-to-use C library conceived to manage binary time series in real time, and to store the resulting binary code words by its decimal value. It allowed to define code words from a stream of bits with a predetermined length and overlap. It also provides a simple API to get statistical data of the words obtained from a binary signal.

*binwords* is based on the definition of two data structures:
-*BitsBuffer*: a circular buffer of bits. It stores the stream of bits. Its size is equal to the predetermined length of the code word. Once a new word is completed after the arrival of its last bit, the contents of this buffer is used to compute the word decimal value, which is stored in another data structure (WordsBuffer).
-*WordsBuffer*: a circular buffer of integers (or long integers). It stores the decimal values of the code words obtained from the processed binary signal. Overlapping between consecutive words can be managed by setting wb->overlap to the desired number of overlapping bits between words.

A histogram of code words stored in *WordsBuffer* after processing a binary signal can be obtained using the function wbCreateHistogram.