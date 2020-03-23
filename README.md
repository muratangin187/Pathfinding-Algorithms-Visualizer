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
