# Pathfinding algorithms visualizer

You can analyze visually selected pathfinding algorithm's behaviour.

## Methods

## Prerequisites

There is no dependencies or prerequisites for this project.(olcPixelGameEngine included in files)

## Installing

You can compile project with main file in linux or windows enviroment.

Linux

```
g++ -o PathfindingVisualizer main.cpp olcPixelGameEngine.h -lX11 -lGL -lpthread -lpng -lstdc++fs
```

VS2019

```
You can import this project with .sln file.
```

## Usage
```
Keyboard S + Mouse Left = Put start point
```

```
Keyboard E + Mouse Left = Put end point
```

```
Mouse Left = Put wall
```

```
Mouse Right = Delete point at mouse position
```


```
Enter = start algorithm
Enter(while running) = stop algorithm
Enter(while stopped) = resume algorithm
```

```
ESC =  reset map
```

```
UP, DOWN ARROWS = selection between algorithms
```

## Used Libraries

[olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)

## Authors

* **Murat ANGIN** - [muratangin187](https://github.com/muratangin187)

## License
The MIT License (MIT)

Copyright (c) 2015 Chris Kibble

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
