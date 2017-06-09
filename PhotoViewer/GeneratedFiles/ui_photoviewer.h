/********************************************************************************
** Form generated from reading UI file 'photoviewer.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHOTOVIEWER_H
#define UI_PHOTOVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "frameswidget.h"
#include "photowidget.h"

QT_BEGIN_NAMESPACE

class Ui_PhotoViewerClass
{
public:
    QAction *actionExit;
    QAction *actionOpen;
    QAction *actionAdd;
    QAction *actionSave_as;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    PhotoWidget *pic;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *back;
    QPushButton *forward;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *ownScaleButton;
    QToolButton *showAllButton;
    QTreeWidget *treeWidget;
    FramesWidget *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PhotoViewerClass)
    {
        if (PhotoViewerClass->objectName().isEmpty())
            PhotoViewerClass->setObjectName(QString::fromUtf8("PhotoViewerClass"));
        PhotoViewerClass->resize(755, 637);
        actionExit = new QAction(PhotoViewerClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionOpen = new QAction(PhotoViewerClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionAdd = new QAction(PhotoViewerClass);
        actionAdd->setObjectName(QString::fromUtf8("actionAdd"));
        actionSave_as = new QAction(PhotoViewerClass);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        centralWidget = new QWidget(PhotoViewerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(-1, -1, 0, -1);
        pic = new PhotoWidget(centralWidget);
        pic->setObjectName(QString::fromUtf8("pic"));

        verticalLayout->addWidget(pic);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        back = new QPushButton(centralWidget);
        back->setObjectName(QString::fromUtf8("back"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(back->sizePolicy().hasHeightForWidth());
        back->setSizePolicy(sizePolicy);
        back->setLayoutDirection(Qt::LeftToRight);
        back->setAutoDefault(false);

        horizontalLayout->addWidget(back);

        forward = new QPushButton(centralWidget);
        forward->setObjectName(QString::fromUtf8("forward"));
        sizePolicy.setHeightForWidth(forward->sizePolicy().hasHeightForWidth());
        forward->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(forward);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        ownScaleButton = new QToolButton(centralWidget);
        ownScaleButton->setObjectName(QString::fromUtf8("ownScaleButton"));

        horizontalLayout->addWidget(ownScaleButton);

        showAllButton = new QToolButton(centralWidget);
        showAllButton->setObjectName(QString::fromUtf8("showAllButton"));

        horizontalLayout->addWidget(showAllButton);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        treeWidget = new QTreeWidget(centralWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        horizontalLayout_2->addWidget(treeWidget);


        verticalLayout_2->addLayout(horizontalLayout_2);

        widget = new FramesWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 201));

        verticalLayout_2->addWidget(widget);


        verticalLayout_3->addLayout(verticalLayout_2);

        PhotoViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PhotoViewerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 755, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        PhotoViewerClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(PhotoViewerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PhotoViewerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionAdd);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(PhotoViewerClass);
        QObject::connect(actionExit, SIGNAL(triggered()), PhotoViewerClass, SLOT(close()));
        QObject::connect(actionOpen, SIGNAL(triggered()), PhotoViewerClass, SLOT(chooseFiles()));
        QObject::connect(back, SIGNAL(clicked()), PhotoViewerClass, SLOT(backPicture()));
        QObject::connect(forward, SIGNAL(clicked()), PhotoViewerClass, SLOT(forwardPicture()));
        QObject::connect(actionAdd, SIGNAL(triggered()), PhotoViewerClass, SLOT(addPictures()));
        QObject::connect(actionSave_as, SIGNAL(triggered()), PhotoViewerClass, SLOT(save()));
        QObject::connect(ownScaleButton, SIGNAL(clicked()), PhotoViewerClass, SLOT(makeOwnScale()));
        QObject::connect(showAllButton, SIGNAL(clicked()), PhotoViewerClass, SLOT(showAll()));

        QMetaObject::connectSlotsByName(PhotoViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *PhotoViewerClass)
    {
        PhotoViewerClass->setWindowTitle(QApplication::translate("PhotoViewerClass", "PhotoViewer", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("PhotoViewerClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("PhotoViewerClass", "Open", 0, QApplication::UnicodeUTF8));
        actionAdd->setText(QApplication::translate("PhotoViewerClass", "Add", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("PhotoViewerClass", "Save as...", 0, QApplication::UnicodeUTF8));
        back->setText(QApplication::translate("PhotoViewerClass", "Back", 0, QApplication::UnicodeUTF8));
        forward->setText(QApplication::translate("PhotoViewerClass", "Forward", 0, QApplication::UnicodeUTF8));
        ownScaleButton->setText(QApplication::translate("PhotoViewerClass", "1:1", 0, QApplication::UnicodeUTF8));
        showAllButton->setText(QApplication::translate("PhotoViewerClass", "Show all", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("PhotoViewerClass", "Value", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("PhotoViewerClass", "Property", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("PhotoViewerClass", "Path", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("PhotoViewerClass", "Size", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget->topLevelItem(2);
        ___qtreewidgetitem3->setText(0, QApplication::translate("PhotoViewerClass", "Compression", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(3);
        ___qtreewidgetitem4->setText(0, QApplication::translate("PhotoViewerClass", "Type of File", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem5 = treeWidget->topLevelItem(4);
        ___qtreewidgetitem5->setText(0, QApplication::translate("PhotoViewerClass", "Number of picture", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem6 = treeWidget->topLevelItem(5);
        ___qtreewidgetitem6->setText(0, QApplication::translate("PhotoViewerClass", "Duration", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem7 = treeWidget->topLevelItem(6);
        ___qtreewidgetitem7->setText(0, QApplication::translate("PhotoViewerClass", "Offset(x)", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem8 = treeWidget->topLevelItem(7);
        ___qtreewidgetitem8->setText(0, QApplication::translate("PhotoViewerClass", "Offset(y)", 0, QApplication::UnicodeUTF8));
        treeWidget->setSortingEnabled(__sortingEnabled);

        menuFile->setTitle(QApplication::translate("PhotoViewerClass", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PhotoViewerClass: public Ui_PhotoViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHOTOVIEWER_H
