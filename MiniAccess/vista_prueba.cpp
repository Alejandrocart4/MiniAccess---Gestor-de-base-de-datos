#include "vista_prueba.h"

#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTabBar>
#include <QToolBar>
#include <QAction>
#include <QTableWidget>
#include <QHeaderView>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFrame>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QMenu>
#include <QMouseEvent>
#include <QApplication>

// ---------- Implementación del delegate ----------

CampoTipoDelegate::CampoTipoDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QStringList CampoTipoDelegate::tipos() const {
    return {"int","float","bool","char[N]","string","fecha","moneda"};
}

QWidget *CampoTipoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                                         const QModelIndex &index) const {
    // Solo permitir el combo en la columna "Haga clic para agregar" (columna 1)
    if (index.column() != 1) return nullptr;
    auto *cb = new QComboBox(parent);
    cb->addItems(tipos());
    cb->setEditable(false);
    return cb;
}

void CampoTipoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *cb = qobject_cast<QComboBox*>(editor);
    if (!cb) return;
    const QString value = index.data(Qt::EditRole).toString();
    const int pos = cb->findText(value);
    cb->setCurrentIndex(pos >= 0 ? pos : 0);
}

void CampoTipoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const {
    auto *cb = qobject_cast<QComboBox*>(editor);
    if (!cb) return;
    model->setData(index, cb->currentText(), Qt::EditRole);
}

// ---------- Utilidad ----------
static QFrame* separadorLinea(Qt::Orientation o = Qt::Horizontal) {
    auto *s = new QFrame();
    s->setFrameShape(o == Qt::Horizontal ? QFrame::HLine : QFrame::VLine);
    s->setFrameShadow(QFrame::Sunken);
    return s;
}

// ---------- VistaPrueba ----------

VistaPrueba::VistaPrueba(QWidget *parent)
    : QWidget(parent)
{
    construirUI();
    poblarDemo();
    conectarEventos();
}

void VistaPrueba::construirUI()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(6,6,6,6);
    root->setSpacing(8);

    // ---------------- IZQUIERDA ----------------
    auto *leftWrap = new QWidget();
    auto *left = new QVBoxLayout(leftWrap);
    left->setSpacing(6);

    auto *lblTodos = new QLabel("Todos los objetos");
    lblTodos->setStyleSheet("font-weight:600;");

    buscadorTablas = new QLineEdit();
    buscadorTablas->setPlaceholderText("Buscar tablas...");
    buscadorTablas->setClearButtonEnabled(true);

    panelTablas = new QListWidget();
    panelTablas->setMinimumWidth(220);
    panelTablas->setAlternatingRowColors(true);

    auto *btnNuevaTabla = new QPushButton("+ Tabla");

    left->addWidget(lblTodos);
    left->addWidget(buscadorTablas);
    left->addWidget(panelTablas, 1);
    left->addWidget(btnNuevaTabla);

    root->addWidget(leftWrap, 2);

    // ---------------- CENTRO (CINTA + TABLA) ----------------
    auto *centerWrap = new QWidget();
    auto *center = new QVBoxLayout(centerWrap);
    center->setSpacing(6);

    // Cinta: tabs + toolbar
    tabCinta = new QTabBar();
    tabCinta->addTab("Inicio");
    tabCinta->addTab("Crear");
    tabCinta->addTab("Datos externos");
    tabCinta->addTab("Herramientas de base de datos");
    tabCinta->addTab("Ayuda");
    tabCinta->addTab("Campos de la tabla");
    tabCinta->addTab("Tabla");
    tabCinta->setExpanding(false);

    barraCinta = new QToolBar();
    barraCinta->setIconSize(QSize(16,16));
    construirCintaInicio(); // por defecto

    auto *lineSup = separadorLinea(Qt::Horizontal);

    // Tabla central
    tabla = new QTableWidget();
    tabla->setColumnCount(2);
    tabla->setHorizontalHeaderLabels({"Id", "Haga clic para agregar"});
    tabla->horizontalHeader()->setStretchLastSection(true);
    tabla->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tabla->verticalHeader()->setVisible(false);
    tabla->setAlternatingRowColors(true);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setEditTriggers(QAbstractItemView::AllEditTriggers);
    tabla->setShowGrid(true);

    // Delegate para el ComboBox en columna 1
    tabla->setItemDelegateForColumn(1, new CampoTipoDelegate(tabla));

    // Menú contextual con los mismos tipos (clic derecho)
    menuTipos = new QMenu(this);
    for (const auto &t : QStringList({"int","float","bool","char[N]","string","fecha","moneda"})) {
        QAction *act = menuTipos->addAction(t);
        connect(act, &QAction::triggered, this, [=](){
            auto idx = tabla->currentIndex();
            if (idx.isValid() && idx.column() == 1)
                tabla->model()->setData(idx, t, Qt::EditRole);
        });
    }

    // Barra de estado
    status = new QStatusBar();
    status->showMessage("Registro: 0 de 0");
    auto *lblBuscar = new QLabel("Buscar:");
    busquedaRapida = new QLineEdit();
    busquedaRapida->setPlaceholderText("Buscar en la tabla...");
    status->addPermanentWidget(lblBuscar);
    status->addPermanentWidget(busquedaRapida, 1);

    center->addWidget(tabCinta);
    center->addWidget(barraCinta);
    center->addWidget(lineSup);
    center->addWidget(tabla, 1);
    center->addWidget(status);

    root->addWidget(centerWrap, 7);

    // ---------------- DERECHA (propiedades resumidas) ----------------
    panelDerecho = new QWidget();
    auto *right = new QVBoxLayout(panelDerecho);
    right->setSpacing(6);

    auto *lblQue = new QLabel("¿Qué desea hacer?");
    lblQue->setStyleSheet("font-weight:600;");

    auto *gbProps = new QGroupBox("Propiedades de campo");
    auto *formProps = new QFormLayout(gbProps);

    cbTipoDato = new QComboBox(); cbTipoDato->addItems({"int","float","bool","char[N]","string","fecha","moneda"});
    spTamCampo = new QSpinBox();  spTamCampo->setRange(1,1024); spTamCampo->setValue(255);
    cbFormato  = new QComboBox(); cbFormato->addItems({"—","DD-MM-YY","DD/MM/YY","DD/MESTEXTO/YYYY","Lps","$","€","Millares"});
    chkRequerido = new QCheckBox("Requerido");
    chkUnico     = new QCheckBox("Único");
    chkIndexado  = new QCheckBox("Indexado");

    formProps->addRow("Tipo de datos:", cbTipoDato);
    formProps->addRow("Tamaño de campo:", spTamCampo);
    formProps->addRow("Formato:", cbFormato);
    formProps->addRow(chkRequerido);
    formProps->addRow(chkUnico);
    formProps->addRow(chkIndexado);

    right->addWidget(lblQue);
    right->addWidget(gbProps);
    right->addStretch(1);

    root->addWidget(panelDerecho, 3);

    // -------- Estilos --------
    setStyleSheet(R"(
        QToolBar { border: none; }
        QTabBar::tab { padding: 6px 12px; }
        QTabBar::tab:selected { font-weight:600; }
        QHeaderView::section {
            background: #1f1f1f; color: #eaeaea;
            padding: 6px; border: 1px solid #3a3a3a;
        }
        QTableWidget { gridline-color: #3a3a3a; }
        QGroupBox { font-weight:600; margin-top: 8px; }
        QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 4px; }
    )");
}

