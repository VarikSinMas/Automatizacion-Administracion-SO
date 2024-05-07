# Automatizacion-Administracion-SO

Descripción 
Desarrollar una pequeña aplicación que sea capaz de modificar parte de la configuración de red de la máquina local, únicamente en lo que tiene que ver con la resolución de nombres de dominio, para un adaptador que se determinará entre los de una lista de servidores DNS escritos en un archivo de texto, en caso de que el servidor DNS actual no sea el más rápido entre los proporcionados.

Objetivos 
El objetivo principal del producto es:
Desarrollar una pequeña aplicación que sea capaz modificar la configuración de red de la máquina local cambiando el servidor DNS de esta por el más rápido entre los testeados.

Pasos a seguir
Los pasos a seguir para llevar a cabo el producto son:

Realizar una pequeña aplicación de consola escrita en C, la funcionalidad de la cual será actualizar la configuración de red en lo que se refiere a los servidores DNS para un adaptador de red dado a partir de los servidores DNS detallados en un archivo de texto, siempre y cuando el servidor DNS más rápido sea uno de los detallados en ese archivo. Para su correcta realización la aplicación debe estar: 
Correctamente modularizada.
Su código ha de estar bien comentado.
Agrupar las funciones creadas en una librería creada por ti.
Trabajar el archivo dnsips.txt de muestra proporcionado.
Preguntar inicialmente el nombre y la ruta del archivo dnsips.txt y mostrar  el contenido de este archivo por pantalla.
Preguntar al usuario el adaptador de red que quiere modificar y mostrar los servidores DNS configurados en este.
Leer el archivo proporcionado inicialmente con las IP de los servidores DNS, y chequear si esas IPs son accesibles, y en caso de que lo sean es necesario escribirlas en un archivo temporal (esto se considerará un requisito).
Abrir el archivo temporal creado en el punto anterior y chequear de entre los dns configurados en el adaptador determinado en el punto 7 y de los escritos en este archivo, cuál de estos es más rápido.
Cambiar en caso de no tener configurado el DNS más rápido, el DNS del adaptador indicado en el punto 6 por el más veloz.
En caso de que dos servidores DNS empaten en velocidad, se reconfigurará el DNS por el que esté a un menor número de saltos, puesto que se considera que la cercanía es un punto a tener en cuenta.
