#include "vista_prueba.h"
#include <QApplication>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>

VistaPrueba::VistaPrueba(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MiniAccess - Diseño de tabla");
    resize(1200, 720);

    aplicarEstiloAccess();
    construirMenus();

    // Contenedor central
    central   = new QWidget(this);
    layCentral = new QVBoxLayout(central);
    layCentral->setContentsMargins(8,8,8,8);
    layCentral->setSpacing(6);

    construirCinta();
    construirVistaDisenio();

    setCentralWidget(central);
}

/* =========================
 *  Menús (fila superior)
 * ========================= */
void VistaPrueba::construirMenus()
{
    auto *mArchivo = menuBar()->addMenu(tr("Archivo"));
    menuBar()->addMenu(tr("Inicio"));
    auto *mCrear = menuBar()->addMenu(tr("Crear"));
    menuBar()->addMenu(tr("Datos externos"));
    menuBar()->addMenu(tr("Herramientas de base de datos"));
    menuBar()->addMenu(tr("Ayuda"));

    actDisenioTabla = new QAction(tr("Diseño de tabla"), this);
    mCrear->addAction(actDisenioTabla);
    connect(actDisenioTabla, &QAction::triggered, this, &VistaPrueba::abrirDisenioTabla);

    // Acciones de archivo (placeholders)
    mArchivo->addAction(tr("Guardar"), this, []{});
    mArchivo->addAction(tr("Salir"), qApp, &QApplication::quit);
}

/* =========================
 *  Cinta estilo Access
 * ========================= */
void VistaPrueba::construirCinta()
{
    cinta = new QTabWidget(central);
    cinta->setDocumentMode(true);
    cinta->setTabPosition(QTabWidget::North);

    cinta->addTab(crearPaginaCampos(), tr("Campos de la tabla"));
    cinta->addTab(crearPaginaTabla(),  tr("Tabla"));

    layCentral->addWidget(cinta);
}

QToolButton* VistaPrueba::botonRibbon(const QString& texto, const QString& icono)
{
    auto *b = new QToolButton();
    b->setText(texto);
    b->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    b->setMinimumSize(96,72);
    b->setIconSize(QSize(32,32));

    if (!icono.isEmpty()) {
        b->setIcon(QIcon(icono));
    } else {
        QPixmap px(32,32); px.fill(QColor("#b0b0b0")); // se ve en dark
        b->setIcon(QIcon(px));
    }
    return b;
}



QGroupBox* VistaPrueba::grupoRibbon(const QString& titulo, const QList<QToolButton*>& botones)
{
    auto *g = new QGroupBox(titulo);
    auto *grid = new QGridLayout(g);
    grid->setContentsMargins(6,6,6,2);
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(6);

    int col = 0;
    for (auto *btn : botones) {
        grid->addWidget(btn, 0, col++);
    }
    // separador inferior sutil (como en Access)
    auto *sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Plain);
    sep->setStyleSheet("color:#d0d0d0;");
    grid->addWidget(sep, 1, 0, 1, col);

    return g;
}

QWidget* VistaPrueba::crearPaginaCampos()
{
    auto *p = new QWidget();
    auto *h = new QHBoxLayout(p);
    h->setContentsMargins(8,8,8,8);
    h->setSpacing(14);

    // Grupos como en Access
    auto *gProp = grupoRibbon(tr("Propiedades"), {
                                                     botonRibbon(tr("Nombre y título")),
                                                     botonRibbon(tr("Tamaño del campo")),
                                                     botonRibbon(tr("Valor predeterminado"))
                                                 });

    auto *gBusq = grupoRibbon(tr("Búsquedas / Índices"), {
                                                             botonRibbon(tr("Modificar búsquedas")),
                                                             botonRibbon(tr("Índices (B/B+/B*)"))
                                                         });

    auto *gFormato = grupoRibbon(tr("Formato"), {
                                                    botonRibbon(tr("Formato de datos")),
                                                    botonRibbon(tr("Más formatos"))
                                                });

    auto *gVal = grupoRibbon(tr("Validación"), {
                                                   botonRibbon(tr("Requerido")),
                                                   botonRibbon(tr("Único")),
                                                   botonRibbon(tr("Validación de campo"))
                                               });

    h->addWidget(gProp);
    h->addWidget(gBusq);
    h->addWidget(gFormato);
    h->addWidget(gVal);
    h->addStretch(1);

    return p;
}

QWidget* VistaPrueba::crearPaginaTabla()
{
    auto *p = new QWidget();
    auto *h = new QHBoxLayout(p);
    h->setContentsMargins(8,8,8,8);
    h->setSpacing(14);

    auto *gTabla = grupoRibbon(tr("Tabla"), {
                                                botonRibbon(tr("Guardar tabla")),
                                                botonRibbon(tr("Relaciones")),
                                                botonRibbon(tr("Propiedades"))
                                            });

    h->addWidget(gTabla);
    h->addStretch(1);
    return p;
}

/* =========================
 *  Vista central (diseño)
 * ========================= */
