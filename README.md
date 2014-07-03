# gilligan - the prototype DCP / Minnow client

## Introduction
This is a client for the Domain Chat Protocol (DCP).

The spec is actively being developed, as is this client, Gilligan, and the
server, [Minnow](https://github.com/InterlinkedIRC/minnow-prototype).

## Requirements
* A C++ compiler.  clang is recommended, but MSVC should probably work.
(Anything not clang is completely untested at this point.)
* Qt 4.7 or later.  Development "theoretically" targets 4.7, but all work
happens on 5.3, so incompatibilities with older version may not be noticed.
If you are running Qt 4.7 or later and cannot build Gilligan, file a bug.
* KDE 4.2 or later.  It wouldn't be very hard to make KDE an optional
dependency (removing credential saving in the process), probably just a few
ifdefs, but the developers are more focused on writing the actual features :)

## Building
This project uses QMake, so it's fairly straightforward:

```
qmake
make
```

It should be that easy!

