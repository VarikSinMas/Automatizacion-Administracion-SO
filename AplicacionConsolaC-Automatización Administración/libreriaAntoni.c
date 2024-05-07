#define _CRT_SECURE_NO_WARNINGS 1

#include "libreriaAntoni.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <windows.h>
#include <iphlpapi.h>
#include <windns.h>
#include <stdbool.h>

#define MAX_IP_LENGTH 20
#define MAX_LINE_LENGTH 100

void mostrarContenidoArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    char linea[100];
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }

    fclose(archivo);
}

void mostrarAdaptadoresRed() {
    // Ejecutar el comando ipconfig para mostrar los adaptadores de red
    printf("\nAdaptadores de red:\n");
    system("ipconfig");
}

void mostrarServidoresDNS(const char* adaptadorRed) {
    char comando[100];
    sprintf(comando, "ipconfig /all | find \"%s\"", adaptadorRed);

    printf("Obteniendo configuracion de red para el adaptador %s...\n", adaptadorRed);
    printf("===============================================\n");

    // Ejecutar el comando ipconfig y capturar su salida
    FILE* fp = _popen(comando, "r");
    if (fp == NULL) {
        printf("Error al ejecutar el comando ipconfig.\n");
        return;
    }

    // Abrir el archivo temporal para escribir la direccion IP del servidor DNS
    FILE* archivoTemporal = fopen("dns_adaptador.txt", "w");
    if (archivoTemporal == NULL) {
        printf("Error al abrir el archivo temporal.\n");
        _pclose(fp);
        return;
    }

    // Leer la salida del comando linea por linea y buscar los servidores DNS
    char linea[200];
    int encontrados = 0;
    while (fgets(linea, sizeof(linea), fp) != NULL) {
        // Buscar la linea que contiene la informacion de los servidores DNS
        if (strstr(linea, "Servidores DNS") != NULL) {
            // Obtener la direccion IP del servidor DNS del adaptador
            char* inicio = strstr(linea, ": ") + 2;
            char* fin = strchr(inicio, '\n');
            *fin = '\0'; // Terminar la cadena en el primer salto de linea
            fprintf(archivoTemporal, "%s", inicio); // Escribir la direccion IP en el archivo temporal
            printf("%s\n", inicio); // Mostrar la direccion IP en la consola
            encontrados = 1;
            break;
        }
    }

    if (!encontrados) {
        printf("No se encontro informacion de servidores DNS para el adaptador %s.\n", adaptadorRed);
    }

    _pclose(fp);
    fclose(archivoTemporal); // Cerrar el archivo temporal
}


void hacerPingGuardarIPs() {
    FILE* archivoDNS = fopen("dnsips.txt", "r");
    FILE* archivoTemporal = fopen("ips_temporal.txt", "w");

    if (archivoDNS == NULL || archivoTemporal == NULL) {
        printf("Error al abrir archivos.\n");
        return;
    }

    char ip[20];

    // Leer cada direccion IP del archivo dnsips.txt y realizar ping
    printf("\nRealizando ping a las direcciones IP de los Servidores DNS proporcionados en archivo...\n");
    while (fgets(ip, sizeof(ip), archivoDNS) != NULL) {
        // Eliminar el salto de linea del final de la direccion IP
        strtok(ip, "\n");

        // Construir el comando de ping
        char comando[100];
        sprintf(comando, "ping -n 4 -w 1000 %s", ip); // Hacer 4 intentos con un tiempo de espera de 5 segundos

        printf("Comando de ping: %s\n", comando); // Mostrar el comando de ping

        // Ejecutar el comando de ping y leer la respuesta
        FILE* pipe = _popen(comando, "r");
        if (pipe == NULL) {
            printf("Error al ejecutar el comando de ping.\n");
            return;
        }

        // Leer y mostrar la salida del comando de ping
        char buffer[128];
        while (fgets(buffer, 128, pipe) != NULL) {
            printf("%s", buffer);
        }

        // Cerrar el pipe y verificar si el ping fue exitoso
        int pingExitoso = _pclose(pipe);
        if (pingExitoso != 0) {
            printf("Ping a %s: No accesible\n", ip);
        }
        else {
            // La direccion IP respondio al ping, escribirla en el archivo temporal
            fprintf(archivoTemporal, "%s\n", ip);
            printf("Ping a %s: Accesible\n", ip);
        }
    }

    fclose(archivoDNS);
    fclose(archivoTemporal);

    printf("Ping completado. Las direcciones IP que respondieron al ping han sido guardadas en ips_temporal.txt\n");
}

