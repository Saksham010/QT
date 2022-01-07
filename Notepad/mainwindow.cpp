#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QTextEdit>
#include <QApplication>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //Setting the window title
    QMainWindow::setWindowTitle("Notepad");
    QMainWindow::setWindowIcon(QIcon(":/new/prefix1/icons/notepad2.ico"));

    //Setting a central widget
    QTextEdit *text = new QTextEdit(this);
    QMainWindow::setCentralWidget(text);

    //
    text->setAutoFillBackground(true);

    //Making a menubar with menu bar function of QMENUBAR
    QMenu *FILE = menuBar()->addMenu("File");
    QMenu *EDIT = menuBar()->addMenu("Edit");
    QMenu *FORMAT = menuBar()->addMenu("Format");
    QMenu *HELP = menuBar()->addMenu("Help");


    //Defining actions
    QAction *open = new QAction("Open");
    QAction *save = new QAction("Save");
    QAction *saveas = new QAction("Save As");
    QAction *quit = new QAction("Quit");
    QAction *copy = new QAction("Copy");
    QAction *paste = new QAction("Paste");
    QAction *undo = new QAction("Undo");
    QAction *redo = new QAction("Redo");
    QAction *about = new QAction("About");
    QAction *about_qt = new QAction("About QT");
    QAction *add_font = new QAction("Font");
    QAction *color_dialog = new QAction("Color");


    //Adding the actions to respective menu
    FILE->addAction(open);
    FILE->addAction(save);
    FILE->addAction(saveas);
    FILE->addAction(quit);

    EDIT->addAction(copy);
    EDIT->addAction(paste);
    EDIT->addAction(undo);
    EDIT->addAction(redo);

    FORMAT->addAction(add_font);
    FORMAT->addAction(color_dialog);

    HELP->addAction(about);
    HELP->addAction(about_qt);

    //Adding icon to actions
    open->setIcon(QIcon(":/new/prefix1/icons/open.ico"));
    save->setIcon(QIcon(":/new/prefix1/icons/save.ico"));
    saveas->setIcon(QIcon(":/new/prefix1/icons/saveas.ico"));
    quit->setIcon(QIcon(":/new/prefix1/icons/exit.ico"));


    copy->setIcon(QIcon(":/new/prefix1/icons/copy.ico"));
    paste->setIcon(QIcon(":/new/prefix1/icons/paste.ico"));
    undo->setIcon(QIcon(":/new/prefix1/icons/undo.ico"));
    redo->setIcon(QIcon(":/new/prefix1/icons/redo.ico"));

    add_font->setIcon(QIcon(":/new/prefix1/icons/font.ico"));
    color_dialog->setIcon(QIcon(":/new/prefix1/icons/color.ico"));

    about->setIcon(QIcon(":/new/prefix1/icons/about.ico"));
    about_qt->setIcon(QIcon(":/new/prefix1/icons/about_qt.ico"));

    //Future reference
    static QString globalfilename = NULL;
    static bool isSaved = false;

    //Connecting the Actions to slots    

    //-1. Open
    connect(open,&QAction::triggered,[=](){
        //Setting the bool flag to true
        isSaved = true;

        //Retrieving the file name
        QString filename =QFileDialog::getOpenFileName(this,"Open");


        if(filename.isEmpty()){
            return;
        }

        //Storing for future use
        globalfilename = filename;

        //For Title of the window
        QString Main_name = url_parser(filename);
        QMainWindow::setWindowTitle(Main_name);


        QFile file(filename);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }


        QTextStream in(&file);

        //Reads the first line
        QString line = in.readLine();
        QString readData;

        while(!line.isEmpty()){

            readData.append(line);

            //Adding spaces after each line
            readData.append("\n");

            //Reading new line
            line = in.readLine();

        }

        //Closing the file
        file.close();


        //Clearing the current text
        text->clear();

        //Updating with the read text
        text->setPlainText(readData);

    });

    //2.Save
    connect(save,&QAction::triggered,[=](){

        QString filename;

        //Retrieving the filename
        if(isSaved == false){
            filename = QFileDialog::getSaveFileName(this,"Save as");
        }
        else{
            //Updating the file name with the previous filename
            filename = globalfilename;
            //Updating the globalfilename with the current file name
            globalfilename = filename;
        }

        if(filename.isEmpty()){
            return;
        }

        //For Title of the window
        QString Main_name = url_parser(filename);
        QMainWindow::setWindowTitle(Main_name);


        QFile file(filename);

        //Creating and opening the file in write mode
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;
        }

        //Writing to the file
        QTextStream out(&file);
        out<<text->toPlainText()<<"\n";

        //Changing the bool flag to true
        isSaved = true;
    });

    //3.Save As
    connect(saveas,&QAction::triggered,[=](){

        QString filename;

        //Retrieving the filename

        filename = QFileDialog::getSaveFileName(this,"Save as");

        if(filename.isEmpty()){
            return;
        }


        //For Title of the window
        QString Main_name = url_parser(filename);
        QMainWindow::setWindowTitle(Main_name);

        QFile file(filename);

        //Creating and opening the file in write mode
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;
        }

        //Writing to the file
        QTextStream out(&file);
        out<<text->toPlainText()<<"\n";

        //Changing the bool flag to true
        isSaved = true;
    });

    // 1. QUIT
    connect(quit,&QAction::triggered,[=](){
        QApplication::quit();
    });

    // 2.Copy
    connect(copy,&QAction::triggered,[=](){
        text->copy();
        statusBar()->showMessage("Copied...");
    });

    //3. Paste
    connect(paste,&QAction::triggered,[=](){
        text->paste();
        statusBar()->showMessage("Pasted...");
    });

    //4. Undo
    connect(undo,&QAction::triggered,[=](){
        text->undo();
        statusBar()->showMessage("Completed...");
    });

    //5. Redo
    connect(redo,&QAction::triggered,[=](){
        text->redo();
        statusBar()->showMessage("Completed...");
    });

    //6. Font
    connect(add_font,&QAction::triggered,[=](){
        bool ok;
        QFont font = QFontDialog::getFont(&ok,QFont("Script MT Bold",10),this);

        if(ok){
            text->setFont(font);
            statusBar()->showMessage("Font applied succesfully....",5000);
        }
        else{
            QMessageBox::information(this,"Message","User did not choose font");
        }


    });

    //7.Color
    connect(color_dialog,&QAction::triggered,[=](){

        //Current color of the text
        QColor mycolor = text->textColor();

        //QColorDialog
        QColor chosenColor = QColorDialog::getColor(mycolor,this,"Choose text color");

        if(chosenColor.isValid()){

            //Returning the returned text
            QString ret = text->toPlainText();

            //Clearing the text
            text->clear();

            //Updating to the new chosen text
            text->setTextColor(chosenColor);

            //Inserting the returned text
            text->textCursor().insertText(ret);

            statusBar()->showMessage("Color applied succesfully....",5000);

        }
        else{
            QMessageBox::information(this,"Message","User did not choose color");
        }

    });

    //8. About
    connect(about,&QAction::triggered,[=](){

       //Setting pixmap for icon
        QPixmap map(":/new/prefix1/icons/notepad2.ico");

        //Initializing messagebox
        QMessageBox message(this);

        //message.setStyleSheet("border-radius:20px;");

        //Setting Icon
        message.setIconPixmap(map);


        //Title
        message.setWindowTitle("Info");
        message.setText("<strong>Open source Notepad</strong> <hr> <strong>Developed by</strong>: <em>Saksham Thapa</em><hr><strong>Contact</strong>: <em>sakshamthapa010@gmail.com</em><hr><strong>Github</strong>: <em><a href='#'>https://github.com/Saksham010/Saksham010</a</em><hr>");



        //Status bar
        statusBar()->showMessage("About...");

        //Showing the message box
        message.exec();

    });

    //9. About QT
    connect(about_qt,&QAction::triggered,[=](){
        QApplication::aboutQt();
        statusBar()->showMessage("About QT...");

    });
}



MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::url_parser(QString fileurl)
{

    QString filename ;

    //Length of the url
    int size_l = fileurl.length();
    int end = size_l-1; //Last index

    //Flag to check '.'
    bool isStart = false;

    //Getting file name
    for(int i =end;fileurl.at(i) != '/';i--){

        if( i != end){
            if(fileurl.at(i+1) == '.'){
                isStart = true;
            }
        }

        if(isStart == true){
            filename.prepend(fileurl.at(i));
        }
    }

    return filename;
}



