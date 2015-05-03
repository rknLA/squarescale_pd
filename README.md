squarescale_pd
==============

A puredata external that scales an input float up or down until it's within
range.

Internally, the object halves or doubles the input until it's within range.

It can be used something like this:

```
| 220 (
  |
| squarescale 60 150 |
  |               |
| 110 )         | 0.5 )
```

The object is especially useful for determining a canonical tempo in bpm for
very large or very small time values.
