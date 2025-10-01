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
g++ main.cpp implementations/cubo.cpp implementations/solver.cpp implementations/bloom.cpp -Iclass -lsfml-window -lsfml-system -lGL -lGLU -o CubMagic
```