void VistaPrueba::construirCintaInicio()
{
    barraCinta->clear();
    // Portapapeles
    barraCinta->addAction("Pegar");
    barraCinta->addSeparator();
    // Orden y filtro
    barraCinta->addAction("Cortar");
    barraCinta->addAction("Copiar");
    barraCinta->addSeparator();
    barraCinta->addAction("Ascendente");
    barraCinta->addAction("Descendente");
    barraCinta->addAction("Filtro");
    barraCinta->addSeparator();
    // Registro
    barraCinta->addAction("Nuevo");
    barraCinta->addAction("Guardar");
    barraCinta->addAction("Eliminar");
    barraCinta->addSeparator();
    // Buscar
    barraCinta->addAction("Buscar");
    barraCinta->addAction("Ir a");
    barraCinta->addAction("Seleccionar");
}

void VistaPrueba::construirCintaCrear()
{
    barraCinta->clear();
    barraCinta->addAction("Tabla");
    barraCinta->addAction("Vista de tabla");
    barraCinta->addAction("Índice B");
    barraCinta->addAction("Índice B+");
    barraCinta->addAction("Índice B*");
}

void VistaPrueba::poblarDemo()
{
    // Tablas ejemplo
    panelTablas->addItem("Tabla1");
    panelTablas->addItem("TablaClientes");
    panelTablas->addItem("TablaCursos");

    // Fila "(Nuevo)"
    tabla->setRowCount(1);
    tabla->setItem(0, 0, new QTableWidgetItem("(Nuevo)"));
    tabla->setItem(0, 1, new QTableWidgetItem("")); // aquí se abrirá el combo

    status->showMessage(QString("Registro: %1 de %2").arg(1).arg(1));
}

void VistaPrueba::conectarEventos()
{
    // Cambiar acciones mostradas según pestaña de cinta
    connect(tabCinta, &QTabBar::currentChanged, this, [this](int idx){
        const QString t = tabCinta->tabText(idx);
        if (t == "Inicio") construirCintaInicio();
        else if (t == "Crear") construirCintaCrear();
        else { barraCinta->clear(); barraCinta->addAction(t); } // placeholders
    });

    // Abrir combo automáticamente al hacer clic en columna "Haga clic para agregar"
    connect(tabla, &QTableWidget::cellActivated, this, [this](int row, int col){
        if (col == 1) {
            tabla->editItem(tabla->item(row, col)); // dispara el delegate (QComboBox)
        }
    });

    // Menú contextual con tipos (clic derecho)
    tabla->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabla, &QTableWidget::customContextMenuRequested, this, [this](const QPoint &pos){
        QModelIndex idx = tabla->indexAt(pos);
        if (idx.isValid() && idx.column() == 1) {
            menuTipos->exec(tabla->viewport()->mapToGlobal(pos));
        }
    });
}

