#include "studentdashboard.h"
#include "bookexplorer.h"
#include "databasehandler.h"
#include <QPainter>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QGroupBox>
#include <QScrollArea>
#include <QScreen>
#include <QApplication>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
StudentDashboard::StudentDashboard(QWidget *parent) : QWidget(parent), totalPrice(0.0) {
    databaseHandler = new DatabaseHandler();
    if (!databaseHandler->initialize()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to connect to database: " + databaseHandler->getLastError());
    }
    setWindowTitle("Student Dashboard");
    setFixedSize(700, 600);
    backgroundImage.load("C:/library_dasboard.jpg");
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QWidget *scrollWidget = new QWidget();
    scrollArea->setWidget(scrollWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);
    titleLabel = new QLabel("Student Dashboard", this);
    QFont titleFont("Arial", 18, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #FFFFFF; background-color: rgba(0, 0, 0, 120); padding: 8px; border-radius: 6px;");
    welcomeLabel = new QLabel("Welcome to the Enchanted Library. What would you like to do today?", this);
    QFont welcomeFont("Georgia", 12);
    welcomeLabel->setFont(welcomeFont);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("color: #FFFFFF; text-shadow: 1px 1px 2px #000000;");
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Search for books...");
    searchEdit->setMinimumHeight(30);
    searchEdit->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 4px; font-size: 12px;");
    searchButton = new QPushButton("Search", this);
    searchButton->setMinimumSize(80, 30);
    searchButton->setStyleSheet("QPushButton {"
                                "background-color: #2196F3;"
                                "color: white;"
                                "border-radius: 5px;"
                                "font-size: 12px;"
                                "font-weight: bold;"
                                "padding: 4px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #0b7dda;"
                                "}");
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    booksTable = new QTableWidget(this);
    booksTable->setColumnCount(6);
    QStringList headers;
    headers << "Book Name" << "Author" << "Condition" << "Available" << "Price" << "Rent Price";
    booksTable->setHorizontalHeaderLabels(headers);
    booksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    booksTable->setStyleSheet("QTableWidget { background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 2px; }"
                              "QHeaderView::section { background-color: #333; color: white; padding: 4px; font-size: 11px; }");
    booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    booksTable->setAlternatingRowColors(true);
    booksTable->verticalHeader()->setDefaultSectionSize(25);
    booksTable->setMinimumHeight(120);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(8);
    recommendedBooksButton = new QPushButton("Recommended", this);
    recommendedBooksButton->setMinimumSize(90, 30);
    recommendedBooksButton->setStyleSheet("QPushButton {"
                                          "background-color: #4CAF50;"
                                          "color: white;"
                                          "border-radius: 5px;"
                                          "font-size: 11px;"
                                          "font-weight: bold;"
                                          "padding: 3px;"
                                          "}"
                                          "QPushButton:hover {"
                                          "background-color: #45a049;"
                                          "}");
    viewBookButton = new QPushButton("View", this);
    viewBookButton->setMinimumSize(70, 30);
    viewBookButton->setStyleSheet("QPushButton {"
                                  "background-color: #9C27B0;"
                                  "color: white;"
                                  "border-radius: 5px;"
                                  "font-size: 11px;"
                                  "font-weight: bold;"
                                  "padding: 3px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #8e24aa;"
                                  "}");
    buyBookButton = new QPushButton("Buy", this);
    buyBookButton->setMinimumSize(70, 30);
    buyBookButton->setStyleSheet("QPushButton {"
                                 "background-color: #FF9800;"
                                 "color: white;"
                                 "border-radius: 5px;"
                                 "font-size: 11px;"
                                 "font-weight: bold;"
                                 "padding: 3px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: #FB8C00;"
                                 "}");
    rentBookButton = new QPushButton("Rent", this);
    rentBookButton->setMinimumSize(70, 30);
    rentBookButton->setStyleSheet("QPushButton {"
                                  "background-color: #007BFF;"
                                  "color: white;"
                                  "border-radius: 5px;"
                                  "font-size: 11px;"
                                  "font-weight: bold;"
                                  "padding: 3px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #0056b3;"
                                  "}");
    returnBookButton = new QPushButton("Return", this);
    returnBookButton->setMinimumSize(70, 30);
    returnBookButton->setStyleSheet("QPushButton {"
                                    "background-color: #17a2b8;"
                                    "color: white;"
                                    "border-radius: 5px;"
                                    "font-size: 11px;"
                                    "font-weight: bold;"
                                    "padding: 3px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: #138496;"
                                    "}");
    buttonLayout->addWidget(recommendedBooksButton);
    buttonLayout->addWidget(viewBookButton);
    buttonLayout->addWidget(buyBookButton);
    buttonLayout->addWidget(rentBookButton);
    buttonLayout->addWidget(returnBookButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    QSpacerItem *spacerItem = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);
    QGroupBox *checkoutGroup = new QGroupBox("Checkout", this);
    checkoutGroup->setStyleSheet("QGroupBox { color: white; background-color: rgba(0, 0, 0, 120); border-radius: 5px; padding: 5px; font-size: 12px; font-weight: bold; }");
    QVBoxLayout *checkoutLayout = new QVBoxLayout(checkoutGroup);
    checkoutLayout->setSpacing(8);
    checkoutLayout->setContentsMargins(10, 15, 10, 10);
    cartTable = new QTableWidget(this);
    cartTable->setColumnCount(3);
    QStringList cartHeaders;
    cartHeaders << "Book Name" << "Type" << "Price";
    cartTable->setHorizontalHeaderLabels(cartHeaders);
    cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    cartTable->setStyleSheet("QTableWidget { background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 2px; }"
                             "QHeaderView::section { background-color: #333; color: white; padding: 3px; font-size: 11px; }");
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setAlternatingRowColors(true);
    cartTable->setMinimumHeight(80);
    cartTable->verticalHeader()->setDefaultSectionSize(22);
    QHBoxLayout *rentalLayout = new QHBoxLayout();
    rentalLayout->setSpacing(8);
    rentalDaysLabel = new QLabel("Rental Period (days):", this);
    rentalDaysLabel->setStyleSheet("color: white; font-size: 11px;");
    rentalDaysSpinBox = new QSpinBox(this);
    rentalDaysSpinBox->setMinimum(1);
    rentalDaysSpinBox->setMaximum(30);
    rentalDaysSpinBox->setValue(7);
    rentalDaysSpinBox->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 3px; padding: 2px; font-size: 11px;");
    rentalDaysSpinBox->setMinimumHeight(22);
    rentalDaysSpinBox->setMinimumWidth(50);
    rentalLayout->addWidget(rentalDaysLabel);
    rentalLayout->addWidget(rentalDaysSpinBox);
    rentalLayout->addStretch();
    totalPriceLabel = new QLabel("Total Price: $0.00", this);
    totalPriceLabel->setStyleSheet("color: white; font-size: 12px; font-weight: bold; padding: 2px;");
    totalPriceLabel->setAlignment(Qt::AlignRight);
    checkoutButton = new QPushButton("Checkout", this);
    checkoutButton->setMinimumSize(100, 30);
    checkoutButton->setStyleSheet("QPushButton {"
                                  "background-color: #28a745;"
                                  "color: white;"
                                  "border-radius: 5px;"
                                  "font-size: 12px;"
                                  "font-weight: bold;"
                                  "padding: 3px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #218838;"
                                  "}");
    checkoutLayout->addWidget(cartTable);
    checkoutLayout->addLayout(rentalLayout);
    checkoutLayout->addWidget(totalPriceLabel);
    QHBoxLayout *checkoutButtonLayout = new QHBoxLayout();
    checkoutButtonLayout->addStretch();
    checkoutButtonLayout->addWidget(checkoutButton);
    checkoutButtonLayout->addStretch();
    checkoutLayout->addLayout(checkoutButtonLayout);
    logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumSize(80, 25);
    logoutButton->setStyleSheet("QPushButton {"
                                "background-color: #f44336;"
                                "color: white;"
                                "border-radius: 4px;"
                                "font-size: 11px;"
                                "font-weight: bold;"
                                "padding: 3px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #d32f2f;"
                                "}");
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(booksTable, 3);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addItem(spacerItem);
    mainLayout->addWidget(checkoutGroup, 2);
    QHBoxLayout *logoutLayout = new QHBoxLayout();
    logoutLayout->addStretch();
    logoutLayout->addWidget(logoutButton);
    mainLayout->addLayout(logoutLayout);
    mainLayout->addSpacing(5);
    connect(searchButton, &QPushButton::clicked, this, &StudentDashboard::onSearchButtonClicked);
    connect(recommendedBooksButton, &QPushButton::clicked, this, &StudentDashboard::onRecommendedBooksButtonClicked);
    connect(viewBookButton, &QPushButton::clicked, this, &StudentDashboard::onViewBookButtonClicked);
    connect(buyBookButton, &QPushButton::clicked, this, &StudentDashboard::onBuyBookButtonClicked);
    connect(rentBookButton, &QPushButton::clicked, this, &StudentDashboard::onRentBookButtonClicked);
    connect(returnBookButton, &QPushButton::clicked, this, &StudentDashboard::onReturnBookButtonClicked);
    connect(checkoutButton, &QPushButton::clicked, this, &StudentDashboard::onCheckoutButtonClicked);
    connect(logoutButton, &QPushButton::clicked, this, &StudentDashboard::onLogoutButtonClicked);
    connect(rentalDaysSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &StudentDashboard::updateTotalPrice);
    refreshBookTable();
}
StudentDashboard::~StudentDashboard() {
    delete databaseHandler;
}
void StudentDashboard::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!backgroundImage.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), backgroundImage);
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 120));
    } else {
        painter.fillRect(0, 0, width(), height(), QColor(50, 50, 50));
    }
    QWidget::paintEvent(event);
}
void StudentDashboard::refreshBookTable() {
    booksTable->setRowCount(0);
    QSqlQuery query = databaseHandler->getAllBooks();
    while (query.next()) {
        QString title = query.value("title").toString();
        QString author = query.value("author").toString();
        QString condition = query.value("condition").toString();
        bool available = (query.value("availability").toString() == "Available");
        addBookToTable(title, author, condition, available, 20.0, 3.0);
    }
}
void StudentDashboard::onSearchButtonClicked() {
    QString searchText = searchEdit->text();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Search", "Please enter a search term.");
        return;
    }
    QSqlQuery query = databaseHandler->searchBooks(searchText);
    booksTable->setRowCount(0);
    while (query.next()) {
        QString title = query.value("title").toString();
        QString author = query.value("author").toString();
        QString condition = query.value("condition").toString();
        bool available = (query.value("availability").toString() == "Available");
        addBookToTable(title, author, condition, available, 20.0, 3.0);
    }
}
void StudentDashboard::onRecommendedBooksButtonClicked() {
    bookExplorer = new BookExplorer(this, false, true);
    bookExplorer->show();
}
void StudentDashboard::onViewBookButtonClicked() {
    QModelIndexList selectedIndexes = booksTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "View Book", "Please select a book to view.");
        return;
    }
    int row = selectedIndexes.first().row();
    QString bookName = booksTable->item(row, 0)->text();
    QString available = booksTable->item(row, 3)->text();
    if (available == "No") {
        QMessageBox::warning(this, "View Book", "This book is not available for viewing.");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT pdf_path FROM books WHERE title = ?");
    query.addBindValue(bookName);
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "View Book", "Could not find PDF path for this book.");
        return;
    }
    QString pdfPath = query.value("pdf_path").toString();
    if (!pdfPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
    } else {
        QMessageBox::information(this, "View Book", "No PDF available for this book.");
    }
}
void StudentDashboard::onBuyBookButtonClicked() {
    QModelIndexList selectedIndexes = booksTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Buy Book", "Please select a book to buy.");
        return;
    }
    int row = selectedIndexes.first().row();
    QString bookName = booksTable->item(row, 0)->text();
    QString available = booksTable->item(row, 3)->text();
    if (available == "No") {
        QMessageBox::warning(this, "Buy Book", "This book is not available for purchase.");
        return;
    }
    double price = 20.0;
    cartItems.append(qMakePair(bookName, price));
    int cartRow = cartTable->rowCount();
    cartTable->insertRow(cartRow);
    QTableWidgetItem *nameItem = new QTableWidgetItem(bookName);
    QTableWidgetItem *typeItem = new QTableWidgetItem("Purchase");
    QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$%1").arg(price, 0, 'f', 2));
    QFont cartFont = nameItem->font();
    cartFont.setPointSize(9);
    nameItem->setFont(cartFont);
    typeItem->setFont(cartFont);
    priceItem->setFont(cartFont);
    cartTable->setItem(cartRow, 0, nameItem);
    cartTable->setItem(cartRow, 1, typeItem);
    cartTable->setItem(cartRow, 2, priceItem);
    totalPrice += price;
    totalPriceLabel->setText(QString("Total Price: $%1").arg(totalPrice, 0, 'f', 2));
    QMessageBox::information(this, "Buy Book", QString("Book '%1' has been added to your cart for purchase.").arg(bookName));
}
void StudentDashboard::onRentBookButtonClicked() {
    QModelIndexList selectedIndexes = booksTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Rent Book", "Please select a book to rent.");
        return;
    }
    int row = selectedIndexes.first().row();
    QString bookName = booksTable->item(row, 0)->text();
    QString available = booksTable->item(row, 3)->text();
    if (available == "No") {
        QMessageBox::warning(this, "Rent Book", "This book is not available for renting.");
        return;
    }
    double rentPrice = 3.0;
    rentedBooks.append(qMakePair(bookName, rentPrice));
    int cartRow = cartTable->rowCount();
    cartTable->insertRow(cartRow);
    QTableWidgetItem *nameItem = new QTableWidgetItem(bookName);
    QTableWidgetItem *typeItem = new QTableWidgetItem("Rental");
    QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$%1/day").arg(rentPrice, 0, 'f', 2));
    QFont cartFont = nameItem->font();
    cartFont.setPointSize(9);
    nameItem->setFont(cartFont);
    typeItem->setFont(cartFont);
    priceItem->setFont(cartFont);
    cartTable->setItem(cartRow, 0, nameItem);
    cartTable->setItem(cartRow, 1, typeItem);
    cartTable->setItem(cartRow, 2, priceItem);
    updateTotalPrice();
    QMessageBox::information(this, "Rent Book", QString("Book '%1' has been added to your cart for rental.").arg(bookName));
}
void StudentDashboard::onReturnBookButtonClicked() {
    QString bookName = QInputDialog::getText(this, "Return Book", "Enter the name of the book you want to return:");
    if (bookName.isEmpty()) {
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE title = ? AND availability = 'Rented'");
    query.addBindValue(bookName);
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Return Book", "Book not found or not currently rented.");
        return;
    }
    QString dueDate = query.value("due_date").toString();
    QDate dueDateObj = QDate::fromString(dueDate, "yyyy-MM-dd");
    QDate currentDate = QDate::currentDate();
    int daysOverdue = 0;
    if (currentDate > dueDateObj) {
        daysOverdue = dueDateObj.daysTo(currentDate);
    }
    double penalty = 0.0;
    if (daysOverdue > 0) {
        penalty = daysOverdue * 1.0;
        QMessageBox::warning(this, "Return Book",
                             QString("The book is %1 days overdue. A penalty of $%2 will be applied.")
                                 .arg(daysOverdue)
                                 .arg(penalty, 0, 'f', 2));
    }
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE books SET availability = 'Available', due_date = NULL WHERE title = ?");
    updateQuery.addBindValue(bookName);
    if (!updateQuery.exec()) {
        QMessageBox::warning(this, "Return Book", "Failed to update book status: " + updateQuery.lastError().text());
        return;
    }
    if (penalty > 0) {
        int cartRow = cartTable->rowCount();
        cartTable->insertRow(cartRow);
        QTableWidgetItem *nameItem = new QTableWidgetItem(bookName + " (Penalty)");
        QTableWidgetItem *typeItem = new QTableWidgetItem("Penalty");
        QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$%1").arg(penalty, 0, 'f', 2));
        QFont cartFont = nameItem->font();
        cartFont.setPointSize(9);
        nameItem->setFont(cartFont);
        typeItem->setFont(cartFont);
        priceItem->setFont(cartFont);
        cartTable->setItem(cartRow, 0, nameItem);
        cartTable->setItem(cartRow, 1, typeItem);
        cartTable->setItem(cartRow, 2, priceItem);
        totalPrice += penalty;
        totalPriceLabel->setText(QString("Total Price: $%1").arg(totalPrice, 0, 'f', 2));
    }
    QMessageBox::information(this, "Return Book",
                             QString("Book '%1' has been successfully returned.").arg(bookName));
    refreshBookTable();
}
void StudentDashboard::onCheckoutButtonClicked() {
    if (cartTable->rowCount() == 0) {
        QMessageBox::warning(this, "Checkout", "Your cart is empty.");
        return;
    }
    updateTotalPrice();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Checkout Confirmation",
                                  QString("Total amount: $%1\nProceed with checkout?").arg(totalPrice, 0, 'f', 2),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        databaseHandler->beginTransaction();
        bool success = true;
        for (const auto &item : cartItems) {
            QString bookName = item.first;
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE books SET availability = 'Sold' WHERE title = ?");
            updateQuery.addBindValue(bookName);
            if (!updateQuery.exec()) {
                QMessageBox::warning(this, "Checkout Error",
                                     "Failed to update book status: " + updateQuery.lastError().text());
                success = false;
                break;
            }
        }
        int rentalDays = rentalDaysSpinBox->value();
        QDate dueDate = QDate::currentDate().addDays(rentalDays);
        QString dueDateStr = dueDate.toString("yyyy-MM-dd");
        for (const auto &item : rentedBooks) {
            QString bookName = item.first;
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE books SET availability = 'Rented', due_date = ? WHERE title = ?");
            updateQuery.addBindValue(dueDateStr);
            updateQuery.addBindValue(bookName);
            if (!updateQuery.exec()) {
                QMessageBox::warning(this, "Checkout Error",
                                     "Failed to update rental status: " + updateQuery.lastError().text());
                success = false;
                break;
            }
        }
        if (success) {
            databaseHandler->commitTransaction();
            cartTable->setRowCount(0);
            cartItems.clear();
            rentedBooks.clear();
            totalPrice = 0.0;
            updateTotalPrice();
            QMessageBox::information(this, "Checkout Complete",
                                     "Your transaction has been completed successfully!");
            refreshBookTable();
        } else {
            databaseHandler->rollbackTransaction();
            QMessageBox::critical(this, "Checkout Failed",
                                  "Transaction failed. No changes were made.");
        }
    }
}
void StudentDashboard::updateTotalPrice() {
    totalPrice = 0.0;
    for (const auto &item : cartItems) {
        totalPrice += 20.0;
    }
    int rentalDays = rentalDaysSpinBox->value();
    for (const auto &item : rentedBooks) {
        totalPrice += 3.0 * rentalDays;
    }
    totalPriceLabel->setText(QString("Total Price: $%1").arg(totalPrice, 0, 'f', 2));
}
void StudentDashboard::onLogoutButtonClicked() {
    this->hide();
    this->parentWidget()->show();
}
void StudentDashboard::addBookToTable(const QString &name, const QString &author, const QString &condition, bool available, double price, double rentPrice) {
    int row = booksTable->rowCount();
    booksTable->insertRow(row);
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    QTableWidgetItem *authorItem = new QTableWidgetItem(author);
    QTableWidgetItem *conditionItem = new QTableWidgetItem(condition);
    QTableWidgetItem *availableItem = new QTableWidgetItem(available ? "Yes" : "No");
    QTableWidgetItem *priceItem = new QTableWidgetItem("$20.00");
    QTableWidgetItem *rentPriceItem = new QTableWidgetItem("$3.00");
    QFont itemFont = nameItem->font();
    itemFont.setPointSize(9);
    nameItem->setFont(itemFont);
    authorItem->setFont(itemFont);
    conditionItem->setFont(itemFont);
    availableItem->setFont(itemFont);
    priceItem->setFont(itemFont);
    rentPriceItem->setFont(itemFont);
    booksTable->setItem(row, 0, nameItem);
    booksTable->setItem(row, 1, authorItem);
    booksTable->setItem(row, 2, conditionItem);
    booksTable->setItem(row, 3, availableItem);
    booksTable->setItem(row, 4, priceItem);
    booksTable->setItem(row, 5, rentPriceItem);
}
