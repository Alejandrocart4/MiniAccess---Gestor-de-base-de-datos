#ifndef VISTA_PRUEBA_H
#define VISTA_PRUEBA_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabWidget>
#include <QGroupBox>
#include <QToolButton>
#include <QTableWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QFileDialog>
#include <QTextStream>
#include <QLabel>
#include <QSpacerItem>

class VistaPrueba : public QMainWindow
{
    Q_OBJECT
public:
    explicit VistaPrueba(QWidget *parent = nullptr);
    ~VistaPrueba() override = default;

private slots:
    void abrirDisenioTabla();
    void agregarFilaCampo();
    void eliminarFilaCampo();
    void guardarMeta();
    void onCeldaEditada(int, int);

private:
    // Construcción
    void construirMenus();
    void construirCinta();
    QWidget* crearPaginaCampos();
    QWidget* crearPaginaTabla();
    void construirVistaDisenio();
    void aplicarEstiloAccess();
    QToolButton* botonRibbon(const QString& texto, const QString& icono = QString());
    QGroupBox* grupoRibbon(const QString& titulo, const QList<QToolButton*>& botones);

    // Utilitarios
    QComboBox* comboTipos(QWidget *parent = nullptr) const;
    void asegurarFilaEditableAlFinal();

private:
    // Menú
    QMenu   *menuCrear = nullptr;
    QAction *actDisenioTabla = nullptr;

    // Cinta
    QWidget    *central = nullptr;
    QVBoxLayout *layCentral = nullptr;
    QTabWidget *cinta = nullptr;

    // Vista de diseño
    QFrame       *barraInferior = nullptr;
    QTableWidget *tablaDisenio = nullptr;
};

#endif // VISTA_PRUEBA_H
