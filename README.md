# Compile o código usando WSL

1. Instalar compiladores
```bash
sudo apt update
```
```bash
sudo apt install g++ build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libsfml-dev -y
```

2. Compilar
```bash
g++ main.cpp implementations/cubo.cpp implementations/solver.cpp -Iclass -o cubo2x2     -lsfml-window -lsfml-system -lsfml-graphics -lGL -lGLU -lpthread
```