void VistaPrueba::construirVistaDisenio()
{
    // Barra delgada separadora como Access
    auto *sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Plain);
    sep->setStyleSheet("color:#cfcfcf;");
    layCentral->addWidget(sep);

    // Barra inferior con acciones rápidas (Agregar / Eliminar / Guardar)
    barraInferior = new QFrame(central);
    auto *h = new QHBoxLayout(barraInferior);
    h->setContentsMargins(0,0,0,0);
    h->setSpacing(8);

    auto *btnAgregar = new QPushButton(tr("Agregar campo"), barraInferior);
    auto *btnEliminar = new QPushButton(tr("Eliminar campo"), barraInferior);
    auto *spacer = new QWidget(); spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto *btnGuardar = new QPushButton(tr("Guardar meta"), barraInferior);

    h->addWidget(btnAgregar);
    h->addWidget(btnEliminar);
    h->addWidget(spacer);
    h->addWidget(btnGuardar);

    layCentral->addWidget(barraInferior);

    // Tabla de diseño
    tablaDisenio = new QTableWidget(central);
    tablaDisenio->setColumnCount(3);
    tablaDisenio->setHorizontalHeaderLabels({tr("Nombre del campo"), tr("Tipo de datos"), tr("Descripción")});
    tablaDisenio->horizontalHeader()->setStretchLastSection(true);
    tablaDisenio->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tablaDisenio->horizontalHeader()->setHighlightSections(false);
    tablaDisenio->verticalHeader()->setVisible(false);
    tablaDisenio->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tablaDisenio->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaDisenio->setSelectionMode(QAbstractItemView::SingleSelection);
    tablaDisenio->setAlternatingRowColors(true);
    tablaDisenio->setShowGrid(true);
    tablaDisenio->setGridStyle(Qt::SolidLine);
    tablaDisenio->setStyleSheet("QTableWidget { gridline-color:#e0e0e0; }");
    tablaDisenio->setRowCount(1);
    tablaDisenio->setItem(0,0,new QTableWidgetItem());
    tablaDisenio->setCellWidget(0,1, comboTipos(tablaDisenio));
    tablaDisenio->setItem(0,2,new QTableWidgetItem());
    tablaDisenio->setRowHeight(0, 26);

    tablaDisenio->setStyleSheet(
        "QTableWidget::item:selected{background:#0a62ff;color:#ffffff;}"
        "QTableWidget{selection-background-color:#0a62ff; selection-color:#ffffff;}"
        );

    layCentral->addWidget(tablaDisenio, 1);

    // Conectar acciones
    connect(btnAgregar, &QPushButton::clicked, this, &VistaPrueba::agregarFilaCampo);
    connect(btnEliminar, &QPushButton::clicked, this, &VistaPrueba::eliminarFilaCampo);
    connect(btnGuardar, &QPushButton::clicked, this, &VistaPrueba::guardarMeta);
    connect(tablaDisenio, &QTableWidget::cellChanged, this, &VistaPrueba::onCeldaEditada);
}

