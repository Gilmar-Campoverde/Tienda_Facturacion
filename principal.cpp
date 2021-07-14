#include "principal.h"
#include "ui_principal.h"
#include <QDebug>

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Lista de productos
    m_productos.append(new Producto(1,"Leche",0.85));
    m_productos.append(new Producto(2,"Pan",0.15));
    m_productos.append(new Producto(3,"Queso",2.00));
    // Mostrar la lista en la ventana
    for (int i = 0; i < m_productos.size(); ++i) {
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    }
    // Colocar cabecera de la tabla
    QStringList cabecera = {"Cantidad", "Producto", "Subtotal"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    // Inicializar subtotal global
    m_subtotal = 0;
    connect(ui->cmdFinalizar, SIGNAL(clicked(bool)),
            this, SLOT(finalizar()));
    close();
}

Principal::~Principal()
{
    delete ui;
}

void Principal::limpiar()
{
    ui->inNombre->setText("");
    ui->inCedula->setText("");
    ui->inEmail->setText("");
    ui->inCantidad->setValue(0);
    ui->inTelefono->setText("");
    ui->inDireccion->setPlainText("");
    ui->outDetalle->setRowCount(0);
}

QString Principal::getDatos()
{
    return  "Cédula: " + ui->inCedula->displayText() + "\n" +
            "\nNombre: " + ui->inNombre->displayText() + "\n" +
            "\nTeléfono: " + ui->inTelefono->displayText() + "\n" +
            "\nE-mail: " + ui->inEmail->displayText() + "\n" +
            "\nDirección: " + ui->inDireccion->toPlainText()+ "\n"+
            "\n========================================================\n"+
            "\nCantidad              Producto               Subtotal\n"+
            "\n========================================================";
}

void Principal::validarCedula()
{
    QString cedula = ui->inCedula->text();
    if (cedula.length() != 10){
        ui->inCedula->setStyleSheet("background-color: rgb(255, 123, 111);;");
    }else if(cedula.length() == 10 && cedula == "9999999999"){
        ui->inCedula->setStyleSheet("background-color:  rgb(147, 255, 172);;");
        ui->inNombre->setText("Consumidor Final");
    }else{
        ui->inCedula->setStyleSheet("background-color:  rgb(147, 255, 172);;");
    }
}

bool Principal::validCedula(QString cedula){
    QString temp[10];
    int aux;
    int sPar = 0;
    int sImpar = 0;
    for(int i = 0; i < 9; i++){
        temp[i] = cedula[i];
        aux = temp[i].toInt();
        if((i+1)%2==0){
            sPar += aux;
        }else{
            if(aux * 2 > 9){
                sImpar = sImpar + ((aux * 2) - 9);
            }else{
                sImpar += aux * 2;
            }
        }
    }
    int ultimoCedula = cedula.toInt()%10;
    int verificador = (sPar + sImpar)%10;
    if(verificador !=0){
        verificador = 10 - verificador;
    }
    return ultimoCedula == verificador? true : false;
}

void Principal::validarNombre()
{
    QString nombre = ui->inNombre->text();
    if (nombre.isEmpty()){
        ui->inNombre->setStyleSheet("background-color: rgb(255, 123, 111);;");      //Color rojo
    }else{
        ui->inNombre->setStyleSheet("background-color: rgb(147, 255, 172);;");      //Color verde
    }
}

void Principal::finalizar()
{
    Finalizar *btFinalizar = new Finalizar();

    QStringList lista;
    QString productos;

    for( int fila = 0; fila < ui->outDetalle->rowCount(); ++fila){
        for( int columna = 0; columna < ui->outDetalle->columnCount(); ++columna){
            lista << ui->outDetalle->item(fila, columna)->text();                       //Da la lista de Strings

            QTableWidgetItem* item = ui->outDetalle->item(fila,columna);
            if (!item || item->text().isEmpty()){
                ui->outDetalle->setItem(fila,columna,new QTableWidgetItem("0"));
            }
            //Implementa los productos ingresados en forma de lista con fila y columna
            productos = lista.join( " " );
            lista<<"                                         ";
        }
        lista << "   ";
        btFinalizar->productos(productos);                          //implementacion de los productos en la pantalla final
    }

    btFinalizar->datos(getDatos());                                 //Invoca la funcione que contiene los datos
    btFinalizar->setSubtotal(ui->outSubtotal->text());
    btFinalizar->setIva(ui->outIva->text());
    btFinalizar->setTotal(ui->outTotal->text());
    btFinalizar->show();                                            //Muestra en pantalla los datos anteriormente colocados/llamados
}

void Principal::on_cmdAgregar_released()
{
    // Validar que no se agregen productos con 0 cantidad
    int cantidad = ui->inCantidad->value();
    if (cantidad == 0){
        return;
    }
    // Obtener datos de la GUI
    int index = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(index);


    // Calcular el subtotal del producto
    float subtotal = cantidad * p->precio();

    // Agregar datos a la tabla
    int fila = ui->outDetalle->rowCount();
    ui->outDetalle->insertRow(fila);
    ui->outDetalle->setItem(fila,0,new QTableWidgetItem(QString::number(cantidad)));
    ui->outDetalle->setItem(fila,1,new QTableWidgetItem(p->nombre()));
    ui->outDetalle->setItem(fila,2,new QTableWidgetItem(QString::number(subtotal,'f',2)));

    // Limpiar datos
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();

    // Invocar a calcular
    calcular(subtotal);

}

void Principal::on_inProducto_currentIndexChanged(int index)
{
    // Obtener el precio del producto
    float precio = m_productos.at(index)->precio();
    // Mostrar el precio
    ui->outPrecio->setText("$ " + QString::number(precio,'f',2));
}

void Principal::calcular(float stProducto)
{
    m_subtotal += stProducto;
    float iva = m_subtotal * IVA / 100;
    float total = m_subtotal + iva;

    ui->outSubtotal->setText(QString::number(m_subtotal,'f',2));
    ui->outIva->setText(QString::number(iva,'f',2));
    ui->outTotal->setText(QString::number(total,'f',2));
}

void Principal::producto()
{
    int index = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(index);
    int fila = ui->outDetalle->rowCount();
    ui->outDetalle->setItem(fila,1,new QTableWidgetItem(p->nombre()));
}

void Principal::on_inNombre_editingFinished(){
    validarNombre();
}
void Principal::on_inCedula_editingFinished(){
    validarCedula();
}
