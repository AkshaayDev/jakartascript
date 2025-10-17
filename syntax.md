# JKS Syntax

## Lexing

### Keywords

Data Types: `bool`, `int`, `string`

Control Flow: `if`, `else`, `for`, `while`, `continue`, `break`

Constants: `true`, `false`

### Identifiers

Identifers can start with any letter (a-z/A-Z) or an underscore (`_`).
Any other character in the identifier can be alphanumeric (1-9/a-z/A-Z) or an underscore.

### Comments

Single Line Comments:
All text after `//` until the end of that same line is ignored by the interpreter.

Multi Line Comments:
All text between `/*` and `*/` are ignored by the interpreter.
Nested multi line comments are not supported and a comment will end as soon as `*/` is encountered.

### Number Literals

Prefixes:

- `0b`/`0B`: binary number literal
- `0o`/`0O`: octal number literal
- `0x`/`0X`: hexadecimal number literal

> An empty non-decimal number is illegal (e.g. `0x` is illegal but `0xFF` is valid)

Decimal points:
Decimal points can be placed anywhere in a **decimal** number literal except the end.
Example:

- `3.14` is valid
- `.01` is valid
- `12.` is invalid (no digit after decimal point)
- `32.4.2` is invalid (2 decimal points)
- `0x3.14` is invalid (non-decimal number with decimal point)

Scientific Notation:
The format `[base][e/E][index]` returns `base` * 10^`index` where `number` is a **decimal** number and `index` is an integer preceded by an optional `+`/`-`.
For non-decimal numbers, the `e` or `E` is considered a new token.
Example:

- `2e5` is valid
- `23e+5` is valid
- `34E-53` is valid
- `42e` is invalid (empty index)
- `0o5e3` is invalid (non-decimal base)
- `4e24.3` is invalid (non-integer index)

Digit Separators:
Apostrophes (`'`) can be put **between 2 digits** for clarity.
Example:

- `300'000` is valid
- `1'2.34'567'8` is valid
- `0xFD'3A` is valid
- `'98` is invalid (cannot start with `'`)
- `91''2` is invalid (two adjacent `'`)
- `342'e4` is invalid (`'` adjacent to non-digit)

### String Literals

String literals are text enclosed with `"`. String literals must be opened and closed in the same line. Placing a backslash (`\`) before a character creates an **escape sequence**.

Escape sequences:

- `\'`: Single quote (optional)
- `\"`: Double quote
- `\\`: Literal backslash
- `a`: Alert (Bell)
- `b`: Backspace
- `f`: Form feed
- `n`: Newline
- `r`: Carriage Return
- `t`: Horizontal Tab
- `v`: Vertical Tab
- `\nnn`: Octal value up to 3 octal digits (0-7)
- `\xnn`: Hexadecimal value with at least 1 digit that terminates at the first non-digit where digits are 0-9/a-f/A-F

### Operators

Assign: `=`

Arithmetic: `+`, `-`, `*`, `/`, `%`, `**`

Arithmetic Assign: `+=`, `-=`, `*=`, `/=`, `%=`, `**=`, `++`, `--`

Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`

Bitwise Assign: `&=`, `|=`, `^=`, `<<=`, `>>=`

Logic: `&&`, `||`, `^^`, `!`

Logic Assign: `&&=`, `||=`, `^^=`

Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`

Terminator: `;`

Property: `.`

### Separators

Brackets: `(`, `)`, `{`, `}`, `[`, `]`

Separator: `,`

## Parsing

### Formats

#### Statements

Declaration: `type id;`

Definition: `id = val;`

Declaration+Definition: `type id = val;`

If: `if (cond) {then} [else if (cond) {then}] [else {elseBranch}]`

For: `for (init; cond; update) {loop}`

While: `while (cond) {loop}`

Continue: `continue`

Break: `break`

#### Expressions

Literals: Boolean literals(`true`/`false`), String literals, Number literals

Identifiers: `id`

Binary: `left` `op` `right`

Unary: `op` `expr`

Group: `(expr)`
