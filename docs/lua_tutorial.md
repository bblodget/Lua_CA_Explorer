---
layout: default
title: Lua Tutorial
nav_order: 7
---

1. TOC
{:toc}

# Quick Introduction to Lua

This is a quick introduction to Lua is targeted towards people
that already have some programming experience in another language.

Lua is a very clean, light and syntactically simple scripting language. Like Java and
Python it is compiled to byte code and then the byte code is interpreted. It is very
efficient and benchmarks show Lua as one of the fastest scripting (interpreted)
languages. Lua was built from the ground up to be extensible. This makes very easy to
add our own custom functionality to Lua.

Lua CA Explorer uses lua-5.4.4 source distribution.  This page gives a brief introduction 
to the core Lua language.  There is much more to the Lua language than what is covered here.
To learn more about the Lua language, here are a couple starting points:
* [https://www.lua.org/pil/](https://www.lua.org/pil/)
* [http://lua-users.org/wiki/TutorialDirectory](http://lua-users.org/wiki/TutorialDirectory)

# Statements

Lua needs no separator between statements, but you can use a semicolon if you wish.
The convention is to use semicolons only to separate two or more statements on the
same line. Line breaks play no role in Lua's syntax.

# Comments

A comment starts with a double hyphen (--) and continues to the end of the line. There
is also a block comment that starts with --[[ and will span multiple lines until it runs into
--]].

# Global Variables

Global variables do not need a declaration. You simply assign a value to a variable and
then use it. If a variable has not been assigned a value yet it has the special value of
**nil**. Here is an example:

```lua
print(b) -- outputs nil
b=7
print(b) -- outputs 7
```

If you want to delete a global variable just assign **nil** to it. A variable is considered
existent if and only if it has a non **nil** value. If it has a **nil** value then the garbage
collector can reclaim that storage space.

# Local Variables

Local variables are declared with the keyword **local**. Local variables are local to their
block. Once the program exits the block the local variables defined in that block go out
of scope. Local variables are more efficient than global variables, so they should be
preferred over global variables. It is possible to have global variables and local variables
that have the same name. The **do/end** pair define an inner block in the following
example:

```lua
x = 10
print(x) -- outputs 10
do
local x = 5
print(x) -- outputs 5
end
print(x) -- outputs 10
```

# Assignments

Setting a variable to a value is an assignment. You can assign any type (number, string,
function) to a variable. Lua is dynamically typed. Example:

```lua
a = 42
b = "The answer: "
c = print -- assigns the print function to c
c(b .. a) -- outputs "The answer: 42"
```

Lua supports multiple assignments in a single statement. Example:

```lua
a, b = 6*9, "six by nine: "
print(b .. a) -- outputs "six by nine: 54"
```

You can use multiple assignment to swap values. Example:

```lua
a, b = 1, 2
print(a, b) -- outputs: 1 2
b, a = a, b
print(a, b) -- outputs: 2 1
```

# Control Structures

Lua has a pretty standard set of control structures such as **while**, **repeat**, **for**, and **if** ...
**then** ... **else** ... **end**.

Here is an example of **while**, **repeat**, and **for** statements. They each count 1, 2, 3:

```lua
print("while:")
i=1
while i<=3 do
    print(i)
    i = i + 1
end

print("repeat:")
i=1
repeat
    print(i)
    i = i + 1
until i>3

i=1
print("for:")
for i=1,3 do
    print(i)
end
print("i: " .. i)
```

In the above example the loop variable in the **for** statement is automatically made a
**local** variable so it does not effect the value of the global variable i.

Each of these loop statements can be exited early by using the keyword **break**. Here is
an example:

```lua
print("while:")
i=1
while true do -- always true
    print(i)
    if i==3 then break end
    i = i + 1
end

print("repeat:")
i=1
repeat
    print(i)
    if i==3 then break end
    i = i + 1
until 1==2 -- never will happen
```

In the for statement the a third number can be provided to the for statement which the
step size. The following example prints out the even numbers less than or equal to 10:

```lua
for i=2,10,2 do
    print(i)
end
```

The **if** ... **then** ... **elseif** ... **else** ... **end** can provide functionality similar to the
switch statement in C. For example:

```lua
choice = 3

if choice < 1 then
    response = "invalid choice"
elseif choice == 1 then
    response = "one"
elseif choice == 2 then
    response = "two"
elseif choice == 3 then
    response = "three"
else
    response = "invalid choice"
end

print(response)
```

# Functions

Functions are Lua's main mechanism for abstracting statements and functionality. Since
Lua is dynamically typed it is not necessary to specify the type of the arguments. The
following example adds three numbers together:

```lua
function add3(a, b, c)
    return a + b + c
end
sum = add3(1,2,3)
print(sum) -- outputs: 6
```

Returning a value is optional in Lua. We can write a function that does not return a
value:

```lua
function print_add3(a,b,c)
    local sum = a + b + c
    print("sum = " .. sum)
end

print_add3(1,2,3) -- outputs: sum = 6
```

One nice feature of Lua functions, that is different from a lot of languages, is that
functions can return multiple values. This can be used nicely with multiple assignment.
Example:

```lua
function next_two(a)
    local n1 = a + 1
    local n2 = a + 2
    return n1, n2
end

a, b = next_two(5)
print(a, b) -- outputs: 6 7
```

Functions are first class object in Lua which means they can be assigned to a variable.
We saw an example of this in the Assignment section of this chapter. Here is another
example:

```lua
function add3(a, b, c)
    return a + b + c
end

sum1 = add3(3,4,5)

a = add3 -- assign function add3 to a
sum2 = a(3,4,5)

print("sum1: " .. sum1) -- outputs: 12
print("sum2: " .. sum2) -- outputs: 12
```

# Tables

Tables are a very powerful data structure in Lua. They can be used as standard arrays,
associative arrays, or records. They also are the basis for object oriented programming
in Lua. They are dynamic structures that grow as items are added. There is no need to
specify a size when the table is created.

## Creating Tables

Tables are created using curly brackets, e.g., { }. The following example shows how to
create an empty table and how to initialize a table at creation time.

```lua
a = {} -- creates an empty table
b = {2, 4, 6, 8} -- array style initialization
c = {hue = 100, sat = 50, val =128} -- associative array style
```

## Tables to hold Records

We can use tables to hold records/fields. The following example shows how we can start
with an empty table or an initialized table:

```lua
lower = {hue = 100, sat = 50, val =128}

upper = {}
upper.hue = 105
upper.sat = 80
upper.val = 255

print("hue: " .. lower.hue .. "- " ..upper.hue)
print("sat: " .. lower.sat .. "- " ..upper.sat)
print("val: " .. lower.val .. "- " ..upper.val)
```

## Tables as Arrays

You can use tables as arrays using square brackets, e.g. [ ]. The convention in Lua is
for arrays to start at index 1. This is different from other languages, such as C, that
start at index 0. You can find the size of a table using the # operator. The # operator
will return the number of elements in our array.

```lua
a = {2, 4, 6, 8, 10}
b = {}

-- copy a to empty b
for i = 1,#a do
    b[i] = a[i]
end

-- print the values of b
for i = 1,#b do
    print(b[i])
end
```

## Tables as Associative Arrays

You can use tables to implement associative arrays. Associated arrays are arrays of
key/value pairs. Other names for associated arrays are *hashes* or *dictionaries*. Lua has
a *generic for* structure that uses an iterator for looping. This is useful for looping
through associative arrays. Detailed explanation of iterators and the generic
for structure is beyond the scope of the brief introduction but here is an example:

```lua
fruit_color = {apple="green", orange="orange", banana="yellow"}

fruit_color["cherry"] = "red" -- add new fruit

-- use generic for to print out fruit_color
for k,v in pairs(fruit_color) do
    print(k, v)
end
```

# Reserved Keywords

The following words are reserved in Lua. They can't be used for variable or function
names.

> **and break do else elseif**
> **end false for function if**
> **in local nil not or**
> **repeat return then true while**

# Operators

* __\-__ substraction/negation
* __\*__ multiplication
* __/__ division
* __^__ exponent
* __==__ equal to
* __~=__ not equal to
* __\>__ greater than
* __<__ less than
* __<=__ less than or equal to
* __\>=__ greater than or equal to
* __..__ concatenation
* __\#__ size of table operator
* __and__ Logical AND
* __or__ Logical OR
* __not__ Logical NOT
* __&__ bitwise AND
* __\|__ bitwise OR
* __~__ bitwise exclusive OR
* __\>>__ right shift
* __\<\<__ left shift
* __~__ unary bitwise NOT




