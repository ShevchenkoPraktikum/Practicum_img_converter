#pragma once

#include "calculator.h"
#include "enums.h"

#include <QMainWindow>
#include <functional>
#include <optional>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetInputText(const std::string& text);
    void SetErrorText(const std::string& text);
    void SetFormulaText(const std::string& text);
    void SetMemText(const std::string& text);
    void SetExtraKey(const std::optional<std::string>& key);

    void SetDigitKeyCallback(std::function<void(int)> cb);
    void SetProcessOperationKeyCallback(std::function<void(Operation)> cb);
    void SetProcessControlKeyCallback(std::function<void(ControlKey)> cb);
    void SetControllerCallback(std::function<void(ControllerType)> cb);

private slots:
    void onDigitButtonClicked(int digit);
    void onOperationButtonClicked(Operation op);
    void onControlButtonClicked(ControlKey key);
    void onControllerTypeChanged(const QString& text);

private:
    Ui::MainWindow *ui;

    std::function<void(int)> digit_cb_;
    std::function<void(Operation)> operation_cb_;
    std::function<void(ControlKey)> control_cb_;
    std::function<void(ControllerType)> controller_cb_;

    ControllerType parseControllerType(const QString& text) const;
};
