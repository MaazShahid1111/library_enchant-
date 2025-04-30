#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

class DatabaseHandler
{
public:
    DatabaseHandler();
    ~DatabaseHandler();
    bool initialize();
    bool addBook(const QString &title, const QString &author, bool availability, const QString &isbn);
    bool removeBook(const QString &isbn);
    bool updateBookAvailability(const QString &isbn, bool availability);
    QSqlQuery getAllBooks();
    QSqlQuery searchBooks(const QString &keyword);
    bool isBookAvailable(const QString &isbn);
    QString getLastError() const;

    // New transaction methods
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

private:
    QSqlDatabase m_database;
    QString m_lastError;
    bool createDatabaseFolder();
    bool createDatabaseTables();
    void setLastError(const QString &error);
};
#endif // DATABASEHANDLER_H
