#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Инициализация UI

    // Подключение сигналов кнопок
    connect(ui->tb_one,     &QPushButton::clicked, this, [this]{ onDigitButtonClicked(1); });
    connect(ui->tb_two,     &QPushButton::clicked, this, [this]{ onDigitButtonClicked(2); });
    connect(ui->tb_three,  &QPushButton::clicked, this, [this]{ onDigitButtonClicked(3); });
    connect(ui->tb_four,   &QPushButton::clicked, this, [this]{ onDigitButtonClicked(4); });
    connect(ui->tb_five,   &QPushButton::clicked, this, [this]{ onDigitButtonClicked(5); });
    connect(ui->tb_six,    &QPushButton::clicked, this, [this]{ onDigitButtonClicked(6); });
    connect(ui->tb_seven,  &QPushButton::clicked, this, [this]{ onDigitButtonClicked(7); });
    connect(ui->tb_eight,  &QPushButton::clicked, this, [this]{ onDigitButtonClicked(8); });
    connect(ui->tb_nine,   &QPushButton::clicked, this, [this]{ onDigitButtonClicked(9); });
    connect(ui->tb_zero,   &QPushButton::clicked, this, [this]{ onDigitButtonClicked(0); });

    connect(ui->tb_add,       &QPushButton::clicked, this, [this]{ onOperationButtonClicked(Operation::ADDITION); });
    connect(ui->tb_substract, &QPushButton::clicked, this, [this]{ onOperationButtonClicked(Operation::SUBTRACTION); });
    connect(ui->tb_multiplicate, &QPushButton::clicked, this, [this]{ onOperationButtonClicked(Operation::MULTIPLICATION); });
    connect(ui->tb_divide,   &QPushButton::clicked, this, [this]{ onOperationButtonClicked(Operation::DIVISION); });
    connect(ui->tb_power,    &QPushButton::clicked, this, [this]{ onOperationButtonClicked(Operation::POWER); });

    connect(ui->tb_equal,    &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::EQUALS); });
    connect(ui->tb_reset,    &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::CLEAR); });
    connect(ui->tb_ms,      &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::MEM_SAVE); });
    connect(ui->tn_mr,     &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::MEM_LOAD); });
    connect(ui->tb_mc,      &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::MEM_CLEAR); });
    connect(ui->tb_negate,  &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::PLUS_MINUS); });
    connect(ui->tb_backspace, &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::BACKSPACE); });
    connect(ui->tb_extra,   &QPushButton::clicked, this, [this]{ onControlButtonClicked(ControlKey::EXTRA_KEY); });

    connect(ui->cmb_controller, &QComboBox::currentTextChanged, this, &MainWindow::onControllerTypeChanged);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetInputText(const std::string &text)
{
    ui->l_result->setStyleSheet("");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetErrorText(const std::string &text)
{
    ui->l_result->setStyleSheet("color: red;");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetFormulaText(const std::string &text)
{
    ui->l_formula->setText(QString::fromStdString(text));
}

void MainWindow::SetMemText(const std::string &text)
{
    ui->l_memory->setText(QString::fromStdString(text));
}

void MainWindow::SetExtraKey(const std::optional<std::string> &key)
{
    if (key.has_value()) {
        ui->tb_extra->setText(QString::fromStdString(key.value()));
        ui->tb_extra->show();
    } else {
        ui->tb_extra->hide();
    }
}

void MainWindow::SetDigitKeyCallback(std::function<void (int)> cb)
{
    digit_cb_ = std::move(cb);
}

void MainWindow::SetProcessOperationKeyCallback(std::function<void (Operation)> cb)
{
    operation_cb_ = std::move(cb);
}

void MainWindow::SetProcessControlKeyCallback(std::function<void (ControlKey)> cb)
{
    control_cb_ = std::move(cb);
}

void MainWindow::SetControllerCallback(std::function<void (ControllerType)> cb)
{
    controller_cb_ = std::move(cb);
}

void MainWindow::onDigitButtonClicked(int digit)
{
    if (digit_cb_) {
        digit_cb_(digit);
    }
}

void MainWindow::onOperationButtonClicked(Operation op)
{
    if (operation_cb_) {
        operation_cb_(op);
    }
}

void MainWindow::onControlButtonClicked(ControlKey key)
{
    if (control_cb_) {
        control_cb_(key);
    }
}

void MainWindow::onControllerTypeChanged(const QString &text)
{
    ControllerType type = parseControllerType(text);
    if (controller_cb_) {
        controller_cb_(type);
    }
}

ControllerType MainWindow::parseControllerType(const QString &text) const
{
    if (text == "double") {
        return ControllerType::DOUBLE;
    } else if (text == "float") {
        return ControllerType::FLOAT;
    } else if (text == "uint8_t") {
        return ControllerType::UINT8_T;
    } else if (text == "int") {
        return ControllerType::INT;
    } else if (text == "int64_t") {
        return ControllerType::INT64_T;
    } else if (text == "size_t") {
        return ControllerType::SIZE_T;
    } else if (text == "Rational") {
        return ControllerType::RATIONAL;
    } else {
        return ControllerType::DOUBLE;
    }
}





