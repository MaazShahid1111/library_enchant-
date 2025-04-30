#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QSpinBox>
#include <QPixmap>
#include <QVector>
#include <QPair>

class BookExplorer;
class DatabaseHandler;

class StudentDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit StudentDashboard(QWidget *parent = nullptr);
    ~StudentDashboard();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void onSearchButtonClicked();
    void onRecommendedBooksButtonClicked();
    void onViewBookButtonClicked();
    void onBuyBookButtonClicked();
    void onRentBookButtonClicked();
    void onReturnBookButtonClicked();
    void onCheckoutButtonClicked();
    void onLogoutButtonClicked();
    void updateTotalPrice();
    void refreshBookTable();

public:
    void addBookToTable(const QString &name, const QString &author, const QString &condition,
                        bool available, double price, double rentPrice);

    DatabaseHandler* databaseHandler;
    BookExplorer* bookExplorer;

    QPixmap backgroundImage;
    QLabel *titleLabel;
    QLabel *welcomeLabel;
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QTableWidget *booksTable;
    QPushButton *recommendedBooksButton;
    QPushButton *viewBookButton;
    QPushButton *buyBookButton;
    QPushButton *rentBookButton;
    QPushButton *returnBookButton;
    QPushButton *checkoutButton;
    QPushButton *logoutButton;
    QLabel *totalPriceLabel;
    QTableWidget *cartTable;
    QLabel *rentalDaysLabel;
    QSpinBox *rentalDaysSpinBox;

    QVector<QPair<QString, double>> cartItems;
    QVector<QPair<QString, double>> rentedBooks;
    double totalPrice;
};

#endif // STUDENTDASHBOARD_H
