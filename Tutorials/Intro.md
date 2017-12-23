## Lexer API Intro

The Lexer API is the primary, public-facing header of the LexBuild library. It provides a class `Lexer` which performs all the necessary
tasks to have a functional lexer. It provides two public functions to allow developers to perform lexical analysis.

The first function is `add_rule()`:

```cpp

void add_rule(std::string regex, int priority,
	 std::function<void(std::string, std::vector<std::unique_ptr<T>>&)> on_accept);

```
This function is used to specify a regular expression rule, its priority, and a function to apply if the rule is matched. This is the
functional equivalent of the format document passed to lexical analysis generators like `(f)lex`.

The second function is `lex()`:

```cpp

bool lex(std::string& data, std::vector<std::unique_ptr<T>>& result);

```
This function performs lexical analysis (based on the provided rules using `add_rule()`) on the provided data and collects the result into
`result`. How the vector is modified is up to the developer using LexBuild. This is where the:

```cpp

std::function<void(std::string, std::vector<std::unique_ptr<T>>&)> on_accept

```

parameter passed to `add_rule()` comes into play. It has complete control over how the passed in vector is modified when a rule is matched.
In fact, `lex()` does not directly manipulate the `result` parameter, **all** the modification is done by the `on_accept` functions passed 
in.

A very simple *lexer* that tokenizes the string 'abc' into '1' and discards whitespace:

```cpp
#include "Lexer/Lexer.hpp"

#include <string>
#include <vector>
#include <memory>

int main()
{
    Lexer<int> lexer;
    
    lexer.add_rule("abc", 2, [](std::string str, std::vector<std::unique_ptr<int>>& vec) -> void
    {
        vec.push_back(std::make_unique<int>(1));
    });

    /* Do nothing (e.g. discard whitespace) */
    lexer.add_rule(" |\t|\n", 2, [](std::string str, std::vector<std::unique_ptr<int>>& vec) -> void {});
                
    std::vector<std::unique_ptr<int>> result;
    std::string data = "abc abc \t\n abc";
    
    lexer.lex(data, result);
    
    /* Now 'result' should contain three 1s, since 'abc' would be matched three times and all the whitespace
       in between was discarded. */
    
    return 0;
}

```

In this code, the priorities set (2 for both rules) does not actually matter at all. The priority is used to break ties
if there is more than one regular expression that can be matched at a time. 

For example, if there is a regular expression for variable names, like: [a-zA-Z]+ and a regular expression for the keyword
'int' like: int. Then, when encountering "int", both [a-zA-Z]+ and int match. 

Which one should be chosen? The one with the higher priority (it would make sense for "int" to have a higher priority, else it
would never be accepted). 

