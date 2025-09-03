# UTF8 Truncate

Imagine you're a social network with a 280 byte limit for certain text. Since some characters require multiple bytes to encode, blindly truncating a string after 280 bytes could leave junk bytes trailing. This problem is both a good excuse to understand how the UTF-8 encoding works, and representative of a common issue in practice!

The "cases" file has one case per line. The first byte is an unsigned integer indicating the number of bytes to which to truncate; the remained is a utf-8 encoded string to truncate. The "expected" file has one expected result per line. Your program should read "cases", parse it, and write one correctly truncated string per line. This output can then be diffed against "expected"."

Note:

- The "cases" file has one case per line. The first byte is an unsigned integer indicating the number of bytes to which to truncate; the remained is a utf-8 encoded string to truncate.
- The "expected" file has one expected result per line.
- Your program should read "cases", parse it, and write one correctly truncated string per line. This output can then be `diff`ed against "expected".
- Do not truncate the string in the middle of a single unicode codepoint! This is the purpose of the exercise.
- None of the cases cover grapheme clusters like 👨‍👩‍👧‍👦 (family of 4) ... this is a challenging stretch goal; see https://unicode.org/reports/tr29/
