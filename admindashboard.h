#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMap>
#include "databasehandler.h"

class AdminDashboard : public QWidget
{
    Q_OBJECT
public:
    AdminDashboard(QWidget *parent = nullptr);
    ~AdminDashboard();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onAddBookButtonClicked();
    void onViewAllBooksButtonClicked();
    void onLogoutButtonClicked();
    void onAttachPdfButtonClicked();
    void onViewBookButtonClicked();
    void refreshBookTable();
private:
    // UI components
    //Labels
    QLabel *titleLabel;
    //TableWidget
    QTableWidget *booksTable;
    //LineEdits
    QLineEdit *bookNameEdit;
    QLineEdit *authorEdit;
    QLineEdit *isbnEdit;
    QLineEdit *pdfPathEdit;
    //QPushButtons
    QPushButton *attachPdfButton;
    QPushButton *addBookButton;
    QPushButton *viewAllBooksButton;
    QPushButton *viewBookButton;
    QPushButton *logoutButton;
    //Qpixmap
    QPixmap backgroundImage;

    // Database handler
    DatabaseHandler m_dbHandler;

    // Functions
    void setupInitialBooks();
    QMap<QString, QString> m_bookPdfPaths; // Store ISBN to PDF path mappings
};
#endif // ADMINDASHBOARD_H