void compararVelocidadesDNS(const char* adaptadorRed) {
    // Abrir el archivo temporal que contiene las direcciones IP de los servidores DNS que respondieron al ping
    FILE* archivoIpsTemporal = fopen("ips_temporal.txt", "r");
    if (archivoIpsTemporal == NULL) {
        printf("Error al abrir el archivo temporal de IPs.\n");
        return;
    }

    // Abrir el archivo temporal que contiene la direccion IP del servidor DNS del adaptador seleccionado
    FILE* archivoDnsAdaptador = fopen("dns_adaptador.txt", "r");
    if (archivoDnsAdaptador == NULL) {
        printf("Error al abrir el archivo temporal de DNS del adaptador.\n");
        fclose(archivoIpsTemporal);
        return;
    }

    char ip[MAX_IP_LENGTH];
    char dnsAdaptador[MAX_IP_LENGTH];
    char dnsTemporal[MAX_IP_LENGTH];
    int velocidadMasRapida = INT_MAX;

    // Leer la direccion IP del servidor DNS del adaptador desde el archivo temporal
    if (fgets(dnsAdaptador, sizeof(dnsAdaptador), archivoDnsAdaptador) == NULL) {
        printf("Error al leer la direccion IP del servidor DNS del adaptador desde el archivo temporal.\n");
        fclose(archivoIpsTemporal);
        fclose(archivoDnsAdaptador);
        return;
    }
    strtok(dnsAdaptador, "\n"); // Eliminar el salto de linea del final de la direccion IP

    printf("Direccion DNS del adaptador seleccionado: %s\n", dnsAdaptador);

    // Leer la direccion IP del servidor DNS temporal
    if (fgets(dnsTemporal, sizeof(dnsTemporal), archivoIpsTemporal) == NULL) {
        printf("Error al leer la direccion IP del servidor DNS temporal desde el archivo temporal.\n");
        fclose(archivoIpsTemporal);
        fclose(archivoDnsAdaptador);
        return;
    }
    strtok(dnsTemporal, "\n"); // Eliminar el salto de linea del final de la direccion IP

    printf("Direccion DNS del archivo proporcionado: %s\n", dnsTemporal);

    // Obtener la velocidad del servidor DNS del adaptador
    int velocidadAdaptador = obtenerVelocidad(dnsAdaptador);
    printf("%s: %d ms\n", dnsAdaptador, velocidadAdaptador);

    // Obtener la velocidad del servidor DNS temporal
    int velocidadTemporal = obtenerVelocidad(dnsTemporal);
    printf("%s: %d ms\n", dnsTemporal, velocidadTemporal);

    // Comparar velocidades
    if (velocidadAdaptador < velocidadTemporal) {
        printf("El servidor DNS mas rapido es: %s con una velocidad de %d ms.\n", dnsAdaptador, velocidadAdaptador);
    }
    else if (velocidadAdaptador > velocidadTemporal) {
        printf("El servidor DNS mas rapido es: %s con una velocidad de %d ms.\n", dnsTemporal, velocidadTemporal);
    }
    else {
        // Si las velocidades son iguales, comparar los saltos
        printf("Las velocidades de los servidores DNS son iguales.\n");
        obtenerSaltosMostrar(dnsAdaptador);
        obtenerSaltosMostrar(dnsTemporal);
    }

    fclose(archivoIpsTemporal);
    fclose(archivoDnsAdaptador);
}


void obtenerSaltosMostrar(const char* ip) {
    char comando[100];
    sprintf(comando, "tracert %s", ip);

    FILE* pipe = _popen(comando, "r");
    if (pipe == NULL) {
        printf("Error al ejecutar el comando tracert.\n");
        return;
    }

    char linea[MAX_LINE_LENGTH];
    int saltos = INT_MAX;

    while (fgets(linea, sizeof(linea), pipe) != NULL) {
        if (strstr(linea, "saltos") != NULL) {
            char* token = strtok(linea, " ");
            while (token != NULL) {
                if (isdigit(token[0])) {
                    saltos = atoi(token);
                    break;
                }
                token = strtok(NULL, " ");
            }
            break;
        }
    }

    _pclose(pipe);

    if (saltos != INT_MAX) {
        printf("Saltos hacia %s: %d\n", ip, saltos);
    }
    else {
        printf("No se pudieron obtener los saltos hacia %s.\n", ip);
    }
}


