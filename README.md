# NBodyCuda
Este proyecto requiere CUDA.

## Instrucciones de uso
Asegurarse que los submodulos se encuentren actualizados:
```bash
git submodule update --init --recursive
```

Para compilar el programa se debe ejecutar el siguiente comando:
```bash
mkdir build && cd build
cmake -B . -S ..
cmake --build .
```

El ejecutable se encontrará en la carpeta `build/src/Debug/` o `build/src/Release/` dependiendo del modo de compilación.

Demo: https://youtu.be/K1jDKlJQqFI 