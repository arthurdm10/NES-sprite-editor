#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QPushButton>


#include <array>


#include "tileview.h"

#include "palette.h"


namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT


protected:
    bool eventFilter(QObject *obj, QEvent *ev);


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void drawGrid();
    void getFileData(const QString& filename);

    void updatePaletteDemo();
    void updatePaletteView();
  

private slots:
    //MENU slots
	void on_actionOpen_triggered();
    void on_actionSave_ROM_triggered();
	void on_actionNew_Tileset_triggered();


	void on_paletteCombobox_currentIndexChanged(int index);

	void on_newPaletteBtn_clicked();
	void on_delPaletteBtn_clicked();

    void on_actionImport_triggered();

private:


	std::array<QPushButton*, 0x04>      colorDemo;
	
	bool isARomFile;

    TileView m_tileView;

    QGraphicsScene m_tileScene,
                   m_paletteScene;


    QGraphicsPixmapItem m_editedPixmapItem;
    


    QPixmap m_editedTiles;

    QPushButton* selectedColor = nullptr;


    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
