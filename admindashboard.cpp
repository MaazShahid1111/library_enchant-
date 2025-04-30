#include "admindashboard.h"
#include <QPainter>
#include <QHeaderView>
#include <QMessageBox>
#include <QFormLayout>
#include <QSqlRecord>

AdminDashboard::AdminDashboard(QWidget *parent) : QWidget(parent)
{
    // Initialize database
    if (!m_dbHandler.initialize()) {
        QMessageBox::critical(this, "Database Error", "Failed to initialize database: " + m_dbHandler.getLastError());
    }

    // Set window properties
    setWindowTitle("Admin Dashboard");
    resize(700, 600);
    // Load background image
    backgroundImage.load("C:/library_dasboard.jpg");
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    // Create title label
    titleLabel = new QLabel("Admin Dashboard", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #FFFFFF; background-color: rgba(0, 0, 0, 120); padding: 10px; border-radius: 5px;");
    // Create books table
    booksTable = new QTableWidget(this);
    booksTable->setColumnCount(5);
    QStringList headers;
    headers << "Book Name" << "Author" << "ISBN" << "Status" << "PDF";
    booksTable->setHorizontalHeaderLabels(headers);
    booksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    booksTable->setStyleSheet("QTableWidget { background-color: rgba(255, 255, 255, 200); border-radius: 5px; }");
    booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Create form layout for adding books
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    // Create input fields
    //BookNameEdit creation and setting style sheet
    bookNameEdit = new QLineEdit(this);
    bookNameEdit->setPlaceholderText("Enter book name");
    bookNameEdit->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 5px;");
    //AuthorEdit creation and setting style sheet
    authorEdit = new QLineEdit(this);
    authorEdit->setPlaceholderText("Enter author name");
    authorEdit->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 5px;");
    //ISBN Edit creation and setting style sheet
    isbnEdit = new QLineEdit(this);
    isbnEdit->setPlaceholderText("Enter ISBN");
    isbnEdit->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 5px;");
    //Pdfpath Edit creations and its stylesheet
    pdfPathEdit = new QLineEdit(this);
    pdfPathEdit->setPlaceholderText("PDF path (optional)");
    pdfPathEdit->setStyleSheet("background-color: rgba(255, 255, 255, 200); border-radius: 5px; padding: 5px;");
    pdfPathEdit->setReadOnly(true);
    // Create labels
    //bookNameLabel
    QLabel *bookNameLabel = new QLabel("Book Name:", this);
    bookNameLabel->setStyleSheet("color: #FFFFFF; font-weight: bold;");
    //authorLabel creation and setting style sheet
    QLabel *authorLabel = new QLabel("Author:", this);
    authorLabel->setStyleSheet("color: #FFFFFF; font-weight: bold;");
    //ISBN Label creation and setting style sheet
    QLabel *isbnLabel = new QLabel("ISBN:", this);
    isbnLabel->setStyleSheet("color: #FFFFFF; font-weight: bold;");
    //PDF path Label creation and setting style sheet
    QLabel *pdfPathLabel = new QLabel("PDF File:", this);
    pdfPathLabel->setStyleSheet("color: #FFFFFF; font-weight: bold;");
    // Create PDF attachment button
    QHBoxLayout *pdfButtonLayout = new QHBoxLayout();
    attachPdfButton = new QPushButton("Attach PDF", this);
    attachPdfButton->setStyleSheet("QPushButton {""background-color: #9C27B0;"
                                   "color: white;""border-radius: 5px;""font-size: 12px;""font-weight: bold;""padding: 5px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: #7B1FA2;"
                                   "}");
    pdfButtonLayout->addWidget(pdfPathEdit);
    pdfButtonLayout->addWidget(attachPdfButton);
    // Add form fields
    formLayout->addRow(bookNameLabel, bookNameEdit);//FOR THE ADDITIION OF BOOKS IN THE COLUMnS
    formLayout->addRow(authorLabel, authorEdit);
    formLayout->addRow(isbnLabel, isbnEdit);
    formLayout->addRow(pdfPathLabel, pdfButtonLayout);
    // Create buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    addBookButton = new QPushButton("Add Book", this);
    addBookButton->setMinimumSize(120, 40);
    addBookButton->setStyleSheet("QPushButton {""background-color: #4CAF50;"
                                 "color: white;""border-radius: 5px;""font-size: 14px;""font-weight: bold;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: #45a049;"
                                 "}");
    viewAllBooksButton = new QPushButton("View All Books", this);
    viewAllBooksButton->setMinimumSize(120, 40);
    viewAllBooksButton->setStyleSheet("QPushButton {""background-color: #2196F3;"
                                      "color: white;""border-radius: 5px;""font-size: 14px;""font-weight: bold;"
                                      "}"
                                      "QPushButton:hover {"
                                      "background-color: #0b7dda;"
                                      "}");
    viewBookButton = new QPushButton("View Book PDF", this);
    viewBookButton->setMinimumSize(120, 40);
    viewBookButton->setStyleSheet("QPushButton {""background-color: #FF9800;"
                                  "color: white;""border-radius: 5px;""font-size: 14px;""font-weight: bold;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #e68a00;"
                                  "}");
    logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumSize(120, 40);
    logoutButton->setStyleSheet("QPushButton {""background-color: #f44336;"
                                "color: white;""border-radius: 5px;""font-size: 14px;""font-weight: bold;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #d32f2f;"
                                "}");
    buttonLayout->addWidget(addBookButton);
    buttonLayout->addWidget(viewAllBooksButton);
    buttonLayout->addWidget(viewBookButton);
    buttonLayout->addWidget(logoutButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    // Add widgets to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(booksTable);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    // Connect signals and slots
    connect(addBookButton, &QPushButton::clicked, this, &AdminDashboard::onAddBookButtonClicked);
    connect(viewAllBooksButton, &QPushButton::clicked, this, &AdminDashboard::onViewAllBooksButtonClicked);
    connect(logoutButton, &QPushButton::clicked, this, &AdminDashboard::onLogoutButtonClicked);
    connect(attachPdfButton, &QPushButton::clicked, this, &AdminDashboard::onAttachPdfButtonClicked);
    connect(viewBookButton, &QPushButton::clicked, this, &AdminDashboard::onViewBookButtonClicked);

    // Initialize table with books from database
    setupInitialBooks();
    refreshBookTable();
}

AdminDashboard::~AdminDashboard()
{
}

void AdminDashboard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // Draw background image
    if (!backgroundImage.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), backgroundImage);
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 120));
    } else {
        painter.fillRect(0, 0, width(), height(), QColor(50, 50, 50));
    }
    QWidget::paintEvent(event);
}