void VistaPrueba::aplicarEstiloAccess()
{
    // Tema oscuro tipo Access “dark”
    qApp->setStyleSheet(R"CSS(
        /* Fondo general */
        QMainWindow, QWidget { background:#1f2125; color:#e6e6e6; }

        /* Menú superior */
        QMenuBar { background:#1f2125; border-bottom:1px solid #2b2e34; }
        QMenuBar::item { padding:6px 12px; color:#e6e6e6; }
        QMenuBar::item:selected { background:#2a2e35; border:1px solid #3b4250; }

        /* Pestañas (cinta) */
        QTabWidget::pane { border:1px solid #2b2e34; background:#1f2125; }
        QTabBar::tab {
            background:#262a31; color:#dcdcdc;
            border:1px solid #2f333b; padding:8px 14px; margin-right:2px;
        }
        QTabBar::tab:selected {
            background:#2e3340; border-color:#44506b; color:#ffffff;
        }

        /* Grupos de la cinta */
        QGroupBox {
            background:#24282f; color:#d7d7d7;
            border:1px solid #2f333b; border-radius:4px;
            margin-top:24px;
        }
        QGroupBox::title {
            subcontrol-origin: margin; subcontrol-position: top center;
            padding:0 6px; color:#c8c8c8; font-weight:600;
        }

        /* Botones grandes estilo ribbon */
        QToolButton {
            background:#2a2e35; color:#e6e6e6;
            border:1px solid #3a3f47; border-radius:3px;
            padding:6px;
        }
        QToolButton:hover { background:#333945; border-color:#4a5870; }
        QToolButton:pressed { background:#394152; }

        /* Botones normales */
        QPushButton {
            background:#2a2e35; color:#e6e6e6;
            border:1px solid #3a3f47; padding:6px 12px; border-radius:3px;
        }
        QPushButton:hover { background:#333945; border-color:#4a5870; }
        QPushButton:pressed { background:#394152; }

        /* Tabla de diseño */
        QHeaderView::section {
            background:#2a2e35; color:#f0f0f0;
            border:1px solid #3a3f47; padding:6px; font-weight:600;
        }
        QTableWidget {
            background:#1f2125; alternate-background-color:#22252b;
            color:#e6e6e6; gridline-color:#333945;
            selection-background-color:#0a62ff;   /* azul Access-like */
            selection-color:#ffffff;
        }
        QTableWidget::item:selected { background:#0a62ff; color:#ffffff; }

        /* Celdas de edición */
        QLineEdit, QTextEdit, QPlainTextEdit {
            background:#24282f; color:#e6e6e6; border:1px solid #3a3f47; border-radius:3px;
            selection-background-color:#0a62ff; selection-color:#ffffff;
        }

        /* ComboBox + desplegable oscuro */
        QComboBox {
            background:#24282f; color:#e6e6e6; border:1px solid #3a3f47; border-radius:3px;
            padding:3px 6px;
        }
        QComboBox::drop-down { border-left:1px solid #3a3f47; width:18px; }
        QComboBox QAbstractItemView {
            background:#24282f; color:#e6e6e6; selection-background-color:#0a62ff;
            selection-color:#ffffff; outline:0; border:1px solid #3a3f47;
        }

        /* Separadores sutiles */
        QFrame[frameShape="4"] { color:#2b2e34; } /* QFrame::HLine */
    )CSS");
}


/* =========================
 *  Utilitarios
 * ========================= */
QComboBox* VistaPrueba::comboTipos(QWidget *parent) const
{
    auto *cb = new QComboBox(parent);
    cb->addItems({"int","float","bool","char[N]","string","fecha","moneda"});
    cb->setEditable(false);
    return cb;
}

void VistaPrueba::asegurarFilaEditableAlFinal()
{
    int last = tablaDisenio->rowCount() - 1;
    if (last < 0) return;
    auto *it = tablaDisenio->item(last,0);
    bool vacia = (!it) || it->text().trimmed().isEmpty();
    if (!vacia) {
        int r = tablaDisenio->rowCount();
        tablaDisenio->insertRow(r);
        tablaDisenio->setItem(r,0,new QTableWidgetItem());
        tablaDisenio->setCellWidget(r,1, comboTipos(tablaDisenio));
        tablaDisenio->setItem(r,2,new QTableWidgetItem());
        tablaDisenio->setRowHeight(r, 26);
    }
}

/* =========================
 *  Slots
 * ========================= */
void VistaPrueba::abrirDisenioTabla()
{
    cinta->setCurrentIndex(0);              // “Campos de la tabla”
    if (tablaDisenio)
        tablaDisenio->setCurrentCell(0,0);
}

void VistaPrueba::agregarFilaCampo()
{
    int r = tablaDisenio->rowCount();
    tablaDisenio->insertRow(r);
    tablaDisenio->setItem(r,0,new QTableWidgetItem());
    tablaDisenio->setCellWidget(r,1, comboTipos(tablaDisenio));
    tablaDisenio->setItem(r,2,new QTableWidgetItem());
    tablaDisenio->setRowHeight(r, 26);
    tablaDisenio->setCurrentCell(r,0);
}

void VistaPrueba::eliminarFilaCampo()
{
    int fila = tablaDisenio->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, tr("Eliminar campo"), tr("Selecciona una fila."));
        return;
    }
    if (tablaDisenio->rowCount() == 1) {
        tablaDisenio->setItem(0,0,new QTableWidgetItem());
        tablaDisenio->setCellWidget(0,1, comboTipos(tablaDisenio));
        tablaDisenio->setItem(0,2,new QTableWidgetItem());
        return;
    }
    tablaDisenio->removeRow(fila);
}

void VistaPrueba::guardarMeta()
{
    QString ruta = QFileDialog::getSaveFileName(
        this, tr("Guardar metadatos"), QString(), tr("Metadatos (*.meta);;Texto (*.txt)")
        );
    if (ruta.isEmpty()) return;

    QFile f(ruta);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo abrir el archivo destino."));
        return;
    }
    QTextStream out(&f);
    out << "# MiniAccess meta (nombre,tipo,descripcion)\n";
    for (int r=0; r<tablaDisenio->rowCount(); ++r) {
        auto *nom = tablaDisenio->item(r,0);
        auto *des = tablaDisenio->item(r,2);
        auto *cb  = qobject_cast<QComboBox*>(tablaDisenio->cellWidget(r,1));
        const QString sNom = nom? nom->text().trimmed(): "";
        const QString sDes = des? des->text().trimmed(): "";
        const QString sTip = cb? cb->currentText(): "";
        if (sNom.isEmpty() && sDes.isEmpty() && sTip.isEmpty())
            continue;
        out << sNom << "," << sTip << "," << sDes << "\n";
    }
    f.close();
    QMessageBox::information(this, tr("Guardado"), tr("Metadatos guardados."));
}

void VistaPrueba::onCeldaEditada(int, int)
{
    asegurarFilaEditableAlFinal();
}
