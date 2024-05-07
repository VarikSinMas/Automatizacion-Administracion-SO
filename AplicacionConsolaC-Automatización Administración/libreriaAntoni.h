#include <stdbool.h>


void mostrarContenidoArchivo(const char* nombreArchivo);
void mostrarServidoresDNS(const char* adaptadorRed);
void leerArchivoDNS(const char* nombreArchivo);
void actualizarDNS(const char* adaptadorRed, const char* nombreArchivo, char* dnsAnterior, char* dnsNuevo); 
void mostrarAdaptadoresRed();
void hacerPingGuardarIPs();
void obtenerSaltosMostrar();
int obtenerVelocidad(const char* ip);
void compararVelocidadesDNS(const char* adaptadorRed); 
bool actualizarConfiguracionDNS(const char* adaptadorRed, const char* nuevaDireccionDNS);
