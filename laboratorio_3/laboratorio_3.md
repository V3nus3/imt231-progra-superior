# Laboratorio 3: ¡Sincronizando clientes!

## Introducción
Seguro que alguna vez has intentado comprar entradas para algún evento (un partido de fútbol, un concierto, etc.). Normalmente, al iniciar el proceso de compra online, quedas en una cola virtual hasta que puedes acceder a la tienda, donde esperas hasta completar la compra. En este primer laboratorio, implementarás la simulación de una tienda de venta de entradas.

---

## Escenario
Dispones de un proyecto en C++ con al menos dos ficheros:
- `main.cpp` (ya implementado, no puedes modificarlo).
- `tienda_tickets.cpp` (archivo donde deberás añadir toda la lógica de concurrencia).

Además, hay un fichero JSON llamado `clientes.json` con información sobre los clientes que quieren comprar. Su estructura es:
```json
[
  {
    "nombre": "Pedro",
    "numeroTickets": 2,
    "tiempoEnTienda": 0.4,
    "retrasoIngreso": 0.0,
    "VIP": false
  },
  ...
]
```
- `nombre`: nombre del cliente.  
- `numeroTickets`: número de entradas a comprar.  
- `tiempoEnTienda`: segundos que permanece en la tienda antes de comprar.  
- `retrasoIngreso`: segundos de espera antes de entrar (desde el inicio del programa).  
- `VIP`: si es cliente VIP o no.  

El `main.cpp` acepta dos argumentos de línea de comandos: el precio de la entrada (`double`) y la capacidad máxima de la tienda (`int`). Ejemplo:
```bash
./main 50.2 3
g++ -pthread -std=c++20 main.cpp tienda_tickets.cpp -o main
```
Significa que cada entrada vale 50.2 unidades monetarias y como máximo pueden estar 3 clientes dentro simultáneamente.

---

## Firma de la función
```cpp
double simular_tienda(const std::vector<Cliente>& clientes,
                      double precio_ticket,
                      int capacidad_maxima,
                      int n_vips);
```
- `clientes`: vector de diccionarios parseados desde JSON.  
- `precio_ticket`: precio de la entrada.  
- `capacidad_maxima`: capacidad máxima simultánea.  
- `n_vips`: número total de clientes VIP.  
- Devuelve las ganancias finales de la tienda.  

---

## Función adicional
```cpp
double get_segundos_transcurridos();
```
Devuelve los segundos transcurridos (con una decimal) desde el inicio del programa. Utilízala para registrar tiempos en tu código.

---

## Comportamiento de cada cliente
Cada cliente es un `std::thread` cuyo nombre será el del cliente. Pasos:
1. Esperar `retrasoIngreso` segundos (`std::this_thread::sleep_for`).  
2. Entrar en la tienda (región crítica) — respetando la capacidad máxima y dando prioridad a VIPs.  
3. Permanecer dentro `tiempoEnTienda` segundos.  
4. Comprar `numeroTickets` entradas: incrementar las ganancias en `precio_ticket * numeroTickets`.  
5. Salir de la tienda (liberar la plaza).  

Cada vez que un cliente entra o sale, imprimir exactamente:
```
<segundos>s: <nombre> <acción>
```
- `<segundos>`: valor devuelto por `get_segundos_transcurridos()`.  
- `<acción>`: `entrando` o `saliendo`.  

No pueden mostrarse otros mensajes.

---

## Clientes VIP
- Los clientes VIP deben poder entrar antes que los no VIP (respetando su `retrasoIngreso`).  
- Ningún cliente “normal” puede entrar mientras haya algún VIP dentro.  

---

## Requisitos
- **Entregable**: un único fichero C++ (`ticket_store.cpp`).  
- No importar ni modificar `main.cpp`.  
- No cambiar la firma de `simular_tienda`.  
- Cada cliente = un `std::thread`.  
- No crear hilos adicionales aparte de los clientes.  
- Usar mecanismos de sincronización de C++ (`std::mutex`, `std::condition_variable`, semáforos, etc.).  
- No debe haber “busy waits”.  
- Implementar la lógica de capacidad (hasta `capacidad_maxima` clientes dentro).  
- Respetar prioridad y exclusión de clientes VIP.  
- Mostrar únicamente los mensajes de entrada/salida indicados.  
- Comprobar que al finalizar `simular_tienda` retorna el total de ganancias calculado. Devolver un valor erróneo provocará error en el programa.

¡Feliz programación! 🤡
