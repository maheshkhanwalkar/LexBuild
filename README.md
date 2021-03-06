# LexBuild

### Important Note:
The project is undergoing a large revamp effort -- which will include major changes to the API and increase
overall functionality and speed. To track those changes, visit the 'dev' branch for more details.

LexBuild is a lexer library that allows developers to integrate lexers into their programs. Traditionally, tools
like 'lex' and 'flex' would take a specification and generate code to tokenize text based on that specification.

However, the generated code is not easy to integrate with the rest of the base and cannot be easily customized. The
generated source shouldn't be modified, since regeneration would wipe out the changes. Plus often times C/C++ code
would be inter-mixed with the format specification to drive specific tokenization work.

LexBuild is a library, so it easily integrates with other code. It allows you to specify what regular expression
rules are required, their priority, and what to do on matching via a simple API. The regular expression generation,
simulation, and tokenization all happen in memory: there is no code generation at all.

Tutorials:

1. [Lexer API Intro](https://github.com/maheshkhanwalkar/LexBuild/blob/master/Tutorials/Intro.md)