int obtenerVelocidad(const char* ip) {
    char comando[100];
    sprintf(comando, "ping -n 4 %s", ip); // Hacer 4 intentos de ping

    FILE* pipe = _popen(comando, "r");
    if (pipe == NULL) {
        printf("Error al ejecutar el comando de ping.\n");
        return INT_MAX; // Valor de velocidad máximo para indicar error
    }

    int tiempo_total = 0;
    int intentos = 0;

    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Buscar líneas que contengan "tiempo="
        char* tiempo_pos = strstr(buffer, "tiempo=");
        if (tiempo_pos != NULL) {
            // Extraer el valor numérico después de "tiempo="
            int tiempo;
            if (sscanf(tiempo_pos, "tiempo=%dms", &tiempo) == 1) {
                tiempo_total += tiempo;
                intentos++;
            }
        }
    }

    _pclose(pipe);

    if (intentos > 0) {
        // Calcular el tiempo promedio de respuesta
        int tiempo_promedio = tiempo_total / intentos;
        return tiempo_promedio;
    }
    else {
        // No se recibieron respuestas válidas
        return INT_MAX;
    }
}




typedef struct {
    char dnsAdaptador[MAX_IP_LENGTH];
    char dnsTemporal[MAX_IP_LENGTH];
    int velocidadAdaptador;
    int velocidadTemporal;
} ResultadosVelocidadesDNS;

void actualizarDNS(const char* adaptadorRed) {
    ResultadosVelocidadesDNS resultados;

    // Obtener las velocidades y direcciones DNS utilizando compararVelocidadesDNS
    compararVelocidadesDNS(adaptadorRed, &resultados);

    if (resultados.velocidadAdaptador < resultados.velocidadTemporal) {
        printf("Se debe actualizar el DNS del adaptador %s.\n", adaptadorRed);
        // Lógica para actualizar el DNS
        if (actualizarConfiguracionDNS(adaptadorRed, resultados.dnsTemporal)) {
            printf("Se ha actualizado el servidor DNS en el adaptador %s.\n", adaptadorRed);
        }
        else {
            printf("Error al actualizar el servidor DNS en el adaptador %s.\n", adaptadorRed);
        }
    }
    else {
        printf("No se realizan cambios en el servidor DNS del adaptador %s.\n", adaptadorRed);
    }
}

bool actualizarConfiguracionDNS(const char* adaptadorRed, const char* nuevaDireccionDNS) {
    FILE* archivoTemporal;
    char direccionDNS[20];
    char comando[100];

    // Abrir el archivo temporal que contiene la dirección IP del nuevo servidor DNS
    archivoTemporal = fopen("ips_temporal.txt", "r");
    if (archivoTemporal == NULL) {
        printf("Error al abrir el archivo temporal.\n");
        return false; // Devolver falso para indicar error
    }

    // Leer la dirección IP del nuevo servidor DNS del archivo temporal
    if (fgets(direccionDNS, sizeof(direccionDNS), archivoTemporal) == NULL) {
        printf("Error al leer la direccion IP del archivo temporal.\n");
        fclose(archivoTemporal);
        return false; // Devolver falso para indicar error
    }
    strtok(direccionDNS, "\n"); // Eliminar el salto de línea al final

    fclose(archivoTemporal);

    // Construir el comando para configurar el servidor DNS del adaptador seleccionado
    sprintf(comando, "netsh interface ipv4 set dns \"%s\" static %s", adaptadorRed, nuevaDireccionDNS);

    // Ejecutar el comando para configurar el servidor DNS
    if (system(comando) != 0) {
        printf("Error al ejecutar el comando para configurar el servidor DNS.\n");
        return false; // Devolver falso para indicar error
    }

    printf("Se ha actualizado la configuracion DNS del adaptador %s a la direccion %s.\n", adaptadorRed, nuevaDireccionDNS);
    return true; // Devolver verdadero para indicar éxito
}