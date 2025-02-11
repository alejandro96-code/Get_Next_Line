# 📚 get_next_line - Lectura eficiente de líneas de archivos en C

## 📌 Descripción

Get_next_line es una función en C que permite leer una línea completa de un archivo, gestionando el búfer de forma eficiente. Ideal para manejar archivos grandes y procesar sus líneas de manera individual.

## 📂 Estructura del Proyecto

📦 get_next_line
├── 📄 get_next_line.h # Archivo de cabecera con los prototipos de funciones
├── 📄 get_next_line.c # Implementación del get next line
├── 📄 get_next_line_utils.c # Implementación de ft_strlen y del strjoin
├── 📄 file.txt # Archivo que leeremos e imprimiremos por terminal
└── 📂 checkers # Archivos de ejemplo para compilar

## 📂 Funcionamiento del Proyecto

El proyecto se encarga de leer un archivo línea por línea utilizando un búfer que almacena temporalmente las partes del archivo que aún no han sido procesadas. La función get_next_line asegura que el archivo se lea de forma eficiente y se devuelvan las líneas completas.

### ¿Cómo funciona get_next_line?

Lectura del archivo:
La función get_next_line abre el archivo y comienza a leer en un búfer. Cuando se encuentra un salto de línea (\n), la línea completa se devuelve y el resto del archivo se guarda en una variable estática (stash) para continuar la lectura en la siguiente llamada.

Manejo del búfer:
Si el búfer no contiene un salto de línea, se sigue leyendo más bytes del archivo y se agregan al contenido almacenado. Cuando se encuentra el salto de línea, se devuelve la línea completa, incluyendo el salto de línea si está presente.

Retorno de la línea:
Se crea una nueva cadena con la línea leída, y cualquier contenido restante se almacena en stash para ser procesado en la siguiente llamada.

## 🔍 Funcionamiento Interno de get_next_line

Este flujo de ejecución supone que BUFFER_SIZE = 10. Es decir, en cada llamada a read(), se leen 10 caracteres del archivo.

🔢 ¿Cómo cambia con otros tamaños de BUFFER_SIZE?
Si BUFFER_SIZE = 1 → Se lee carácter por carácter, lo que hace más llamadas a read(), pero garantiza que cada \n se detecta de inmediato.
Si BUFFER_SIZE = 100 → Se leería el archivo completo en una sola llamada a read(), lo que optimiza rendimiento pero puede desperdiciar memoria si el archivo es muy grande.
En general, elegir un BUFFER_SIZE entre 10 y 42 es un buen equilibrio entre eficiencia y memoria. 🚀

### 📂 Ejemplo de archivo (file.txt)

Línea 1
Línea 2
Línea 3
Línea 4

### 🛠 Primera llamada a get_next_line(fd);


line = get_next_line(fd);
1️⃣ Inicialización
fd es el descriptor de archivo del archivo abierto.
Se declara static char *stash = NULL;
→ La primera vez que se llama, stash está vacía.
Se reserva memoria para el buffer.

2️⃣ Lectura con read(fd, buffer, BUFFER_SIZE);
Supongamos que BUFFER_SIZE = 10, el read() lee los primeros 10 caracteres de file.txt:



"Línea 1\nLí"
Como hay un \n, la línea está completa.

3️⃣ Construcción de la línea con ft_line(stash);
ft_line(stash) copia "Línea 1\n" desde stash a line y lo devuelve.

4️⃣ Actualizar stash con ft_remainder(stash);
Se llama a ft_remainder(stash), que guarda lo que queda en stash:

"Lí"

📌 Salida de la primera llamada: "Línea 1\n"

### 🛠 Segunda llamada a get_next_line(fd);


line = get_next_line(fd);
1️⃣ Inicialización
stash ya tiene "Lí" de la llamada anterior.
2️⃣ Lectura con read()
Se leen 10 caracteres más:



"nea 2\nLínea "
stash se actualiza:



"Línea 2\nLínea "
3️⃣ Construcción de la línea con ft_line(stash);
ft_line(stash) extrae "Línea 2\n".
4️⃣ Actualizar stash con ft_remainder(stash);
ft_remainder(stash) deja en stash:



"Línea "
📌 Salida de la segunda llamada: "Línea 2\n"

### 🛠 Tercera llamada a get_next_line(fd);


line = get_next_line(fd);
1️⃣ Inicialización
stash tiene "Línea " de la llamada anterior.
2️⃣ Lectura con read()
Se leen 10 caracteres más:



"3\nLínea 4"
stash se actualiza:



"Línea 3\nLínea 4"
3️⃣ Construcción de la línea con ft_line(stash);
ft_line(stash) extrae "Línea 3\n".
4️⃣ Actualizar stash con ft_remainder(stash);
stash ahora contiene:



"Línea 4"
📌 Salida de la tercera llamada: "Línea 3\n"

### 🛠 Cuarta llamada a get_next_line(fd);


line = get_next_line(fd);
1️⃣ Inicialización
stash tiene "Línea 4".
2️⃣ Lectura con read()
Se leen 10 caracteres, pero se llega al final del archivo.
Se devuelve lo que queda en stash:



"Línea 4"
3️⃣ Construcción de la línea con ft_line(stash);
ft_line(stash) extrae "Línea 4".
4️⃣ Actualizar stash con ft_remainder(stash);
ft_remainder(stash) vacía stash porque no queda nada.
### 📌 Salida de la cuarta llamada: "Línea 4"

🛠 Quinta llamada a get_next_line(fd); (EOF)


line = get_next_line(fd);
Como stash está vacío y read(fd, buffer, BUFFER_SIZE) devuelve 0 (fin de archivo), get_next_line devuelve NULL.
### 📌 Salida de la quinta llamada: NULL (Fin del archivo)

### 📌 Resumen del flujo
Llamada	buffer leído	stash antes	stash después	line devuelta
1️⃣	"Línea 1\nLí"	""	"Lí"	"Línea 1\n"
2️⃣	"nea 2\nLínea "	"Lí"	"Línea "	"Línea 2\n"
3️⃣	"3\nLínea 4"	"Línea "	"Línea 4"	"Línea 3\n"
4️⃣	EOF (Restos)	"Línea 4"	""	"Línea 4"
5️⃣	EOF	""	""	NULL

### 📌 Explicación de las funciones clave
🔹 get_next_line(fd)

Administra la lectura del archivo línea por línea.
Usa read() para obtener datos.
Usa stash para guardar datos entre llamadas.
Devuelve la línea completa encontrada.
🔹 ft_line(stash)

Extrae la línea hasta el primer \n.
🔹 ft_remainder(stash)

Guarda lo que queda en stash para la siguiente llamada.
🔹 ft_strlen(s)

Calcula la longitud de la cadena.
🔹 ft_strjoin(s1, s2)

Une stash con el nuevo contenido leído.

### 📌 Ejemplo visual del proceso
📂 Archivo de entrada (file.txt)

Línea 1
Línea 2
Línea 3
Línea 4

Llamada	Entrada desde file.txt	Salida de get_next_line(fd)
1️⃣	"Línea 1\nLí"	"Línea 1\n"
2️⃣	"nea 2\nLínea "	"Línea 2\n"
3️⃣	"3\nLínea 4"	"Línea 3\n"
4️⃣	EOF (Restos)	"Línea 4"
5️⃣	EOF	NULL
