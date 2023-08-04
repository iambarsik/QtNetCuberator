#pragma once

#include <QDir>
#include <QFile>
#include <QDate>
#include <QTextStream>
#include <QTextCodec>
#include <QTime>

class loggingQ  {
public:

    loggingQ() {
        m_file_name = "";
    };
    loggingQ(QString file_name) {
        setFileName(" " + file_name);
    };

    ~loggingQ() {
        pushTextInFile("============================ end log =============================");
    }

    void setFileName(QString file_name = "") {
        if(file_name.isEmpty()) {
            QDate log_date = QDate::currentDate();
            m_file_name = QString("log_%1-%2-%3").arg(log_date.day()).arg(log_date.month()).arg(log_date.year());
        } else {
            m_file_name = file_name;
        }
        pushTextInFile("=========================== begin log ============================");
    }

    void operator << (QString text)  {
        pushTextInFile(text);
    }

    void pushTextInFile(QString text)   {
        if(m_file_name.isEmpty())
            setFileName();
        QString filename = QDir::currentPath() + "/log/" + m_file_name;
        QFile file(m_file_name);
        file.open(QIODevice::Append);
        QTextStream out(&file);
        out.setCodec(QTextCodec::codecForName("UTF-8"));
        QString st = QTime::currentTime().toString();
        out << QString("%1 : %2").arg(st).arg(text) << "\n";
        file.close();
    }


private:
    QString m_file_name;

};

