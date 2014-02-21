#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QStandardItemModel>
#include <QTime>

#include "HarrixQtLibrary.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //Сборка проекта Harrix Library
    QTime myTimer;
    myTimer.start();//запуск таймера работы сборки библиотеки

    ui->pushButton->setEnabled(false);

    ui->textEdit->clear();

    QString DS="\\";
    QString path=QDir::toNativeSeparators(QGuiApplication::applicationDirPath())+DS+".."+DS+"source_library"+DS;//путь к папке
    QString temp_library_path;//папка где находятся собранные файлы
    temp_library_path=QDir::toNativeSeparators(QGuiApplication::applicationDirPath())+DS+".."+DS+"temp_library"+DS;//путь к папке

    if (!(QDir(temp_library_path).exists()==true) )
    {
        QDir().mkdir(temp_library_path);
    }

    if (!(QDir(temp_library_path+DS+"images"+DS).exists()==true) )
    {
        QDir().mkdir(temp_library_path+DS+"images"+DS);
    }

    QString ResultCpp;//итоговый cpp документ
    QString ResultTpp;//сюда пишем шаблоны, которые потом воткнем в h файл
    QString ResultH;//итоговый h файл
    QString ResultTex;//итоговый cpp документ
    QString ResultTexList;//временный список функций
    QString ResultTexFunctions;//временная перемнная для справки по функциям
    QString Temp;//переменная для временного содержания загружаемых файлов
    QString MessageError;//Текущая ошиьбка
    QString AllMessageError;//Все ошибки
    int countoferrors=0;//количество ошибок
    int countneed;//количество рассмотренных нами файлов
    int countcpp=0;//количество функций в виде cpp файлов
    int counth=0;//количество функций в виде h файлов
    int counttex=0;//количество функций в виде tex файлов
    int countdesc=0;//количество функций в виде desc файлов
    int countpng=0;//количество функций в виде png файлов
    int countpdf=0;//количество функций в виде pdf файлов
    int countuse=0;//количество функций в виде use файлов

    int countfunction=0;//количество функций

    bool b;
    ui->textEdit->insertHtml("<font color=\"#858585\">Начало формирования файлов библиотеки...<\font><br>");

    ResultTpp += "//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    ResultTpp += "// РЕАЛИЗАЦИЯ ШАБЛОНОВ\n";//добавляем название папки (раздела)
    ResultTpp += "//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";

    ResultH += "//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    ResultH += "// ОБЪЯВЛЕНИЯ ФУНКЦИЙ\n";//добавляем название папки (раздела)
    ResultH += "//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";

    //дополнительные переменные
    QString dirname,filename,path2,exp;
    QStringList listfiles,listdirs;
    int i,j,n,m;

    //Теперь начинаем пробегать по всем папкам
    listdirs = HQt_ListDirsInDirQStringList(path);//список папок

    n=listdirs.count();//количество папок разделов
    ui->textEdit->insertHtml("<br>Было найдено  <b>"+QString::number(n)+"</b> папок - разделов библиотеки<br>");
    if (n==0) {MessageError="<font color=\"red\">Не найдены разделы библиотек. Это очень плохо.<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

    ResultTexList="\\newpage\n\\section{Список функций}\\label{section_listfunctions}\n";
    ResultTexFunctions="\\newpage\n\\section{Функции}\n";

    for (i=0;i<n;i++)//пробегаем по всем разделам
    {
        dirname=listdirs.at(i);//получаем имя папки
        path2=path+dirname+"/";
        ui->textEdit->insertHtml("<br>Рассматриваем папку <b>"+dirname+"</b>");

        listfiles=HQt_ListFilesInDirQStringList(path2);//список файлов в папке

        m=listfiles.count();//количество файлов в папке
        ui->textEdit->insertHtml("<br>Было найдено  <b>"+QString::number(m)+"</b> файлов в папке<br><br>");
        if (m==0) {MessageError="<font color=\"red\">Не найдены файлы в разделе. То есть в разделе нет функций?<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}


        ResultCpp += "//*****************************************************************\n";
        ResultCpp += "//"+dirname+"\n";//добавляем название папки (раздела)
        ResultCpp += "//*****************************************************************\n";

        ResultTpp += "//*****************************************************************\n";
        ResultTpp += "//"+dirname+"\n";//добавляем название папки (раздела)
        ResultTpp += "//*****************************************************************\n";

        ResultH   += "//"+dirname+"\n";//добавляем название папки (раздела)

        ResultTexList += "\\textbf{"+dirname+"}\n";
        ResultTexList += "\\begin{enumerate}\n\n";

        ResultTexFunctions += "\\subsection{"+dirname+"}\n\n";

        countneed=0;

        for (j=0;j<m;j++)//пробегаем по файлам папки
        {
            filename=listfiles.at(j);//получаем имя файла
            exp=HQt_GetExpFromFilename(filename);//расширение файла

            if (exp=="desc")
            {
                countneed++;
                countdesc++;

                QString nameof_func=HQt_GetNameFromFilename(filename);//имя функции по названию файла
                QString nameof_func_=nameof_func;//имя функции с преобразованными знаками _
                nameof_func_=nameof_func_.replace("_","\\_");

                Temp = HQt_ReadFile(path2+filename)+"\n";//добавляем текст файла

                ResultTexList += "\\item \\textbf{\\hyperref[" + nameof_func + "]{" + nameof_func_ + "}} --- " + Temp+"\n";

                ResultTexFunctions+="\\subsubsection{" + nameof_func_ + "}\\label{"+nameof_func+"}\n\n" + Temp+"\n\n";

                if (!(Temp.trimmed().isEmpty())) ui->textEdit->insertHtml("Загрузили файл <b>"+filename+"</b><br>");
                else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

                //Проверим наличие сопутствующих файлов
                QString F=HQt_GetNameFromFilename(filename);
                if (HQt_FileExists(path2+F+".h")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".h</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if ((HQt_FileExists(path2+F+".cpp")==false)&&(HQt_FileExists(path2+F+".tpp")==false)) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".cpp</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".tex")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".tex</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".use")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".use</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

            }
            if ((exp=="cpp")||(exp=="tpp"))
            {
                countneed++;
                countcpp++;

                Temp = HQt_ReadFile(path2+filename)+"\n";//добавляем текст файла в cpp файл

                if (exp=="cpp")
                {
                    ResultCpp += Temp;
                    ResultCpp += "//---------------------------------------------------------------------------\n";
                    if (!(Temp.trimmed().isEmpty())) ui->textEdit->insertHtml("Загрузили файл <b>"+filename+"</b><br>");
                    else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                }

                if (exp=="tpp")
                {
                    ResultTpp += Temp;
                    ResultTpp += "//---------------------------------------------------------------------------\n";
                    if (!(Temp.trimmed().isEmpty())) ui->textEdit->insertHtml("Загрузили файл <b>"+filename+"</b><br>");
                    else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                }

                //Проверим наличие сопутствующих файлов
                QString F=HQt_GetNameFromFilename(filename);
                if (HQt_FileExists(path2+F+".h")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".h</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".desc")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".desc</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".tex")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".tex</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".use")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".use</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
            }
            if (exp=="h")
            {
                countneed++;
                counth++;

                QString nameof_func=HQt_GetNameFromFilename(filename);//имя функции по названию файла
                //QString nameof_func_=nameof_func;//имя функции с преобразованными знаками _
                //nameof_func_=nameof_func_.replace("_","\\_");


                Temp = HQt_ReadFile(path2+filename)+"\n";//добавляем текст файла в h файл
                ResultH += Temp;
                if (!(Temp.trimmed().isEmpty())) ui->textEdit->insertHtml("Загрузили файл <b>"+filename+"</b><br>");
                else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

                countfunction+=Temp.count(";");//количество функций считаем по количеству знаков ; в h файлах

                ResultTexFunctions+="\\begin{lstlisting}[label=code_syntax_"+nameof_func+",caption=Синтаксис]\n";
                ResultTexFunctions+=Temp;
                ResultTexFunctions+="\\end{lstlisting}\n\n";

                //Проверим наличие сопутствующих файлов
                QString F=HQt_GetNameFromFilename(filename);
                if ((HQt_FileExists(path2+F+".cpp")==false)&&(HQt_FileExists(path2+F+".tpp")==false)) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".cpp</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".desc")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".desc</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".tex")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".tex</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".use")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".use</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
            }
            if (exp=="tex")
            {
                countneed++;
                counttex++;

                Temp = HQt_ReadFile(path2+filename)+"\n";//добавляем текст файла

                ResultTexFunctions+=Temp+"\n\n";

                if (!(Temp.trimmed().isEmpty())) ui->textEdit->insertHtml("Загрузили файл <b>"+filename+"</b><br>");
                else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

                //Проверим наличие сопутствующих файлов
                QString F=HQt_GetNameFromFilename(filename);
                if (HQt_FileExists(path2+F+".h")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".h</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".desc")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".desc</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if ((HQt_FileExists(path2+F+".cpp")==false)&&(HQt_FileExists(path2+F+".tpp")==false)) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".cpp</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".use")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".use</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
            }
            if (exp=="png")
            {
                countneed++;
                countpng++;

                b=HQt_CopyFile(path2+filename, temp_library_path+DS+"images"+DS,true);
                if (b) ui->textEdit->insertHtml("Скопировали файл <b>"+filename+"</b><br>");
                else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
            }
            if (exp=="pdf")
            {
                countneed++;
                countpdf++;

                b=HQt_CopyFile(path2+filename, temp_library_path+DS+"images"+DS,true);
                if (b) ui->textEdit->insertHtml("Скопировали файл <b>"+filename+"</b><br>");
                else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
            }
            if (exp=="use")
            {
                countneed++;
                countuse++;

                QString nameof_func=HQt_GetNameFromFilename(filename);//имя функции по названию файла
                QString nameof_func_=nameof_func;//имя функции с преобразованными знаками _
                nameof_func_=nameof_func_.replace("_","\\_");

                Temp = HQt_ReadFile(path2+filename)+"\n";//добавляем текст файла

                //удаляем пробелы
                QStringList TempWithSpace = Temp.split( "\n" );
                QStringList TempWithoutSpace;

                QString FirstLine = TempWithSpace.at(0);

                QString st,StringFromTempWithoutSpace;

                int countspace=0;

                bool beginsting=true;

                for (int i=0;i<FirstLine.length();i++)
                {
                    if ((FirstLine.at(i)==' ')&&(beginsting==true))
                    {
                        countspace++;
                    }
                    if (FirstLine.at(i)!=' ')
                        beginsting=false;
                }

                QString Space;
                for (int i=0;i<countspace;i++) Space+=" ";

                for (int i=0;i<TempWithSpace.count();i++)
                {
                    st = TempWithSpace.at(i);

                    if (st.mid(0,countspace)==Space)
                        StringFromTempWithoutSpace = st.mid(countspace,-1);
                    else
                        StringFromTempWithoutSpace=st;

                    TempWithoutSpace << StringFromTempWithoutSpace;

                }

                Temp=HQt_QStringListToQString(TempWithoutSpace);
                //закончили удалять пробелы

                ResultTexFunctions+="\\begin{lstlisting}[label=code_use_"+nameof_func+",caption=Пример использования]\n";
                ResultTexFunctions+=Temp;
                ResultTexFunctions+="\\end{lstlisting}\n\n";


                if (!(Temp.trimmed().isEmpty())) ui->textEdit->insertHtml("Загрузили файл <b>"+filename+"</b><br>");
                else {MessageError="<font color=\"red\">Ошибка с файлом <b>"+filename+"</b><\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

                //Проверим наличие сопутствующих файлов
                QString F=HQt_GetNameFromFilename(filename);
                if (HQt_FileExists(path2+F+".h")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".h</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if ((HQt_FileExists(path2+F+".cpp")==false)&&(HQt_FileExists(path2+F+".tpp")==false)) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".cpp</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".tex")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".tex</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}
                if (HQt_FileExists(path2+F+".desc")==false) {MessageError="<font color=\"red\">Не обнаружен <b>"+F+".desc</b> файл<\font><br>";AllMessageError+=MessageError;ui->textEdit->insertHtml(MessageError);countoferrors++;}

            }

            QGuiApplication::processEvents();

            QTextCursor c =  ui->textEdit->textCursor();
            c.movePosition(QTextCursor::End);
            ui->textEdit->setTextCursor(c);
        }

        ui->textEdit->insertHtml("Из <b>"+QString::number(m)+"</b> файлов нужными нам оказалось <b>"+QString::number(countneed)+"</b> файлов в папке<br>");

        ResultCpp += "\n";
        ResultH   += "\n";

        ResultTexList += "\\end{enumerate}\n\n";

    }

    ResultH += ResultTpp;

    ResultTex+=ResultTexList+"\n";
    ResultTex+=ResultTexFunctions;
//    ResultTex+="\\phantomsection\n";
//    ResultTex+="\\addcontentsline{toc}{section}{Список литературы}\n";
//    ResultTex+="\\bibliographystyle{utf8gost705u}  %% стилевой файл для оформления по ГОСТу\n";
//    ResultTex+="\\bibliography{biblio}     %% имя библиографической базы (bib-файла)\n";
//    ResultTex+="\\newpage\n\n\\end{document}";


    HQt_SaveFile(ResultCpp,temp_library_path+"Library.cpp");
    ui->textEdit->insertHtml("<br>Сохранили файл <b>Library.cpp</b><br>");
    HQt_SaveFile(ResultH,temp_library_path+"Library.h");
    ui->textEdit->insertHtml("Сохранили файл <b>Library.h</b><br>");

    HQt_SaveFile(ResultTex,temp_library_path+"Library_Help.tex");
    ui->textEdit->insertHtml("Сохранили файл <b>Library_Help.tex</b><br>");

    if (countoferrors==0) ui->textEdit->insertHtml("<br>Ошибки не были зафиксированы.<br>");
    else ui->textEdit->insertHtml("<br><font color=\"red\">Ошибок <b>"+QString::number(countoferrors)+"</b> штук<\font><br>");

    ui->textEdit->insertHtml("<font color=\"#858585\">Конец формирования файлов библиотеки.<\font><br>");

    ui->textEdit->insertHtml("<font color=\"#858585\">Было обнаружено "+QString::number(countfunction)+" функций.<\font><br>");

    int nMilliseconds = myTimer.elapsed();
    ui->textEdit->insertHtml("<font color=\"#858585\">Потребовалось времени: "+HQt_WriteTime(nMilliseconds)+"</font><br>");

    if (AllMessageError.length()>2)
    {
        ui->textEdit->insertHtml("<b>Список всех ошибок</b><br>");
        ui->textEdit->insertHtml(AllMessageError);
    }

    QTextCursor c =  ui->textEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(c);

    ui->pushButton->setEnabled(true);

    //открытие папки с собранными файлами
    QDesktopServices::openUrl(QUrl::fromLocalFile(temp_library_path));

}