#include "producto.h"

QString Producto::nombre() const{
    return m_nombre;
}
void Producto::setNombre(const QString &nombre){
    m_nombre = nombre;
}
int Producto::codigo() const{
    return m_codigo;
}
void Producto::setCodigo(int codigo){
    m_codigo = codigo;
}
float Producto::precio() const{
    return m_precio;
}
void Producto::setPrecio(float precio){
    m_precio = precio;
}

Producto::Producto(QObject *parent) : QObject(parent)
{
    m_nombre = "";
    m_codigo = 0;
    m_precio = 0;
}

Producto::Producto(int codigo, QString nombre, float precio)
{
    m_nombre = nombre;
    m_codigo = codigo;
    m_precio = precio;
}
