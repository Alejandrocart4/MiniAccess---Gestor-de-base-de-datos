#ifndef VISTA_PRUEBA_H
#define VISTA_PRUEBA_H

#include <QWidget>
#include <QStyledItemDelegate>

class QListWidget;
class QLineEdit;
class QTabBar;
class QToolBar;
class QTableWidget;
class QStatusBar;
class QLabel;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QMenu;

// Delegate que muestra un QComboBox para el tipo de dato
class CampoTipoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CampoTipoDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
private:
    QStringList tipos() const;
};

class VistaPrueba : public QWidget
{
    Q_OBJECT
public:
    explicit VistaPrueba(QWidget *parent = nullptr);
    ~VistaPrueba() override = default;

private:
    void construirUI();
    void poblarDemo();
    void construirCintaInicio();     // acciones de "Inicio"
    void construirCintaCrear();      // acciones de "Crear"
    void conectarEventos();

    // Izquierda
    QListWidget *panelTablas{};
    QLineEdit   *buscadorTablas{};

    // Superior (cinta)
    QTabBar     *tabCinta{};
    QToolBar    *barraCinta{};

    // Centro
    QTableWidget *tabla{};

    // Menú contextual de tipos
    QMenu       *menuTipos{};

    // Derecha (propiedades mínimas)
    QWidget     *panelDerecho{};
    QComboBox   *cbTipoDato{};
    QSpinBox    *spTamCampo{};
    QComboBox   *cbFormato{};
    QCheckBox   *chkRequerido{};
    QCheckBox   *chkUnico{};
    QCheckBox   *chkIndexado{};

    // Inferior
    QStatusBar  *status{};
    QLineEdit   *busquedaRapida{};
};

#endif // VISTA_PRUEBA_H