void AdminDashboard::refreshBookTable()
{
    // Clear the table
    booksTable->setRowCount(0);

    // Fetch all books from the database
    QSqlQuery query = m_dbHandler.getAllBooks();

    // Add books to the table
    while (query.next()) {
        int row = booksTable->rowCount();
        booksTable->insertRow(row);

        QTableWidgetItem *nameItem = new QTableWidgetItem(query.value("title").toString());
        QTableWidgetItem *authorItem = new QTableWidgetItem(query.value("author").toString());
        QTableWidgetItem *isbnItem = new QTableWidgetItem(query.value("isbn").toString());
        QTableWidgetItem *statusItem = new QTableWidgetItem(query.value("availability").toString());

        // Check if there's a PDF path stored
        QString pdfPath = query.value("pdf_path").toString();
        QTableWidgetItem *pdfItem = new QTableWidgetItem(pdfPath.isEmpty() ? "No" : "Yes");

        // Store PDF path in our map for quick access
        if (!pdfPath.isEmpty()) {
            m_bookPdfPaths[query.value("isbn").toString()] = pdfPath;
        }

        booksTable->setItem(row, 0, nameItem);
        booksTable->setItem(row, 1, authorItem);
        booksTable->setItem(row, 2, isbnItem);
        booksTable->setItem(row, 3, statusItem);
        booksTable->setItem(row, 4, pdfItem);
    }
}

void AdminDashboard::onAddBookButtonClicked()
{
    QString bookName = bookNameEdit->text();
    QString author = authorEdit->text();
    QString isbn = isbnEdit->text();
    QString pdfPath = pdfPathEdit->text();

    if (bookName.isEmpty() || author.isEmpty() || isbn.isEmpty()) {
        QMessageBox::warning(this, "Add Book", "Please fill in all required fields (Book Name, Author, ISBN).");
        return;
    }

    // Create SQL query to add the book
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, availability, isbn, pdf_path) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(bookName);
    query.addBindValue(author);
    query.addBindValue("Available");
    query.addBindValue(isbn);
    query.addBindValue(pdfPath);

    if (!query.exec()) {
        QMessageBox::critical(this, "Add Book", "Failed to add book: " + query.lastError().text());
        return;
    }

    // Clear the input fields
    bookNameEdit->clear();
    authorEdit->clear();
    isbnEdit->clear();
    pdfPathEdit->clear();

    // Refresh the table to show the new book
    refreshBookTable();

    QMessageBox::information(this, "Add Book", "Book added successfully!");
}

