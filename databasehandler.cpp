#include "databasehandler.h"

DatabaseHandler::DatabaseHandler()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseHandler::~DatabaseHandler()
{
    if (m_database.isOpen())
        m_database.close();
}

bool DatabaseHandler::initialize()
{
    if (!createDatabaseFolder())
        return false;

    // Set database path
    QString dbPath = QDir::currentPath() + "/../library-database/library.db";
    m_database.setDatabaseName(dbPath);

    // Try to open the database
    if (!m_database.open()) {
        setLastError("Failed to open database: " + m_database.lastError().text());
        return false;
    }

    // Create tables if they don't exist
    if (!createDatabaseTables()) {
        m_database.close();
        return false;
    }

    return true;
}

bool DatabaseHandler::addBook(const QString &title, const QString &author, bool availability, const QString &isbn)
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, availability, isbn) VALUES (?, ?, ?, ?)");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(availability ? "Available" : "Not Available");
    query.addBindValue(isbn);

    if (!query.exec()) {
        setLastError("Failed to add book: " + query.lastError().text());
        return false;
    }

    return true;
}

bool DatabaseHandler::removeBook(const QString &isbn)
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE isbn = ?");
    query.addBindValue(isbn);

    if (!query.exec()) {
        setLastError("Failed to remove book: " + query.lastError().text());
        return false;
    }

    return true;
}

bool DatabaseHandler::updateBookAvailability(const QString &isbn, bool availability)
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE books SET availability = ? WHERE isbn = ?");
    query.addBindValue(availability ? "Available" : "Not Available");
    query.addBindValue(isbn);

    if (!query.exec()) {
        setLastError("Failed to update book availability: " + query.lastError().text());
        return false;
    }

    return true;
}

QSqlQuery DatabaseHandler::getAllBooks()
{
    QSqlQuery query;

    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return query;
    }

    if (!query.exec("SELECT * FROM books")) {
        setLastError("Failed to get all books: " + query.lastError().text());
    }

    return query;
}

QSqlQuery DatabaseHandler::searchBooks(const QString &keyword)
{
    QSqlQuery query;

    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return query;
    }

    query.prepare("SELECT * FROM books WHERE title LIKE ? OR author LIKE ? OR isbn LIKE ?");
    QString wildcardKeyword = "%" + keyword + "%";
    query.addBindValue(wildcardKeyword);
    query.addBindValue(wildcardKeyword);
    query.addBindValue(wildcardKeyword);

    if (!query.exec()) {
        setLastError("Failed to search books: " + query.lastError().text());
    }

    return query;
}

bool DatabaseHandler::isBookAvailable(const QString &isbn)
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT availability FROM books WHERE isbn = ?");
    query.addBindValue(isbn);

    if (!query.exec() || !query.next()) {
        setLastError("Failed to check book availability: " + query.lastError().text());
        return false;
    }

    return query.value(0).toString() == "Available";
}

QString DatabaseHandler::getLastError() const
{
    return m_lastError;
}

bool DatabaseHandler::createDatabaseFolder()
{
    // Check if we're in a project directory
    QDir currentDir = QDir::current();
    QDir parentDir = currentDir;
    parentDir.cdUp();

    // Path to database folder
    QString dbFolderPath = parentDir.absolutePath() + "/library-database";
    QDir dbFolder(dbFolderPath);

    // Check if folder exists
    if (!dbFolder.exists()) {
        // Create folder if it doesn't exist
        if (!dbFolder.mkdir(dbFolderPath)) {
            setLastError("Failed to create database folder: " + dbFolderPath);
            return false;
        }
        qDebug() << "Created database folder at:" << dbFolderPath;
    } else {
        qDebug() << "Database folder already exists at:" << dbFolderPath;
    }

    return true;
}

bool DatabaseHandler::createDatabaseTables()
{
    QSqlQuery query;

    // Create books table
    if (!query.exec("CREATE TABLE IF NOT EXISTS books ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "title TEXT NOT NULL,"
                    "author TEXT NOT NULL,"
                    "availability TEXT NOT NULL,"
                    "isbn TEXT NOT NULL UNIQUE,"
                    "pdf_path TEXT"
                    ")")) {
        setLastError("Failed to create books table: " + query.lastError().text());
        return false;
    }

    return true;
}

void DatabaseHandler::setLastError(const QString &error)
{
    m_lastError = error;
    qDebug() << "DatabaseHandler error:" << error;
}

// New transaction methods implementation
bool DatabaseHandler::beginTransaction()
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }
    return m_database.transaction();
}

bool DatabaseHandler::commitTransaction()
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }
    return m_database.commit();
}

bool DatabaseHandler::rollbackTransaction()
{
    if (!m_database.isOpen()) {
        setLastError("Database is not open");
        return false;
    }
    return m_database.rollback();
}
