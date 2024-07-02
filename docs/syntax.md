### All these examples are just ideas right now
- *everything* is subject to change


```
-- This is a single-line comment

===
this
is
a
multi-line
comment
===
```

```
-- basic primitive datatypes
bool: b = true;
int: i = 5;
flt: f = 4.2;

-- can concatenate str + str or str + chr
chr: c = '!';
str: s = "Hello, world" + c;
-- print string s to stdout
disp(s);
```

```
flt: func multiply(flt: x, flt: y) {
	ret x * y;
};

flt: result = call multiply(2.0, 6.2);
```
