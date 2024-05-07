#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "libreriaAntoni.h"

int main() {
    char nombreArchivo[100]; // Variable para almacenar el nombre del archivo
    char adaptadorRed[50];
    char dnsAnterior[100] = "";
    char dnsNuevo[100] = "";

    // Pedir al usuario el nombre del archivo dnsips.txt
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombreArchivo);

    // Mostrar el contenido del archivo
    printf("\nContenido del archivo %s:\n", nombreArchivo);
    mostrarContenidoArchivo(nombreArchivo);

    // Mostrar los adaptadores de red disponibles
    printf("\nAdaptadores de red:\n");
    mostrarAdaptadoresRed();

    // Pedir al usuario el nombre del adaptador de red
    printf("\nIngrese el numero del adaptador de red que desea modificar: ");
    scanf("%s", adaptadorRed);

    // Mostrar los servidores DNS configurados en el adaptador de red especificado
    printf("\nServidores DNS configurados en el adaptador %s:\n", adaptadorRed);
    mostrarServidoresDNS(adaptadorRed);

    // Realizar ping a las direcciones IP del archivo dnsips.txt y guardar las que funcionan en un archivo temporal
    hacerPingGuardarIPs();

    // Chequear los DNS configurados y cambiarlos si es necesario
    printf("\nChequeando velocidad de los DNS configurados y obteniendo saltos hacia los servidores DNS (si se requiere)...\n");
    actualizarDNS(adaptadorRed, nombreArchivo, dnsAnterior, dnsNuevo);

    // Mostrar el resultado de la actualización del DNS
    if (strcmp(dnsAnterior, "") != 0 && strcmp(dnsNuevo, "") != 0) {
        printf("\nSe ha cambiado el servidor DNS en el adaptador %s:\n", adaptadorRed);
        printf("DNS anterior: %s\n", dnsAnterior);
        printf("Nuevo DNS: %s\n", dnsNuevo);
    }
    else {
        printf("\nEl servidor DNS actual ya era el mas rapido. No se han realizado cambios.\n");
    }

    return 0;
}