void AdminDashboard::onViewAllBooksButtonClicked()
{
    // Just refresh the table
    refreshBookTable();
    QMessageBox::information(this, "View All Books", "Displaying all books in the library.");
}

void AdminDashboard::onLogoutButtonClicked()
{
    // Hide this window and show the main window
    this->hide();
    this->parentWidget()->show();
}

void AdminDashboard::onAttachPdfButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select PDF File", "", "PDF Files (*.pdf)");
    if (!filePath.isEmpty()) {
        pdfPathEdit->setText(filePath);
    }
}

void AdminDashboard::onViewBookButtonClicked()
{
    // Get the selected row
    QModelIndexList selectedIndexes = booksTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "View Book", "Please select a book to view.");
        return;
    }

    int row = selectedIndexes.first().row();
    QString isbn = booksTable->item(row, 2)->text();
    QString pdfStatus = booksTable->item(row, 4)->text();

    if (pdfStatus == "No") {
        QMessageBox::warning(this, "View Book", "No PDF available for this book.");
        return;
    }

    // Get PDF path from our map
    QString pdfPath = m_bookPdfPaths.value(isbn);

    if (!pdfPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
    } else {
        QMessageBox::information(this, "View Book", "PDF path not found for this book.");
    }
}

void AdminDashboard::setupInitialBooks()
{
    // Check if there are books in the database
    QSqlQuery query = m_dbHandler.getAllBooks();
    if (!query.next()) {
        // Database is empty, add initial books
        m_dbHandler.addBook("To Kill a Mockingbird", "Harper Lee", true, "9780061120084");
        m_dbHandler.addBook("1984", "George Orwell", true, "9780451524935");
        m_dbHandler.addBook("The Great Gatsby", "F. Scott Fitzgerald", true, "9780743273565");
        m_dbHandler.addBook("The Catcher in the Rye", "J.D. Salinger", true, "9780316769488");
        m_dbHandler.addBook("The Hobbit", "J.R.R. Tolkien", true, "9780547928227");
        m_dbHandler.addBook("The Lord of the Rings", "J.R.R. Tolkien", true, "9780618640157");
        m_dbHandler.addBook("Harry Potter and the Sorcerer's Stone", "J.K. Rowling", true, "9780590353427");
        m_dbHandler.addBook("Pride and Prejudice", "R.L.Stevenson", true, "97805903565353");

        // Add PDF paths to the database
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE books SET pdf_path = ? WHERE isbn = ?");

        // To Kill a Mockingbird
        updateQuery.addBindValue("C:/Books/ToKillaMockingbird.pdf");
        updateQuery.addBindValue("9780061120084");
        updateQuery.exec();

        // 1984
        updateQuery.addBindValue("C:/Books/1984.pdf");
        updateQuery.addBindValue("9780451524935");
        updateQuery.exec();

        // The Great Gatsby
        updateQuery.addBindValue("C:/Books/TheGreatGatsby.pdf");
        updateQuery.addBindValue("9780743273565");
        updateQuery.exec();

        // The Catcher in the Rye
        updateQuery.addBindValue("C:/Books/TheCatcherintheRye.pdf");
        updateQuery.addBindValue("9780316769488");
        updateQuery.exec();

        // The Hobbit
        updateQuery.addBindValue("C:/Books/TheHobbit.pdf");
        updateQuery.addBindValue("9780547928227");
        updateQuery.exec();

        // The Lord of the Rings
        updateQuery.addBindValue("C:/Books/TheLordoftheRings.pdf");
        updateQuery.addBindValue("9780618640157");
        updateQuery.exec();

        // Harry Potter
        updateQuery.addBindValue("C:/Books/HarryPotterandtheSorcerer'sStone.pdf");
        updateQuery.addBindValue("9780590353427");
        updateQuery.exec();

        // Treasure Island
        updateQuery.addBindValue("C:/Books/PrideandPrejudice.pdf");
        updateQuery.addBindValue("97805903565353");
        updateQuery.exec();
    }
}
