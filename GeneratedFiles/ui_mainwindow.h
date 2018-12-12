/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave_ROM;
    QAction *actionNew_Tileset;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *editedTilesView;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_2;
    QPushButton *newPaletteBtn;
    QHBoxLayout *horizontalLayout;
    QComboBox *paletteCombobox;
    QToolButton *delPaletteBtn;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *palColor0;
    QPushButton *palColor1;
    QPushButton *palColor2;
    QPushButton *palColor3;
    QLabel *currentColorRgbLabel;
    QGraphicsView *paletteView;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(636, 558);
        MainWindow->setStyleSheet(QStringLiteral(""));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave_ROM = new QAction(MainWindow);
        actionSave_ROM->setObjectName(QStringLiteral("actionSave_ROM"));
        actionNew_Tileset = new QAction(MainWindow);
        actionNew_Tileset->setObjectName(QStringLiteral("actionNew_Tileset"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        editedTilesView = new QGraphicsView(centralWidget);
        editedTilesView->setObjectName(QStringLiteral("editedTilesView"));
        editedTilesView->setStyleSheet(QStringLiteral("background-color: rgb(47,47,47);"));

        gridLayout->addWidget(editedTilesView, 0, 3, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetFixedSize);
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_4->addItem(verticalSpacer_2);

        newPaletteBtn = new QPushButton(centralWidget);
        newPaletteBtn->setObjectName(QStringLiteral("newPaletteBtn"));

        verticalLayout_4->addWidget(newPaletteBtn);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        paletteCombobox = new QComboBox(centralWidget);
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->addItem(QString());
        paletteCombobox->setObjectName(QStringLiteral("paletteCombobox"));
        paletteCombobox->setStyleSheet(QStringLiteral(""));

        horizontalLayout->addWidget(paletteCombobox);

        delPaletteBtn = new QToolButton(centralWidget);
        delPaletteBtn->setObjectName(QStringLiteral("delPaletteBtn"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        delPaletteBtn->setFont(font);

        horizontalLayout->addWidget(delPaletteBtn);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        palColor0 = new QPushButton(centralWidget);
        palColor0->setObjectName(QStringLiteral("palColor0"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(palColor0->sizePolicy().hasHeightForWidth());
        palColor0->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(6);
        font1.setBold(true);
        font1.setWeight(75);
        palColor0->setFont(font1);
        palColor0->setCursor(QCursor(Qt::ArrowCursor));
        palColor0->setCheckable(true);
        palColor0->setFlat(false);

        horizontalLayout_2->addWidget(palColor0);

        palColor1 = new QPushButton(centralWidget);
        palColor1->setObjectName(QStringLiteral("palColor1"));
        sizePolicy.setHeightForWidth(palColor1->sizePolicy().hasHeightForWidth());
        palColor1->setSizePolicy(sizePolicy);
        palColor1->setFont(font1);
        palColor1->setCursor(QCursor(Qt::ArrowCursor));
        palColor1->setCheckable(true);
        palColor1->setFlat(false);

        horizontalLayout_2->addWidget(palColor1);

        palColor2 = new QPushButton(centralWidget);
        palColor2->setObjectName(QStringLiteral("palColor2"));
        sizePolicy.setHeightForWidth(palColor2->sizePolicy().hasHeightForWidth());
        palColor2->setSizePolicy(sizePolicy);
        palColor2->setFont(font1);
        palColor2->setCursor(QCursor(Qt::ArrowCursor));
        palColor2->setCheckable(true);
        palColor2->setFlat(false);

        horizontalLayout_2->addWidget(palColor2);

        palColor3 = new QPushButton(centralWidget);
        palColor3->setObjectName(QStringLiteral("palColor3"));
        sizePolicy.setHeightForWidth(palColor3->sizePolicy().hasHeightForWidth());
        palColor3->setSizePolicy(sizePolicy);
        palColor3->setFont(font1);
        palColor3->setCursor(QCursor(Qt::ArrowCursor));
        palColor3->setCheckable(true);
        palColor3->setChecked(true);
        palColor3->setFlat(false);

        horizontalLayout_2->addWidget(palColor3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        currentColorRgbLabel = new QLabel(centralWidget);
        currentColorRgbLabel->setObjectName(QStringLiteral("currentColorRgbLabel"));
        QFont font2;
        font2.setPointSize(11);
        currentColorRgbLabel->setFont(font2);

        verticalLayout_4->addWidget(currentColorRgbLabel);

        paletteView = new QGraphicsView(centralWidget);
        paletteView->setObjectName(QStringLiteral("paletteView"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(paletteView->sizePolicy().hasHeightForWidth());
        paletteView->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(paletteView);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_4->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout_4, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 636, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave_ROM);
        menuFile->addAction(actionNew_Tileset);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open ...", nullptr));
        actionSave_ROM->setText(QApplication::translate("MainWindow", "Save ROM", nullptr));
        actionNew_Tileset->setText(QApplication::translate("MainWindow", "New Tileset", nullptr));
        newPaletteBtn->setText(QApplication::translate("MainWindow", "New palette", nullptr));
        paletteCombobox->setItemText(0, QApplication::translate("MainWindow", "Palette 1", nullptr));
        paletteCombobox->setItemText(1, QApplication::translate("MainWindow", "Palette 2", nullptr));
        paletteCombobox->setItemText(2, QApplication::translate("MainWindow", "Palette 3", nullptr));
        paletteCombobox->setItemText(3, QApplication::translate("MainWindow", "Palette 4", nullptr));
        paletteCombobox->setItemText(4, QApplication::translate("MainWindow", "Palette 5", nullptr));
        paletteCombobox->setItemText(5, QApplication::translate("MainWindow", "Palette 6", nullptr));
        paletteCombobox->setItemText(6, QApplication::translate("MainWindow", "Palette 7", nullptr));
        paletteCombobox->setItemText(7, QApplication::translate("MainWindow", "Palette 8", nullptr));

        delPaletteBtn->setText(QApplication::translate("MainWindow", "X", nullptr));
        palColor0->setText(QString());
        palColor1->setText(QString());
        palColor2->setText(QString());
        palColor3->setText(QString());
        currentColorRgbLabel->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
