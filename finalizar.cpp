#include "finalizar.h"
#include "ui_finalizar.h"
#include "principal.h"

Finalizar::Finalizar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Finalizar)
{
    ui->setupUi(this);
    close();
    connect(ui->okBtm, SIGNAL(accepted()),
            this, SLOT(limpiarOk()));
}

Finalizar::~Finalizar()
{
    delete ui;
}

void Finalizar::datos(QString datos){
    ui->outResultadod->setPlainText(datos);
}

void Finalizar::productos(QString productos){
    ui->outResultadop->setPlainText(productos);
}

void Finalizar::setIva(QString iva){
    ui->outIva->setText(iva);
}

void Finalizar::setSubtotal(QString subtotal){
    ui->outSubtotal->setText(subtotal);
}

void Finalizar::setTotal(QString total){
    ui->outTotal->setText(total);
}

void Finalizar::limpiarOk()
{
    Principal *principal = new Principal(this);
    principal->limpiar();
    principal->show();

}
